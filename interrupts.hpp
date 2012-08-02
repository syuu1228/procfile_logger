#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"

void init_interrupts_logger(boost::program_options::variables_map& vm,
	std::vector<std::shared_ptr<procfile_logger> >& loggers);

#endif
