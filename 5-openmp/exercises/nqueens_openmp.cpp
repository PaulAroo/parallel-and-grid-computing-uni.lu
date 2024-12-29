#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

// Check if placing a queen is safe
bool isSafe(const std::vector<int>& board, int row, int col) {
  for (int i = 0; i < row; ++i) {
    if (board[i] == col || board[i] == col - row + i || board[i] == col + row - i) {
      return false;
    }
  }
  return true;
}

// Depth-first search for solutions
void dfs(std::vector<int>& board, int depth, int N, size_t& num_sol, size_t& exploredTree) {
  if (depth == N) {
    num_sol++;
    return;
  }
  for (int i = 0; i < N; ++i) {
    if (isSafe(board, depth, i)) {
      board[depth] = i;
      exploredTree++;
      dfs(board, depth + 1, N, num_sol, exploredTree);
    }
  }
}

// Parallel N-Queens solver
void parallelNQueens(int N, int num_threads, size_t& num_sol, size_t& exploredTree) {

  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp single
    {
      for (int i = 0; i < N; ++i) {
        #pragma omp task shared(num_sol)
        {
          std::vector<int> board(N, -1);
          board[0] = i;
          size_t local_sol = 0;
          size_t local_exploredTree = 0;
          dfs(board, 1, N, local_sol, local_exploredTree);
          #pragma omp atomic
          num_sol += local_sol;
          exploredTree += local_exploredTree;
        }
      }
    }
  }

}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <number of queens> <number of threads>" << std::endl;
    return 1;
  }

  int N = std::stoi(argv[1]);
  int num_threads = std::stoi(argv[2]);
  std::cout << "Solving " << N << "-Queens problem\n";

  auto start = std::chrono::steady_clock::now();

  size_t num_sol = 0;
  size_t exploredTree = 0;

  parallelNQueens(N, num_threads, num_sol, exploredTree);
  auto end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Time taken: " << duration.count() << " ms\n";
  std::cout << "Total solutions: " << num_sol << std::endl;
  std::cout << "Total exploredTree: " << exploredTree << std::endl;

  return 0;
}
