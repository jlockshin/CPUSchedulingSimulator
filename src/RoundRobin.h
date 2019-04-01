// Jane Lockshin
// Round Robin Scheduling Algorithm

#ifndef CPU_SIMULATOR_RR_H
#define CPU_SIMULATOR_RR_H

#include "Thread.h"
#include "SchedulingAlgorithm.h"
#include <iostream>
#include <queue>

using namespace std;

struct RR_Algorithm : public SchedulingAlgorithm {

  // The time quantum (this number can be changed)
  const size_t quantum = 3;

  // The queue for to execute threads
  queue<Thread *> threadQueue;

  // Add a new thread to the thread queue
  virtual void add(Thread *thread) {
      threadQueue.push(thread);
  }

  // Check to see if the thread is ready
  virtual bool hasReadyThread() {
      return (bool) threadQueue.size();
  }

  // Get the next thread in the priority queue
  virtual Thread *getNextThread() {
      return threadQueue.front();
  }

  // Dispatch the thread
  virtual Thread *dispatch() {
      runningThread = threadQueue.front();
      threadQueue.pop();
      return runningThread;
  }

  // Execute the event
  virtual Event execute(size_t sysTime) {
      size_t cpuBurst = runningThread->nextCPUBurst(sysTime);
      // If the CPU burst is longer than the time quantum, the burst will preempt
      if (cpuBurst > quantum) {
          runningThread->currBurstTime(quantum);
          // Return preempted thread event
          return Event(sysTime + quantum, THREAD_PREEMPTED, *runningThread);
      } else {
          Type event = (runningThread->isLast()) ? THREAD_COMPLETED : CPU_BURST_COMPLETED;
          return Event(sysTime + cpuBurst, event, *runningThread);
      }
  }

  // Output for "DISPATCHER_INVOKED"
  virtual std::string toString() {
      string output("Selected from ");
      output += std::to_string(threadQueue.size());
      output += " threads; allotted time slice of 3.";
      return output;
  }
};

#endif
