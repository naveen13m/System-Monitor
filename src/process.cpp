#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
Process::Process(int pid) : pid_(pid) {
  this->cpu_util_ = this->CpuUtilization();
}

int Process::Pid() {
  return this->pid_;
}

//void Process::SetPid(int pid) {
//  this->pid_ = pid;
//}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return LinuxParser::CpuUtilization(this->pid_);
}

// TODO: Return the command that generated this process
string Process::Command() {
  //return !startup_command_.empty() ? LinuxParser::Command(this->pid_) : startup_command_;
  startup_command_ = LinuxParser::Command(this->pid_);
  return startup_command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  this->ram_ = LinuxParser::Ram(this->pid_);
  return (this->ram_);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  return LinuxParser::User(this->pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  // System uptime - process start time
  return LinuxParser::UpTime() - LinuxParser::StartTime(this->pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->cpu_util_ > a.cpu_util_;
}