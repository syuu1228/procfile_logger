#include <unistd.h>
#include <string>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "procfile_logger.hpp"

using namespace std;
using namespace boost::program_options;
using namespace boost::algorithm;

int main(int argc, char **argv)
{
	options_description opt("options");
	opt.add_options()
		("help,h", "show help")
		("output,o",
			value<string>()->default_value("."), "output dir")
		("duration,d", 
			value<int>()->default_value(1), "duration sec")
		("terminate,t", 
			value<int>()->default_value(60), "terminate sec");
	variables_map vm;
	store(parse_command_line(argc, argv, opt), vm);
	notify(vm);

	if (vm.count("help")) {
		cout << opt << endl;
		return 1;
	}else{
		int nevents = 0;
		string header;
		{
			ifstream interrupts("/proc/net/netstat");
			string line;
			vector<string> cols;
			getline(interrupts, line);
			split(cols, line, is_any_of(" "));
			for (int i = 1; i < cols.size(); i++) {
				header += cols[i] + ",";
				nevents++;
			}
		}

		procfile_logger logger(
			"/proc/net/netstat",
			vm["output"].as<string>() + "/netstat_tcpext.log",
			1,
			1,
			1,
			nevents,
			header,
			true,
			PF_DIRECTION_ROW,
			" ",
			false
		);
		for (int i = 0; i <= vm["terminate"].as<int>(); i++) {
			logger.update();
			if (i == 0)
			logger.start();
			sleep(vm["duration"].as<int>());
		}
	}

	return 0;
}
