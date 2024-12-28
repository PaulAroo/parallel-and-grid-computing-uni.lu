
/*
 * Author: Guillaume HELBECQUE (Université du Luxembourg)
 * Date: 10/10/2024
 *
 * Description:
 * This program solves the N-Queens problem using a sequential Depth-First tree-Search
 * (DFS) algorithm. It serves as a basis for task-parallel implementations.
 */

#include <iostream>
#include <cstring>
#include <utility>
#include <vector>
#include <chrono>
#include <stack>

#include <mutex>
#include <atomic>
#include <thread>

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
  Node& operator=(Node&&) = default;
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
void evaluate_and_branch(const Node& parent, std::stack<Node>& pool, std::atomic<size_t>& tree_loc, std::atomic<size_t>& num_sol, std::mutex& mutex)
{
  int depth = parent.depth;
  int N = parent.board.size();

  // if the given node is a leaf, then update counter and do nothing
  if (depth == N) {
    num_sol++;
  }
  // if the given node is not a leaf, then update counter and evaluate/branch it
  else {
    for (int j = depth; j < N; j++) {
      if (isSafe(parent.board, depth, parent.board[j])) {
        Node child(parent);
        std::swap(child.board[depth], child.board[j]);
        child.depth++;
        {
          std::lock_guard<std::mutex> guard(mutex);
          pool.push(std::move(child));
        }
        tree_loc++;
      }
    }
  }
}

void worker(std::stack<Node>& pool, std::mutex& mutex, std::atomic<size_t>& exploredBranches, std::atomic<size_t>& num_sol) {

  while(true) {

    Node currentNode;
    {
      std::lock_guard<std::mutex> guard(mutex);
      if(pool.empty()) return;
      currentNode = std::move(pool.top());
      pool.pop();
    }
    evaluate_and_branch(currentNode, pool, exploredBranches, num_sol, mutex);

  }
}

int main(int argc, char** argv) {
  // helper
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " <number of queens> <number of threads> " << std::endl;
    exit(1);
  }

  // problem size (number of queens)
  size_t N = std::stoll(argv[1]);
  int num_threads = std::stoi(argv[2]);
  std::cout << "Solving " << N << "-Queens problem\n" << std::endl;

  std::mutex m;

  // initialization of the root node (the board configuration where no queen is placed)
  Node root(N);

  // initialization of the pool of nodes (stack -> DFS exploration order)
  std::stack<Node> pool;
  pool.push(std::move(root));

  // statistics to check correctness (number of nodes explored and number of solutions found)
  std::atomic<size_t> exploredTree = 0;
  std::atomic<size_t> exploredSol = 0;

  // beginning of the Depth-First tree-Search
  auto start = std::chrono::steady_clock::now();

  std::vector<std::thread> threads;

  for (int i = 0; i < num_threads; i++) {
    threads.emplace_back(
      worker,
      std::ref(pool), std::ref(m), std::ref(exploredTree), std::ref(exploredSol)
    );
  }

  for (auto& t : threads) {
    if(t.joinable()) {
      t.join();
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
