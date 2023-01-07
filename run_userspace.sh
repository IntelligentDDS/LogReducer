# bin/bash

log_array=(50 100 1000 2000 5000 10000) # per 100ms
sleep_time=10m

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
echo python3 python/hello.py --lognumber=$element > ./log/benchmark.log
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello.py  --template  "not" --logfile '/data/home/logbench/python/python.log' --language $file --lognumber $lognumber > ./log/reducer.log 2>&1 &
sleep 5s
echo  python3 logReducer.py  --program hello.py  --template  "not" --logfile '/data/home/logbench/python/python.log' --language $file --lognumber $lognumber
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReader.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello.py | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s

fi

## java
echo "start java"
file="java" 

java -jar ./java/helloLog/target/helloLog-jar-with-dependencies.jar -l $element > ./log/benchmark.log 2>&1 & 
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program helloLog  --template  "not" --logfile '/data/home/logbench/java/java.log' --language $file --lognumber $lognumber  > ./log/reducer.log 2>&1 &
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReader.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep helloLog | grep -v grep | awk '{print $2}' | xargs kill -9

sleep 10s

## go
echo "start go"
file="go" 

./golang/hello  --lognumber=$element > ./log/benchmark.log 2>&1 & 
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello  --template  "not" --logfile '/data/home/logbench/golang/golang.log' --language $file --lognumber $lognumber  > ./log/reducer.log 2>&1 & 
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReader.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello | grep -v grep | awk '{print $2}' | xargs kill -9
sleep 10s

## 
echo "start c++"
file="c++" 

./c++/hello -lognumber $element > ./log/benchmark.log 2>&1 & 
sleep 5s
echo "start benchmark"

nohup python3 logReducer.py  --program hello  --template  "not" --logfile '/data/home/logbench/c++/c++.log' --language $file --lognumber $lognumber  > ./log/reducer.log 2>&1 &
sleep 5s
echo "start log reducer"

sleep $sleep_time
ps -ef | grep logReader.py  | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep hello | grep -v grep | awk '{print $2}' | xargs kill -9

done 