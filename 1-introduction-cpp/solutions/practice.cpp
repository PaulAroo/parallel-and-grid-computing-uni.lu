
#include "vector_thread_safe.hpp"
#include <thread>


int main() {

  Buffer<int> data;

  std::jthread a([&]() {
    for(int i = 0; i < 5; i++) {
      data.produce(i);
    }
  });

  std::jthread b([&]() {
    for(int i = 0; i < 5; i++) {
      data.consume();
    }
  });

}