#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid) {
    //std::cout << "\n Process("<<pid<<")";
    init();
}

bool Process::init() {

    return true;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
    long uptime = LinuxParser::UpTime();
    auto res = LinuxParser::extract_p( string{"/proc/"+to_string(pid_)+"/stat"},
                                       string{"\\-?\\d+\\s+\\(\\w+\\)\\s+\\w+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+(\\-?\\d+)\\s+(\\-?\\d+)\\s+(\\-?\\d+)\\s+(\\-?\\d+)\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+(\\-?\\d+)\\s+\\-?\\d+.*"} );
            //string{"\\d+\\s+\\(\\w+\\)\\s+\\w+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+.*"} );
    char* endS;
    if(res.size() >= 5){
        float utime = strtof(res[0].c_str(), &endS);
        float stime = strtof(res[1].c_str(), &endS);
        float cutime = strtol(res[2].c_str(), &endS, 10);
        float cstime = strtol(res[3].c_str(), &endS, 10);
        float starttime = strtol(res[4].c_str(), &endS, 10);
        float total_time = utime + stime + cutime + cstime;
        float Hertz = sysconf(_SC_CLK_TCK);
        float seconds = uptime - (starttime / Hertz);
        return  ((total_time / Hertz) / seconds);
    }
    return 0.0f; }

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() {
    auto res = LinuxParser::extract_p( string{"/proc/"+to_string(pid_)+"/status"}, string{"VmSize:\\s+(\\d+)\\s+.*"});
    if(res.size())
        return res[0];
    return string("0"); }

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()  const{
    return LinuxParser::UpTime(pid_);
    }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return CpuUtilization() > a.CpuUtilization();
    //return UpTime() > a.UpTime();
     }