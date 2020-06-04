#include "../include/ncurses_display.h"
#include "../include/system.h"
#include "../include/linux_parser.h"
#include <iostream>


int main() {
  //System system;
  std::cout<<LinuxParser::ActiveJiffies(1)<<std::endl;  
  //std::cout<<LinuxParser::UpTime()<<std::endl;
  //std::cout<<LinuxParser::TotalProcesses()<<std::endl;
  //std::cout<<LinuxParser::MemoryUtilization()<<std::endl;
 // LinuxParser::CpuUtilization();
  //NCursesDisplay::Display(system);
}