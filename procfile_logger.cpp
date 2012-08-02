#include <boost/format.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>

#include "procfile_logger.hpp"

using namespace std;
using namespace boost;
using namespace boost::algorithm;

const vector<string> EMPTY_VECTOR;

procfile_logger::procfile_logger(
	const std::string& procfn, const std::string& logfn_fmt,
	const int _start_row, const int _num_rows, 
	const int _start_col, const int _num_cols, 
	const std::string& header,
	const std::vector<std::string>& names,
	const bool _incremental,
	const enum procfile_direction _dir, const std::string& _delim,
	const bool multilog) 
	throw(num_cols_too_large, num_rows_too_large) :
	procfile(procfn),
	start_row(_start_row), num_rows(_num_rows),
	start_col(_start_col), num_cols(_num_cols),
	incremental(_incremental), dir(_dir), delim(_delim),
	started(false), 
	logfiles(dir == PF_DIRECTION_ROW ? num_rows : num_cols),
	prev(num_rows)
{
	int x;

	if (incremental) {
		for (int i = 0; i < num_rows; i++) 
			prev[i].reserve(num_cols);
	}
	x = dir == PF_DIRECTION_ROW ? num_rows : num_cols; 
	if (x > 1 && !multilog) {
		if (dir == PF_DIRECTION_ROW)
			throw num_rows_too_large();
		else
			throw num_cols_too_large();
	}

	if (!multilog) {
		logfiles[0].open(logfn_fmt);
		logfiles[0] << header << endl;
	}else{
		for (int i = 0; i < x; i++) {
			string filename;
			if (names == EMPTY_VECTOR)
				filename = (format(logfn_fmt) % i).str();
			else
				filename = (format(logfn_fmt) % names[i]).str();
			logfiles[i].open(filename);
			logfiles[i] << header << endl;
		}
	}
}
		
void procfile_logger::update() throw(num_cols_too_large, num_rows_too_large)
{
	int x;

	if (!incremental && !started)
		return;
	
	procfile.seekg(0, ios::beg);
	for (int i = 0; i < start_row; i++) {
		string line;
		getline(procfile, line);
	}

	for (int i = 0; i < num_rows; i++) {
		string line;
		vector<string> cols;

		getline(procfile, line);
		trim_all(line);
		split(cols, line, is_any_of(delim));
		if (cols.size() < (start_col + num_cols - 1)) {
			throw num_cols_too_large();
		}

		for (int j = 0; j < num_cols; j++) {
			long val = stol(cols[start_col + j]);
			if (started) {
				if (incremental) {
					if (dir == PF_DIRECTION_ROW)
						logfiles[i] << val - prev[i][j] << ",";
					else
						logfiles[j] << val - prev[i][j] << ",";
				}else{
					if (dir == PF_DIRECTION_ROW)
						logfiles[i] << val << ",";
					else
						logfiles[j] << val << ",";
				}
			}
			if (incremental)
				prev[i][j] = val;
		}
		if (started && dir == PF_DIRECTION_ROW)
			logfiles[i] << endl;
	}
	if (started && dir == PF_DIRECTION_COL)
		for (int j = 0; j < num_cols; j++) 
			logfiles[j] << endl;
}

void procfile_logger::start()
{
	started = true;
}

