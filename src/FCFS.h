// Jane Lockshin
// Header file for FCFS algorithm

#ifndef CPU_SIMULATOR_FCFS_H
#define CPU_SIMULATOR_FCFS_H

#include "SchedulingAlgorithm.h"
#include <queue>

using namespace std;

struct FCFS_Algorithm : public SchedulingAlgorithm {
    // Threads for scheduler
    queue<Thread *> threadQueue;

    // Add a new thread to the thread queue
    void add(Thread *thread) {
      threadQueue.push(thread);
    }

    // Check to see if the thread is ready
    bool hasReadyThread() {
      return (bool) threadQueue.size();
    }

    // Get the next thread in the priority queue
    Thread *getNextThread() {
      return threadQueue.front();
    }

    // Dispatch the thread
    Thread *dispatch() {
      runningThread = threadQueue.front();
      threadQueue.pop();
      return runningThread;
    }

    // Execute the event
    Event execute(size_t sysTime) {
      size_t time = sysTime + runningThread->nextCPUBurst(sysTime);
      Type event = (runningThread->isLast()) ? THREAD_COMPLETED : CPU_BURST_COMPLETED;
      return Event(time, event, *runningThread);
    }

    // Output for "DISPATCHER_INVOKED"
    string toString() {
      string result("Selected from ");
      result += std::to_string(threadQueue.size());
      result += " threads; will run to completion of burst";
      return result;
    }
};

#endif
