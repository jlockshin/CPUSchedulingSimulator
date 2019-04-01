// Jane Lockshin
// Header file for custom scheduling algorithm

#ifndef CPU_SIMULATOR_CUSTOM_H
#define CPU_SIMULATOR_CUSTOM_H

#include "SchedulingAlgorithm.h"
#include "Thread.h"
#include <cstdlib>
#include <iostream>

using namespace std;

// Custom algorithm
#ifndef CPU_CUSTOM
#define CPU_CUSTOM

namespace Custom {
  // The priority of the last thread to execute
  struct priority {
    size_t last;
    Thread *thread;

    priority(Thread *t) {
        last = t->prevTime;
        thread = t;
    }
  };

  // Compares priority types
  struct compare {
    bool operator()(const priority &a, const priority &b) {
        return a.last > b.last;
    }
  };

  // Custom priority queues
  typedef priority_queue<Custom::priority, vector<Custom::priority>, Custom::compare>priorityQueue;
}

#endif

struct Custom_Algorithm : public SchedulingAlgorithm {

  // The custom priority queues to execute threads:
  // A vector of 4 priority queues and 1 event queue (shown in simulator.cpp)
  vector<Custom::priorityQueue> threadQueues;

  // Ensure that there are 4 priority queues being used
  Custom_Algorithm() {
      threadQueues.resize(4);
      srand(time(0));
  }

  // Declare variables used for custom algorithm
  size_t currentRandom = 0;
  size_t random;
  size_t calculatedRandom = INT32_MAX;
  size_t nextPriorityType;
  const size_t quantums[4] = {3, 4, 5, 6};

  // Add a new thread to the thread queue
  virtual void add(Thread *thread) {
      threadQueues[thread->ptype].push(Custom::priority(thread));
  }

  // Check to see if the thread is ready (same as Priority algorithm)
  virtual bool hasReadyThread() {
      for (Custom::priorityQueue &queue: threadQueues) {
          if (queue.size())
              return true;
      }
      return false;
  }

  // Get the next thread in the priority queue
  virtual Thread *getNextThread() {
      size_t selectPriority;
      Thread *nextThread;

      // Calculate random number
      if (currentRandom!= calculatedRandom) {
          random = rand() % 10;
          calculatedRandom = currentRandom;
      }

      // Select priority based on random number
      if (random < 1) selectPriority = 3;
      if (random < 4) selectPriority = 2;
      if (random < 8) selectPriority = 1;
      if (random < 10) selectPriority = 0;

      // Select one thread per each thread queue
      // (If no random priority is selected)
      if (threadQueues[selectPriority].size() == 0) {
          for (size_t j = 0; j < 4; j++) {
              if (threadQueues[j].size()) {
                  nextThread = threadQueues[j].top().thread;
                  break;
              }
          }
      } else {
        // Select the thread based on the randonmized priority
        nextThread = threadQueues[selectPriority].top().thread;
      }

      // Return the next thread
      nextPriorityType = nextThread->ptype;
      return nextThread;
  }

  // Dispatch the thread (similar to Priority algorithm)
  virtual Thread *dispatch() {
      runningThread = getNextThread();
      threadQueues[nextPriorityType].pop();
      currentRandom++;
      return runningThread;
  }

  // Execute the event (this uses premption - similar to RR algorithm)
  virtual Event execute(size_t sysTime) {
      size_t cpuBurst = runningThread->nextCPUBurst(sysTime);
      size_t quantum = quantums[runningThread->ptype];
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
      output += std::to_string(threadQueues.size());
      output += " threads; will run to completion of burst";
      return output;
  }
};

#endif
