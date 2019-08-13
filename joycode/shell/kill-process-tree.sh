#!/bin/bash

# 通过参数区分构建多层进程树
param=$1
if [ -z $param ]; then
	param=0
fi

# 打印当前进程pid
mypid=$$
echo mypid is $mypid

# 通过打开和关闭sleep 1s，查看孙子进程已启动和未启动两种情况下，功能是否全部符合预期
#sleep 1s
# 用sleep 1m &来模拟通用子进程的启动
sleep 1m &
echo "do sleep 1m; $!"
# 通过自身递归，建立进程树，将进程树长度延伸至两层以上
if [ $param -eq 0 ]; then
	sh ./kill-process-tree.sh 1 &
	echo "sub-script: $!"
else
	sleep 1
	exit 0;
fi

ps -ef | fgrep $mypid

# 销毁整个进程数，包括自身进程
kill -- -$mypid

echo 'this message can not be seen'

