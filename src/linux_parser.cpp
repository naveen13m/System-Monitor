#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
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
  string os, update, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> update >> kernel;
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

  std::ofstream os("output.txt");
  for(auto& itr : pids)
    os << itr << std::endl;

  os << "----------------" << std::endl;

  closedir(directory);
  return pids;
}

vector<string> LinuxParser::GetValuesofLine(const string &filepath, const string &key) {
  std::ifstream is(filepath);
  string line, curr_key, curr_val;
  vector<string> values;

  if (is.is_open()) {
    while (std::getline(is, line)) {
      std::istringstream iss(line);
      if (!key.empty()) {
        iss >> curr_key;
        if (key == curr_key) {
          while (iss >> curr_val) {
            values.push_back(curr_val);
          }
          break;
        }
      }
      else {
        while (iss >> curr_val) {
          values.push_back(curr_val);
        }
        break;
      }
    }
  }

  return values;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  const string filepath = LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename;
  vector<string> total_mem = LinuxParser::GetValuesofLine(filepath, "MemTotal:");
  vector<string> available_mem = LinuxParser::GetValuesofLine(filepath, "MemAvailable:");

  return ((stof(total_mem.at(0)) - stof(available_mem.at(0))) / stof(total_mem.at(0)));
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  const string filepath = LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename;
  // Passing empty string as argument retuns the entire first line
  vector<string> up_time = LinuxParser::GetValuesofLine(filepath, "");

  return stol(up_time.at(0));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  const string filepath = LinuxParser::kProcDirectory
                   + to_string(pid) + LinuxParser::kStatFilename;

  vector<string> cpu_util = LinuxParser::GetValuesofLine(filepath, "");

  return (stol(cpu_util.at(13)) + stol(cpu_util.at(14)));
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(const vector<long> &run_time) {
  return (run_time.at(LinuxParser::kUser_)
          + run_time.at(LinuxParser::kNice_)
          + run_time.at(LinuxParser::kSystem_)
          + run_time.at(LinuxParser::kIRQ_)
          + run_time.at(LinuxParser::kSoftIRQ_)
          + run_time.at(LinuxParser::kSteal_));
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(const vector<long> &run_time) {
  return (run_time.at(LinuxParser::kIdle_)
          + run_time.at(LinuxParser::kIOwait_));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  const string filepath = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  vector<string> cpu_util = LinuxParser::GetValuesofLine(filepath, "cpu");

//  std::ofstream os("output.txt");
//  for(auto& itr : cpu_util)
//    os << itr << std::endl;
//
//  os << "----------------" << std::endl;

  return cpu_util;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  const string filepath = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  vector<string> total_processes = LinuxParser::GetValuesofLine(filepath, "processes");
  return stoi(total_processes.at(0));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  const string filepath = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  vector<string> processes_running = LinuxParser::GetValuesofLine(filepath, "procs_running");
  return stoi(processes_running.at(0));
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  const string filepath = LinuxParser::kProcDirectory
                          + to_string(pid) + LinuxParser::kCmdlineFilename;

  vector<string> command_segments = LinuxParser::GetValuesofLine(filepath, "");

  if (!command_segments.empty()) {
    return (command_segments.at(0));
  }

  return "";
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  long size_mb;
  long size_kb;

  const string filepath = LinuxParser::kProcDirectory
                          + to_string(pid) + LinuxParser::kStatusFilename;

  auto size = GetValuesofLine(filepath, "VmSize:");

  if (!size.empty()) {
    size_kb = stol(size.at(0));
    size_mb = size_kb / 1024;
  }
  else {
    // Because some processes don't have "VmSize:" in their respective stat files
    // https://unix.stackexchange.com/questions/500212/memory-usage-info-in-proc-pid-status-missing-when-program-is-about-to-terminate
    size_mb = (long)0;
  }

  return to_string(size_mb);
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  const string filepath = LinuxParser::kProcDirectory
                          + to_string(pid) + LinuxParser::kStatusFilename;

  return GetValuesofLine(filepath, "Uid:").at(0);
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::ifstream is(LinuxParser::kPasswordPath);
  string line, user_id, uid;

  if (is.is_open()) {
    while (std::getline(is, line)) {
      std::istringstream ss(line);

      std::getline(ss, user_id,':');
      std::getline(ss, uid,':');
      std::getline(ss, uid,':');

      if (LinuxParser::Uid(pid) == uid) {
        return user_id;
      }
    }
  }

  return "Not Available";
}

// TODO: Read and return the uptime of a process
// This is the process start time
long LinuxParser::StartTime(int pid) {

  const string filepath = LinuxParser::kProcDirectory
                          + to_string(pid) + LinuxParser::kStatFilename;

  long start_time = stol(GetValuesofLine(filepath, "").at(21));
  long start_time_secs = start_time / sysconf(_SC_CLK_TCK);

  return start_time_secs;
}

float LinuxParser::CpuUtilization(int pid) {
  long system_up_time_secs, process_start_time_secs,
      process_utilized_time_secs, available_time_secs;

  system_up_time_secs = LinuxParser::UpTime();
  process_start_time_secs = LinuxParser::StartTime(pid);
  process_utilized_time_secs = LinuxParser::ActiveJiffies(pid) / sysconf(_SC_CLK_TCK);

  available_time_secs = system_up_time_secs - process_start_time_secs;

  return (float)process_utilized_time_secs / (float)available_time_secs;
}