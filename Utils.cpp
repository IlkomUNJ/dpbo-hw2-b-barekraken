#include "Utils.h"
#include <ctime>
#include <cstdio>

namespace Utils {

std::string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return std::string(buffer);
}

std::string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[20];
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", 
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return std::string(buffer);
}

int daysBetween(const std::string& date1, const std::string& date2) {
    // Implementasi yang disederhanakan/stub.
    // Implementasi penuh memerlukan parsing tanggal string ke time_t.
    return 0; 
}

} // namespace Utils