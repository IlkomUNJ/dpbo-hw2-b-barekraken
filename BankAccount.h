#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>

class BankAccount {
private:
    int id;
    int userId;
    double balance;
    std::string createdDate;
    
public:
    BankAccount();
    BankAccount(int id, int userId, double balance);
    
    int getId() const;
    int getUserId() const;
    double getBalance() const;
    std::string getCreatedDate() const;
    
    bool debit(double amount);
    void credit(double amount);
    
    std::string serialize() const;
    static BankAccount deserialize(const std::string& data);
    
    // Setter ini digunakan oleh deserialize
    std::string createdDateSetter(std::string date) { createdDate = date; return createdDate; }
};

#endif