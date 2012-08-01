#ifndef PROCFILE_LOGGER_HPP
#define PROCFILE_LOGGER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <exception>

enum procfile_direction {
	PF_DIRECTION_ROW,
	PF_DIRECTION_COL
};

class num_cols_too_large : std::exception {};
class num_rows_too_large : std::exception {};

class procfile_logger {
public:
	procfile_logger(const std::string procfn, const std::string logfn_fmt,
 		const int _start_row, const int _num_rows, 
		const int _start_col, const int _num_cols, 
		const std::string header = "", const bool _incremental = false,
		enum procfile_direction _dir = PF_DIRECTION_ROW, 
		const std::string _delim = " ", const bool multifile = true)
		throw(num_cols_too_large, num_rows_too_large);
	void update() throw(num_cols_too_large, num_rows_too_large);
	void start();
private:
	const int start_row, num_rows, start_col, num_cols;
	const bool incremental;
	bool started;
	enum procfile_direction dir;
	const std::string delim;
	std::ifstream procfile;
	std::vector<std::ofstream> logfiles;
	std::vector<std::vector<long> > prev;
};

#endif

