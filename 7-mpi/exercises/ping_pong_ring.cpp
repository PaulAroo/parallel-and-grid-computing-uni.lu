#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
  int rank, num_procs;

  /* Initialize the infrastructure necessary for communication */
  MPI_Init(&argc, &argv);

  if(argc < 2) {
    printf("usage: %s <num-ping-pong>\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int num_ping_pong = std::stoi(argv[1]);

  /* Identify this process */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Find out how many total processes are active */
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  /** TODO: ping pong ring. */
  std::string ping("ping");
  std::string pong("pong");
  std::string rdata(4, ' ');
  int next_rank = (rank + 1) % num_procs;
  int prev_rank = ((rank + num_procs) - 1) % num_procs;

  MPI_Request rq_send;
  // MPI_Request rq_receive;
  // int flag;

  for(int i = 0; i < num_ping_pong; i++) {
    MPI_Isend(ping.data(), ping.size(), MPI_CHAR, next_rank, 0, MPI_COMM_WORLD, &rq_send);
    printf("%d: send ping to %d...\n", rank, next_rank);

    MPI_Recv(rdata.data(), rdata.size(), MPI_CHAR, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d: receive ping from %d...\n", rank, prev_rank);

    MPI_Wait(&rq_send, MPI_STATUS_IGNORE);

    // send pong
    MPI_Isend(pong.data(), pong.size(), MPI_CHAR, prev_rank, 0, MPI_COMM_WORLD, &rq_send);
    printf("%d: send pong to %d...\n", rank, prev_rank);

    MPI_Recv(rdata.data(), rdata.size(), MPI_CHAR, next_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d: receive pong from %d...\n", rank, next_rank);

    MPI_Wait(&rq_send, MPI_STATUS_IGNORE);
  }

  /* Tear down the communication infrastructure */
  MPI_Finalize();
  return 0;
}