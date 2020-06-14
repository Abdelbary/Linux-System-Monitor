#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    string time = "";
    time += std::to_string(seconds/3600);
    time += ":";
    seconds %=3600;
    time += std::to_string(seconds/60);
    time += ":";
    seconds %=60;
    time += std::to_string(seconds);
    return time;
 }