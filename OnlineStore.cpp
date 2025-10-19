#include "OnlineStore.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <iomanip>

OnlineStore::OnlineStore(Bank* bankPtr) : bank(bankPtr), nextUserId(1), nextItemId(1), nextTransactionId(1001) {
    loadUsers();
    loadItems();
    loadTransactions();
}

OnlineStore::~OnlineStore() {
    saveUsers();
    saveItems();
    saveTransactions();
}

void OnlineStore::loadUsers() {
    std::ifstream buyerFile("data/users_buyers.csv");
    if (buyerFile.is_open()) {
        std::string line;
        while (std::getline(buyerFile, line)) {
            if (!line.empty()) {
                users.push_back(User::deserialize(line));
            }
        }
        buyerFile.close();
    }
    
    std::ifstream sellerFile("data/users_sellers.csv");
    if (sellerFile.is_open()) {
        std::string line;
        while (std::getline(sellerFile, line)) {
            if (!line.empty()) {
                users.push_back(User::deserialize(line));
            }
        }
        sellerFile.close();
    }
    
    if (!users.empty()) {
        // Temukan ID tertinggi untuk menghindari konflik
        int maxId = 0;
        for(const auto& user : users) {
            if(user.getId() > maxId) maxId = user.getId();
        }
        nextUserId = maxId + 1;
    }
}

void OnlineStore::loadItems() {
    std::ifstream file("data/stock_2.csv");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                items.push_back(Item::deserialize(line));
            }
        }
        file.close();
        if (!items.empty()) {
            int maxId = 0;
            for(const auto& item : items) {
                if(item.getId() > maxId) maxId = item.getId();
            }
            nextItemId = maxId + 1;
        }
    }
}

