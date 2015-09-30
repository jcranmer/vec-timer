#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include <bitmask.h>
#include <cpuset.h>
#include <sched.h>

#include <stdlib.h>
#include <string.h>
#define stringify_helper(x) #x
#define abortOnFailure(call) printErrorMessage(call, stringify_helper(call))
void printErrorMessage(int error, const char *msg) {
  if (error == -1) {
    std::cerr << "Error when calling " << msg << ": " << strerror(errno)
      << std::endl;
    abort();
  }
}

#if 0
class AutoCpuset {
  cpuset *set;
  char buf[1024];
public:
  AutoCpuset();
  ~AutoCpuset();
};

#define CPUSET_NAME "testcpu"
AutoCpuset::AutoCpuset()
: set(cpuset_alloc()) {
  abortOnFailure(cpuset_create(CPUSET_NAME, set));
  abortOnFailure((intptr_t)cpuset_getcpusetpath(0, buf, sizeof(buf)) - 1);
  int ncpus = cpuset_cpus_nbits();
  bitmask *allowed = bitmask_alloc(ncpus);
  bitmask_clearall(allowed);
  bitmask_setbit(allowed, 0);
  bitmask_setbit(allowed, 1);
  abortOnFailure(cpuset_setcpus(set, allowed));
  bitmask_free(allowed);
  abortOnFailure(cpuset_move(0, buf));
}

AutoCpuset::~AutoCpuset()
{
  abortOnFailure(cpuset_move(0, buf));
  abortOnFailure(cpuset_delete(CPUSET_NAME));
  cpuset_free(set);
}
#endif

typedef std::chrono::high_resolution_clock ntime;

extern void init();
extern void func(bool);

static void test_func(bool verify) {
  asm("");
  func(verify);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " out.csv" << std::endl;
    return 0;
  }

  init();

  //AutoCpuset set;
  //cpu_set_t set;
  //int cpuid;
  //abortOnFailure(cpuid = cpuset_where());
  //std::cerr << "Running on CPU #" << cpuid << std::endl;
  //CPU_ZERO(&set); CPU_SET(cpuid, &set);
  //abortOnFailure(sched_setaffinity(getpid(), sizeof(cpu_set_t), &set));

  // Warm up the cache by checking the value as well
  for (int i = 0; i < 1000; i++) {
    test_func(true);
  }

  std::ofstream out(argv[1], std::ios_base::app);
  typedef std::chrono::duration<uint32_t, std::chrono::nanoseconds::period> ns;
  static std::array<ns, 1000> measurements;
  for (int i = 0; i < measurements.size(); i++) {
    ntime::time_point before = ntime::now();
    test_func(false);
    ntime::time_point after = ntime::now();
    measurements[i] = after - before;
    out << measurements[i].count() << std::endl;
  }

  // Sort the array
  std::sort(measurements.begin(), measurements.end());
  size_t count = measurements.size();
  auto start = measurements.begin();
  auto end = measurements.end();
  uint64_t sum = 0, sumsquared = 0;
  for (auto i = start; i < end; i++) {
    sum += i->count();
    sumsquared += uint64_t(i->count()) * i->count();
  }
  count = end - start;
  double mean = double(sum) / count;
  double variance = (sumsquared * count - sum * sum) /
    double(count * (count - 1));
  std::cout << "Took " << mean << "±" <<
    std::sqrt(variance) << "ns" << std::endl;
  return 0;
}
