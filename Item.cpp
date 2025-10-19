#include "Item.h"
#include <sstream>
#include <vector>

Item::Item() : id(0), sellerId(0), price(0.0), stock(0) {}

Item::Item(int id, int sellerId, std::string name, double price, int stock)
    : id(id), sellerId(sellerId), name(name), price(price), stock(stock) {}

int Item::getId() const { return id; }
int Item::getSellerId() const { return sellerId; }
std::string Item::getName() const { return name; }
double Item::getPrice() const { return price; }
int Item::getStock() const { return stock; }

void Item::setPrice(double newPrice) { price = newPrice; }

void Item::addStock(int amount) { stock += amount; }

bool Item::removeStock(int amount) {
    if (stock >= amount) {
        stock -= amount;
        return true;
    }
    return false;
}

std::string Item::serialize() const {
    return std::to_string(id) + "," + std::to_string(sellerId) + "," + name + "," + 
           std::to_string(price) + "," + std::to_string(stock);
}

Item Item::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    return Item(std::stoi(tokens[0]), std::stoi(tokens[1]), tokens[2], 
                std::stod(tokens[3]), std::stoi(tokens[4]));
}