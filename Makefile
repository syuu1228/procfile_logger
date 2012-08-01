CXXFLAGS=-std=c++0x -g
CXX=clang++

all: stat_logger interrupts_logger netstat_tcpext_logger netstat_ipext_logger
clean:
	rm -fv *.o *_logger

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

stat_logger: procfile_logger.o stat.o
	$(CXX) -o $@ $^ -lboost_filesystem -lboost_system -lboost_program_options

netstat_tcpext_logger: procfile_logger.o netstat_tcpext.o
	$(CXX) -o $@ $^ -lboost_filesystem -lboost_system -lboost_program_options

netstat_ipext_logger: procfile_logger.o netstat_ipext.o
	$(CXX) -o $@ $^ -lboost_filesystem -lboost_system -lboost_program_options

interrupts_logger: procfile_logger.o interrupts.o
	$(CXX) -o $@ $^ -lboost_filesystem -lboost_system -lboost_program_options
