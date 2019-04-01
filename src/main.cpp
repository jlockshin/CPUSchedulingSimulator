// Jane Lockshin
// Runs CPU Scheduler

#include "Simulator.h"

using namespace std;

int main(int argc, char **argv) {

  // Start simulation
  Simulator sim(parseArgs(argc, argv));
  sim.simulation();

  return 0;
}
