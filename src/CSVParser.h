#pragma once
#include <string>
#include <vector>
#include "Performance.h"


std::vector<std::string> splitLine(const std::string& line);
std::vector<Performance> loadPerformances(const std::string& path);