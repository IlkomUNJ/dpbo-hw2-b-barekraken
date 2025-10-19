#ifndef ONLINESTORE_H
#define ONLINESTORE_H

#include "User.h"
#include "Item.h"
#include "Transaction.h"
#include "Bank.h"
#include <vector>
#include <string>

class OnlineStore {
private:
    std::vector<User> users;
    std::vector<Item> items;
    std::vector<Transaction> transactions;
    Bank* bank;
    int nextUserId;
    int nextItemId;
    int nextTransactionId;

    void loadUsers();
    void loadItems();
    void loadTransactions();
    
public:
    OnlineStore(Bank* bankPtr);
    ~OnlineStore();

    void saveUsers();
    void saveItems();
    void saveTransactions();

    int registerUser(std::string name, std::string type);
    User* login(int userId);
    void showAllUsers();

    int addItem(int sellerId, std::string name, double price, int stock);
    Item* getItem(int itemId);
    std::vector<Item> getSellerItems(int sellerId);
    std::vector<Item> getAllItems() const; // Ditambahkan untuk menu pembeli

    bool purchaseItem(int buyerId, int itemId, int quantity);

    void listTransactionsLastKDays(int k); // Logika stubbed
    void listPaidNotCompleted();
    void listTopMItems(int m);
    void listActiveBuyers();
    void listActiveSellers();

    std::vector<Transaction> getUserOrders(int userId, std::string filter = "all");
    double getUserSpending(int userId, int days = -1); // Logika stubbed

    Bank* getBank();
};

#endif