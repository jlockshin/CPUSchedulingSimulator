// Jane Lockshin
// Header file for Priority Algorithm

#ifndef CPU_SIMULATOR_PRIORITY_ALGORITHM_H
#define CPU_SIMULATOR_PRIORITY_ALGORITHM_H

#include "SchedulingAlgorithm.h"
#include "Thread.h"
#include "Priority.h"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Priority_Algorithm : public SchedulingAlgorithm {

  // A vector of the four queues for to execute threads
  vector<queue<Thread *>> threadQueues;
  size_t nextPriorityType;

  // Ensure that there are four thread queues
  Priority_Algorithm() {
    threadQueues.resize(4);
  }

  // Add a new thread to the thread queue (based on priority)
  virtual void add(Thread *thread) {
      threadQueues[thread->ptype].push(thread);
  }

  // Check to see if the thread queue contains a thread
  virtual bool hasReadyThread() {
      for (queue<Thread *> &queue: threadQueues) {
          if (queue.size())
              return true;
      }
      return false;
  }

  // Get the next thread in the current priority queue
  virtual Thread *getNextThread() {
      Thread *nextPriority;
      for (queue<Thread *> &queue: threadQueues) {
          if (queue.size()) {
              nextPriority = queue.front();
              nextPriorityType = nextPriority->ptype;
              break;
          }
      }
      return nextPriority;
  }

  // Dispatch the thread
  virtual Thread *dispatch() {
      runningThread = getNextThread();
      threadQueues[nextPriorityType].pop();
      return runningThread;
  }

  // Execute the event
  virtual Event execute(size_t sysTime) {
      size_t time = sysTime + runningThread->nextCPUBurst(sysTime);
      Type event = (runningThread->isLast()) ? THREAD_COMPLETED : CPU_BURST_COMPLETED;
      return Event(time, event, *runningThread);
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
