#include <unistd.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "schedstat.hpp"

using namespace std;
using namespace boost::algorithm;
using namespace boost::program_options;

static void init_schedstat_cpu_logger(variables_map& vm,
	vector<shared_ptr<global_stats_logger> >& loggers, int offset, string name)
{
	string output = vm["output"].as<string>();
	vector<string> names;
	names.push_back(name);
	shared_ptr<global_stats_logger> logger(
		new global_stats_logger(
			"/proc/schedstat",
			output + "/schedstat_%s.csv",
			offset,
			1,
			1,
			9,
			"yld_count,zero,sched_count,sched_goidle,ttwu_count,ttwu_local,rq_cpu_time,rq_sched_info.run_delay,rq_sched_info.pcount",
			names,
			true
		));
	loggers.push_back(logger);
}

static void init_schedstat_domain_logger(variables_map& vm,
	vector<shared_ptr<global_stats_logger> >& loggers, int offset, string name)
{
	string output = vm["output"].as<string>();
	vector<string> names;
	names.push_back(name);
	shared_ptr<global_stats_logger> logger(
		new global_stats_logger(
			"/proc/schedstat",
			output + "/schedstat_%s.csv",
			offset,
			1,
			2,
			36,
			"lb_count[CPU_IDLE],lb_balanced[CPU_IDLE],lb_failed[CPU_IDLE],lb_imbalance[CPU_IDLE],lb_gained[CPU_IDLE],lb_hot_gained[CPU_IDLE],lb_nobusyq[CPU_IDLE],lb_nobusyg[CPU_IDLE],"
			"lb_count[CPU_NOT_IDLE],lb_balanced[CPU_NOT_IDLE],lb_failed[CPU_NOT_IDLE],lb_imbalance[CPU_NOT_IDLE],lb_gained[CPU_NOT_IDLE],lb_hot_gained[CPU_NOT_IDLE],lb_nobusyq[CPU_NOT_IDLE],lb_nobusyg[CPU_NOT_IDLE],"
			"lb_count[CPU_NEWLY_IDLE],lb_balanced[CPU_NEWLY_IDLE],lb_failed[CPU_NEWLY_IDLE],lb_imbalance[CPU_NEWLY_IDLE],lb_gained[CPU_NEWLY_IDLE],lb_hot_gained[CPU_NEWLY_IDLE],lb_nobusyq[CPU_NEWLY_IDLE],lb_nobusyg[CPU_NEWLY_IDLE],"
			"alb_count,alb_failed,alb_pushed,sbe_count,sbe_balanced,sbe_pushed,sbf_count,sbf_balanced,sbf_pushed,ttwu_wake_remote,ttwu_move_affine,ttwu_move_balance",
			names,
			true
		));
	loggers.push_back(logger);
}


void init_schedstat_logger(variables_map& vm,
	vector<shared_ptr<global_stats_logger> >& logger)
{
	ifstream schedstat("/proc/schedstat");
	string line;
	string cpu;
	vector<string> cols;
	int i = 2;
	getline(schedstat, line);
	getline(schedstat, line);
	while(getline(schedstat, line)) {
		vector<string> cols;
		split(cols, line, is_any_of(" "));
		if (cols[0].compare(0, 3, "cpu") == 0) {
			init_schedstat_cpu_logger(vm, logger, i, cols[0]);
			cpu = cols[0];
		} else if (cols[0].compare(0, 6, "domain") == 0) {
			init_schedstat_domain_logger(vm, logger, i, cpu + "-" + cols[0]);
		}
		i++;
	}
}

