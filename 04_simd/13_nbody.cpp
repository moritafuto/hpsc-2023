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
       __m256 fxvec = _mm256_load_ps(&(fx[i]));
       __m256 fyvec = _mm256_load_ps(&(fy[i]));
       __m256 mask = _mm_cmpneq_ps(i, j);
       __m256 rxvec = _mm256_sub_ps(xivec, xjvec);
       __m256 ryvec = _mm256_sub_ps(yivec, yjvec);
       __m256 invrvec = _mm256_rsqrt_ps(rxvec*rxvec+ryvec*ryvec);
       fxvec = _mm256_sub_ps(fxvec, rxvec * mjvec *invrvec * invrvec * invrvec);
       fyvec = _mm256_sub_ps(fyvec, ryvec * mjvec *invrvec * invrvec * invrvec);
       _mm256_store_ps(fx, fxvec);
       _mm256_store_ps(fy, fyvec);
       for(int i=0; i<N; i++)
        printf("%d %g %g\n",i,fx[i],fy[i]);
    }
   }
}
