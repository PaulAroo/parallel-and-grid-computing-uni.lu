#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

std::vector<int> filter_leq(std::vector<int> init_v, int k) {
  std::vector<int> v;
  for(int val : init_v ) {
    if(val <= k) {
      v.push_back(val);
    }
  }

  return std::move(v);
}

std::vector<int> filter_leq_parallel(std::vector<int> init_v, int k) {
  std::vector<int> mainv;
  std::vector<int> v;

  #pragma omp parallel firstprivate(v)
  {
    #pragma omp for
    for(int i : init_v) {
      if(i <= k) {
        v.push_back(i);
      }
    }

    #pragma omp critical
    {
      mainv.insert(mainv.end(), v.begin(), v.end());
    }
  }

  return std::move(mainv);
}

std::vector<int> filter_leq_parallel_guided(std::vector<int> init_v, int k) {
  std::vector<int> v;
  std::vector<int> mainv;

  #pragma omp parallel firstprivate(v)
  {
    #pragma omp for schedule(guided)
    for(int i : init_v) {
      if(i <= k) {
        v.push_back(i);
      }
    }

    #pragma omp critical
    {
      mainv.insert(mainv.end(), v.begin(), v.end());
    }
  }

  return std::move(mainv);
}

std::vector<int> init_random_vector(size_t n) {
  std::vector<int> v(n);
  std::mt19937 m{std::random_device{}()};
  std::uniform_int_distribution<int> dist{1, 99};
  std::generate(v.begin(), v.end(), [&dist, &m](){return dist(m);});
  return std::move(v);
}

int main() {
  std::vector<int> a = init_random_vector(1000000000);
  auto start = std::chrono::steady_clock::now();
  std::vector<int> v = filter_leq(a, 6);
  auto end = std::chrono::steady_clock::now();
  std::cout << "time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  auto start_p = std::chrono::steady_clock::now();
  std::vector<int> v_parallel = filter_leq_parallel(a, 6);
  auto end_p = std::chrono::steady_clock::now();

  std::cout << "parallel time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end_p - start_p).count() << "ms\n";

  auto start_pp = std::chrono::steady_clock::now();
  std::vector<int> v_parallel_g = filter_leq_parallel_guided(a, 6);
  auto end_pp = std::chrono::steady_clock::now();

  std::cout << "guided parallel time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end_pp - start_pp).count() << "ms\n";


  // check_equal_vector(v, v_parallel);
  std::cout << v.size() << " " << v_parallel.size() << " " << v_parallel_g.size() << std::endl;
}
