#include "Transaction.h"
#include "Utils.h"
#include <sstream>
#include <vector>

Transaction::Transaction() : id(0), buyerId(0), sellerId(0), itemId(0), quantity(0), totalAmount(0.0) {}

Transaction::Transaction(int id, int buyerId, int sellerId, int itemId, int quantity, double totalAmount)
    : id(id), buyerId(buyerId), sellerId(sellerId), itemId(itemId), 
      quantity(quantity), totalAmount(totalAmount), status("paid"), date(Utils::getCurrentDate()) {}

int Transaction::getId() const { return id; }
int Transaction::getBuyerId() const { return buyerId; }
int Transaction::getSellerId() const { return sellerId; }
int Transaction::getItemId() const { return itemId; }
int Transaction::getQuantity() const { return quantity; }
double Transaction::getTotalAmount() const { return totalAmount; }
std::string Transaction::getStatus() const { return status; }
std::string Transaction::getDate() const { return date; }

void Transaction::setStatus(std::string newStatus) { status = newStatus; }

void Transaction::dateSetter(std::string newDate) { date = newDate; } // Implementasi ditambahkan

std::string Transaction::serialize() const {
    return std::to_string(id) + "," + std::to_string(buyerId) + "," + std::to_string(sellerId) + "," +
           std::to_string(itemId) + "," + std::to_string(quantity) + "," + std::to_string(totalAmount) + "," +
           status + "," + date;
}

Transaction Transaction::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    Transaction trans(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]), 
                     std::stoi(tokens[3]), std::stoi(tokens[4]), std::stod(tokens[5]));
    if (tokens.size() > 6) trans.setStatus(tokens[6]); // Diperbaiki dari statusSetter ke setStatus
    if (tokens.size() > 7) trans.dateSetter(tokens[7]); // Menggunakan dateSetter
    return trans;
}