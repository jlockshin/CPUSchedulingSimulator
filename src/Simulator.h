// Jane Lockshin
// Header file for Simulation

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include "Process.h"
#include "Event.h"
#include "ParseArgs.h"
#include "SchedulingAlgorithm.h"

using namespace std;

struct Simulator {

  // Input file
  string inputfile;
  // -v flag
  bool verbose;
  // -t flag
  bool perThread;
  // Scheduling algorithm
  Algorithm algorithm;
  // Number of processes in the input file
  size_t numProcesses;
  // Thread Switch Overhead from input file
  size_t threadSwitchOverhead;
  // Process Switch Overhead from input file
  size_t processSwitchOverhead;
  // The last process ID of the simulation
  size_t lastProcessID;
  // Time
  size_t sysTime;
  // Checks to see if the process has a running/scheduled thread
  bool threadScheduledOrRunning;
  // Processes from input file
  vector<Process> processes;
  // Priority Queue for Events
  priority_queue<Event, vector<Event>, std::greater<Event>> eventQueue;
  // The scheduling algorithm
  SchedulingAlgorithm *scheduler;
  // CPU utlization
  double cpuUtilization;
  // CPU efficiency
  double cpuEfficency;
  // Total service time
  size_t totalServiceTime = 0;
  // Total IO time
  size_t totalIOTime = 0;
  // Total idle time
  size_t totalIdleTime = 0;
  // Total dispatch time
  size_t totalDispatchTime = 0;
  // Total elapsed time
  size_t totalTime = 0;
  // Regular output message
  ostringstream message;
  // Per-thread output message
  ostringstream perthreadMsg;
  // Verbose output message
  ostringstream verboseMsg;

  // Constructor for Simulator
  Simulator(const Scheduler input);
  // Start simulation
  void simulation();
  // Load input file
  void loadFile();
  // Load Processes
  Process loadProcess(istream &in);
  // Load Threads
  Thread loadThread(istream &in, Process &process, size_t threadID);
  // Load Bursts
  Burst loadBurst(istream &in, bool lastOne);
  // Loads next event
  Event loadNextEvent();
  // Gets next event
  void nextEvent();
  // Handles Events
  void eventHandler(const Event &event);
  // Populates priority queue
  void populateQueue();
  // Outputs message to terminal
  void outputMessage();
  // Chooses scheduling Algorithm
  void chooseSchedule();
  // Outputs per-thread message
  void perthreadMessage();
  // Outputs verbose message
  void verboseMessage(const Event &event);
  // Outputs usage analysis
  void outputAnalysis();
};
