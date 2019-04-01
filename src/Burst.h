// Jane Lockshin
// Header file for Bursts

#ifndef CPU_SIMULATOR_BURST_H
#define CPU_SIMULATOR_BURST_H

#include <cstddef>

struct Burst {
  // CPU Burst
  size_t CPU = 0;
  // IO Burst
  size_t IO = 0;

  // Constructor for Bursts
  Burst() {}
};

#endif
