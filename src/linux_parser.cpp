#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::for_each;

// DONE: An example of how to read data from the filesystem
// TODO clean this up, use the extract_p
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}
vector<int> LinuxParser::Pids2() {
    vector<int> pids;
    auto pp = fs::path(kProcDirectory.c_str() );
    auto dir_itr = fs::directory_iterator( pp); //fs::current_path());
    for_each( begin(dir_itr), end(dir_itr), [&](auto& p){
        string fn= p.path().filename().string();
        if( fn.end() == find_if( begin(fn), end(fn), [&](char ch){
            return !isdigit(ch);
        })) {
	    pids.push_back( stoi(fn) );
        }
    }) ;
   return pids;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  return Pids2();
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
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    auto res = extract_p(string{"/proc//uptime"}, string{"(.*)\\s+.*"});
    return (res.size()) ? stol(res[0]) : 1;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    auto res_running =  extract_p( std::string{kProcDirectory+kStatFilename}, std::string{"processes\\s+(\\d+).*"});
    return (res_running.size() > 0) ? stoi(res_running[0]) : -100;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    auto res_running =  extract_p( std::string{kProcDirectory+kStatFilename}, std::string{"procs_running\\s+(\\d+).*"});
    return (res_running.size() > 0) ? stoi(res_running[0]) : -100;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    auto res = extract_p(string{"/proc/" + to_string(pid) + "/cmdline"}, string{"(.*)"});
    //TODO clamp to cap the max.
    return (res.size()) ? res[0] : "unknown cmd";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    auto res_uuid = extract_p( string{"/proc/"+to_string(pid)+"/status"},
                               string{"Uid:\\s+(\\d+)\\s+.*"});
    if(res_uuid.size())
        return res_uuid[0];

    return string("0000"); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    auto res_uuid = extract_p( string{"/proc/"+to_string(pid)+"/status"},
            string{"Uid:\\s+(\\d+)\\s+.*"});
    if(res_uuid.size()){
        string uuid = res_uuid[0];
        auto res_usr = extract_p(string{"/etc/passwd"}, string{"(.*):x:"}+uuid+":"+uuid+":.*");
        if(res_usr.size())
            return res_usr[0];
    }
    return string("nouser"); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    // if file does not exist return -1?

    // get the latest from /proc/{PID}/stat column #22
    auto res = extract_p( string{"/proc/"+to_string(pid)+"/stat"},
           string{"[\\d+\\s+]{21}(\\d+)\\s+.*"} );
    if(res.size()){
        char* ends;
        return strtol(res[0].c_str(), &ends, 10)  /  sysconf(_SC_CLK_TCK);
    }

    return 0.0;
}

/*
 * Given a file path and a regex pattern extract matches.
 * Return the first match only.
 */
std::vector<std::string> LinuxParser::extract_p(std::string path, std::string pattern){

    std::string line;
    std::vector<string> results;
    std::ifstream f(path);
    if (f.is_open()) {
        while (std::getline(f, line)) {
            std::regex rgx{pattern};
            std::cmatch match;
            if (regex_search(line.c_str(), match, rgx))
                return std::vector<string>{next(begin(match)), end(match)};
                //results.push_back(match[1]);
        }
    }
    return results;
}

std::string LinuxParser::extract(std::string path, std::string pattern){
  std::string extracted;
  std::ifstream f(path, std::ios::in | std::ios::binary);

  // Obtain the size of the file.
  const auto sz = fs::file_size( fs::path(path.c_str()) );

  // Create a buffer.
  std::string result(sz, '\0');

  // Read the whole file into the buffer.
  f.read(result.data(), sz);

  std::regex rgx(pattern);
  std::cmatch match;
  //std::basic_regex match;
  if (std::regex_search(result.c_str(), match , rgx)) {
      //std::cout << "match: " << match[1];
      extracted = match[1];
  }
  return extracted;
}