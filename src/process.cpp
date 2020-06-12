#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
{
    pid_ = pid;
}

// TODO: Return this process's ID
int Process::GetPid() 
{ 
    return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    float cpuLoad = 0;
    long totalTime = LinuxParser::ActiveJiffies(pid_)[0] - lastActiveJif_;
    long startTime = LinuxParser::ActiveJiffies(pid_)[1];
    long upTime = LinuxParser::UpTime();
    float seconds = (upTime - ((float)startTime/(float)sysconf(_SC_CLK_TCK))) - lastTotalTime_;

    cpuLoad = (float)totalTime/(float)sysconf(_SC_CLK_TCK)/seconds;

    lastActiveJif_ = totalTime;
    lastTotalTime_ = seconds;
    cpuLoad_ = cpuLoad;
    return (cpuLoad);
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{ 
    string ramUsed = LinuxParser::Ram(pid_);
    ram_ = stol(ramUsed)/1024/1024;
    return std::to_string(ram_);
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{
    return LinuxParser::User(LinuxParser::Uid(pid_));
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{
    return  LinuxParser::UpTime() - LinuxParser::UpTime(pid_); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{  
    return  a.cpuLoad_ < cpuLoad_;
}