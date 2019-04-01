// Jane Lockshin
// Header file for scheduling algorithms

#ifndef CPU_SCHEDULING_ALGORITHM_H
#define CPU_SCHEDULING_ALGORITHM_H

#include "Process.h"
#include "Event.h"
#include <string>

// Struct for scheduling algorithm
struct SchedulingAlgorithm {
  // Initialize current running thread
  Thread *runningThread = 0;
  // Adds a new thread to the scheduler
  virtual void add(Thread *thread) = 0;
  // Checks to see if the process has a thread ready
  virtual bool hasReadyThread() = 0;
  // Retrieves the next thread
  virtual Thread *getNextThread() = 0;
  // The thread dispatcher
  virtual Thread *dispatch() = 0;
  // Executes the next event
  virtual Event execute(size_t) = 0;
  // Converts to string
  virtual std::string toString() = 0;
};

#endif
