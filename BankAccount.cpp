#include "BankAccount.h"
#include "Utils.h"
#include <sstream>
#include <vector>
#include <string>

BankAccount::BankAccount() : id(0), userId(0), balance(0.0) {
    createdDate = Utils::getCurrentDate();
}

BankAccount::BankAccount(int id, int userId, double balance)
    : id(id), userId(userId), balance(balance) {
    createdDate = Utils::getCurrentDate();
}

int BankAccount::getId() const { return id; }
int BankAccount::getUserId() const { return userId; }
double BankAccount::getBalance() const { return balance; }
std::string BankAccount::getCreatedDate() const { return createdDate; }

bool BankAccount::debit(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

void BankAccount::credit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

std::string BankAccount::serialize() const {
    return std::to_string(id) + "," + std::to_string(userId) + "," + 
           std::to_string(balance) + "," + createdDate;
}

BankAccount BankAccount::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    // Buat akun dengan data yang di-parse
    BankAccount acc(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stod(tokens[2]));
    
    // Set tanggal jika ada di file (tokens.size() > 3)
    if (tokens.size() > 3) {
        acc.createdDateSetter(tokens[3]);
    }
    return acc;
}