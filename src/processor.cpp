#include "processor.h"
#include "linux_parser.h"

Processor::Processor()
{

}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
    return LinuxParser::CpuUtilization();
}