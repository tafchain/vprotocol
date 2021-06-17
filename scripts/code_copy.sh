#!/bin/bash
if [ ! -d "/home/fw/work/" ];then
        echo "Path does not exist: /home/fw/work/"
        cp -Rf /var/lib/jenkins/workspace/pipeline-192.168.0.205 /home/fw/work
else
    echo "rsync exec"
        rsync -vzurtopg --progress --delete /var/lib/jenkins/workspace/pipeline-192.168.0.205/src/ /home/fw/work/src/
fi

chmod +x /home/fw/work/scripts/code_copy.sh
chmod +x /home/fw/work/scripts/code_make.sh
chmod +x /home/fw/work/scripts/performance_testing.sh
chmod +x /home/fw/work/scripts/stopdsc.sh