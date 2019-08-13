#!/bin/bash

set -x

source $SHELL_DIR/log.sh
source $SHELL_DIR/func.sh

n=$1
eval fullpath=\$PROCESS_PATH_$n
eval dir=\$PROCESS_DIR_$n
eval file=\$PROCESS_FILE_$n
eval tag=\$PROCESS_TAG_$n
eval queue=\$PROCESS_QUEUE_$n
eval in=\$PROCESS_QUEUE_IN_$n
eval out=\$PROCESS_QUEUE_OUT_$n

# 控制并发
eval concurrence=\$PROCESS_CONCURRENCE_$n
if [ -z $concurrence ];then
	concurrence=1
fi
count=0

con_queue=$TMP_DIR/$tag.con
mkfifo -m 620 $con_queue
CHK_RET FATAL "mkfifo -m 620 $con_queue"

for i in `seq 1 $concurrence`;do
    echo > $con_queue &
done

NOTICE "$tag started" | tee $STATE_QUEUE

while true;do
	while read line;do
		if [ $line = 'FINISH' ];then
			while [ $count -ne $concurrence ];do                                                                                                       
				let count=$count+`cat $con_queue | wc -c`
				# 这是为了解决高并发下，管道会丢数据的bug
				# 不过，如果最后一个进程的数据丢了，那就无解了
				c=`pgrep -P $$`
				if [ -z $c ];then
					break
				fi
			done
			NOTICE "$tag finished" | tee $STATE_QUEUE
			echo "$line" >> $out
			let n=$n+1
			if [ $n -eq $PROCESS_COUNT ];then
				echo 'FINISH' > $STATE_QUEUE
			fi
			exit 0
		fi

		# 控制并发
		if [ $count -eq 0 ];then
			count=`cat $con_queue | wc -c`
		fi

		{
			sh -x $fullpath "$line" "$out"
#			CHK_RET WARNING "sh -x $fullpath" "$line" "$out"
			echo > $con_queue
		} &
		let count=$count-1
	done < $in
done

