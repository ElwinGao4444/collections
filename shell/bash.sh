#!/bin/env bash

# 通过获取绝对地址，以确保脚本的执行不受执行脚本时的“当前路径”所干扰
full_path=`readlink -f $0`
BASE_DIR=`dirname $full_path`
FILE_NAME=`basename $full_path`
echo 'BASE_DIR:' $BASE_DIR
echo 'FILE_NAME:' $FILE_NAME

