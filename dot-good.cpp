
float dot_product_good(const int N, const float *X, const int incX, const float *Y,
                  const int incY) __attribute__((__target__("arch=atom")));

float dot_product_good(const int N, const float *X, const int incX, const float *Y,
                  const int incY) {
  float dot = 0;
  for (int i = 0; i < N; i++) {
    dot += *X * *Y;
    X += incX;
    Y += incY;
  }
  return dot;
}

#ifndef good_file
#include "dot.cpp"
#endif
