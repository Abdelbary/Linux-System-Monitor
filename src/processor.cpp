#include "processor.h"
#include "../include/linux_parser.h"
#include <chrono>
#include <thread>

// all info are in /proc/stat , you will need something about jiffies!!
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
   vector<long long int> values = std::move(LinuxParser::CpuUtilization());
   float totalTime = LinuxParser::Jiffies(values);
  float activeTime = LinuxParser::ActiveJiffies(values) ;
  
  //std::this_thread::sleep_for(std::chrono::seconds(1));

  //vector<long long int> v2 = std::move(LinuxParser::CpuUtilization());
 // totalTime = LinuxParser::Jiffies(v2) - totalTime;
  //activeTime = LinuxParser::ActiveJiffies(v2) - activeTime;

  return activeTime/totalTime;

 }