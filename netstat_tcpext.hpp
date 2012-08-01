#ifndef NETSTAT_TCPEXT_HPP
#define NETSTAT_TCPEXT_HPP

#include <memory>
#include <boost/program_options.hpp>
#include "procfile_logger.hpp"

std::shared_ptr<procfile_logger> init_netstat_tcpext_logger(boost::program_options::variables_map& vm);

#endif
