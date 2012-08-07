#ifndef TIMER_RUNNER_HPP
#define TIMER_RUNNER_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include "global_stats_logger.hpp"

class no_logger_registered : std::exception {};

class timer_runner {
public:
	timer_runner(std::vector<std::shared_ptr<global_stats_logger> >& _loggers,
		boost::program_options::variables_map& vm);

	void run() throw(no_logger_registered);

private:
	const std::vector<std::shared_ptr<global_stats_logger> >& loggers;
	const int duration, terminate;
};

#endif
