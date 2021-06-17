#!/bin/sh
#
if [ $# -lt 1 ]; then
        PROCESSNAMES=("bc_sdk_perf_test" "dsc_dog" "bc_kernel" "bc_ca_server")
else
        PROCESSNAMES=$*
fi

#echo ${PROCESSNAMES[@]}
function killit(){
        echo killing process $1
        PROCESS=`ps -ef|grep $1|grep -v grep|grep -v PPID|awk '{ print $2}'`
        for i in $PROCESS
        do
                echo "Kill the $1 process [ $i ]"
                kill -9 $i
        done
}


for VAR in ${PROCESSNAMES[@]}
do
        killit $VAR
done
