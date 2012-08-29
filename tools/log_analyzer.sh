#!/bin/sh

LOGDIR=$1

if [ "$LOGDIR" = "" ];then
	echo "log_analyzer.sh [logdir]"
	exit 1
fi

NCPUS=`cat /proc/cpuinfo |grep processor|wc -l`
NDOMAINS=`ls $LOGDIR/schedstat_cpu0-domain*|wc -l`

ruby sum_row.rb $LOGDIR/interrupts%d.csv $LOGDIR/merged_interrupts interrupts%d.csv $NCPUS
ruby sum_cpus.rb $LOGDIR/merged_interrupts%d.csv $LOGDIR/sum_merged_interrupts.csv $NCPUS
ruby sum_col.rb $LOGDIR/sum_merged_interrupts.csv $LOGDIR/sumall_merged_interrupts.csv
ruby bias_cpus.rb $LOGDIR/merged_interrupts%d.csv $LOGDIR/bias_merged_interrupts.csv $NCPUS
ruby sum_col.rb $LOGDIR/bias_merged_interrupts.csv $LOGDIR/biasall_merged_interrupts.csv

ruby sum_col.rb $LOGDIR/netstat_ipext.csv $LOGDIR/sumall_netstat_ipext.csv
ruby sum_col.rb $LOGDIR/netstat_tcpext.csv $LOGDIR/sumall_netstat_tcpext.csv

ruby sum_col.rb $LOGDIR/proc_sched_a.csv $LOGDIR/sumall_proc_sched_a.csv
ruby sum_col.rb $LOGDIR/proc_sched_b.csv $LOGDIR/sumall_proc_sched_b.csv
ruby sum_col.rb $LOGDIR/proc_sched_c.csv $LOGDIR/sumall_proc_sched_c.csv

ruby sum_cpus.rb $LOGDIR/schedstat_cpu%d.csv $LOGDIR/sum_schedstat_cpu.csv $NCPUS
ruby sum_col.rb $LOGDIR/sum_schedstat_cpu.csv $LOGDIR/sumall_schedstat_cpu.csv
ruby bias_cpus.rb $LOGDIR/schedstat_cpu%d.csv $LOGDIR/bias_schedstat_cpu.csv $NCPUS
ruby sum_col.rb $LOGDIR/bias_schedstat_cpu.csv $LOGDIR/biasall_schedstat_cpu.csv

for i in `seq 0 $(($NCPUS - 1))`; do
	ruby sum_cpus.rb $LOGDIR/schedstat_cpu${i}-domain%d.csv $LOGDIR/schedstat_domain${i}.csv $NDOMAINS
done
ruby sum_cpus.rb $LOGDIR/schedstat_domain%d.csv $LOGDIR/sum_schedstat_domain.csv $NCPUS
ruby sum_col.rb $LOGDIR/sum_schedstat_domain.csv $LOGDIR/sumall_schedstat_domain.csv
ruby bias_cpus.rb $LOGDIR/schedstat_domain%d.csv $LOGDIR/bias_schedstat_domain.csv $NCPUS
ruby sum_col.rb $LOGDIR/bias_schedstat_domain.csv $LOGDIR/biasall_schedstat_domain.csv


for i in `seq 0 $(($NDOMAINS - 1))`; do
	ruby sum_cpus.rb $LOGDIR/schedstat_cpu%d-domain${i}.csv $LOGDIR/sum_schedstat_domain${i}.csv $NCPUS
	ruby sum_col.rb $LOGDIR/sum_schedstat_domain${i}.csv $LOGDIR/sumall_schedstat_domain${i}.csv
	ruby bias_cpus.rb $LOGDIR/schedstat_cpu%d-domain${i}.csv $LOGDIR/bias_schedstat_domain${i}.csv $NCPUS
	ruby sum_col.rb $LOGDIR/bias_schedstat_domain${i}.csv $LOGDIR/biasall_schedstat_domain${i}.csv
done

ruby sum_cpus.rb $LOGDIR/stat%d.csv $LOGDIR/sum_stat.csv $NCPUS
ruby sum_col.rb $LOGDIR/sum_stat.csv $LOGDIR/sumall_stat.csv
ruby bias_cpus.rb $LOGDIR/stat%d.csv $LOGDIR/bias_stat.csv $NCPUS
ruby sum_col.rb $LOGDIR/bias_stat.csv $LOGDIR/biasall_stat.csv
