#include "User.h"

User::User() : id(0), bankAccountId(-1) {}

User::User(int id, std::string name, std::string type) 
    : id(id), name(name), type(type), bankAccountId(-1) {}

int User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getType() const { return type; }
int User::getBankAccountId() const { return bankAccountId; }
void User::setBankAccountId(int accId) { bankAccountId = accId; }

std::string User::serialize() const {
    return std::to_string(id) + "," + name + "," + type + "," + std::to_string(bankAccountId);
}

User User::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    User user(std::stoi(tokens[0]), tokens[1], tokens[2]);
    if (tokens.size() > 3) user.setBankAccountId(std::stoi(tokens[3]));
    return user;
}