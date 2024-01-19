#!/usr/bin/env bash
# Author: Elwin.Gao
# Created Time : Fri Jan 19 13:21:35 2024
# File Name: run.sh
# Description:

as -o hello.o hello.s
ld -o hello hello.o
./hello
rm ./hello ./hello.o
