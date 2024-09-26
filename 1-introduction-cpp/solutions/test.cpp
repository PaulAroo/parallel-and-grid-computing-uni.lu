#include <iostream>
#include "vector.hpp"


int main() {
  // Vector<char> vec(2);

  // vec[0] = '1';
  // vec[1] = '2';

  // Vector<char> vec3 = vec;
  // vec3.reserve(4);
  // vec3.push_back('b');

  // std::cout << vec3.back() << std::endl;
  // std::cout << "size: " << vec3.size() << std::endl;
  // std::cout << "capacity: " << vec3.capacity() << std::endl;


  // auto glambda = [](auto a, auto&& b) { 
  //   return a > b;
  // };
  // bool b = glambda(3, 3.14); // OK

  Vector<int> vec(3);

  vec[0] = 1;
  vec[1] = -5;
  vec[2] = 6;

  // map(vec, [](int x) { return x * 2; });

  // for(size_t i = 0; i < vec.size(); ++i) {
  //   std::cout << vec[i] << " ";
  // }

  // std::cout << std::endl;

  // Implement a left-fold function (aka. "reduce") which takes a vector, an accumulator and a function f. For instance, fold_left(v, 0, [](int accu, int x) { return accu - x; }) returns the value 2 (the difference of the integers in vector v from left to right). More information.

  int y = fold_left(vec, 0, [](int accu, int x) {
    std::cout << accu << ". " << x << std::endl;
    return accu - x; 
  });

  std::cout << y << std::endl;

  


}