#include <cstdio>
#include <cstdlib>
#include <vector>

int main() {
  int n = 50;
  int range = 5;
  std::vector<int> key(n);
  for (int i=0; i<n; i++) {
    key[i] = rand() % range;
    printf("%d ",key[i]);
  }
  printf("\n");


__global__ void bucket(int *n, int &key) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
}
int main() {
  const int N=128;
  int *n;
  cudaMallocManaged(&n, N*sizeof(int));
  for (int i=0; i<range; i++) {
    bucket[i] = 0;
  }
  for (int i=0; i<n; i++) {
    bucket[key[i]]++;
  }
  for (int i=0, j=0; i<range; i++) {
    for (; bucket[i]>0; bucket[i]--) {
      key[j++] = i;
    }
  }
  bucket<<<1,N>>>(key, n);
  cudaDeviceSynchronize();
  for (int i=0; i<n; i++) {
    printf("%d ",key[i]);
  }
  printf("\n");
  cudaFree(n);
}  
 
