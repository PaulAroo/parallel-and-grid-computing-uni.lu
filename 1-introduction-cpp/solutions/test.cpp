#include <iostream>
#include "int_vector.hpp"


int main() {
  IntVector vec(2);

  vec[0] = 1;
  vec[1] = 2;

  IntVector vec3 = vec;
  vec3.reserve(4);

  std::cout << vec3.back() << std::endl;
}