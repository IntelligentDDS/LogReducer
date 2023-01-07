#include <linux/skbuff.h>
#include <uapi/linux/ip.h>
#include <uapi/linux/ptrace.h>

#define MAX_NB_PACKETS 1000
#define LEGAL_DIFF_TIMESTAMP_PACKETS 1000000
#define MAX_FILTER_SIZE 4
#define MAX_FILTER_LENGTH 64
#define MAX_FILE_NAME 64
#define MAX_PATH_NAME 128
#define MAX_LOG_SIZE 1024
#define LOG_BLOCK_SIZE 64
struct filterNode
{
    char filerExpr[MAX_FILTER_LENGTH];
};

struct listenKey
{
    u32 pid;
    u32 fd;
};

struct listenVal
{
    int needListen;
    char fileName[MAX_FILE_NAME];
};

struct writePackages
{
    u32 pid;
    u32 fd;
    char contents[LOG_BLOCK_SIZE];
    char fileName[MAX_FILE_NAME];
};

BPF_HASH(filter_map, u32, struct filterNode);
BPF_HASH(start_map, u32, u32);
BPF_HASH(pid_map, u32, u32);
BPF_HASH(file_name_map, u32, const char *);
BPF_HASH(listen_map, struct listenKey, struct listenVal);

// https://github.com/iovisor/bcc/blob/e83019bdf6c400b589e69c7d18092e38088f89a8/tests/python/test_call1.c
BPF_PROG_ARRAY(jump, 16);

BPF_PERF_OUTPUT(events_write);
BPF_PERF_OUTPUT(events_find);

int detect_file_open(struct pt_regs *ctx)
{ 
    // 监听程序准备打开一个文件，这时候我们可以获得文件名，保存起来
    u64 tgid = bpf_get_current_pid_tgid();
    u32 pid = tgid >> 32;

    if (!pid_map.lookup(&pid))
    {
        return 0;
    }
    // bpf_trace_printk("open filter pid %d\n", pid);

    // bpf_probe_read_kernel_str in 5.5 or latest
    const char *name = (const char __user *)PT_REGS_PARM2(ctx);
    bpf_trace_printk("open: %s, pid: %d\n", name, pid);

    // A simple postfix comparision, for postfix without duplicate character
    int state = 0;
    char postfix[] = ".log";
    for (int i = 0; i < MAX_FILE_NAME; i++)
    {
        for (int j = sizeof(postfix) - 1; j >= 0; j--)
        {
            if (state == j)
            {
                state = (name[i] == postfix[state]) ? state + 1 : 0;
            }
        }

        if (name[i] == '\0')
        {
            break;
        }
    }

    if (state == sizeof(postfix))
    {
        // bpf_trace_printk("update\n");
        file_name_map.update(&pid, &name);
    }

    return 0;
}

int detect_file_open_ret(struct pt_regs *ctx)
{ 
    // 监听程序完成了文件打开操作，这时候我们可以获取到文件的fd，保存起来
    const char **name;
    u64 tgid = bpf_get_current_pid_tgid();
    u32 pid = tgid >> 32;
    name = file_name_map.lookup(&pid);
    if (!name)
    {
        return 0;
    }

    // bpf_trace_printk("pass\n");
    struct listenKey key = {};
    key.pid = pid;
    key.fd = PT_REGS_RC(ctx);

    struct listenVal val = {};
    bpf_probe_read_kernel(val.fileName, MAX_FILE_NAME, *name);

    file_name_map.delete(&pid);
    listen_map.update(&key, &val);

    return 0;
}

