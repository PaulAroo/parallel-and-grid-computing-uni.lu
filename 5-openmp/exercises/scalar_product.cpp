#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

int scalar_product(const std::vector<int>& a, const std::vector<int>& b) {
  assert(a.size() == b.size());
  int s = 0;
  for(int i = 0; i < a.size(); ++i) {
    s += a[i] * b[i];
  }
  return s;
}

int scalar_product_parallel(const std::vector<int>& a, const std::vector<int>& b) {
  assert(a.size() == b.size());
  int s = 0;
  // int globalsum = 0;
  #pragma omp parallel for reduction(+:s)
  for(int i = 0; i < a.size(); ++i) {
    // #pragma omp atomic
    s += a[i] * b[i];
  }
  return s;
}

// int scalar_product_parallel(const std::vector<int>& a, const std::vector<int>& b) {
//   assert(a.size() == b.size());
//   int s = 0;
//   int globalsum = 0;
//   #pragma omp parallel firstprivate(s)
//   {
//     #pragma omp for schedule(guided)
//     for(int i = 0; i < a.size(); ++i) {
//       // #pragma omp atomic
//       s += a[i] * b[i];
//     }

//     #pragma omp atomic
//     globalsum += s;
//   }
//   return globalsum;
// }

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{0, 1};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

int main() {
  std::vector<int> a = init_random_vector(50000000);
  std::vector<int> b = init_random_vector(50000000);
  auto s_start = std::chrono::steady_clock::now();
  int result = scalar_product(a, b);
  auto s_end = std::chrono::steady_clock::now();
  std::cout << "Sequential Result: " << result << std::endl;
  std::cout << "time = " << std::chrono::duration_cast<std::chrono::milliseconds>(s_end - s_start).count() << "ms\n";

  auto start = std::chrono::steady_clock::now();
  int p_result = scalar_product_parallel(a, b);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Parallel Result: " << p_result << std::endl;
  std::cout << "time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
}
