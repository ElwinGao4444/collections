#!/bin/bash

set -x

input_line=$1
output_queue=$2

echo $input_line
echo $input_line.step2 >> $output_queue

