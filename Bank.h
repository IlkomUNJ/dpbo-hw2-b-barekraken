#ifndef BANK_H
#define BANK_H

#include "BankAccount.h"
#include "BankTransaction.h"
#include <vector>
#include <string>

class Bank {
private:
    std::vector<BankAccount> accounts;
    std::vector<BankTransaction> transactions;
    int nextAccountId;
    int nextTransactionId;

    void loadAccounts();
    void loadTransactions();
    void saveAccounts();
    void saveTransactions();

public:
    Bank();
    ~Bank();

    int createAccount(int userId, double initialBalance = 0.0);
    BankAccount* getAccount(int accountId);
    bool topup(int accountId, double amount);
    bool withdraw(int accountId, double amount);
    bool transfer(int fromAccountId, int toAccountId, double amount, std::string description = "Transfer");
    
    void listAllCustomers();
    void listTransactionsLastWeek(); // Logika stubbed
    void listDormantAccounts(); // Logika stubbed
    void listTopUsers(int n);
    
    std::vector<BankTransaction> getAccountTransactions(int accountId, std::string period = "all");
};

#endif