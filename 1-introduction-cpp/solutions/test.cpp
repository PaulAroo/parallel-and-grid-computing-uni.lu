#include <iostream>
#include <thread>
#include <random>
#include <semaphore>

#include "vector_thread_safe.hpp"
// #include "vector.hpp"

using namespace std::chrono_literals;



class Buffer {
  // size_t n;
  Vector<int> vec;
  std::counting_semaphore<2> semaphore;

  // std::binary_semaphore semaphore {0};


  public:
  Buffer() : semaphore(0) {};

  void produce(size_t v) {
    vec.push_back(v);
    semaphore.release();

  }

  void consume() {
    semaphore.acquire();
    vec.pop_back();
    semaphore.release();
  }
};


int main(int argc, char** argv) {

  // // test the map function
  Vector<int> vec;
  Buffer buff;

  // vec[0] = 1;
  // vec[1] = -5;
  // vec[2] = 6;


  std::mt19937 m{1}; // fix the seed to ease debugging.
  std::uniform_int_distribution<int> u{1, 9};


  // map(vec, [](int x) { return x * 2; });
  // for(size_t i = 0; i < vec.size(); ++i) {
  //   std::cout << vec[i] << " ";
  // }

  // std::cout << std::endl;

  for(int i = 0; i < 11; ++i) {

    std::thread a([&]() {buff.produce(u(m));});
    std::thread b([&]() {buff.consume();});

    std::this_thread::sleep_for(1000ms);

    a.join();
    b.join();
  }


  // printf("size of array %d", vec.size());
  // std::cout << "Size of array " << vec.size() << std::endl;

}