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
    for(int j=0; j<N; j++) {
    __m256 xivec = _mm256_load_ps(&(x[i]));
    __m256 yivec = _mm256_load_ps(&(y[i]));
    __m256 mivec = _mm256_load_ps(&(m[i]));
    __m256 xjvec = _mm256_load_ps(&(x[j]));
    __m256 yjvec = _mm256_load_ps(&(y[j]));
    __m256 mjvec = _mm256_load_ps(&(m[j]));
    __m256 mask = _mm256_cmpneq_ps(i, j, CMPNEQPS);
    __m256 rxvec = _mm256_sub_ps(xivec, xjvec);
    __m256 ryvec = _mm256_sub_ps(yivec, yjvec);
    __m256 rvec = _mm256_rsqrt_ps(rxvec, ryvec);
    


      if(i != j) {
        float rx = x[i] - x[j];
        float ry = y[i] - y[j];
        float r = std::sqrt(rx * rx + ry * ry);
        fx[i] -= rx * m[j] / (r * r * r);
        fy[i] -= ry * m[j] / (r * r * r);
      }
    }
    printf("%d %g %g\n",i,fx[i],fy[i]);
  }
}
