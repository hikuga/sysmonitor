#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

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
    //return processes_;
    processes_.clear();
    auto pids = LinuxParser::Pids2();

    for_each(begin(pids), end(pids), [&](int pid){
       processes_.emplace_back( Process(pid) );
    });
    std::sort(begin(processes_), end(processes_));
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    return cpu_.MemUsage();

}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    //return "NO-OS";
    return LinuxParser::OperatingSystem();
    }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();

}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime(); }