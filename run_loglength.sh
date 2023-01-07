# bin/bash

log_array=(20 50 100 200 500 1000) # per 100ms
sleep_time=5m

for element in ${log_array[@]}
do

front="log"
loglength="$front$element"

str="start"
print_str="$str $lognumber"
echo $print_str

## python
echo "start python"
file="python" 

nohup python3 python/hello.py --lognumber=1000 --length=$loglength> ./log/benchmark.log 2>&1 &
echo python3 python/hello.py --lognumber=1000 --length=$loglength
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello.py --template '2022-08-16 20:11:31 - ERROR - [python/hello.py:46]' --language python > ./log/reduer.log 2>&1 &
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello.py | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s


## java
echo "start java"
file="java" 

java -jar ./java/helloLog/target/helloLog-jar-with-dependencies.jar -l $loglength -n 1000 > ./log/benchmark.log 2>&1 & 
echo java -jar ./java/helloLog/target/helloLog-jar-with-dependencies.jar -l $loglength -n 1000
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program helloLog --template '2022-08-16 19:47:26,505 ERROR com.yuxiaoba.hello.Hello [Hello.java:104]' --language python > ./log/reduer.log 2>&1 &
sleep 5s
echo "start log reducer"


sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep helloLog | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s

## go
echo "start go"
file="go" 

./golang/hello  --lognumber=1000   --length=$loglength > ./log/benchmark.log 2>&1 & 
echo ./golang/hello  --lognumber=1000   --length=$loglength
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello --template '"level":"error","ts":"2022-08-16 19:49:04.210","caller":"golang/hello.go:99"' --language golang > ./log/reduer.log 2>&1 & 
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s

## 
echo "start c++"
file="c++" 

./c++/hello -lognumber 1000  -length $loglength > ./log/benchmark.log 2>&1 & 
echo ./c++/hello -lognumber 1000  -length $loglength
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py --program hello --template '2022/08/24 08:29:03,534.244 ERROR - [hello.cpp:60]' --language python > ./log/reduer.log 2>&1 &
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello | grep -v grep | awk '{print $2}' | xargs kill -9

done 