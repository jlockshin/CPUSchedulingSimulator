// Jane Lockshin
// Header file for Processes

#ifndef CPU_SIMULATOR_PROCESS_H
#define CPU_SIMULATOR_PROCESS_H

#include <vector>
#include <cstddef>
#include "Thread.h"

struct Process {
  // Process id
  size_t pid;
  // Process type
  size_t ptype;
  // Number of threads
  size_t num_thread;
  // Threads per process
  std::vector<Thread> threads;

  // Constructor for Process
  Process() {
    pid = 0;
    ptype = 0;
    num_thread = 0;
  }
};

#endif
