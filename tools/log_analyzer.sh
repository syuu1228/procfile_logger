#!/bin/sh

LOGDIR=$1

if [ "$LOGDIR" = "" ];then
	echo "log_analyzer.sh [logdir]"
	exit 1
fi

ruby sum_row.rb $LOGDIR/interrupts%d.log $LOGDIR/merged_interrupts interrupts%d.log
ruby sum_cpus.rb $LOGDIR/merged_interrupts%d.log $LOGDIR/sum_merged_interrupts.log
ruby sum_col.rb $LOGDIR/sum_merged_interrupts.log $LOGDIR/sumall_merged_interrupts.log
ruby bias_cpus.rb $LOGDIR/merged_interrupts%d.log $LOGDIR/bias_merged_interrupts.log

ruby sum_col.rb $LOGDIR/netstat_ipext.log $LOGDIR/sumall_netstat_ipext.log
ruby sum_col.rb $LOGDIR/netstat_tcpext.log $LOGDIR/sumall_netstat_tcpext.log

ruby sum_col.rb $LOGDIR/proc_sched_a.log $LOGDIR/sumall_proc_sched_a.log
ruby sum_col.rb $LOGDIR/proc_sched_b.log $LOGDIR/sumall_proc_sched_b.log
ruby sum_col.rb $LOGDIR/proc_sched_c.log $LOGDIR/sumall_proc_sched_c.log

ruby sum_cpus.rb $LOGDIR/schedstat_cpu%d.log $LOGDIR/sum_schedstat_cpu.log
ruby sum_col.rb $LOGDIR/sum_schedstat_cpulog $LOGDIR/sumall_schedstat_cpu.log
ruby bias_cpus.rb $LOGDIR/schedstat_cpu%d.log $LOGDIR/bias_schedstat_cpu.log

ruby sum_cpus.rb $LOGDIR/stat%d.log $LOGDIR/sum_stat.log
ruby sum_col.rb $LOGDIR/sum_stat.log $LOGDIR/sumall_stat.log
ruby bias_cpus.rb $LOGDIR/stat%d.log $LOGDIR/bias_stat.log
