#!/bin/sh

LOGDIR=$1

if [ "$LOGDIR" = "" ];then
	echo "log_analyzer.sh [logdir]"
	exit 1
fi

NCPUS=`cat /proc/cpuinfo |grep processor|wc -l`
NDOMAINS=`ls $LOGDIR/schedstat_cpu0-domain*|wc -l`

ruby sum_row.rb $LOGDIR/interrupts%d.log $LOGDIR/merged_interrupts interrupts%d.log $NCPUS
ruby sum_cpus.rb $LOGDIR/merged_interrupts%d.log $LOGDIR/sum_merged_interrupts.log $NCPUS
ruby sum_col.rb $LOGDIR/sum_merged_interrupts.log $LOGDIR/sumall_merged_interrupts.log
ruby bias_cpus.rb $LOGDIR/merged_interrupts%d.log $LOGDIR/bias_merged_interrupts.log $NCPUS

ruby sum_col.rb $LOGDIR/netstat_ipext.log $LOGDIR/sumall_netstat_ipext.log
ruby sum_col.rb $LOGDIR/netstat_tcpext.log $LOGDIR/sumall_netstat_tcpext.log

ruby sum_col.rb $LOGDIR/proc_sched_a.log $LOGDIR/sumall_proc_sched_a.log
ruby sum_col.rb $LOGDIR/proc_sched_b.log $LOGDIR/sumall_proc_sched_b.log
ruby sum_col.rb $LOGDIR/proc_sched_c.log $LOGDIR/sumall_proc_sched_c.log

ruby sum_cpus.rb $LOGDIR/schedstat_cpu%d.log $LOGDIR/sum_schedstat_cpu.log $NCPUS
ruby sum_col.rb $LOGDIR/sum_schedstat_cpu.log $LOGDIR/sumall_schedstat_cpu.log
ruby bias_cpus.rb $LOGDIR/schedstat_cpu%d.log $LOGDIR/bias_schedstat_cpu.log $NCPUS

for i in `seq 0 $(($NCPUS - 1))`; do
	ruby sum_cpus.rb $LOGDIR/schedstat_cpu${i}-domain%d.log $LOGDIR/schedstat_domain${i}.log $NDOMAINS
done
ruby sum_cpus.rb $LOGDIR/schedstat_domain%d.log $LOGDIR/sum_schedstat_domain.log $NCPUS
ruby sum_col.rb $LOGDIR/sum_schedstat_domain.log $LOGDIR/sumall_schedstat_domain.log
ruby bias_cpus.rb $LOGDIR/schedstat_domain%d.log $LOGDIR/bias_schedstat_domain.log $NCPUS


for i in `seq 0 $(($NDOMAINS - 1))`; do
	ruby sum_cpus.rb $LOGDIR/schedstat_cpu%d-domain${i}.log $LOGDIR/sum_schedstat_domain${i}.log $NCPUS
	ruby sum_col.rb $LOGDIR/sum_schedstat_domain${i}.log $LOGDIR/sumall_schedstat_domain${i}.log
	ruby bias_cpus.rb $LOGDIR/schedstat_cpu%d-domain${i}.log $LOGDIR/bias_schedstat_domain${i}.log $NCPUS
done

ruby sum_cpus.rb $LOGDIR/stat%d.log $LOGDIR/sum_stat.log $NCPUS
ruby sum_col.rb $LOGDIR/sum_stat.log $LOGDIR/sumall_stat.log
ruby bias_cpus.rb $LOGDIR/stat%d.log $LOGDIR/bias_stat.log $NCPUS
