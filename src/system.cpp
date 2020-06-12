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

System::System(){}

// TODO: Return the system's CPU
Processor& System::Cpu() 
{ 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{ 
    vector<int> pidList = LinuxParser::Pids();
    vector<Process> processList;
    for(int i = 0; i < pidList.size(); i++)
    {
        bool pidFound = false;
        for(int j = 0; j < processes_.size(); j++)
        {
            if(processes_[j].GetPid() == pidList[i])
            {
                pidFound = true;
                break;
            }
        }
        
        if(pidFound == false)
        {
            processes_.push_back(Process(pidList[i]));
        }
    }
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() 
{ 
    return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() 
{ 
    return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() 
{ 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() 
{ 
    return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() 
{ 
    return LinuxParser::UpTime(); 
}