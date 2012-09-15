#ifndef CORETEMP_HPP
#define CORETEMP_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include "global_stats_logger.hpp"

void init_coretemp_logger(boost::program_options::variables_map& vm,
	std::vector<std::shared_ptr<global_stats_logger> >& loggers);

#endif
