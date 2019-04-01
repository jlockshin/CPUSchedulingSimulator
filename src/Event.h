// Jane Lockshin
// Header file for Events

#ifndef CPU_SIMULATOR_EVENT_H
#define CPU_SIMULATOR_EVENT_H

#include <iostream>
#include <string>
#include <vector>
#include "Thread.h"

// Enum for Event Type
#ifndef EVENT_EVENTTYPE
#define EVENT_EVENTTYPE
enum Type {
    THREAD_ARRIVED = 0,
    THREAD_DISPATCH_COMPLETED,
    PROCESS_DISPATCH_COMPLETED,
    CPU_BURST_COMPLETED,
    IO_BURST_COMPLETED,
    THREAD_COMPLETED,
    THREAD_PREEMPTED,
    DISPATCHER_INVOKED
};
#endif

struct Event {
  // Event Time
  size_t etime;
  // Thread ID
  size_t tid;
  // Process ID
  size_t pid;
  // Process Type
  size_t ptype;
  // Event type
  Type etype;

  // Constructor for Event
  Event(size_t time, Type type, Thread &thread) {
      etime = time;
      tid = thread.tid;
      pid = thread.pid;
      ptype = thread.ptype;
      etype = type;
  }

  Event &operator=(const Event &event) {
      this->etime = event.etime;
      this->tid = event.tid;
      this->pid = event.pid;
      this->ptype = event.ptype;
      this->etype = event.etype;
      return *this;
  }

  // Event Strings
  const std::vector<std::string> estr =
    {"THREAD_ARRIVED",
    "THREAD_DISPATCH_COMPLETED",
    "PROCESS_DISPATCH_COMPLETED",
    "CPU_BRUST_COMPLETED",
    "IO_BRUST_COMPLETED",
    "THREAD_COMPLETED",
    "THREAD_PREEMPTED",
    "DISPATCHER_INVOKED"};

  // Function to return Event Time
  bool operator>(const Event &event) const {
      return (this->etime > event.etime);
  }

  // Function to return Event Type
  friend std::ostream &operator<<(std::ostream &os, const Event &event) {
      return os << event.estr[event.etype];
  }
};

#endif
