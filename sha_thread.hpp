#ifndef SHA_THREAD_HPP
#define SHA_THREAD_HPP

#include <vector>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>

void init_sha_thread(boost::program_options::variables_map& vm,
	std::vector<std::shared_ptr<boost::thread> >& threads);

#endif
