#include <boost/filesystem.hpp>
#include "procwatch_runner.hpp"

using namespace std;
using namespace boost;
using namespace boost::program_options;

procwatch_runner::procwatch_runner(
	vector<std::shared_ptr<procfile_logger> >& _loggers, variables_map& vm,
	const std::string _comm, const int _threshold) :
	loggers(_loggers),
	comm(_comm),
	duration(vm["duration"].as<int>()), 
	terminate(vm["terminate"].as<int>()),
	threshold(_threshold)
{
};

void procwatch_runner::run()
{
	bool started = false;

	while (!started) {
		for (auto l : loggers) 
			l->update();
		if (iterate_proc() > threshold) {
			started = true;
			for (auto l : loggers) 
				l->start();
		}
		sleep(duration);
	}

	for (int i = 0;
		terminate != 0 ? i <= terminate : true; i++) {
		if (iterate_proc() == threshold)
			break;
		for (auto l : loggers)
			l->update();
		sleep(duration);
	}
}

int procwatch_runner::iterate_proc()
{
	filesystem::path proc("/proc");
	filesystem::directory_iterator end;
	int nprocs = 0;

	for (filesystem::directory_iterator it(proc);
		it != end; ++it) {
		const string filename = it->path().filename().string();
		bool is_numeric = false;
		try {
			stoi(filename);
			is_numeric = true;
		} catch (invalid_argument) {}
	
		if (filesystem::is_directory(*it) && is_numeric) {
			ifstream comm_file(it->path().string() + "/comm");
			string val;
			comm_file >> val;
			if (val != comm)
				continue;
			++nprocs;
		}
	}

	return nprocs;
}


