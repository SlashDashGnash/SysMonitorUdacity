#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int totalSeconds) 
{
    int hours = totalSeconds/3600;
    int minutes = ((int)((float)totalSeconds - hours*3600))/60;
    int seconds = ((int)((float)totalSeconds - minutes*60 - hours*3600));

    string hoursStr = std::to_string(hours);
    string minutesStr = std::to_string(minutes);
    string secondsStr = std::to_string(seconds);

    if(hoursStr.length()<2) hoursStr = "0" + hoursStr;
    if(minutesStr.length()<2) minutesStr = "0" + minutesStr;
    if(secondsStr.length()<2) secondsStr = "0" + secondsStr;

    return hoursStr + ":" + minutesStr + ":" + secondsStr; 
}