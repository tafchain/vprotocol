#!/bin/bash
build(){
    cd /home/fw/work/ && make clean && make
	if [ $? -ne 0 ]; then
	    echo "Error building"
	    exit 1
	fi
}

start(){
    scripts/stopdsc.sh
    sleep 3
	cd /home/fw/work/bin
    dsc_dog -h /home/fw/work/ &
}

if [ ! -d "/home/fw/" ];then
	echo "Path does not exist: /home/fw/"
else
	source /home/fw/.bash_profile
	if [ ! -d "/home/fw/work/" ];then
		echo "Path does not exist: /home/fw/work/"
	else
		build
		start
	fi
fi