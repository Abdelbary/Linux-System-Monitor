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
        long long int pid = std::stoll(filename.c_str());
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
long long int LinuxParser::UpTime() 
{
  std::string line;
  long long int uptime = -1;

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
long long int LinuxParser::Jiffies()
{
  return  LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long long int LinuxParser::ActiveJiffies(int pid) 
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
    for(std::string value ; linestream>>value;)
      if(std::all_of(value.begin(),value.end(),isdigit))
      {
        jiffiesValue.push_back(std::stoul(value.c_str())); 

      }
 
    processActiveJiffies  = jiffiesValue[utime] + jiffiesValue[stime] + jiffiesValue[cutime] +jiffiesValue[cstime];
  }
  
  return processActiveJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long long int LinuxParser::ActiveJiffies() 
{
vector<long long int> values = LinuxParser::CpuUtilization() ;
  return ((values[kUser_]) +
          (values[kNice_]) + 
          (values[kSystem_]) + 
          (values[kIdle_]) + 
          (values[kSoftIRQ_]) + 
          (values[kSteal_]) + 
          (values[kGuest_]) + 
          (values[kGuestNice_]));
}

// TODO: Read and return the number of idle jiffies for the system
long long int LinuxParser::IdleJiffies() 
{
  vector<long long int> values = LinuxParser::CpuUtilization() ;
  return ((values[kIdle_]) + (values[kIOwait_]));
}

// TODO: Read and return CPU utilization
vector<long long int> LinuxParser::CpuUtilization(){ 
  std::string line;
  std::string readSample;
  vector<long long int> cpuUtil;
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
      cpuUtil.push_back(std::stoll(readSample.c_str()));
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
int LinuxParser::RunningProcesses() 
{
  std::string line,processId,state,comm;
  vector<long long int> jiffiesValue;
  int totalProc = -1;
  vector<int> pids = LinuxParser::Pids();
  int runningProc =  0;
  for(int pid : pids)
  {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::istringstream linestream;
  if(stream.is_open())
    {

      std::getline(stream,line);
      linestream.str(line);
      linestream>>processId>>comm>>state; 
      runningProc += (state == "S")?1:0;
    }
  }
  return runningProc;
  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
    std::string cmd;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::istringstream linestream;
  if(stream.is_open())
  {
    std::string line;
    std::getline(stream,line);
    linestream.str(line);

    linestream >> cmd;  
  }
  
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
  std::string key = "Uid:";
  std::string processId;
  std::string word ;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::istringstream linestream;
  if(stream.is_open())
  {
    std::string line ;
    do
    {
       std::getline(stream,line);
       linestream.str(line);
       linestream>>word;
    } 
    while (word != key);

    linestream >> processId;  
  }
  
  return processId;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string uid = LinuxParser::Uid(pid);
  string line, key;
  string username, dummy, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>> username >> dummy >> user;
      if(user == uid)
        return username;
    }
  }
  return "NULL";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long long int LinuxParser::UpTime(int pid) 
{
  long starttime{0};
  long number;
  string line, word;
  vector<long> data;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '(', '_');
    std::replace(line.begin(), line.end(), ')', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);
  
    for (int i=1; i<4; i++) { 
        linestream >> word;
        data.push_back(0);
    }
    for (int i=4; i<26; i++) { 
        linestream >> number;
        data.push_back(number);
    }
    //uptime is numberes at pos 22 and we r working with 0 indexed so pos is 21
    starttime = data[22-1]; 
  }

  return starttime;
}