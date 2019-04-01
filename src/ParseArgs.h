// Jane Lockshin
// Header file for inputs on the command line

#ifndef CPU_SIMULATOR_PARSE_ARGS_H
#define CPU_SIMULATOR_PARSE_ARGS_H

#include <stdlib.h>

enum Algorithm {
  FCFS, // First-come, first-served (default)
  RR, // Round Robin
  PRIORITY, // Process-Priority Scheduling
  CUSTOM // Custom algorithm
};

struct Scheduler {
  // The input file required for simulation
  char *inputFile;
  // -t flag, output additiona per-thread stats
  bool perThread;
  // -v flag, output verbose information
  bool verbose;
  // -a flag, the scheduling algorithm to use
  Algorithm algorithm;

  // Constructor for Scheduler
  Scheduler() {
      inputFile = NULL;
      perThread = false;
      verbose = false;
      algorithm = FCFS; // FCFS is default algorithm
  }
};

Scheduler parseArgs(int argc, char **argv);

#endif
