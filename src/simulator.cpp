// Jane Lockshin
// CPU Scheduling Simulator

#include <iomanip>
#include "Simulator.h"
#include "Priority.h"
#include "FCFS.h"
#include "RoundRobin.h"
#include "PriorityAlgorithm.h"
#include "CustomAlgorithm.h"

using namespace std;

// Create a new simulation per input file
Simulator::Simulator(const Scheduler input) {
    this->inputfile = input.inputFile;
    this->algorithm = input.algorithm;
    this->perThread = input.perThread;
    this->verbose = input.verbose;

    // Initialize system time and last process ID
    lastProcessID = INT32_MAX;
    sysTime = 0;
}

// Load input file
void Simulator::loadFile() {
  ifstream fin(inputfile);
  // Print error if the file cannot be opened
  if (!fin) {
      cerr << "Cannot open simulation file: " << inputfile << endl;
      exit(EXIT_FAILURE);
  }
  // Fill in the values from the first line of the input file
  fin >> numProcesses >> threadSwitchOverhead >> processSwitchOverhead;
  for (size_t i = 0; i < numProcesses; ++i) {
    // Load the processes from the file
    processes.push_back(loadProcess(fin));
  }
  // Close the file
  fin.close();
}

// Load processes from the input file
Process Simulator::loadProcess(istream &input) {
  Process process;
  size_t pid, type, num_thread;
  input >> pid >> type >> num_thread;
  // Populate the process ID, number of threads, and process type
  process.pid = pid;
  process.num_thread = num_thread;
  process.ptype = type;
  // Load the threads for each process
  for (size_t i = 0; i < num_thread; ++i)
      process.threads.push_back(loadThread(input, process, i));
  // Return process
  return process;
}

// Load threads from the input file
Thread Simulator::loadThread(istream &input, Process &process, size_t tid) {
  Thread thread;
  size_t arrivalTime, num_burst;
  input >> arrivalTime >> num_burst;
  // Populate the provided fields for threads
  thread.arrivalTime = arrivalTime;
  thread.num_burst = num_burst;
  thread.tid = tid;
  thread.pid = process.pid;
  thread.ptype = process.ptype;
  // Load the bursts for each thread
  for (size_t i = 0; i < num_burst; ++i) {
      thread.bursts.push_back(loadBurst(input, (num_burst - i) == 1));
  }
  // Return thread
  return thread;
}

// Load bursts from the input file
Burst Simulator::loadBurst(istream &input, bool isLast) {
  Burst burst;
  size_t CPU = 0, IO = 0;
  input >> CPU;
  if (!isLast) input >> IO;
  // Populate the CPU and IO bursts
  burst.CPU = CPU;
  burst.IO = IO;
  // Return burst
  return burst;
}

// Populate event priotity queue
void Simulator::populateQueue() {
  // Populate priority queue based on arrival time
  for (Process &p : processes) {
      for (Thread &t : p.threads) {
          eventQueue.push(Event(t.arrivalTime, THREAD_ARRIVED, t));
      }
  }
}

