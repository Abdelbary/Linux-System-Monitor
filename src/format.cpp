#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
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