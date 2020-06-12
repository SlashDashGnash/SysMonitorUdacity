#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
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
  string os, kernel;
  string line;
  string unused;
  
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> unused >> kernel;
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
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
    vector<float> memValuesVector = {};
    std::ifstream stream(kProcDirectory + kMeminfoFilename);

    if (stream.is_open()) 
    {
        for(int i = 0; i < 2; i++)
        {
            float size;
            string line;
            string memName;
            std::getline(stream, line);
            std::istringstream linestream(line);
            linestream >> memName >> size;
            memValuesVector.push_back(size);
        }
    }
    return (memValuesVector[0] - memValuesVector[1])/ memValuesVector[0]; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
    long uptime;
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);

    if (stream.is_open()) 
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> uptime;
    }
        return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
    std::string line;
    std::string unused;

    long user, nice, system, irq, softIrq, steal, idle, ioWait;
    long totalIdle, totalNonIdle;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream>>unused>>user>>nice>>system>>idle>>ioWait>>irq>>softIrq>>steal;
    }

    totalIdle = idle + ioWait;
    totalNonIdle = user + nice + system + irq + softIrq + steal;

    return (totalIdle+totalNonIdle); 
}

long LinuxParser::ExtractProcessInformation(int pid, int infoPositionInFile)
{
    std::string line;
    std::string extracted;

    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < infoPositionInFile; i++) 
    {
        linestream >> extracted;
    }
    return stol(extracted);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
vector<long> LinuxParser::ActiveJiffies(int pid) 
{
    long extracted = ExtractProcessInformation(pid, 14) + ExtractProcessInformation(pid, 15)
                    + ExtractProcessInformation(pid, 16) + + ExtractProcessInformation(pid, 17);

    return {extracted, ExtractProcessInformation(pid, 22)};
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{ 
    std::string line;
    std::string unused;

    long user, nice, system, irq, softIrq, steal;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream>>unused>>user>>nice>>system>>unused>>unused>>irq>>softIrq>>steal;
    }
    return (user + nice + system + irq + softIrq + steal); 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
    std::string line;
    std::string unused;

    long idle, ioWait;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream>>unused>>unused>>unused>>unused>>idle>>ioWait;
    }

    return (idle+ioWait); 
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() 
{
    float totalIdle, totalNonIdle, cpuLoad;
    static float prevTotalIdle = 0, prevTotalNonIdle = 0;

    totalIdle = IdleJiffies();
    totalNonIdle = ActiveJiffies();
    cpuLoad = (totalNonIdle - prevTotalNonIdle) / 
        ((totalNonIdle + totalIdle) - (prevTotalNonIdle + prevTotalIdle));

    prevTotalIdle = totalIdle;
    prevTotalNonIdle = totalNonIdle;

    return cpuLoad; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
    std::string line;
    std::string unused;

    int numberOfProcs;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
        while(std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream>>unused;
            if(unused == "processes")
            {   
                linestream>>numberOfProcs;
                return numberOfProcs;
            }
        }
    }
    return -1;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
    std::string line;
    std::string unused;

    int numberOfProcs;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
        while(std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream>>unused;
            if(unused == "procs_running")
            {   
                linestream>>numberOfProcs;
                return numberOfProcs;
            }
            
        }
    }
    return -1; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
    std::string line;
    std::string cmd;

    string uid;

    std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '-', ' ');
    std::istringstream linestream(line);
    linestream>>cmd;
    return cmd;

}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
    return std::to_string(ExtractProcessInformation(pid, 23));
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
    std::string line;
    std::string unused;

    string uid;

    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) 
    {
        while(std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream>>unused;
            if(unused == "Uid:")
            {   
                linestream>>uid;
                return uid;
            }
            
        }
    }
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(std::string uid) 
{ 
    std::string line;
    std::string user;
    string unused;

    string key;

    std::ifstream stream(kPasswordPath);
    if (stream.is_open()) 
    {
        while(std::getline(stream, line))
        {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream>>user>>unused>>key;
            if(key == uid)
            {   
                return user;
            }
        }
            
    }
    return std::to_string(-1);
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
    return ExtractProcessInformation(pid, 22);
}