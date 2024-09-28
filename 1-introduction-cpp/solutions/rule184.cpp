#include <iostream>
#include <random>
#include <vector>
#include <thread>

using namespace std::chrono_literals;

std::vector<int> initialize_cells(size_t n) {
  std::vector<int> cells(n);
  // std::mt19937 m{std::random_device{}()};
  std::mt19937 m{1}; // fix the seed to ease debugging.
  std::uniform_int_distribution<int> u{0, 1};
  for(int i = 0; i < cells.size(); ++i) {
    cells[i] = u(m);
  }
  return std::move(cells);
}


void print(std::vector<int> cells) {
  for(int i : cells) {
      if(i == 0) {
        std::cout << "🔳";
      }
      else{
        std::cout << "⚪";
      }
    }
}


void simulate_step(std::vector<int> currentCells, std::vector<int>& nextCells) {
  // std::mt19937 m{0};
  // std::uniform_int_distribution<int> u{0, 1};

  // treat as circular

  for(int j = 0; j < currentCells.size(); ++j) {
    // cell has state 1, pick from the right
    if(currentCells[j] == 1) { 
      if(j == currentCells.size() - 1) { //on the right edge
        nextCells[j] = currentCells[0];
      }
      else {
        nextCells[j] = currentCells[j + 1];
      }
    }
     // cell has state 0, pick from the left
    else {
      if(j == 0) { //on the left edge
        nextCells[j] = currentCells[currentCells.size() - 1];
      }
      else {
        nextCells[j] = currentCells[j - 1];
      };
    }
  }
}

void computeLongestConsequetiveOnes(std::vector<int>& cells) {
  size_t count = 0;
  size_t max = 0;
  for(int i : cells) {
    if(i == 1) {
      count += 1;
      max = count > max ? count : max;
    }else {
      count = 0;
    }
  }

  std::cout << " Longest 1s: " << max;
}

void simulate(size_t steps, std::vector<int> currentCells, std::vector<int>& nextCells) {
  for(int i = 0; i < steps; ++i) {
    simulate_step(currentCells, nextCells);
    print(nextCells);

    computeLongestConsequetiveOnes(nextCells);

    currentCells = nextCells;
    std::cout << std::endl;


    // pause program by a second between 2 iterations
    if(i % 2 == 0) {
      std::this_thread::sleep_for(1000ms);
    }
  }
};


int main(int argc, char** argv) {

  const size_t n = 50;
  const size_t steps = 10;
  std::vector<int> cells = initialize_cells(n);
  std::vector<int> nextCells(n);

  simulate(steps, cells, nextCells);


  /** Notes:
   * `std::this_thread::sleep_for(1000ms);` sleeps the program for 1 second.
   * `u(m)` randomly generates either 0 or 1 following a uniform distribution.
   * You can use `u(m)` to generate whether a car enters the queue (cells[0]) at the current step.

   * rule 184: If a cell has state 0, its new state is taken from the cell to its left.
               Otherwise, its new state is taken from the cell to its right.
  */
  return 0;
}
