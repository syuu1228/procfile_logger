#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "proc_sched.hpp"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::algorithm;
using namespace boost::program_options;

void init_proc_sched_logger(variables_map& vm,
	vector<std::shared_ptr<local_stats_logger> >& loggers)
{
	string output = vm["output"].as<string>();
	std::shared_ptr<local_stats_logger> l1(
		new proc_sched(
			output + "/proc_sched_a.log",
			17, 15
		));
	std::shared_ptr<local_stats_logger> l2(
		new proc_sched(
			output + "/proc_sched_b.log",
			34, 3
		));
	std::shared_ptr<local_stats_logger> l3(
		new proc_sched(
			output + "/proc_sched_c.log",
			37, 4, false
		));

	loggers.push_back(l1);
	loggers.push_back(l2);
	loggers.push_back(l3);
}

proc_sched::proc_sched(
	const std::string &logfn, const int _start_row, const int _num_rows,
	const bool _incremental) : 
	curr(num_rows), prev(num_rows), logfile(logfn), 
	start_row(_start_row), num_rows(_num_rows), incremental(_incremental)
{
	ifstream schedfile("/proc/self/sched");

	for (int i = 0; i < start_row; i++) {
		string line;
		getline(schedfile, line);
	}

	for (int i = 0; i < num_rows; i++) {
		vector<string> cols;
		string line;
		getline(schedfile, line);
		trim_all(line);
		split(cols, line, is_any_of(":"));
		logfile << cols[0] + ",";
	}
	logfile << endl;
}

void proc_sched::handle_process(directory_iterator it)
{
	ifstream schedfile(it->path().string() + "/sched");

	for (int i = 0; i < start_row; i++) {
		string line;
		getline(schedfile, line);
	}

	for (int i = 0; i < num_rows; i++) {
		string line;
		vector<string> cols;
		getline(schedfile, line);
		trim_all(line);
		split(cols, line, is_any_of(":"));
		curr[i] = stol(cols[1]);
	}
}

void proc_sched::update()
{
	for (int i = 0; i < curr.size(); i++) {
		if (incremental) {
			logfile << curr[i] - prev[i] << ",";
			prev[i] = curr[i];
		}else{
			logfile << curr[i] << ",";
		}
	}
	logfile << endl;
}
