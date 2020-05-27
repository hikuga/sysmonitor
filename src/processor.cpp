#include "processor.h"
#include "linux_parser.h"
#include <algorithm>
#include <iterator>

using std::string;
using std::vector;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    //return 0.2;
    std::string cpu_pattern{"cpu\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)"};
    std::vector<std::string> extracted_vals = LinuxParser::extract_p( std::string{"/proc/stat"}, cpu_pattern);
    std::vector<long> curr_(extracted_vals.size(),0);

    // these are not longer than longs?
    char* ends;
    std::transform( begin(extracted_vals), end(extracted_vals), begin(curr_), [&](std::string elem){
        return std::strtol(elem.c_str(), &ends, 10);});

    float utilization = 0.0;
    if(prev_.size() ){
     // ref. 0,user    1,nice   2,system  3,idle  4,iowait  5,irq  6,softirq  7,steal  8,guest  9,guest_nice
     /*
      * PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
      */
     float prevIdle = prev_[3] + prev_[4];
     float idle = curr_[3] + curr_[4];
     float prevNonIdle = prev_[0] + prev_[1] + prev_[2] + prev_[5] + prev_[6] + prev_[7];
     float nonIdle = curr_[0] + curr_[1] + curr_[2] + curr_[5] + curr_[6] + curr_[7];
     float totald = idle+nonIdle - (prevIdle + prevNonIdle);
     float idled = idle - prevIdle;
     utilization = (totald != 0) ? float(totald - idled) / float(totald) : -0.1;

    }
    prev_=curr_;

    return utilization;
}

float Processor::MemUsage() {
    auto mem_total = LinuxParser::extract_p(string{"/proc/meminfo"}, string{"\\s*MemTotal:\\s+(\\d+)\\s+.*"});
    auto mem_free = LinuxParser::extract_p(string{"/proc/meminfo"}, string{"\\s*MemFree:\\s+(\\d+)\\s+.*"});

    char* endS;
    if( mem_total.size() && mem_free.size() )
        return 1.0 - strtof(mem_free[0].c_str(), &endS) / strtof(mem_total[0].c_str(), &endS);

    return 0.0;

}