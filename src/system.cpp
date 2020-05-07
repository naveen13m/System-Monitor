#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <fstream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  this->processes_.clear();

  for (int pid : pids) {
    Process process(pid);
    this->processes_.push_back(process);
  }

  std::sort(this->processes_.begin(), this->processes_.end());

  return this->processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  // Since the system kernel does not change every time, it can be updated only once
  return kernel_.empty() ? LinuxParser::Kernel() : kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
  // Since the system name does not change every time, it can be updated only once
  return os_name_.empty() ? LinuxParser::OperatingSystem() : os_name_;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}