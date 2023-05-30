#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <immintrin.h>

int main() {
  const int N = 8;
  const int vectorSize = 8;
  const int iterations = N / vectorSize;
  float x[N], y[N], m[N], fx[N], fy[N];
  for(int i = 0; i < N; i++) {
    x[i] = drand48();
    y[i] = drand48();
    m[i] = drand48();
    fx[i] = fy[i] = 0;
  }
  
  for (int i = 0; i < iterations; i++) {
    __m256 fx_vec = _mm256_setzero_ps();
    __m256 fy_vec = _mm256_setzero_ps();
    __m256 x_vec = _mm256_load_ps(&x[i * vectorSize]);
    __m256 y_vec = _mm256_load_ps(&y[i * vectorSize]);
    __m256 m_vec = _mm256_load_ps(&m[i * vectorSize]);

    for (int j = 0; j < N; j += vectorSize) {
      __m256i mask = _mm256_set1_epi32(-1);
      mask = _mm256_andnot_si256(_mm256_cmpeq_epi32(_mm256_set1_epi32(i), _mm256_set1_epi32(j / vectorSize)), mask);

      __m256 xj_vec = _mm256_maskload_ps(&x[j], mask);
      __m256 yj_vec = _mm256_maskload_ps(&y[j], mask);

      __m256 rx_vec = _mm256_sub_ps(x_vec, xj_vec);
      __m256 ry_vec = _mm256_sub_ps(y_vec, yj_vec);

      __m256 rx2_vec = _mm256_mul_ps(rx_vec, rx_vec);
      __m256 ry2_vec = _mm256_mul_ps(ry_vec, ry_vec);

      __m256 r2_vec = _mm256_add_ps(rx2_vec, ry2_vec);
      __m256 r_vec = _mm256_rsqrt_ps(r2_vec);
      __m256 r3_vec = _mm256_mul_ps(r_vec, _mm256_mul_ps(r2_vec, r_vec));

      __m256 rxm_vec = _mm256_mul_ps(m_vec, rx_vec);
      __m256 fxj_vec = _mm256_mul_ps(rxm_vec, _mm256_mul_ps(r3_vec, r_vec));
      __m256 fyj_vec = _mm256_mul_ps(_mm256_mul_ps(m_vec, ry_vec), _mm256_mul_ps(r3_vec, r_vec));

      fx_vec = _mm256_masksub_ps(fx_vec, mask, fxj_vec);
      fy_vec = _mm256_masksub_ps(fy_vec, mask, fyj_vec);
    }

    _mm256_store_ps(&fx[i * vectorSize], fx_vec);
    _mm256_store_ps(&fy[i * vectorSize], fy_vec);
  }

  for (int i = 0; i < N; i++) {
    printf("%d %g %g\n", i, fx[i], fy[i]);
  }

  return 0;
}
