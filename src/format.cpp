#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
     int hh = seconds / 360;
     int mm = (seconds - hh * 360) / 60;
     int ss = (seconds - hh * 360 - mm * 60);
    return string{to_string(hh)+":"+to_string(mm)+":"+to_string(ss)};
}