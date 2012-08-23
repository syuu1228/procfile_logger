#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "proc_nprocs.hpp"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::algorithm;
using namespace boost::program_options;

void init_proc_nprocs_logger(variables_map& vm,
	vector<std::shared_ptr<local_stats_logger> >& loggers)
{
	string output = vm["output"].as<string>();
	std::shared_ptr<local_stats_logger> logger(
		new proc_nprocs(
			output + "/proc_nprocs.log"
		));
	loggers.push_back(logger);
}

proc_nprocs::proc_nprocs(const std::string &logfn) : 
	curr(0), logfile(logfn)
{
	logfile << "nprocs" << endl;
}

void proc_nprocs::handle_process(directory_iterator it)
{
	++curr;
}

void proc_nprocs::update()
{
	if (!started)
		return;

	logfile << curr << endl;
	curr = 0;
}
