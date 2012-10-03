#ifndef TIMER_RUNNER_HPP
#define TIMER_RUNNER_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include "global_stats_logger.hpp"
#include "local_stats_logger.hpp"

class no_logger_registered : std::exception {};
class sighandler_register_error : std::exception {};

class timer_runner {
public:
	timer_runner(
		std::vector<std::shared_ptr<global_stats_logger> >& _gloggers,
		std::vector<std::shared_ptr<local_stats_logger> >& _lloggers,
		std::vector<std::shared_ptr<boost::thread> >& _threads,
		boost::program_options::variables_map& vm);

	void run() throw(no_logger_registered, sighandler_register_error)
;

private:
	const std::vector<std::shared_ptr<global_stats_logger> >& gloggers;
	const std::vector<std::shared_ptr<local_stats_logger> >& lloggers;
	const std::vector<std::shared_ptr<boost::thread> >& threads;
	const int duration, terminate;
	const std::string comm;
};

#endif
