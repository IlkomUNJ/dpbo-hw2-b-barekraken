#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
private:
    int id;
    int sellerId;
    std::string name;
    double price;
    int stock;

public:
    Item();
    Item(int id, int sellerId, std::string name, double price, int stock);

    int getId() const;
    int getSellerId() const;
    std::string getName() const;
    double getPrice() const;
    int getStock() const;

    void setPrice(double newPrice);
    void addStock(int amount);
    bool removeStock(int amount);

    std::string serialize() const;
    static Item deserialize(const std::string& data);
};

#endif