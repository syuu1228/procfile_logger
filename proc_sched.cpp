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
	std::shared_ptr<local_stats_logger> logger(
		new proc_sched(
			output + "/proc_sched.log"
		));
	loggers.push_back(logger);
}

proc_sched::proc_sched(const std::string &logfn) : 
	curr(23), prev(23), logfile(logfn)
{
	ifstream schedfile("/proc/self/sched");

	for (int i = 0; i < 17; i++) {
		string line;
		getline(schedfile, line);
	}

	for (int i = 0; i < curr.size(); i++) {
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

	for (int i = 0; i < 17; i++) {
		string line;
		getline(schedfile, line);
	}

	for (int i = 0; i < curr.size(); i++) {
		string line;
		vector<string> cols;
		getline(schedfile, line);
		trim_all(line);
		split(cols, line, is_any_of(":"));
		curr[i] += stol(cols[1]);
	}
}

void proc_sched::update()
{
	for (int i = 0; i < curr.size(); i++) {
		logfile << curr[i] - prev[i] << ",";
		prev[i] = curr[i];
	}
	logfile << endl;
}
