#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    int id;
    int buyerId;
    int sellerId;
    int itemId;
    int quantity;
    double totalAmount;
    std::string status;
    std::string date;

public:
    Transaction();
    Transaction(int id, int buyerId, int sellerId, int itemId, int quantity, double totalAmount);

    int getId() const;
    int getBuyerId() const;
    int getSellerId() const;
    int getItemId() const;
    int getQuantity() const;
    double getTotalAmount() const;
    std::string getStatus() const;
    std::string getDate() const;

    void setStatus(std::string newStatus);
    void dateSetter(std::string newDate); // Ditambahkan untuk deserialize

    std::string serialize() const;
    static Transaction deserialize(const std::string& data);
};

#endif