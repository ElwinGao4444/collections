#!/bin/env bash

################################################################
#                         Inner Func                          #
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
#                          Base Env                           #
################################################################
# 通过获取绝对地址，以确保脚本的执行不受执行脚本时的“当前路径”所干扰
full_path=`readlink -f $0`
BASE_DIR=`dirname $full_path`
FILE_NAME=`basename $full_path`
echo 'BASE_DIR:' $BASE_DIR
echo 'FILE_NAME:' $FILE_NAME

################################################################
#                         Main Func                           #
################################################################
echo_ok 'ok'
echo_err 'err'
echo_msg 'msg'
echo_normal 'normal'

