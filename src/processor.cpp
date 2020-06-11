#include "processor.h"
#include "../include/linux_parser.h"
#include <chrono>
#include <thread>

// all info are in /proc/stat , you will need something about jiffies!!
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
   float totalTime = LinuxParser::Jiffies();
  float activeTime = LinuxParser::ActiveJiffies() ;
  float result = 1.0 * (activeTime/totalTime);
  
  std::this_thread::sleep_for(std::chrono::seconds(1));

  totalTime = LinuxParser::Jiffies() - totalTime;
  activeTime = LinuxParser::ActiveJiffies() - activeTime;

  return activeTime/totalTime;

 }