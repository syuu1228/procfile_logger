#ifndef PROC_SCHED_HPP
#define PROC_SCHED_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include "local_stats_logger.hpp"

class proc_sched : public local_stats_logger {
public:
	proc_sched(const std::string &logfn);
	virtual void handle_process(boost::filesystem::directory_iterator it);
	virtual void update();
private:
	std::ofstream logfile;
	std::vector<long> prev, curr;
};

void init_proc_sched_logger(boost::program_options::variables_map& vm,
	std::vector<std::shared_ptr<local_stats_logger> >& loggers);

#endif

