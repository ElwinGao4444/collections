#!/bin/bash

cc getopt.c
./a.out -a	# 选项a后面的value不能为空
./a.out -a1	# 选项a后面可以不加空格
./a.out -a 1	# 选项a后面也可以加空格
./a.out -b	# 选项b后面可以不跟value
./a.out -b1	# 选项b如果后面跟value则中间不能有关空格
./a.out -b 1	# 如果选项b后面有空格则视同于没有value
./a.out -c	# 选项c为空参选
./a.out -c 1	# 选项c后面没有value，写了也没用
./a.out -d	# 与选项c同理
./a.out -d 1	# 与选项c同理
./a.out -e	# 与选项c同理
./a.out -e 1	# 与选项c同理

./a.out -a 1 -b2 -c	# 选项的组合使用

