#include "BankTransaction.h"
#include "Utils.h"
#include <sstream>
#include <vector>

BankTransaction::BankTransaction() : id(0), accountId(0), amount(0.0) {}

BankTransaction::BankTransaction(int id, int accountId, std::string type, double amount, std::string desc)
    : id(id), accountId(accountId), type(type), amount(amount), 
      date(Utils::getCurrentDateTime()), description(desc) {}

int BankTransaction::getId() const { return id; }
int BankTransaction::getAccountId() const { return accountId; }
std::string BankTransaction::getType() const { return type; }
double BankTransaction::getAmount() const { return amount; }
std::string BankTransaction::getDate() const { return date; }
std::string BankTransaction::getDescription() const { return description; }

void BankTransaction::dateSetter(std::string newDate) { date = newDate; } // Implementasi ditambahkan

std::string BankTransaction::serialize() const {
    return std::to_string(id) + "," + std::to_string(accountId) + "," + type + "," + 
           std::to_string(amount) + "," + date + "," + description;
}

BankTransaction BankTransaction::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    BankTransaction trans(std::stoi(tokens[0]), std::stoi(tokens[1]), tokens[2], 
                         std::stod(tokens[3]), tokens.size() > 5 ? tokens[5] : "");
    if (tokens.size() > 4) trans.dateSetter(tokens[4]); // Menggunakan dateSetter
    return trans;
}