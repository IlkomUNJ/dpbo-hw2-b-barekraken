#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    std::string getCurrentDate();
    std::string getCurrentDateTime();
    int daysBetween(const std::string& date1, const std::string& date2);
}

#endif