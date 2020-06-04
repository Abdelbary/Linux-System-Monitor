#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "../include/linux_parser.h"




using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel , os_version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os  >>os_version >> kernel; 
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memtotal,memFree,memAvailable,Buffers,cached;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if(stream.is_open())
  {
    string key,line,alis;
    int value;
    //get memtotal info
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> alis >> memtotal;
    //get memfree
    std::getline(stream,line);
    linestream.str(line);
    linestream >> alis >> memFree;
    //get memAvialble
    std::getline(stream,line);
    linestream.str(line);
    linestream >> alis >> memAvailable;
    //get Buffers info
    std::getline(stream,line);
    linestream.str(line);
    linestream >> alis >> Buffers;
    //get cached info
    std::getline(stream,line);
    linestream.str(line);
    linestream >> alis >> cached;
  }
   return (memtotal- memFree)/memtotal; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  std::string line;
  long uptime = -1;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>uptime;
  }

  return uptime;
}
 

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  //get system process ids 
  //for each process git total jiffs
  //sum all the jiffs
  //get all system process ids
  vector<int> pids = LinuxParser::Pids();
  std::string line,processId,state,comm;
  int totalProc = -1;
  //number of skiiped values in the start of the file
  //pid , comm , state
  int skipped_values = 3;

  //position of the process user time and kernel time for the process
  //and its children 
  int utime = 13  - skipped_values;
  int stime = 14  - skipped_values;
  int cutime = 15 - skipped_values;
  int cstime = 16 - skipped_values;
   // active jiffies is the sum of [utime + stime + cutime + cstime]
  long long int Jiffies;
  for(int Id : pids)
  {

  std::ifstream stream(kProcDirectory + std::to_string(id) + kStatFilename);
  std::istringstream linestream;
  if(stream.is_open())
  {
    std::getline(stream,line);
    linestream.str(line);
    linestream>>processId>>comm>>state;
    for(long long int value ; linestream>>value;)
      Jiffies += value; 
  }
  
  }
  return Jiffies; 


}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{
  std::string line,processId,state,comm;
  vector<long long int> jiffiesValue;
  int totalProc = -1;
  //number of skiiped values in the start of the file
  //pid , comm , state
  int skipped_values = 3;

  //position of the process user time and kernel time for the process
  //and its children 
  int utime = 13  - skipped_values;
  int stime = 14  - skipped_values;
  int cutime = 15 - skipped_values;
  int cstime = 16 - skipped_values;
   // active jiffies is the sum of [utime + stime + cutime + cstime]
  long long int processActiveJiffies;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::istringstream linestream;
  if(stream.is_open())
  {

    std::getline(stream,line);
    linestream.str(line);
    linestream>>processId>>comm>>state;
    for(long long int value ; linestream>>value;)
      jiffiesValue.push_back(value); 
 
    processActiveJiffies  = jiffiesValue[utime] + jiffiesValue[stime] + jiffiesValue[cutime] +jiffiesValue[cstime];
  }
  
  return processActiveJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
long totalActiveJiffies = 0;
vector<int> pids = LinuxParser::Pids();
for(int pid : pids)
  totalActiveJiffies += LinuxParser::ActiveJiffies(pid);

return totalActiveJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  
  return LinuxParser::Jiffies()-LinuxParser::ActiveJiffies();
}

// TODO: Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() { 
  std::string line;
  std::string readSample;
  vector<int> cpuUtil;
  std::ifstream stream(kProcDirectory + kStatFilename);
  //data about cpu utiliztaotion is tored in /proc/stat
  //we are intersted in the first line where aggrigated cpu
  //utiliztion is listed in 10 spereated numbers
  if(stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> readSample;
    for(int i = 0 ; i  < 10 ; i++)
    {
      linestream >> readSample;
      cpuUtil.push_back(stoi(readSample));
    }
      
  }

  return cpuUtil;
   }


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line,word;
  const std::string key{"processes"};
  std::string readSample;
  int totalProc = -1;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::istringstream linestream;
  if(stream.is_open())
  {
    do
    {
       std::getline(stream,line);
       linestream.str(line);
       linestream>>word;
    } while (word != key);

    linestream >> totalProc;  
  }

  return totalProc;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }