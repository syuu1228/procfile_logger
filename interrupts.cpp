#include <unistd.h>
#include <string>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/classification.hpp>
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
		int ncpus = sysconf(_SC_NPROCESSORS_ONLN);
		int nintrs = 0;
		string header;
		{
			ifstream interrupts("/proc/interrupts");
			string line;
			getline(interrupts, line);
			while(getline(interrupts, line)) {
				vector<string> cols;
				trim_all(line);
				split(cols, line, is_any_of(" "));
				if (cols[0] == "ERR:")
					break;
				header += cols[0] + ",";
				nintrs++;
			}
		}
		procfile_logger logger(
			"/proc/interrupts",
			vm["output"].as<string>() + "/interrupts%d.log",
			1,
			nintrs,
			1,
			ncpus,
			header,
			true,
			PF_DIRECTION_COL
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
