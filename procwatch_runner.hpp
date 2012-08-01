#ifndef PROCWATCH_RUNNER_HPP
#define PROCWATCH_RUNNER_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"

class procwatch_runner {
public:
	procwatch_runner(std::vector<std::shared_ptr<procfile_logger> >& _loggers,
		boost::program_options::variables_map& vm,
		const std::string _comm, const int _threshold);

	void run();

private:
	int iterate_proc();
	const std::vector<std::shared_ptr<procfile_logger> >& loggers;
	const std::string comm;
	const int duration, terminate, threshold;
};

#endif
