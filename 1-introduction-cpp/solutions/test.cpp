#include <iostream>
#include "vector.hpp"


int main() {

  // test the map function
  Vector<int> vec(3);

  vec[0] = 1;
  vec[1] = -5;
  vec[2] = 6;


  map(vec, [](int x) { return x * 2; });
  for(size_t i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }

    std::cout << std::endl;

}