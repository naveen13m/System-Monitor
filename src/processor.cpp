#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::stol;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  const int num_cpu_states = 10;
  static vector<long> prev_run_time(num_cpu_states, 0);
  vector<long> curr_run_time(num_cpu_states, 0);
  long prev_idle_time, idle_time;
  long nonidle_time, prev_nonidle_time;
  long delta_nonidle, delta_idle, delta_total;

  vector<string> string_run_time = LinuxParser::CpuUtilization();

  for (int index = 0; index < num_cpu_states; index++) {
    curr_run_time.at(index) = stol(string_run_time.at(index));
  }

  idle_time = LinuxParser::IdleJiffies(curr_run_time);
  prev_idle_time = LinuxParser::IdleJiffies(prev_run_time);

  nonidle_time = LinuxParser::ActiveJiffies(curr_run_time);
  prev_nonidle_time = LinuxParser::ActiveJiffies(prev_run_time);

  delta_nonidle = nonidle_time - prev_nonidle_time;
  delta_idle = idle_time - prev_idle_time;
  delta_total = delta_nonidle + delta_idle;

  prev_run_time = curr_run_time;

  return (float)(delta_nonidle) / (float)(delta_total);
}