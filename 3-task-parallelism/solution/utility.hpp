
#include <iostream>
#include <random> // For std::random_device, std::mt19937, std::uniform_int_distribution

// generate an integer for range [0, N-1]
size_t generateRandomNumber(size_t N) {
    // Ensure N is not 0 to avoid generating numbers in an empty range
    if (N == 0) {
        throw std::invalid_argument("N must be greater than 0");
    }

    // Random number engine seeded with a non-deterministic value (if available)
    std::random_device rd;
    
    // Standard Mersenne Twister engine
    std::mt19937 generator(rd());

    // Uniform distribution for range [0, N-1]
    std::uniform_int_distribution<size_t> distribution(0, N - 1);

    // Generate and return the random number
    return distribution(generator);
}
