#include "Bank.h"
#include "OnlineStore.h"
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>  // Add this include
#ifdef _WIN32
#include <direct.h>
#endif

// Forward declaration for mainMenu to avoid requiring Menus.h here
void mainMenu(OnlineStore* store);

// Function to create directory
bool createDirectory(const std::string& path) {
#ifdef _WIN32
    return mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

int main() {
    // Create data directory if it doesn't exist
    createDirectory("data");
    
    Bank bank;
    OnlineStore store(&bank);
    
    std::cout << "========================================" << std::endl;
    std::cout << "  SISTEM SIMULASI TOKO ONLINE & BANK   " << std::endl;
    std::cout << "========================================" << std::endl;
    
    mainMenu(&store);
    
    return 0;
}