#include <immintrin.h>

#include <iostream>

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
  // a0 a1 a2 a3 a4 a5 a6 a7 -> b0 b1 b2 b3
  __m128 accum2 = _mm_add_ps(_mm256_castps256_ps128(accum),
      _mm256_extractf128_ps(accum, 1));
  // b0 b1 b2 b3 -> c0 c1 b2 b3
  accum2 = _mm_add_ps(accum2,
      _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(accum2), 8)));
  __m128 final_val = _mm_add_ss(
      _mm_insert_ps(accum2, accum2, 0x4e), accum2);
  // Add the high and low halves
  return final_val[0];
}

#define good_file "dot-good.cpp"

#include "dot.cpp"
