#!/usr/bin/env bash
# Author: Elwin.Gao
# Created Time : Fri Jan 19 13:26:27 2024
# File Name: run.sh
# Description:

gcc -ftest-coverage -fprofile-arcs foo.c
./a.out
gcov -o foo.c foo.gcda
foo.c.gcov

cat foo.c.gcov

rm foo.c.gcov foo.gcda foo.gcno a.out
