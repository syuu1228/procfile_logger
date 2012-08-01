#ifndef NETSTAT_IPEXT_HPP
#define NETSTAT_IPEXT_HPP

#include <memory>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"

std::shared_ptr<procfile_logger> init_netstat_ipext_logger(boost::program_options::variables_map& vm);

#endif
