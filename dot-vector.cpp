#include <immintrin.h>

#include <iostream> 
float dot_product(const int N, const float *X, const int incX, const float *Y,
                  const int incY) {
  //__m256 dot = _mm256_setzero_ps();
  float dot = 0;
  for (int i = 0; i < N; i += 8, X += 8 * incX, Y += 8 * incY) {
    __m256 xval = _mm256_load_ps(X);
    __m256 yval = _mm256_load_ps(Y);
    __m256 val = _mm256_dp_ps(xval, yval, 255);
    //__m256 val = xval * yval;
    //dot += val[0] + val[1] + val[2] + val[3] + val[4] + val[5] + val[6] + val[7];
    dot += val[0] + val[7];
  }
  return dot;
}

#define good_file "dot-good.cpp"

#include "dot.cpp"
