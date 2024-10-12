
#include "utility.hpp"

int main(int argc, char** argv) {

  const size_t N = std::stoll(argv[1]);

  const size_t x = generateRandomNumber(N);

  std::cout << x << std::endl;
}