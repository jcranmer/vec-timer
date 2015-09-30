#include <array>
#include <random>
#include <iostream>
#include <stdlib.h>

struct SparseMatrix {
  int N;
  float *numbers;
  int *
};

static std::array<float, 1048576> x;
static std::array<float, 1048576> y;

void init() {
  std::default_random_engine rng(0);
  std::uniform_real_distribution<float> distribution(0, 1);
  for (float &f : x) {
    f = distribution(rng);
  }
  for (float &f : y) {
    f = distribution(rng);
  }
}

#ifdef good_file
#include good_file
#else
#define dot_product dot_product_good
#endif

void func(bool verify) {
  float dot = dot_product(x.size(), &x[0], 1, &y[0], 1);
  if (verify) {
    float dot_good = dot_product_good(x.size(), &x[0], 1, &y[0], 1);
    if (std::abs(dot_good - dot) >= 1e-3 * dot) {
      std::cerr << std::abs(dot_good - dot) << std::endl;
      std::cerr << "Got " << dot << ", expected " << dot_good << std::endl;
      exit(1);
    }
  }
}
