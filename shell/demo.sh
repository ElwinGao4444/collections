#!/bin/env bash

################################################################
#                         Inner Func                           #
################################################################
function echo_ok() {
	echo -e "\033[32m[OK] \033[0m$1\033[0m"
}

function echo_err() {
	echo -e "\033[31m[ERR] \033[0m$1\033[0m"
}

function echo_msg() {
	echo -e "\033[33m[MSG] \033[0m$1\033[0m"
}

function echo_normal() {
	echo -e "\033[0m$1\033[0m"
}

################################################################
#                          Base Env                            #
################################################################
# 通过获取绝对地址，以确保脚本的执行不受执行脚本时的“当前路径”所干扰
FULLPATH=`readlink -f $0`
BASE_DIR=`dirname $FULLPATH`
FILE_NAME=`basename $FULLPATH`
echo 'BASE_DIR:' $BASE_DIR
echo 'FILE_NAME:' $FILE_NAME

################################################################
#                         Demo Func                            #
################################################################
function echo_log() {
	echo_ok 'ok'
	echo_err 'err'
	echo_msg 'msg'
	echo_normal 'normal'
}

function show_diff_or_same() {
	echo "a\nb\nc\nd" > file1
	echo "a\nx\nc\ny" > file2

	# 输出不同的行：
	echo 'diff: '
	grep -v -wvf file1 file2

	# 输出相同的行：
	echo 'same: '
	grep -wf file1 file2

	rm -f file1
	rm -f file2
}

function show_pre_space() {
	a='  ff  ff'
	# 直接打印b变量，前置空格会被丢掉
	echo $a
	# 如果想打印前置空格，需要将变量用双引号括起来
	echo "$a"
}

function show_pre_space() {
	tab_string="a\tb"
	echo $tab_string

	# sed法，sed法的好处在于可以指定tab的空格数量
	echo $tab_string | sed 's/\t/    /g'

	# tr法，tr法的问题在于tab只能替换成1个空格
	echo $tab_string | tr  "\t" " "

	# col法，col法的好处在于，系统会自动根据tab与空格的对应关系进行补齐
	echo $tab_string | col -x
}

function use_sleep() {
	# sleep最常用的用法为：
	sleep 1
	# 这种用法与
	sleep 1s # MAC不支持这种用法
	# 是一样的
	# 并且，s还可以用m, h, d替换，分别代表分，时，日

	# 此外，sleep也是支持使用小数的
	# 如：
	sleep 0.1
	# 同样可以写成：
	sleep 0.1s # MAC不支持这种用法
}
################################################################
#                         Main Func                            #
################################################################
echo_log
show_diff_or_same
show_pre_space
tab_to_space
use_sleep
