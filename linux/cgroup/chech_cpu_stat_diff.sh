#!/usr/bin/env bash

CG_DIR=/sys/fs/cgroup/cpu/test

cd $CG_DIR

nr_periods=`cat cpu.stat | fgrep nr_periods | cut -d ' ' -f 2`
nr_throttled=`cat cpu.stat | fgrep nr_throttled | cut -d ' ' -f 2`
throttled_time=`cat cpu.stat | fgrep throttled_time | cut -d ' ' -f 2`

while [ 1 ];do
	# read -s -n1 -p "Press any key to continue ... "
	read -p "Press entry to continue ... "
	old_nr_periods=$nr_periods
	old_nr_throttled=$nr_throttled
	old_throttled_time=$throttled_time

	nr_periods=`cat cpu.stat | fgrep nr_periods | cut -d ' ' -f 2`
	nr_throttled=`cat cpu.stat | fgrep nr_throttled | cut -d ' ' -f 2`
	throttled_time=`cat cpu.stat | fgrep throttled_time | cut -d ' ' -f 2`

	nr_periods_diff=`expr $nr_periods - $old_nr_periods`
	nr_throttled_diff=`expr $nr_throttled - $old_nr_throttled`
	throttled_time_diff=`expr $throttled_time - $old_throttled_time`

	echo "nr_periods_diff: $nr_periods_diff"
	echo "nr_throttled_diff: $nr_throttled_diff"
	echo "throttled_time_diff: $throttled_time_diff"
done
