#!/bin/bash
if [ ! -d "/home/fw/work/bin" ];then
	echo "Path does not exist: /home/fw/work/bin"
else
	cd /home/fw/work/bin
	bc_sdk_perf_test $1 $2
fi