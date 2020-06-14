#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

//Return the system's CPU
Processor& System::Cpu() { return cpu_; }

///return an array of all process to display them in the monitor main screen
vector<Process>& System::Processes() {
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        processes_.push_back(Process(pid));
    }
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());
    return processes_;
}

std::string System::Kernel() { 
    return LinuxParser::Kernel();
 }

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
 }

std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses();
 }

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
 }

long int System::UpTime() { 
    return (long int) LinuxParser::UpTime();
 }