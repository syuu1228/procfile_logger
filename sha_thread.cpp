#include <boost/format.hpp>
#include <fstream>
#include <sched.h>
#include "sha_thread.hpp"
#include "sha2.h"

using namespace std;
using namespace boost;
using namespace boost::program_options;

#define SHA_BUF_SIZE 1 * 1024 * 1024 /* 1MB */

void sha_thread_func(variables_map& vm, int cpu)
{
	cpu_set_t mask;
	SHA512_CTX ctx;
	int cnt = 0;
	char *buf = new char[SHA_BUF_SIZE];
	ifstream urandom("/dev/urandom");
	string output = vm["output"].as<string>();
	ofstream logfile((format("%s/sha%d.csv") % output % cpu).str());

	try {
		logfile << "nexecuted" << endl;
		CPU_ZERO(&mask);
		CPU_SET(cpu, &mask);
		if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
			perror("Failed to set CPU affinity");
			exit(-1);
		}
		urandom.read(buf, SHA_BUF_SIZE);
		urandom.close();
		SHA512_Init(&ctx);
		while (true) {
			SHA512_Update(&ctx, (const uint8_t *)buf, SHA_BUF_SIZE);
			SHA512_End(&ctx, buf);
			cnt++;
			this_thread::interruption_point();
		}
	} catch (thread_interrupted const&) {
		delete buf;
		logfile << cnt << endl;
		logfile.close();
	}
}

void init_sha_thread(variables_map& vm,
	vector<std::shared_ptr<thread> >& threads)
{
		int ncpus = sysconf(_SC_NPROCESSORS_ONLN);
		for (int i = 0; i < ncpus; i++) {
			std::shared_ptr<thread> t(
				new thread(bind(sha_thread_func, vm, i))
			);
			threads.push_back(t);
		}
}
