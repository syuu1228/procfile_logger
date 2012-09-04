CXXFLAGS=-std=c++0x -g
CXX=g++

all: procfile_logger
clean:
	rm -fv *.o procfile_logger

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

procfile_logger: global_stats_logger.o main.o interrupts.o netstat.o stat.o schedstat.o proc_sched.o proc_nprocs.o timer_runner.o
	$(CXX) -o $@ $^ -lboost_filesystem -lboost_system -lboost_program_options
