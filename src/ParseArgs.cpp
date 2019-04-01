// Jane Lockshin
// Parses command line arguments

#include "ParseArgs.h"
#include <iostream>
#include <cstring>
#include <getopt.h>

using namespace std;

// The command line arguments that can be used when calling the program
const struct option flagDetails[] = {
    {"per_thread", no_argument,       0, 't'},
    {"verbose",    no_argument,       0, 'v'},
    {"help",       no_argument,       0, 'h'},
    {"algorithm",  required_argument, 0, 'a'},
    {0, 0,                            0, 0}};

// The short version of the command line arguments
const char *flags = "tvha:";

// Function to validate the scheduling algorithms
Algorithm selectSchedulingAlgorithm(char *str) {
  // First-come, First-served
  if (!strcmp("FCFS", str)) {
      return FCFS;
  }
  // Round robin
  if (!strcmp("RR", str)) {
      return RR;
  }
  // Process-priority
  if (!strcmp("PRIORITY", str)) {
      return PRIORITY;
  }
  // Custom algorithm
  if (!strcmp("CUSTOM", str)) {
      return CUSTOM;
  }
  // Print error message if the algorithm is unknown
  cerr << "Error: Unknown algorithm: " << str << endl;
  exit(EXIT_FAILURE);
}

// Function to print the 'help' message to the terminal
void printHelp() {
    cout << "Program to run a scheduler simulation." << endl << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help:" << endl << "\tPrint this help message for more information." << endl;
    cout << "  -t, --per_thread:" << endl << "\tOutput additional per-thread statistics." << "at the end of the simulation."
         << endl;
    cout << "  -v, --verbose:" << endl << "\tOutput information about every state-changing event and scheduling decision." << endl;
    cout << "  -a, --algorithm <algorithm>:" << endl << "\tThe scheduler algorithm to use. One of:" << endl;
    cout << "  \tFCFS: First-come, First-serve (default)" << endl
         << "  \tRR: Round-Robin scheduler" << endl
         << "  \tPRIORITY: Priority Scheduler" << endl
         << "  \tCUSTOM: Custom algorithm" << endl;
}

// Function to parse command line arguments
Scheduler parseArgs(int argc, char **argv) {
    Scheduler result;
    int flag = 0;
    int option = 0;
    while ((flag = getopt_long(argc, argv, flags, flagDetails, &option)) != -1) {
        switch (flag) {
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            case 't':
                result.perThread = true;
                break;
            case 'v':
                result.verbose = true;
                break;
            case 'a':
                result.algorithm = selectSchedulingAlgorithm(optarg);
                break;
            case '?':
                printHelp();
                exit(1);
            default:
                abort();
        }
    }

    // If the format is incorrect, print an error message
    if ((argc - optind) != 1) {
        cerr << "Incorrect commandline format." << endl;
        printHelp();
        exit(EXIT_FAILURE);
    }

    // Save the input file
    result.inputFile = argv[argc - 1];

    return result;
}
