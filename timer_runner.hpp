#ifndef TIMER_RUNNER_HPP
#define TIMER_RUNNER_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"

class timer_runner {
public:
	timer_runner(std::vector<std::shared_ptr<procfile_logger> >& _loggers,
		boost::program_options::variables_map& vm);

	void run();

private:
	const std::vector<std::shared_ptr<procfile_logger> >& loggers;
	const int duration, terminate;
};

#endif