// Event handler
void Simulator::eventHandler(const Event &event) {
  sysTime = event.etime;
  size_t ioBurstTime;
  Thread *thread = &processes[event.pid].threads[event.tid];

  // Types of events
  switch (event.etype) {
      case THREAD_ARRIVED:
        // Add the thread to the scheduler
        scheduler->add(thread);
        break;
      case THREAD_DISPATCH_COMPLETED:
      case PROCESS_DISPATCH_COMPLETED:
        eventQueue.push(scheduler->execute(sysTime));
        break;
      case CPU_BURST_COMPLETED:
        // Set the performance stats for the burst
        totalServiceTime += (sysTime - thread->prevTime);
        threadScheduledOrRunning = false;
        // Retrieve the burst time
        ioBurstTime = thread->nextIOBurst();
        sysTime += ioBurstTime;
        totalIOTime += ioBurstTime;
        eventQueue.push(Event(sysTime, IO_BURST_COMPLETED, *thread));
        break;
      case IO_BURST_COMPLETED:
        // Add the thread to the scheduler
        scheduler->add(thread);
        break;
      case THREAD_COMPLETED:
        // Set the service time when the thread is completed
        totalServiceTime += (sysTime - thread->prevTime);
        threadScheduledOrRunning = false;
        thread->finishTime = sysTime;
        break;
      case THREAD_PREEMPTED:
        // Set the service time for the thread
        totalServiceTime += (sysTime - thread->prevTime);
        threadScheduledOrRunning = false;
        // Add it back to the scheduler
        scheduler->add(thread);
        break;
      case DISPATCHER_INVOKED:
        Type eventType;
        threadScheduledOrRunning = true;
        // Functionality for thread switches
        if (lastProcessID == event.pid) {
          sysTime += threadSwitchOverhead;
          totalDispatchTime += threadSwitchOverhead;
          eventType = THREAD_DISPATCH_COMPLETED;
        } else {
          // Funtionality for process switches
          lastProcessID = event.pid;
          sysTime += processSwitchOverhead;
          totalDispatchTime += processSwitchOverhead;
          eventType = PROCESS_DISPATCH_COMPLETED;
        }
        // Dispatch the thread
        thread = scheduler->dispatch();
        eventQueue.push(Event(sysTime, eventType, *thread));
        break;
  }
  // Print the appropriate verbose message if the flag is set
  if (verbose) {
    verboseMessage(event);
  }
}

// Retrieves the next event in the priority queue
void Simulator::nextEvent() {
  Event event = eventQueue.top();
  eventQueue.pop();
  eventHandler(event);

  // If the thread is ready, invoke the dispatcher
  if (!threadScheduledOrRunning && scheduler->hasReadyThread()) {
    Thread *thread = scheduler->getNextThread();
    eventQueue.push(Event(event.etime, DISPATCHER_INVOKED, *thread));
    threadScheduledOrRunning = true;
  }

  // Set total time and total idle time
  totalTime = event.etime;
  totalIdleTime = totalTime - totalServiceTime - totalDispatchTime;
}

// Choose scheduling algorithm
void Simulator::chooseSchedule() {
    switch (algorithm) {
        case Algorithm::FCFS:
            scheduler = new FCFS_Algorithm;
            break;
        case Algorithm::RR:
            scheduler = new RR_Algorithm;
            break;
        case Algorithm::PRIORITY:
            scheduler = new Priority_Algorithm;
            break;
        case Algorithm::CUSTOM:
            scheduler = new Custom_Algorithm;
            break;
        default:
            cout << "Algorithm not recognized." << endl;
            exit(EXIT_FAILURE);
    }
}

// Prints the regular output message to the terminal
void Simulator::outputMessage() {
  vector<size_t> counts(4, 0);
  vector<size_t> responseTimes(4, 0);
  vector<size_t> turnaroundTimes(4, 0);

  // Retrieve the statistics for each process
  for (Process &p:processes) {
      for (Thread &t:p.threads) {
          counts[t.ptype]++;
          responseTimes[t.ptype] += t.responseTime;
          turnaroundTimes[t.ptype] += t.turnaroundTime;
      }
  }

  message << fixed << setprecision(2);

  // Retrieve the performance stats per thread priority type
  for (int i = 0; i < 4; i++) {
      size_t total = counts[i];
      double averageResponse;
      double averageTAT;
      // Calculate the average response time and average turnaround time
      if (total) {
          averageResponse = double(responseTimes[i]) / total;
          averageTAT = double(turnaroundTimes[i]) / total;
      } else averageResponse = averageTAT = 0.;

      // Print the stats per priority type
      message << PriorityStr[i] << " THREADS:" << endl
                    << setw(25) << left << "    Total count:"
                    << setw(7) << right << counts[i] << endl
                    << setw(25) << left << "    Avg response time:"
                    << setw(7) << right << averageResponse << endl
                    << setw(25) << left << "    Avg turnaround time:"
                    << setw(7) << right << averageTAT << endl << endl;
  }

  // Print the overall performance stats
  message << setw(25) << left << "Total elapsed time:"
                << setw(7) << right << totalTime << endl
                << setw(25) << left << "Total service time:"
                << setw(7) << right << totalServiceTime << endl
                << setw(25) << left << "Total I/O time:"
                << setw(7) << right << totalIOTime << endl
                << setw(25) << left << "Total dispatch time:"
                << setw(7) << right << totalDispatchTime << endl
                << setw(25) << left << "Total idle time:"
                << setw(7) << right << totalIdleTime << endl << endl;
  message << setw(25) << left << "CPU utilization:"
                << setw(6) << right << cpuUtilization * 100 << "%" << endl
                << setw(25) << left << "CPU efficiency:"
                << setw(6) << right << cpuEfficency * 100 << "%" << endl << endl;

  cout << message.str();
  message.str(string());
}

