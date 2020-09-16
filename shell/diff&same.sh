#!/bin/bash

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

