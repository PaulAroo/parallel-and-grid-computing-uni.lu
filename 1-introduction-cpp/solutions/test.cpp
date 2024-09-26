#include <iostream>
#include "vector.hpp"


int main() {
  Vector<char> vec(2);

  vec[0] = '1';
  vec[1] = '2';

  Vector<char> vec3 = vec;
  vec3.reserve(4);
  vec3.push_back('b');

  std::cout << vec3.back() << std::endl;
  std::cout << "size: " << vec3.size() << std::endl;
  std::cout << "capacity: " << vec3.capacity() << std::endl;
}