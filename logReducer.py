from ast import arg
from bcc import BPF
# from elasticsearch import Elasticsearch
import ctypes as ct
import datetime
import argparse
import time
import os
import re
import yaml


def get_pid(program_name):
    get_pid_command = "ps -ef | grep %s | grep -v grep |grep -v program | awk \'{print $2}\'" % program_name
    result = os.popen(get_pid_command)
    pid = result.read()

    if "\n" in pid:
        pid = pid.strip("\n")

    print(pid)
    if pid == "":
        print("program not found")
        return 0
    return int(pid)

def extract_python_template(template):
    start = len(template.split("[")[0])
    location = "[" + template.split("[")[1]
    print("python", start,location)
    return start, location 

def extract_golang_template(template):
    start = len(template.split("/")[0]) + 2
    location = template.split("/")[1].split(",")[0]
    print("golang", start,location)
    return start, location 

parser = argparse.ArgumentParser(
    description="filter logs of specific program and location",
    formatter_class=argparse.RawDescriptionHelpFormatter)
# parser.add_argument("pid", nargs='+', type=int,
#                     help="only trace those process whose name here")

# with open("./config.yaml", "r", encoding="utf8") as f:
#     config = yaml.load(f)
#     print(config)
parser.add_argument("--program", type=str, default="hello",
                    help="only filter the program named here")
parser.add_argument("--language", type=str, default="python",
                    help="for filter template")
parser.add_argument("--template", type=str, default="2022-08-15 10:13:22 - ERROR - [/data/home/logbench/python/hello.py:26]",
                    help="only filter the template named here")

args = parser.parse_args()

pid = get_pid(args.program)

if args.language=="python":
    start, location = extract_python_template(args.template)
elif args.language=="golang":
    start, location = extract_golang_template(args.template)

class WritePackage(ct.Structure):
    _fields_ = [
        ("pid", ct.c_uint),
        ("fd", ct.c_uint),
        ("contents", ct.c_char * 64),
        ("fileName", ct.c_char * 32)
    ]


# Loads eBPF program
b = BPF(src_file="logReducer.c")

# Attach tail call
# https://github.com/iovisor/bcc/blob/e83019bdf6c400b589e69c7d18092e38088f89a8/tests/python/test_call1.py
# https://programs.team/ebpf-from-bpf-to-bpf-calls-to-tail-calls.html
# check_fn = b.load_func("check_template", BPF.KPROBE)
# upload_fn = b.load_func("upload_and_block", BPF.KPROBE)
# jump = b.get_table("jump")
# # jump[ct.c_int(1)] = ct.c_int(check_fn.fd)
# jump[ct.c_int(2)] = ct.c_int(upload_fn.fd)

# Attach kprobe
# 这里监听了文件打开和写入两部分系统调用
#  - 打开是为了获取到文件名和fd的对应关系
#  - 写入是实际要监听的内容
b.attach_kprobe(event="do_sys_open", fn_name="detect_file_open")
b.attach_kprobe(event=b.get_syscall_fnname(
    "write"), fn_name="detect_file_write")
b.attach_kretprobe(event="do_sys_open", fn_name="detect_file_open_ret")

# 监听文件触发写入，需要执行的动作写这里


def trigger_alert_write(cpu, data, size):
    # global table
    event = ct.cast(data, ct.POINTER(WritePackage)).contents
    contents = event.contents.decode('utf-8', 'ignore')
    timestamp = datetime.datetime.now()
    print("%-29s (pid:%ld fd:%ld name:%s):%s" % (
        timestamp,
        event.pid,
        event.fd,
        event.fileName,
        contents))

    # # 考虑存一段时间再push
    # if es is not None:
    #     result = es.index(index='logs', doc_type=event.fileName, body={
    #         "timestamp": timestamp,
    #         "pid": event.pid,
    #         "fd": event.fd,
    #         "contents": contents
    #     })
    #     print(result)

# 程序运行中发现新文件写入操作，决策是否监听拦截


def trigger_alert_find(cpu, data, size):
    global b
    listen_map = b["listen_map"]
    print("trigger")
    for listen_key in listen_map:
        listen_val = listen_map[listen_key]
        if listen_val.needListen == 0:
            path = os.readlink("/proc/%d/fd/%d" %
                               (listen_key.pid, listen_key.fd))
            filename = path.split("/")[-1]
            if re.match(r".+\.log$", filename):
                listen_map[listen_key] = listen_map.Leaf(
                    1, bytes(filename, encoding="utf-8"))
            else:
                listen_map[listen_key] = listen_map.Leaf(2, b"")


b["events_write"].open_perf_buffer(trigger_alert_write)
b["events_find"].open_perf_buffer(trigger_alert_find)

# 初始化时遍历监听进程正在写入的文件，决策是否需要监听和拦截
listen_map = b.get_table("listen_map")
pid_map = b.get_table("pid_map")
#for pid in args.pid:
path = '/proc/%d/fd' % pid
for fd in os.listdir(path):
    filename = os.readlink("%s/%s" % (path, fd)).split("/")[-1]
    if filename:
        print(pid, filename, fd)
        listen_key = listen_map.Key(pid, int(fd))
        if re.match(r".+\.log$", filename):
            listen_map[listen_key] = listen_map.Leaf(
                1, bytes(filename, encoding="utf-8"))
        else:
            listen_map[listen_key] = listen_map.Leaf(2, b"")
    pid_map[ct.c_uint(pid)] = ct.c_uint(pid)

# 拦截内容匹配，只有匹配上的写入才会进行监听和拦截
filter_map = b.get_table("filter_map")
start_map = b.get_table("start_map")


filter_map[ct.c_uint(0)] = filter_map.Leaf(
    bytes(location, encoding="utf-8"))
start_map[ct.c_uint(0)] = ct.c_uint(start)

# for idx, filter in enumerate(config["filter"]):
#     filter_map[ct.c_uint(idx)] = filter_map.Leaf(
#         bytes(filter, encoding="utf-8"))
#     start_map[ct.c_uint(idx)] = ct.c_uint(29)

print("start reading... ")
# b.trace_print()
while 1:
    try:
        b.perf_buffer_poll()
    except KeyboardInterrupt:
        exit()
