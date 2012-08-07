CXXFLAGS=-std=c++0x -g
CXX=clang++

all: procfile_logger
clean:
	rm -fv *.o procfile_logger

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

procfile_logger: global_stats_logger.o main.o interrupts.o netstat.o stat.o schedstat.o timer_runner.o
	$(CXX) -o $@ $^ -lboost_system -lboost_program_options
