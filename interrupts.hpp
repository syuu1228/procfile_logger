#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <memory>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"

std::shared_ptr<procfile_logger> init_interrupts_logger(boost::program_options::variables_map& vm);

#endif
