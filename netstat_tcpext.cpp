#include <unistd.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "netstat_tcpext.hpp"

using namespace std;
using namespace boost::algorithm;
using namespace boost::program_options;

shared_ptr<procfile_logger> init_netstat_tcpext_logger(variables_map& vm)
{
	string output = vm["output"].as<string>();
	int nevents = 0;
	string header;
	ifstream interrupts("/proc/net/netstat");
	string line;
	vector<string> cols;
	getline(interrupts, line);
	split(cols, line, is_any_of(" "));
	for (int i = 1; i < cols.size(); i++) {
		header += cols[i] + ",";
		nevents++;
	}
	shared_ptr<procfile_logger> logger(
		new procfile_logger(
			"/proc/net/netstat",
			output + "/netstat_tcpext.log",
			1,
			1,
			1,
			nevents,
			header,
			EMPTY_VECTOR,
			true,
			PF_DIRECTION_ROW,
			" ",
			false
		));
	return logger;
}
