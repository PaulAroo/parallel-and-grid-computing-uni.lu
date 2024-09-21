#include <iostream>
#include <random>
#include <vector>
#include <thread>

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

// rule 184: If a cell has state 0, its new state is taken from the cell to its left.
          // Otherwise, its new state is taken from the cell to its right.

int main(int argc, char** argv) {
  using namespace std::chrono_literals;
  std::mt19937 m{0};
  std::uniform_int_distribution<int> u{0, 1};
  size_t n = 50;
  size_t steps = 10;
  std::vector<int> cells = initialize_cells(n);

  for(int i = 0; i < steps; ++i) {
    std::vector<int> nextCells(n);

    // loop through each cell in cells and compute the array of new cells
    for(int j = 0; j < cells.size(); ++j) {
      int currentCell = cells[j];

      if(currentCell == 1) { // cell has state 1, pick from the right
        if(j == 0) {
          nextCells[j] = cells[j + 1];
        }
        else if(j == cells.size() - 1) {
          nextCells[j] = u(m);
        }
        else {
          nextCells[j] = cells[j + 1];
        }
      }
      else { // cell has state 0, pick from the left
        if(j == 0) {
          nextCells[j] = u(m);
        }
        else if(j == cells.size() - 1) {
          nextCells[j] = cells[j - 1];
        }
        else {
          nextCells[j] = cells[j - 1];
        };
      }
    }

    for(int i : nextCells) {
      if(i == 0) {
        std::cout << "🔳";
      }
      else{
        std::cout << "⚪";
      }
    }

    cells = nextCells;
    std::cout << std::endl;
  }


  /** Notes:
   * `std::this_thread::sleep_for(1000ms);` sleeps the program for 1 second.
   * `u(m)` randomly generates either 0 or 1 following a uniform distribution.
   * You can use `u(m)` to generate whether a car enters the queue (cells[0]) at the current step.
  */
  return 0;
}
