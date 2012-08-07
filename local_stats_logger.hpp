#ifndef LOCAL_STATS_LOGGER_HPP
#define LOCAL_STATS_LOGGER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <boost/filesystem.hpp>

class local_stats_logger {
public:
	virtual void handle_process(boost::filesystem::directory_iterator it) = 0;
	virtual void update() = 0;
	void inline start() {
		started = true;
	}
private:
	bool started;
};

#endif

