#!/usr/bin/env bash
# Author: Elwin.Gao
# Created Time : Tue Oct  8 11:50:00 2024
# File Name: compile.sh
# Description:

# build
go build .

# run
protoc --helloworld_out=./out message.proto
