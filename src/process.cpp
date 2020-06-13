#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "../include/linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const  { return this->pid_;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const
{

   
    float processTimeInSec = Process::UpTime();
    float processActiveTimeInSec = (LinuxParser::ActiveJiffies(this->pid_)/ (float)sysconf(_SC_CLK_TCK));

    float cpu_usage = (processActiveTimeInSec/processTimeInSec);

}

// TODO: Return the command that generated this process
string Process::Command() const
 {
      return LinuxParser::Command(this->pid_);
 }

// TODO: Return this process's memory utilization
string Process::Ram() const 
{
    string mem_util_mb = to_string((int)((float) stol(LinuxParser::Ram(this->pid_), nullptr, 10) / 1024.0));
    return mem_util_mb;
}


// TODO: Return the user (name) that generated this process
string Process::User() const  { return LinuxParser::User(this->pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const
{
    //get the up time of the process
    //get the uptime of the system
    //calcuate  total elapsed time in seconds = (sysuptime - processuptime)/Hertz
    float processUpTime = LinuxParser::UpTime(this->pid_);
    float systemUpTime  = LinuxParser::UpTime();
    float processTimeInSec = systemUpTime - (processUpTime/ (float) sysconf(_SC_CLK_TCK)) ;
    return (long) processTimeInSec;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{
     return a.Process::CpuUtilization() > this->Process::CpuUtilization()? true : false ;
}