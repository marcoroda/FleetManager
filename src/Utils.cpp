#include <iostream>
#include <vector>
#include <sstream>
#include <FleetManager/Utils.h>
#include "imgui.h"

namespace Utils {

std::vector<int> split(const std::string& str, char delim)
{
    std::vector<int> result;
    std::stringstream ss(str);
    std::string item;

    if (!str.empty()) {
        while (getline(ss, item, delim)) {
            result.push_back(std::stoi(item));
        }
    }
    return result;
}

} // namespace utils
