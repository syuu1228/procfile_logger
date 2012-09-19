#ifndef PROC_NPROCS_HPP
#define PROC_NPROCS_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include "local_stats_logger.hpp"

class proc_nprocs : public local_stats_logger {
public:
	proc_nprocs(const std::string &logfn);
	virtual void handle_process(boost::filesystem::directory_iterator it);
	virtual void update();
	~proc_nprocs();
private:
	std::ofstream logfile;
	int curr;
};

void init_proc_nprocs_logger(boost::program_options::variables_map& vm,
	std::vector<std::shared_ptr<local_stats_logger> >& loggers);

#endif