void OnlineStore::loadTransactions() {
    std::ifstream file("data/transaction_items_1001.csv");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                transactions.push_back(Transaction::deserialize(line));
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

void OnlineStore::saveUsers() {
    std::ofstream buyerFile("data/users_buyers.csv");
    std::ofstream sellerFile("data/users_sellers.csv");
    
    for (const auto& user : users) {
        if (user.getType() == "Pembeli") {
            buyerFile << user.serialize() << std::endl;
        } else if (user.getType() == "Penjual") {
            sellerFile << user.serialize() << std::endl;
        }
    }
    
    buyerFile.close();
    sellerFile.close();
}

void OnlineStore::saveItems() {
    std::ofstream file("data/stock_2.csv");
    for (const auto& item : items) {
        file << item.serialize() << std::endl;
    }
    file.close();
}

void OnlineStore::saveTransactions() {
    std::ofstream file("data/transaction_items_1001.csv");
    for (const auto& trans : transactions) {
        file << trans.serialize() << std::endl;
    }
    file.close();
}

int OnlineStore::registerUser(std::string name, std::string type) {
    User user(nextUserId++, name, type);
    users.push_back(user);
    saveUsers();
    std::cout << "User berhasil didaftarkan dengan ID: " << user.getId() << std::endl;
    return user.getId();
}

User* OnlineStore::login(int userId) {
    for (auto& user : users) {
        if (user.getId() == userId) {
            return &user;
        }
    }
    return nullptr;
}

void OnlineStore::showAllUsers() {
    std::cout << "\n===== Daftar Semua Pengguna Terdaftar (Urut ID) =====" << std::endl;
    // Salin dan urutkan berdasarkan ID
    std::vector<User> sortedUsers = users;
    std::sort(sortedUsers.begin(), sortedUsers.end(), 
        [](const User& a, const User& b){ return a.getId() < b.getId(); });

    for (const auto& user : sortedUsers) {
        std::cout << "- ID: " << user.getId() 
             << ", Nama: " << user.getName() 
             << ", Tipe: " << user.getType() << std::endl;
    }
}

int OnlineStore::addItem(int sellerId, std::string name, double price, int stock) {
    Item item(nextItemId++, sellerId, name, price, stock);
    items.push_back(item);
    saveItems();
    return item.getId();
}

Item* OnlineStore::getItem(int itemId) {
    for (auto& item : items) {
        if (item.getId() == itemId) {
            return &item;
        }
    }
    return nullptr;
}

std::vector<Item> OnlineStore::getSellerItems(int sellerId) {
    std::vector<Item> result;
    for (const auto& item : items) {
        if (item.getSellerId() == sellerId) {
            result.push_back(item);
        }
    }
    return result;
}

// Implementasi fungsi baru
std::vector<Item> OnlineStore::getAllItems() const {
    return items;
}


bool OnlineStore::purchaseItem(int buyerId, int itemId, int quantity) {
    User* buyer = login(buyerId);
    Item* item = getItem(itemId);
    
    if (!buyer || !item) {
        std::cout << "Buyer atau Item tidak ditemukan!" << std::endl;
        return false;
    }
    
    if (item->getStock() < quantity) {
        std::cout << "Stok tidak mencukupi!" << std::endl;
        return false;
    }
    
    double totalAmount = item->getPrice() * quantity;
    int buyerAccountId = buyer->getBankAccountId();
    
    if (buyerAccountId == -1) {
        std::cout << "Anda (Pembeli) belum memiliki akun bank! Silakan buat akun di menu pembeli." << std::endl;
        return false;
    }
    
    BankAccount* buyerAccount = bank->getAccount(buyerAccountId);
    if (!buyerAccount || buyerAccount->getBalance() < totalAmount) {
        std::cout << "Saldo tidak mencukupi!" << std::endl;
        return false;
    }
    
    User* seller = login(item->getSellerId());
    if (!seller) {
         std::cout << "Penjual item ini tidak ditemukan (ID: " << item->getSellerId() << ")" << std::endl;
         return false;
    }
    
    if (seller->getBankAccountId() == -1) {
        std::cout << "Penjual tidak memiliki akun bank! Pembelian tidak dapat dilanjutkan." << std::endl;
        return false;
    }
    
    if (bank->transfer(buyerAccountId, seller->getBankAccountId(), totalAmount, "Purchase: " + item->getName())) {
        item->removeStock(quantity);
        Transaction trans(nextTransactionId++, buyerId, item->getSellerId(), itemId, quantity, totalAmount);
        transactions.push_back(trans);
        saveItems();
        saveTransactions();
        std::cout << "Pembelian berhasil!" << std::endl;
        return true;
    }
    
    std::cout << "Transfer bank gagal." << std::endl;
    return false;
}

void OnlineStore::listTransactionsLastKDays(int k) {
    std::cout << "\n===== Transaksi " << k << " Hari Terakhir =====" << std::endl;
    // STUB: Logika daysBetween() tidak diimplementasikan, jadi tampilkan semua transaksi.
    for (const auto& trans : transactions) {
        std::cout << "ID: " << trans.getId()
             << ", Buyer: " << trans.getBuyerId()
             << ", Seller: " << trans.getSellerId()
             << ", Item: " << trans.getItemId()
             << ", Total: Rp " << std::fixed << std::setprecision(0) << trans.getTotalAmount()
             << ", Status: " << trans.getStatus()
             << ", Date: " << trans.getDate() << std::endl;
    }
}

void OnlineStore::listPaidNotCompleted() {
    std::cout << "\n===== Transaksi Dibayar Tapi Belum Selesai =====" << std::endl;
    for (const auto& trans : transactions) {
        if (trans.getStatus() == "paid") {
            std::cout << "ID: " << trans.getId()
                 << ", Buyer: " << trans.getBuyerId()
                 << ", Seller: " << trans.getSellerId()
                 << ", Total: Rp " << std::fixed << std::setprecision(0) << trans.getTotalAmount()
                 << ", Date: " << trans.getDate() << std::endl;
        }
    }
}

void OnlineStore::listTopMItems(int m) {
    std::cout << "\n===== Top " << m << " Item Paling Sering Ditransaksikan =====" << std::endl;
    std::map<int, int> itemCount;
    for (const auto& trans : transactions) {
        itemCount[trans.getItemId()] += trans.getQuantity();
    }
    
    std::vector<std::pair<int, int>> sortedItems(itemCount.begin(), itemCount.end());
    std::sort(sortedItems.begin(), sortedItems.end(),
         [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
             return a.second > b.second;
         });
    
    int count = 0;
    for (const auto& pair : sortedItems) {
        if (count >= m) break;
        Item* item = getItem(pair.first);
        if (item) {
            std::cout << "Item: " << item->getName()
                 << ", Jumlah Terjual: " << pair.second << std::endl;
        }
        count++;
    }
}

void OnlineStore::listActiveBuyers() {
    std::cout << "\n===== Pembeli Paling Aktif (Berdasarkan Jumlah Transaksi Per Hari) =====" << std::endl;
    std::map<int, int> buyerCount;
    std::string today = Utils::getCurrentDate();
    
    for (const auto& trans : transactions) {
        if (trans.getDate() == today) {
            buyerCount[trans.getBuyerId()]++;
        }
    }
    
    std::vector<std::pair<int, int>> sortedBuyers(buyerCount.begin(), buyerCount.end());
    std::sort(sortedBuyers.begin(), sortedBuyers.end(),
         [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
             return a.second > b.second;
         });
    
    for (const auto& pair : sortedBuyers) {
        User* user = login(pair.first);
        if (user) {
            std::cout << "Buyer: " << user->getName()
                 << ", Transaksi: " << pair.second << std::endl;
        }
    }
}

void OnlineStore::listActiveSellers() {
    std::cout << "\n===== Penjual Paling Aktif (Berdasarkan Jumlah Transaksi Per Hari) =====" << std::endl;
    std::map<int, int> sellerCount;
    std::string today = Utils::getCurrentDate();
    
    for (const auto& trans : transactions) {
        if (trans.getDate() == today) {
            sellerCount[trans.getSellerId()]++;
        }
    }
    
    std::vector<std::pair<int, int>> sortedSellers(sellerCount.begin(), sellerCount.end());
    std::sort(sortedSellers.begin(), sortedSellers.end(),
         [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
             return a.second > b.second;
         });
    
    for (const auto& pair : sortedSellers) {
        User* user = login(pair.first);
        if (user) {
            std::cout << "Seller: " << user->getName()
                 << ", Transaksi: " << pair.second << std::endl;
        }
    }
}

std::vector<Transaction> OnlineStore::getUserOrders(int userId, std::string filter) {
    std::vector<Transaction> result;
    for (const auto& trans : transactions) {
        if (trans.getBuyerId() == userId) {
            if (filter == "all" || trans.getStatus() == filter) {
                result.push_back(trans);
            }
        }
    }
    return result;
}

double OnlineStore::getUserSpending(int userId, int days) {
    double total = 0;
    std::string today = Utils::getCurrentDate();
    // STUB: Parameter 'days' diabaikan karena daysBetween() tidak diimplementasikan.
    // Menghitung total pengeluaran sepanjang masa.
    for (const auto& trans : transactions) {
        if (trans.getBuyerId() == userId) {
            total += trans.getTotalAmount();
        }
    }
    return total;
}

Bank* OnlineStore::getBank() { return bank; }