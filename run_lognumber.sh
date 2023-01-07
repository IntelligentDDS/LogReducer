# bin/bash

log_array=(50 100 1000 2000 5000 10000) # per 100ms
sleep_time=5m

for element in ${log_array[@]}
do

lognumber=`expr $element \* 10`

str="start"
print_str="$str $lognumber"
echo $print_str

if (($lognumber < 20001))
then
## python
echo "start python"
file="python" 

nohup python3 python/hello.py --lognumber=$element > ./log/benchmark.log 2>&1 &
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello.py --template '2022-08-16 20:11:31 - ERROR - [python/hello.py:37]' --language python > ./log/reduer.log 2>&1 &
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello.py | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s

fi

## java
echo "start java"
file="java" 

java -jar ./java/helloLog/target/helloLog-jar-with-dependencies.jar -n $element > ./log/benchmark.log 2>&1 & 
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program helloLog --template '2022-08-16 19:47:26,505 ERROR com.yuxiaoba.hello.Hello [Hello.java:90]' --language python > ./log/reduer.log 2>&1 &
sleep 5s
echo "start log reducer"


sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep helloLog | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s

## go
echo "start go"
file="go" 

./golang/hello  --lognumber=$element > ./log/benchmark.log 2>&1 & 
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello --template '"level":"error","ts":"2022-08-16 19:49:04.210","caller":"golang/hello.go:86"' --language golang > ./log/reduer.log 2>&1 & 
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s

## 
echo "start c++"
file="c++" 

./c++/hello -lognumber $element > ./log/benchmark.log 2>&1 & 
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello --template '2022/08/16 19:32:16,512.177 ERROR - This is log hotspots 6 [hello.cpp:51]' --language python > ./log/reduer.log 2>&1 &
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReducer.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello | grep -v grep | awk '{print $2}' | xargs kill -9

done 