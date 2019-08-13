#!/bin/bash

set -x

basepath=$(cd `dirname $0`; pwd)
top_dir=`dirname $basepath`
CONF_DIR=$top_dir/conf

source $CONF_DIR/framework.conf
source $SHELL_DIR/log.sh
source $SHELL_DIR/func.sh

INIT
ADD_PROCESS $SHELL_DIR/demo-step-1.sh
ADD_PROCESS $SHELL_DIR/demo-step-1.sh
ADD_PROCESS $SHELL_DIR/demo-step-2.sh 2
ADD_PROCESS $SHELL_DIR/demo-step-2.sh 4

START_PROCESS

in=$PROCESS_INPUT
out=$PROCESS_OUTPUT

for i in `seq 0 9`;do
	echo "input-$i" >> $in
done
echo "FINISH" >> $in

wait

