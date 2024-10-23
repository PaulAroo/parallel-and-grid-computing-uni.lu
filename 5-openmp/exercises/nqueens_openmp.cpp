
#include <iostream>
#include <cstring>
#include <utility>
#include <vector>
#include <chrono>
// #include <omp.h>


// N-Queens node
struct Node {
  int depth; // depth in the tree
  std::vector<int> board; // board configuration (permutation)

  Node(size_t N): depth(0), board(N) {
    for (int i = 0; i < N; i++) {
      board[i] = i;
    }
  }
  Node(const Node&) = default;
  Node(Node&&) = default;
  Node() = default;
};

// check if placing a queen is safe (i.e., check if all the queens already placed share
// a same diagonal)
bool isSafe(const std::vector<int>& board, const int row, const int col)
{
  for (int i = 0; i < row; ++i) {
    if (board[i] == col - row + i || board[i] == col + row - i) {
      return false;
    }
  }

  return true;
}

// evaluate a given node (i.e., check its board configuration) and branch it if it is valid
// (i.e., generate its child nodes.)
void evaluate_and_branch(const Node& parent, size_t& exploredTree, size_t& exploredSol)
{
  int depth = parent.depth;
  int N = parent.board.size();

  // if the given node is a leaf, then update counter and do nothing
  if (depth == N) {
    #pragma omp atomic
    exploredSol++;
  }
  // if the given node is not a leaf, then update counter and evaluate/branch it
  else {
    for (int j = depth; j < N; j++) {
      if (isSafe(parent.board, depth, parent.board[j])) {
        Node child(parent);
        std::swap(child.board[depth], child.board[j]);
        child.depth++;

        #pragma omp atomic
        exploredTree++;

        #pragma omp task
        {
          #pragma omp critical
          evaluate_and_branch(child, exploredTree, exploredSol);
        }
      }
    }
  }
}


int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " <number of queens> " << " <num_threads>" << std::endl;
    exit(1);
  }


  // problem size (number of queens) and thread number
  size_t N = std::stoll(argv[1]);
  size_t num_threads = std::stoi(argv[2]);
  std::cout << "Solving " << N << "-Queens problem with " << num_threads << " threads\n" << std::endl;

  // root node
  Node root(N);

  // statistics to check correctness (number of nodes explored and number of solutions found)
  size_t exploredTree = 0;
  size_t exploredSol = 0;

  // beginning of the Depth-First tree-Search
  auto start = std::chrono::steady_clock::now();

  #pragma omp parallel
  {
    #pragma omp single
    {
      #pragma omp task
      evaluate_and_branch(root, exploredSol, exploredSol);
    }
  }

  auto end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // outputs
  std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;
  std::cout << "Total solutions: " << exploredSol << std::endl;
  std::cout << "Size of the explored tree: " << exploredTree << std::endl;

  return 0;

}