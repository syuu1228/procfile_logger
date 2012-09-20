#!/bin/sh

BASEDIR=$1
TARGETS=$2
TARGETS_ARRAY=`echo $2|sed -e "s/,/ /g"`

if [ "$BASEDIR" = "" -o "$TARGETS" = "" ];then
	echo "graph_generator.sh [basedir] [targets]"
	exit 1
fi

log_analyze()
{
	NAME=$1
	for i in $TARGETS_ARRAY
	do
		sh log_analyzer.sh $BASEDIR/$i/
	done
}

get_csv_array()
{
	NAME=$1
	for i in $TARGETS_ARRAY
	do
		echo $BASEDIR/$i/$NAME.csv
	done
}

get_csv_names()
{
	NAME=$1
	ARRAY=`get_csv_array $NAME`
	echo $ARRAY|sed -e "s/ /,/g"
}

graph_bar()
{
	NAME=$1
	ruby graph_bar.rb $NAME $BASEDIR/$NAME `get_csv_names $NAME` $TARGETS
}

graph_line()
{
	NAME=$1
	ruby graph_line.rb $NAME $BASEDIR/$NAME `get_csv_names $NAME` $TARGETS
}

percpu_graph_html()
{
	ruby percpu_graph_html.rb $1 > $BASEDIR/percpu_graph$1.html
}

sum_graph_html()
{
	cp sum_graph.html $BASEDIR/
}

bias_graph_html()
{
	cp bias_graph.html $BASEDIR/
}

NCPUS=`cat /proc/cpuinfo |grep processor|wc -l`
NDOMAINS=`grep domain /proc/schedstat |awk '{print $1}'|sort|uniq|wc -l`

log_analyze
sum_graph_html
bias_graph_html

for i in `seq 0 $(($NCPUS - 1))`; do
	graph_line stat$i
	graph_line merged_interrupts$i
	graph_line schedstat_cpu$i
	for j in `seq 0 $(($NDOMAINS - 1))`; do
		graph_line schedstat_cpu$i-domain$j
	done
	percpu_graph_html $i
done
graph_line sum_merged_interrupts
graph_line bias_merged_interrupts
graph_bar sumall_merged_interrupts
graph_bar biasall_merged_interrupts
graph_line netstat_ipext
graph_line netstat_tcpext
graph_bar sumall_netstat_ipext
graph_bar sumall_netstat_tcpext
graph_line proc_sched_a
graph_line proc_sched_b
graph_line proc_sched_c
graph_bar sumall_proc_sched_a
graph_bar sumall_proc_sched_b
graph_bar sumall_proc_sched_c
graph_line sum_schedstat_cpu
graph_line bias_schedstat_cpu
graph_bar sumall_schedstat_cpu
graph_bar biasall_schedstat_cpu
graph_line sum_schedstat_domain
graph_line bias_schedstat_domain
graph_bar sumall_schedstat_domain
graph_bar biasall_schedstat_domain
for i in `seq 0 $(($NDOMAINS - 1))`; do
	graph_line sum_schedstat_domain$i
	graph_line bias_schedstat_domain$i
	graph_bar sumall_schedstat_domain$i
	graph_bar biasall_schedstat_domain$i
done
graph_line sum_stat
graph_line bias_stat
graph_bar sumall_stat
graph_bar biasall_stat
