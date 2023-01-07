```
tar xjvf log4cplus-2.0.8.tar.bz2

cd log4cplus-2.0.8

./configure --prefix=/usr/local

make

make install

vim /etc/ld.so.conf

/usr/local/lib

ldconfig


g++ hello.cpp -o hello -llog4cplus -std=gnu++17

./hello -lognumber 1000 

```
