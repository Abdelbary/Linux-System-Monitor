#include "../include/ncurses_display.h"
#include "../include/system.h"
#include "../include/linux_parser.h"
#include <iostream>

#if Debug
int main() {
  //System system;
  std::cout<<LinuxParser::ActiveJiffies(1)<<std::endl;  
  std::cout<<LinuxParser::Jiffies()<<std::endl; 
  std::cout<<LinuxParser::ActiveJiffies()<<std::endl; 
  std::cout<<LinuxParser::IdleJiffies()<<std::endl;  
  std::cout<<"-----------------------------------------"<<std::endl;  
  std::cout<<LinuxParser::RunningProcesses()<<std::endl;  
  std::cout<<LinuxParser::Uid(1)<<std::endl;  
  std::cout<<LinuxParser::User(1)<<std::endl;  
  std::cout<<LinuxParser::Command(1)<<std::endl;  
  //std::cout<<LinuxParser::UpTime()<<std::endl;
  //std::cout<<LinuxParser::TotalProcesses()<<std::endl;
  //std::cout<<LinuxParser::MemoryUtilization()<<std::endl;
 // LinuxParser::CpuUtilization();
  //NCursesDisplay::Display(system);
}
#endif