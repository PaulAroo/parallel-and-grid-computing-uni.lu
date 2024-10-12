
#include <cstring>
#include <utility>
#include <vector>
#include <chrono>
#include <stack>
#include <mutex>
#include <atomic>
#include <thread>
#include "utility.hpp"

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
void evaluate_and_branch(const Node& parent, std::stack<Node>& pool, std::atomic<size_t>& tree_loc, std::atomic<size_t>& num_sol, std::mutex& pool_mutex)
{
  int depth = parent.depth;
  int N = parent.board.size();

  tree_loc++;
  // if the given node is a leaf, then update counter and do nothing
  if (depth == N) {
    num_sol++;
  }
  // if the given node is not a leaf, then update counter and evaluate/branch it
  else {
    for (int j = depth; j < N; j++) {
      // const size_t idx = j >= pools.size() ? 0 : j;

      if (isSafe(parent.board, depth, parent.board[j])) {
        Node child(parent);
        std::swap(child.board[depth], child.board[j]);
        child.depth++;

        std::lock_guard<std::mutex> lock(pool_mutex);
        pool.push(std::move(child));
      }
    }
  }
}

void worker(size_t index, std::vector<std::stack<Node>>& pools, std::vector<std::mutex>& pool_mutexs, std::atomic<size_t>& tree_loc, std::atomic<size_t>& num_sol) {

    while (true){
      Node currentNode;
      {
        std::lock_guard<std::mutex> guard(pool_mutexs[index]);
        if(pools[index].empty()) {
          // try to steal from random pool
          size_t random_index = generateRandomNumber(pools.size());
          // std::lock_guard<std::mutex> guard(pool_mutexs[random_index]);
          if(pools[random_index].size() > 2) {
            for(size_t j = 0; j < pools[random_index].size() / 2 ; ++j) {
              pools[index].push(pools[random_index].top());
              pools[random_index].pop();
            }
          }
          else {
            return;
          }
        }

        currentNode = std::move(pools[index].top());
        pools[index].pop();
      }

      evaluate_and_branch(currentNode, pools[index], tree_loc, num_sol, pool_mutexs[index]);
    }
  };

int main(int argc, char** argv) {
  // helper
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " <number of queens> " << "<num threads>" << std::endl;
    exit(1);
  }

  // problem size (number of queens) and thread number
  size_t N = std::stoll(argv[1]);
  size_t num_threads = std::stoi(argv[2]);
  std::cout << "Solving " << N << "-Queens problem with " << num_threads << " threads\n" << std::endl;

  // initialization of the root node (the board configuration where no queen is placed)
  Node root(N);

  // initialization of the pool of nodes (stack -> DFS exploration order)
  std::vector<std::stack<Node>> pools(num_threads);
  pools[0].push(std::move(root));

  // initalize pool_mutexs
  std::vector<std::mutex> pool_mutexs(num_threads);

  // statistics to check correctness (number of nodes explored and number of solutions found)
  std::atomic<size_t> exploredTree = 0;
  std::atomic<size_t> exploredSol = 0;

  std::vector<std::thread> threads(num_threads);

  // beginning of the Depth-First tree-Search
  auto start = std::chrono::steady_clock::now();

  // launch threads
  for(size_t i = 0; i < num_threads; ++i) {
    threads.emplace_back(
      worker,
      i,
      std::ref(pools), std::ref(pool_mutexs),
      std::ref(exploredTree), std::ref(exploredSol)
    );
  }

  for(auto& t : threads) {
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



