#!/bin/bash

# **************************************************************** #
#                            日志及报警                            #
# **************************************************************** #

# **************** #
#    FATAL日志     #
# **************** #
function FATAL() {
	echo "[FATAL] `date +"%F %T"` : $@" >&2
	SUBJECT="[FATAL]$USER@$HOSTNAME : $@"
	MESSAGE="[FATAL]$USER@$HOSTNAME : $@"
	ALARM_EMAIL "$SUBJECT" "$MESSAGE"
	ALARM_MOBILE_LIST "$SUBJECT"
	exit -1;
}

# **************** #
#   WARNING日志    #
# **************** #
function WARNING() {
	echo "[WARNING] `date +"%F %T"` : $@" >&2
	SUBJECT="[WARNING]$USER@$HOSTNAME : $@"                                                                                                                                    
	MESSAGE="[WARNING]$USER@$HOSTNAME : $@"
	ALARM_EMAIL "$SUBJECT" "$MESSAGE"
}

# **************** #
#    NOTICE日志    #
# **************** #
function NOTICE() {
	echo "[NOTICE] `date +"%F %T"` : $@"
}

# **************** #
#    检测返回值    #
# **************** #
function CHK_RET() {                                                                                                                               
	if [ $? -ne 0 ]; then
		$@
	fi
}

# **************** #
#   生成邮件列表   #
# **************** #
function CAT_EMAIL_LIST() {
	ELIST=""
	for i in $*;do
		if [ -z $ELIST ]; then
			ELIST=$i
		else
			ELIST=$ELIST,$i
		fi
	done

	echo $ELIST
}

# **************** #
#   发送报警邮件   #
# **************** #
function ALARM_EMAIL() {
	if [ $# -ne 2 ]; then
		return 1
	fi

	ELIST="$(CAT_EMAIL_LIST $EMAIL_LIST)"
	echo "$2" | mail -s "$1" "$ELIST"
	return 0
}

# **************** #
#   发送报警短信   #
# **************** #
function ALARM_MOBILE() {
	if [ $# -ne 2 ]; then
		return 1
	fi

	gsmsend -s $GSMSERVER:$GSMPORT "$1@$2"
	if [ $? -ne 0 ]; then
		gsmsend -s $GSMSERVER:$GSMPORT "$1@Some problem happened, please check email"
		return 1
	else
		return 0
	fi
}

# **************** #
#   群发报警短信   #
# **************** #
function ALARM_MOBILE_LIST() {
	if [ $# -ne 1 ]; then
		return 1;
	fi

	for i in $MOBILE_LIST;do
		ALARM_MOBILE "$i" "$1"
	done
}

