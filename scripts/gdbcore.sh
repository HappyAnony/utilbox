#!/bin/sh
# gdb attach the assigned app and dump the stack info

date=`date +"%Y%m%d_%H"`
coredumpfile=corestack_${date}
appname=app

echo "--------------------------------------------------------------------------------------">> ${coredumpfile}
echo " ">> ${coredumpfile}
echo " ">> ${coredumpfile}
echo " ">> ${coredumpfile}
echo `date +"%Y-%m-%d %H:%M:%S"` begin >> ${coredumpfile}

gdb attach `ps | grep ${appname} | grep -v grep | awk '{print $1}'` <<SHIT >> ${coredumpfile}
bt
thread apply all bt full

quit
SHIT

echo -n " " >> ${coredumpfile}
echo `date +"%Y-%m-%d %H:%M:%S"` end >> ${coredumpfile}
echo " ">> ${coredumpfile}
echo " ">> ${coredumpfile}
echo " ">> ${coredumpfile}
echo "---------------------------------------------------------------------------------------">> ${coredumpfile}

