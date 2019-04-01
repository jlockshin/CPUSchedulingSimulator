// Jane Lockshin
// Header file for Threads

#ifndef CPU_SIMULATOR_THREAD_H
#define CPU_SIMULATOR_THREAD_H

#include <vector>
#include <cstddef>
#include "Burst.h"

struct Thread {
  // Thread ID
  size_t tid = 0;
  // Process ID
  size_t pid = 0;
  // Priority Type
  size_t ptype = 0;
  // Number of bursts
  size_t num_burst = 0;
  // Current burst
  size_t curr_burst = 0;
  // Check to see if thread has started
  bool started = false;
  // Bursts per thread
  std::vector<Burst> bursts;
  // Arrival time
  size_t arrivalTime = 0;
  // Start time
  size_t startTime = 0;
  // Finsh time
  size_t finishTime = 0;
  // Time of last execution
  size_t prevTime = 0;
  // Total CPU time
  size_t totalCPUTime = 0;
  // Total IO time
  size_t totalIOTime = 0;
  // Response time
  size_t responseTime = 0;
  // Turn-around time
  size_t turnaroundTime = 0;

  // Function to retrieve the next CPU burst
  size_t nextCPUBurst(size_t time) {
    if (!started) {
        started = true;
        startTime = time;
    }

    // Add this burst to total CPU time
    prevTime = time;
    size_t burst = bursts[curr_burst].CPU;
    totalCPUTime += burst;
    return burst;
  }

  // Function to retrieve the next IO burst
  size_t nextIOBurst() {
      size_t burst = bursts[curr_burst].IO;
      totalIOTime += burst;
      curr_burst++;
      return burst;
  }

  // Function if CPU burst is preempted
  void currBurstTime(size_t time) {
      bursts[curr_burst].CPU -= time;
      totalCPUTime -= bursts[curr_burst].CPU;
  }

  // Function to check if the current burst is the last burst
  bool isLast() {
      return ((curr_burst + 1) == num_burst);
  }

  // Constructor for Threads
  Thread() {}
};

#endif
