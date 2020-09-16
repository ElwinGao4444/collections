#!/bin/bash

a='  ff  ff'

# 直接打印b变量，前置空格会被丢掉
echo $a

# 如果想打印前置空格，需要将变量用双引号括起来
echo "$a"

