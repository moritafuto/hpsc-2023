#include <iostream>

int main() {
#pragma omp parallel
  std::cout << “hello\n";
}
