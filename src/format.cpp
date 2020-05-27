#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    //TODO really there's no library call for this? strftime
     int hh = seconds / 360;
     int mm = (seconds - hh * 360) / 60;
     int ss = (seconds - hh * 360 - mm * 60);
    return string{ ((hh > 9) ? to_string(hh) : ("0"+to_string(hh)) )\
               +":"+((mm > 9) ? to_string(mm) : ("0"+to_string(mm)) )\
               +":"+((ss > 9) ? to_string(ss) : ("0"+to_string(ss)) )};
}