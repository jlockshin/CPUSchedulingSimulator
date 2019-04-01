// Jane Lockshin
// Header file to define priority types

#ifndef CPU_SIMULATOR_PRIORITY_H
#define CPU_SIMULATOR_PRIORITY_H

#include <cstdlib>

// Priority Types
const size_t SYSTEM = 0;
const size_t INTERACTIVE = 1;
const size_t NORMAL = 2;
const size_t BATCH = 3;

// String for priority types
extern const char *PriorityStr[];

#endif
