#include <immintrin.h>

#include <iostream>

void dump_vector(__m256 v) {
  std::cerr << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << ",";
  std::cerr << v[4] << "," << v[5] << "," << v[6] << "," << v[7] << std::endl;
}
void dump_vector(__m128 v) {
  std::cerr << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << std::endl;
}
float dot_product(const int N, const float *X, const int incX, const float *Y,
                  const int incY) {
  __m256 accum = _mm256_setzero_ps();
  for (int i = 0; i < N; i += 8, X += 8 * incX, Y += 8 * incY) {
    __m256 xval = _mm256_load_ps(X);
    __m256 yval = _mm256_load_ps(Y);
    __m256 val = _mm256_mul_ps(xval, yval);
    accum = _mm256_add_ps(val, accum);
  }
  // Reduce the values in accum into the smallest 32-bit subsection
  // a0 a1 a2 a3 a4 a5 a6 a7 -> b0 b1 0 0 b2 b3 0 0
  accum = _mm256_hadd_ps(accum, _mm256_setzero_ps());
  // b0 b1 0 0 b2 b3 0 0 -> c0 0 c1 0
  __m128 accum2 = _mm_hadd_ps(_mm256_castps256_ps128(accum),
    _mm256_extractf128_ps(accum, 1));
  // Add the high and low halves
  return accum2[0] + accum2[2];
}

#define good_file "dot-good.cpp"

#include "dot.cpp"