// Prints the per-thread message
void Simulator::perthreadMessage() {
  // Print the appropriate message for each thread in the process
  for (Process &p:processes) {
      message << "Process " << p.pid << " [" << PriorityStr[p.ptype] << "]:" << endl;
      for (Thread &t:p.threads) {
          message << "    Thread " << t.tid
                  << ":\tARR: " << setw(5) << left << t.arrivalTime
                  << "CPU: " << setw(5) << left << t.totalCPUTime
                  << "I/O: " << setw(5) << left << t.totalIOTime
                  << "TRT: " << setw(5) << left << t.turnaroundTime
                  << "END: " << setw(5) << left << t.finishTime << endl;
      }
      message << endl;
  }
  cout << message.str();
  message.str(string());
}

// Prints the verbose message to terminal
void Simulator::verboseMessage(const Event &event) {
  // Priority types as strings
  const char *PriorityType[] = {"SYSTEM", "INTERACTIVE", "NORMAL", "BATCH"};
  // Create message
  message << "At time " << event.etime << ":" << endl
                 << "    " << event << endl
                 << "    Thread " << event.tid << " in process " << event.pid
                 << " [" << PriorityType[event.ptype] << "]"
                 << endl << "    ";
  // Display message based on event type
  switch (event.etype) {
      case THREAD_ARRIVED:
          message << "Transitioned from NEW to READY";
          break;
      case THREAD_DISPATCH_COMPLETED:
      case PROCESS_DISPATCH_COMPLETED:
          message << "Transitioned from READY to RUNNING";
          break;
      case CPU_BURST_COMPLETED:
          message << "Transitioned from RUNNING to BLOCKED";
          break;
      case IO_BURST_COMPLETED:
          message << "Transitioned from BLOCKED to READY";
          break;
      case THREAD_COMPLETED:
          message << "Transitioned from RUNNING to EXIT";
          break;
      case THREAD_PREEMPTED:
          message << "Transitioned from RUNNING to READY";
          break;
      case DISPATCHER_INVOKED:
          message << scheduler->toString();
          break;
  }
  message << endl << endl;
  // Print message
  cout << message.str();
  message.str(string());
}

// Prints performance statistics to terminal
void Simulator::outputAnalysis() {
    for (Process &p:processes) {
        for (Thread &t:p.threads) {
            t.responseTime = t.startTime - t.arrivalTime;
            t.turnaroundTime = t.finishTime - t.arrivalTime;
        }
    }
    // Calculates CPU performance stats
    cpuEfficency = double(totalServiceTime) / totalTime;
    cpuUtilization = 1.0 - double(totalIdleTime) / totalTime;
}

// Function to start simulation
void Simulator::simulation() {
  // Choose scheduling algorithm
  chooseSchedule();

  // Load input file
  loadFile();

  // Populate priority queue
  populateQueue();

  // Handle events
  while (!eventQueue.empty()) {
    nextEvent();
  }

  // Retrieve statistics
  outputAnalysis();

  // If -t flag, print per-thread analysis
  if (perThread) {
    perthreadMessage();
  }

  // Print "Simulation Completed"
  cout << "SIMULATION COMPLETED!" << endl << endl;

  // Print regular message
  outputMessage();
}
