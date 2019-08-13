#!/bin/bash

# **************************************************************** #
#                            流水线控制                            #
# **************************************************************** #

# **************** #
#     过程注册     #
# **************** #
function ADD_PROCESS() {
	fullpath=$1
	dir=`dirname $fullpath`
	file=`basename $fullpath`
	tag=`echo $file | cut -d '.' -f 1`

	n=$PROCESS_COUNT
	eval export PROCESS_PATH_$n=$fullpath
	eval export PROCESS_DIR_$n=$dir
	eval export PROCESS_FILE_$n=$file
	eval export PROCESS_TAG_$n=$tag"_"$n
	eval export PROCESS_QUEUE_$n=$TMP_DIR/$tag"_"$n.queue

	eval export PROCESS_QUEUE_IN_$n=''
	eval export PROCESS_QUEUE_OUT_$n=''

	eval export PROCESS_CONCURRENCE_$n=$2

	let PROCESS_COUNT=$PROCESS_COUNT+1

	return 0
}

# **************** #
#    清理进程树    #
# **************** #
function KILL_TREE() {
	exit 0
}

# **************** #
#     超时终止     #
# **************** #
function OVER_TIME_KILL() {
	return 0
}

# **************** #
#   清理执行环境   #
# **************** #
function CLEAN() {
#	rm -f $LOG_DIR/*
	rm -f $TMP_DIR/*
	return 0
}

# **************** #
#  创建流水线队列  #
# **************** #
function CREATE_QUEUE() {
	# 创建状态队列
	mkfifo -m 620 $STATE_QUEUE 
	CHK_RET FATAL "mkfifo -m 620 $STATE_QUEUE"
	{ while true;do
		while read line;do
			if [ "$line" = 'FINISH' ];then
				NOTICE "destroy $STATE_QUEUE" >> $STATE_LOG
				exit 0
			fi
			echo "$line" >> $STATE_LOG
		done < $STATE_QUEUE 
	done } &
	NOTICE 'create STATE_QUEUE' > $STATE_QUEUE

	# 创建流程队列
	let n=$PROCESS_COUNT-1
	for i in `seq 0 $n`;do
		eval queue=\$PROCESS_QUEUE_$i
		if [ $i -lt $n ];then
			mkfifo -m 620 $queue
			CHK_RET FATAL "mkfifo -m 620 $queue"
		fi

		let j=i-1
		if [ $j -ge 0 ];then
			eval pre_queue=\$PROCESS_QUEUE_$j
		fi

		if [ $BLOCK_STATE = true ];then
			eval PROCESS_QUEUE_IN_$i=$pre_queue
			eval PROCESS_QUEUE_OUT_$i=$queue
		else
			eval PROCESS_QUEUE_IN_$i=$pre_queue
			eval PROCESS_QUEUE_OUT_$i=$queue.seq
			tail -F $queue.seq > $queue &
		fi
	done

	PROCESS_INPUT=$PROCESS_QUEUE_OUT_0
	eval PROCESS_OUTPUT=\$PROCESS_QUEUE_$n

	return 0
}

# **************** #
#  初始化执行环境  #
# **************** #
function INIT() {
	CLEAN
	ADD_PROCESS input
	return 0
}

# **************** #
#    启动流水线    #
# **************** #
function START_PROCESS() {
	CREATE_QUEUE

	let n=$PROCESS_COUNT-1
	for i in `seq 1 $n`;do
		eval path=\$PROCESS_PATH_$i
		eval tag=\$PROCESS_TAG_$i
		sh "$SHELL_DIR/process.sh" $i > $LOG_DIR/$tag.log 2> $LOG_DIR/$tag.log.wf &
	done
	return 0
}

