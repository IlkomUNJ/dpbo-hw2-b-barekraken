#include "Bank.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <iomanip>

Bank::Bank() : nextAccountId(1), nextTransactionId(1) {
    loadAccounts();
    loadTransactions();
}

Bank::~Bank() {
    saveAccounts();
    saveTransactions();
}

void Bank::loadAccounts() {
    std::ifstream file("data/accounts.csv");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                accounts.push_back(BankAccount::deserialize(line));
            }
        }
        file.close();
        if (!accounts.empty()) {
            // Temukan ID tertinggi untuk menghindari konflik
            int maxId = 0;
            for(const auto& acc : accounts) {
                if(acc.getId() > maxId) maxId = acc.getId();
            }
            nextAccountId = maxId + 1;
        }
    }
}

void Bank::loadTransactions() {
    std::ifstream file("data/bank_history.csv");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                transactions.push_back(BankTransaction::deserialize(line));
            }
        }
        file.close();
        if (!transactions.empty()) {
            int maxId = 0;
            for(const auto& trans : transactions) {
                if(trans.getId() > maxId) maxId = trans.getId();
            }
            nextTransactionId = maxId + 1;
        }
    }
}

void Bank::saveAccounts() {
    std::ofstream file("data/accounts.csv");
    for (const auto& acc : accounts) {
        file << acc.serialize() << std::endl;
    }
    file.close();
}

void Bank::saveTransactions() {
    std::ofstream file("data/bank_history.csv");
    for (const auto& trans : transactions) {
        file << trans.serialize() << std::endl;
    }
    file.close();
}

int Bank::createAccount(int userId, double initialBalance) {
    BankAccount acc(nextAccountId++, userId, initialBalance);
    accounts.push_back(acc);
    saveAccounts();
    return acc.getId();
}

BankAccount* Bank::getAccount(int accountId) {
    for (auto& acc : accounts) {
        if (acc.getId() == accountId) {
            return &acc;
        }
    }
    return nullptr;
}

bool Bank::topup(int accountId, double amount) {
    BankAccount* acc = getAccount(accountId);
    if (acc) {
        acc->credit(amount);
        BankTransaction trans(nextTransactionId++, accountId, "credit", amount, "Topup");
        transactions.push_back(trans);
        saveAccounts();
        saveTransactions();
        return true;
    }
    return false;
}

bool Bank::withdraw(int accountId, double amount) {
    BankAccount* acc = getAccount(accountId);
    if (acc && acc->debit(amount)) {
        BankTransaction trans(nextTransactionId++, accountId, "debit", amount, "Withdrawal");
        transactions.push_back(trans);
        saveAccounts();
        saveTransactions();
        return true;
    }
    return false;
}

bool Bank::transfer(int fromAccountId, int toAccountId, double amount, std::string description) {
    BankAccount* fromAcc = getAccount(fromAccountId);
    BankAccount* toAcc = getAccount(toAccountId);
    
    if (fromAcc && toAcc && fromAcc->debit(amount)) {
        toAcc->credit(amount);
        BankTransaction debitTrans(nextTransactionId++, fromAccountId, "debit", amount, description);
        BankTransaction creditTrans(nextTransactionId++, toAccountId, "credit", amount, description);
        transactions.push_back(debitTrans);
        transactions.push_back(creditTrans);
        saveAccounts();
        saveTransactions();
        return true;
    }
    return false;
}

void Bank::listAllCustomers() {
    std::cout << "\n===== Daftar Semua Pengguna Terdaftar (Urut ID) =====" << std::endl;
    for (const auto& acc : accounts) {
        std::cout << "- ID: " << acc.getId() 
             << ", User ID: " << acc.getUserId()
             << ", Saldo: Rp " << std::fixed << std::setprecision(0) << acc.getBalance() << std::endl;
    }
}

void Bank::listTransactionsLastWeek() {
    std::cout << "\n===== Transaksi Bank 7 Hari Terakhir =====" << std::endl;
    std::string today = Utils::getCurrentDate();
    // STUB: Logika daysBetween() tidak diimplementasikan, jadi tampilkan semua transaksi.
    for (const auto& trans : transactions) {
        std::cout << "ID: " << trans.getId() 
             << ", Account: " << trans.getAccountId()
             << ", Type: " << trans.getType()
             << ", Amount: Rp " << std::fixed << std::setprecision(0) << trans.getAmount()
             << ", Date: " << trans.getDate() << std::endl;
    }
}

void Bank::listDormantAccounts() {
    std::cout << "\n===== Akun Tidak Aktif (Tidak Ada Transaksi 30 Hari) =====" << std::endl;
    std::string today = Utils::getCurrentDate();
    // STUB: Logika daysBetween() tidak diimplementasikan.
    // Ini hanya akan menampilkan akun yang TIDAK PERNAH bertransaksi.
    for (const auto& acc : accounts) {
        bool hasRecentTransaction = false;
        for (const auto& trans : transactions) {
            if (trans.getAccountId() == acc.getId()) {
                hasRecentTransaction = true; // Seharusnya memeriksa tanggal di sini
                break;
            }
        }
        if (!hasRecentTransaction) {
            std::cout << "- Account ID: " << acc.getId() 
                 << ", User ID: " << acc.getUserId()
                 << ", Saldo: Rp " << std::fixed << std::setprecision(0) << acc.getBalance() << std::endl;
        }
    }
}

void Bank::listTopUsers(int n) {
    std::cout << "\n===== Top " << n << " User Transaksi Terbanyak Hari Ini =====" << std::endl;
    std::map<int, int> userTransCount;
    std::string today = Utils::getCurrentDate();
    
    for (const auto& trans : transactions) {
        if (trans.getDate().substr(0, 10) == today) {
            userTransCount[trans.getAccountId()]++;
        }
    }
    
    std::vector<std::pair<int, int>> sortedUsers(userTransCount.begin(), userTransCount.end());
    std::sort(sortedUsers.begin(), sortedUsers.end(), 
         [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
             return a.second > b.second;
         });
    
    int count = 0;
    for (const auto& pair : sortedUsers) {
        if (count >= n) break;
        std::cout << "Account ID: " << pair.first 
             << ", Transaksi: " << pair.second << std::endl;
        count++;
    }
}

std::vector<BankTransaction> Bank::getAccountTransactions(int accountId, std::string period) {
    std::vector<BankTransaction> result;
    std::string today = Utils::getCurrentDate();
    
    for (const auto& trans : transactions) {
        if (trans.getAccountId() == accountId) {
            if (period == "today" && trans.getDate().substr(0, 10) == today) {
                result.push_back(trans);
            } else if (period == "month") {
                // STUB: Logika "month" tidak diimplementasikan
                result.push_back(trans);
            } else if (period == "all") {
                result.push_back(trans);
            }
        }
    }
    return result;
}