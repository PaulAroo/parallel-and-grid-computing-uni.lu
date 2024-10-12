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

void worker(std::stack<Node>& pool, std::mutex& pool_mutex, std::atomic<size_t>& tree_loc, std::atomic<size_t>& num_sol, std::atomic_bool& done) {
    while(true) {
      Node currentNode;
      {
        std::lock_guard<std::mutex> lock(pool_mutex);
        if(pool.empty()) {
          return;
          // if(done) {
          //   return;
          // } else {
          //   continue;
          // }
        }

        currentNode = std::move(pool.top());
        pool.pop();
      }
      evaluate_and_branch(currentNode, pool, tree_loc, num_sol, pool_mutex);
    }
  };

int main(int argc, char** argv) {
  // helper
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " <number of queens> " << "<num threads>" << std::endl;
    exit(1);
  }

  std::mutex pool_mutex;
  std::atomic_bool done(false);

  // problem size (number of queens)
  size_t N = std::stoll(argv[1]);
  size_t num_threads = std::stoi(argv[2]);
  std::cout << "Solving " << N << "-Queens problem with " << num_threads << " threads\n" << std::endl;

  // initialization of the root node (the board configuration where no queen is placed)
  Node root(N);

  // initialization of the pool of nodes (stack -> DFS exploration order)
  std::stack<Node> pool;
  pool.push(std::move(root));

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
      std::ref(pool), std::ref(pool_mutex),
      std::ref(exploredTree), std::ref(exploredSol), std::ref(done)
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