int detect_file_write(struct pt_regs *ctx)
{
    // 判断是否是监听文件的写入
    u64 tgid = bpf_get_current_pid_tgid();
    u32 pid = tgid >> 32;

    if (!pid_map.lookup(&pid))
    {
        return 0;
    }

    struct pt_regs reg = {};
    char *buffer = (char *)PT_REGS_PARM1(ctx); // 拿出函数调用时的用户态寄存器地址
    bpf_probe_read(&reg, sizeof(struct pt_regs), buffer); // 将寄存器内容拷贝一份到ebpf
    struct listenKey key = {}; // 构建哈希表查找key，用于获取当前写入的文件
    key.pid = pid;
    key.fd = (u32)PT_REGS_PARM1(&reg);

    // bpf_trace_printk("write pid: %d fd: %d\n", pid, key.fd);

    struct listenVal *keep_listen;
    keep_listen = listen_map.lookup(&key); // 判断当前写入文件是否需要监听
    if (!keep_listen)
    { 
        // 没有查到监听信息，上报到用户端程序中由它来判断
        struct listenVal needListen = {};
        needListen.needListen = 0;
        listen_map.update(&key, &needListen);
        events_find.perf_submit(ctx, &needListen.needListen, sizeof(int));
        return 0;
    }
    else if (keep_listen->needListen != 1)
    { 
        // 有监听信息且用户端认为不需要监听，跳过
        return 0;
    }

    // 该写入需要监听，判断写入内容是否匹配模板
    // bpf_trace_printk("pass %d name: %s\n", pid, keep_listen->fileName);
    char block_buffer[LOG_BLOCK_SIZE];
    int fail;
    struct filterNode *node;
    u32 *start;
    struct writePackages writePackage = {};

    // 获取写操作传入内容
    void *ptr = (void *)PT_REGS_PARM2(&reg);

    // 给予count边界，确保循环有界以通过编译
    size_t count = (size_t)PT_REGS_PARM3(&reg);
    if (count <= 0)
    {
        return 0;
    }
    else if (count > MAX_LOG_SIZE)
    {
        count = MAX_LOG_SIZE;
    }

    // 开始匹配模板
    int filter_key = 0;
    node = filter_map.lookup(&filter_key);
    start = start_map.lookup(&filter_key);
    if (!start)
    {   
        return 0;        
    }

    // bpf_trace_printk("start: %d", *start);  
    int str = *start;
    // bpf_trace_printk("str: %d", str);
    // 从 ptr 开始读，读取日志前LOG_BLOCK_SIZE个字符
    bpf_probe_read(&block_buffer, sizeof(char) * LOG_BLOCK_SIZE, ptr+str); // 从内核中读取数据

    // bpf_trace_printk("block_buffer: %s");

    if (!node)
    {   
        return 0;
    }

    for (int i = 0; i < LOG_BLOCK_SIZE; i++)
    {
        if (node->filerExpr[i] == '\0')
        { // 筛选成功，拦截
                bpf_override_return(ctx, count);
        }

        if (block_buffer[i] != node->filerExpr[i])
        { // 不满足前缀，跳过当前配置
            break;
        }
    }
    
    return 0;

// success:
//     // 上报数据并拦截实际磁盘写入
//     writePackage.fd = key.fd;
//     writePackage.pid = key.pid;
//     __builtin_memcpy(writePackage.fileName, keep_listen->fileName, MAX_FILE_NAME);
//     bpf_trace_printk("length %d name: \n", count);

//     for (int cur = 0; cur < count; cur += LOG_BLOCK_SIZE)
//     {
//         // Use bpf_probe_read_kernel_str instead in 5.5 or later
//         if (count - cur <= LOG_BLOCK_SIZE)
//         {
//             bpf_probe_read(&writePackage.contents, sizeof(char) * (count - cur), ptr + cur);
//             events_write.perf_submit(ctx, &writePackage, sizeof(writePackage));
//             break;
//         }
//         else
//         {
//             bpf_probe_read(&writePackage.contents, sizeof(char) * LOG_BLOCK_SIZE, ptr + cur);
//             events_write.perf_submit(ctx, &writePackage, sizeof(writePackage));
//         }
//     }
//     bpf_override_return(ctx, count);

//     // 尾调用函数无法调用bpf_override_return，原因不明
//     // jump.call(ctx, 2);
//     return 0;

}

// int upload_and_block(struct pt_regs *ctx)
// {
//     bpf_override_return(ctx, count);
//     return 0;
// }