#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <immintrin.h>

int main() {
  const int N = 8;
  const int vectorSize = 8;
  const int iterations = N / vectorSize;
  float x[N], y[N], m[N], fx[N], fy[N];
  for(int i=0; i<N; i++) {
    x[i] = drand48();
    y[i] = drand48();
    m[i] = drand48();
    fx[i] = fy[i] = 0;
  }
  
  __m256 rx_vec, ry_vec, r_vec, m_vec, rx2_vec, ry2_vec, r3_vec;
  __m256 fx_vec, fy_vec;
 
  for (int i = 0; i < iterations; i++) {
    fx_vec = _mm256_setzero_ps();
    fy_vec = _mm256_setzero_ps();

    // Load x, y, and m vectors outside the loop
    __m256 x_vec = _mm256_load_ps(&x[i * vectorSize]);
    __m256 y_vec = _mm256_load_ps(&y[i * vectorSize]);
    __m256 m_vec = _mm256_load_ps(&m[i * vectorSize]);

    for (int j = 0; j < N; j += vectorSize) {
      // Calculate the mask vector
      int mask = (i == (j / vectorSize)) ? -1 : 0;

      __m256i mask_vec = _mm256_set1_epi32(mask);

      // Load x[j], y[j] vectors using the mask
      __m256 xj_vec = _mm256_maskload_ps(&x[j], mask_vec);
      __m256 yj_vec = _mm256_maskload_ps(&y[j], mask_vec);

      // Calculate differences
      rx_vec = _mm256_sub_ps(x_vec, xj_vec);
      ry_vec = _mm256_sub_ps(y_vec, yj_vec);

      rx2_vec = _mm256_mul_ps(rx_vec, rx_vec);
      ry2_vec = _mm256_mul_ps(ry_vec, ry_vec);

      r_vec = _mm256_rsqrt_ps(_mm256_add_ps(rx2_vec, ry2_vec));
      r3_vec = _mm256_mul_ps(r_vec, _mm256_mul_ps(r_vec, r_vec));

      // Multiply rx, ry with m[j] and divide by r^3 using the mask
      __m256 rxm_vec = _mm256_and_ps(m_vec, _mm256_castsi256_ps(mask_vec)) * rx_vec;
      __m256 r3inv_vec = _mm256_div_ps(_mm256_set1_ps(1.0f), r3_vec);
      fx_vec -= _mm256_and_ps(m_vec, _mm256_castsi256_ps(mask_vec)) * (rxm_vec * r3inv_vec);
      fy_vec -= _mm256_and_ps(_mm256_mul_ps(ry_vec, _mm256_and_ps(m_vec, _mm256_castsi256_ps(mask_vec))), r3inv_vec);
    }

    _mm256_store_ps(&fx[i * vectorSize], fx_vec);
    _mm256_store_ps(&fy[i * vectorSize], fy_vec);
  }

  for (int i = 0; i < N; i++) {
    printf("%d %g %g\n", i, fx[i], fy[i]);
  }

  return 0;
}   
