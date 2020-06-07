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

enum ProcessCPUStates {
    kUtime_ = 14,
    kStime_ = 15,
    kCutime_ = 16,
    kCstime_ = 17,
    kStarttime_ = 22
};


Process::Process(int pid):pid_(pid) {
    //std::cout << "\n Process("<<pid<<")";
    init();
}

bool Process::init() {

    return true;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

float Process::CpuUtilization() const {
    vector<float> cpuValues{};
    string line;
    float time = 0;
    string value;
    long uptime = LinuxParser::UpTime();
    std::ifstream filestream(string{"/proc/"+to_string(pid_)+"/stat"});
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            for (int i = 1; i <= kStarttime_; i++) {
                linestream >> value;
                if (i == kUtime_ || i == kStime_ || i == kCutime_ || i == kCstime_ ||
                    i == kStarttime_) {
                    time = std::stof(value) / sysconf(_SC_CLK_TCK);
                    cpuValues.push_back(time);
                }
            }
        }
    }
    if(cpuValues.size() == 5){
        float utime = cpuValues[0];
        float stime = cpuValues[1];
        float cutime = cpuValues[2];
        float cstime = cpuValues[3];
        float starttime = cpuValues[4];
        float total_time = utime + stime + cutime + cstime;
        float Hertz = sysconf(_SC_CLK_TCK);
        float seconds = uptime - (starttime / Hertz);
        return  ((total_time / Hertz) / seconds);
    }
    return 0.0;
}
/*
// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
    //return 0.54321;
    long uptime = LinuxParser::UpTime();
    auto res = LinuxParser::extract_p( string{"/proc/"+to_string(pid_)+"/stat"},
                                       string{"\\-?\\d+\\s+\\(\\w+\\)\\s+\\w+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+(\\-?\\d+)\\s+(\\-?\\d+)\\s+(\\-?\\d+)\\s+(\\-?\\d+)\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+\\-?\\d+\\s+(\\-?\\d+)\\s+\\-?\\d+.*"} );
            //string{"\\d+\\s+\\(\\w+\\)\\s+\\w+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+.*"} );


    char* endS1;
    char* endS2;
    char* endS3;
    char* endS4;
    char* endS5;
    if(res.size() >= 5){
        float utime = strtof(res[0].c_str(), &endS1);
        float stime = strtof(res[1].c_str(), &endS2);
        float cutime = strtof(res[2].c_str(), &endS3);
        float cstime = strtof(res[3].c_str(), &endS4);
        float starttime = strtof(res[4].c_str(), &endS5);
        float total_time = utime + stime + cutime + cstime;
        float Hertz = sysconf(_SC_CLK_TCK);
        float seconds = uptime - (starttime / Hertz);
        return  ((total_time / Hertz) / seconds);
    }
    return 0.0f; }

*/

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