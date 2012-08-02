CXXFLAGS=-std=c++0x -g
CXX=clang++

all: procfile_logger
clean:
	rm -fv *.o procfile_logger

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

procfile_logger: procfile_logger.o main.o interrupts.o netstat.o stat.o timer_runner.o procwatch_runner.o
	$(CXX) -o $@ $^ -lboost_filesystem -lboost_system -lboost_program_options
