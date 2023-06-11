#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <immintrin.h>

int main() {
  const int N = 8;
  float x[N], y[N], m[N], fx[N], fy[N];
  for(int i=0; i<N; i++) {
    x[i] = drand48();
    y[i] = drand48();
    m[i] = drand48();
    fx[i] = fy[i] = 0;
  }
  for(int i=0; i<N; i++) {
    __m256 rx_vec = _mm256_set1_ps(x[i]);
    __m256 ry_vec = _mm256_set1_ps(y[i]);
    __m256 fx_vec = _mm256_set1_ps(fx[i]);
    __m256 fy_vec = _mm256_set1_ps(fy[i]);

    for(int j=0; j<N; j+=8) {
      __m256 xj_vec = _mm256_load_ps(&x[j]);
      __m256 yj_vec = _mm256_load_ps(&y[j]);
      __m256 mj_vec = _mm256_load_ps(&m[j]);

      __m256 mask = _mm256_castsi256_ps(_mm256_cmp_epi32(_mm256_set1_epi32(i), _mm256_set_epi32(j+7, j+6, j+5, j+4, j+3, j+2, j+1, j), _MM_CMPINT_NE));

      __m256 rx_diff = _mm256_sub_ps(rx_vec, xj_vec);
      __m256 ry_diff = _mm256_sub_ps(ry_vec, yj_vec);

      __m256 r2_vec = _mm256_add_ps(_mm256_mul_ps(rx_diff, rx_diff), _mm256_mul_ps(ry_diff, ry_diff));
      __m256 r_vec = _mm256_rsqrt_ps(r2_vec);

      __m256 r3_vec = _mm256_mul_ps(r_vec, _mm256_mul_ps(r2_vec, r2_vec));

      __m256 factor_vec = _mm256_div_ps(_mm256_mul_ps(_mm256_div_ps(mj_vec, r3_vec), mask), r_vec);

      fx_vec = _mm256_sub_ps(fx_vec, _mm256_mul_ps(rx_diff, factor_vec));
      fy_vec = _mm256_sub_ps(fy_vec, _mm256_mul_ps(ry_diff, factor_vec));
    }

    _mm256_store_ps(&fx[i], fx_vec);
    _mm256_store_ps(&fy[i], fy_vec);

    printf("%d %g %g\n", i, fx[i], fy[i]);
  }
}
