#include <string>
#include "iostream"

#include "format.h"

#define SECS_TO_MINS  (uint)(60)
#define MINS_TO_HOURS (uint)(60)

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds [[maybe_unused]]) {
  uint hours, mins, secs;

  secs = seconds % SECS_TO_MINS;
  mins = seconds / SECS_TO_MINS;
  hours = mins / MINS_TO_HOURS;
  mins = mins % MINS_TO_HOURS;

  return (ToString(hours) + ":" + ToString(mins) + ":" + ToString(secs));
}

inline std::string Format::ToString(uint num) {
  return (num < 10) ? ("0" + std::to_string(num)) : std::to_string(num);
}
