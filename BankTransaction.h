#ifndef BANKTRANSACTION_H
#define BANKTRANSACTION_H

#include <string>

class BankTransaction {
private:
    int id;
    int accountId;
    std::string type;
    double amount;
    std::string date;
    std::string description;

public:
    BankTransaction();
    BankTransaction(int id, int accountId, std::string type, double amount, std::string desc);

    int getId() const;
    int getAccountId() const;
    std::string getType() const;
    double getAmount() const;
    std::string getDate() const;
    std::string getDescription() const;
    
    void dateSetter(std::string newDate); // Ditambahkan untuk deserialize

    std::string serialize() const;
    static BankTransaction deserialize(const std::string& data);
};

#endif