// Continue from Menus.cpp Part 1
#include "BankAccount.h"
#include "BankTransaction.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include "OnlineStore.h"
#include "User.h"
#include "Item.h"
#include "Transaction.h"

// forward declaration for buyerMenu implemented elsewhere
void buyerMenu(OnlineStore* store, User* user);

void buyerMenu(OnlineStore* store, User* user) {
    while (true) {
        std::cout << "\n===== Menu Pembeli: " << user->getName() << " =====" << std::endl;
        std::cout << "1. Lihat Semua Item Tersedia" << std::endl;
        std::cout << "2. Beli Item" << std::endl;
        std::cout << "3. Lihat Pesanan Saya" << std::endl;
        std::cout << "4. Buat Akun Bank" << std::endl;
        std::cout << "5. Top Up Saldo" << std::endl;
        std::cout << "6. Cek Saldo" << std::endl;
        std::cout << "7. Lihat Transaksi Bank" << std::endl;
      
        std::cout << "8. Tarik Saldo" << std::endl;
        std::cout << "9. Logout" << std::endl;
        
        std::cout << "Pilih opsi: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
          
            std::cout << "\n--- Semua Item Tersedia ---" << std::endl;
            auto items = store->getAllItems();
            for (const auto& item : items) {
                if (item.getStock() > 0) {
                     std::cout << "ID: " << item.getId()
                         << ", Nama: " << item.getName()
                         << ", Harga: Rp " << std::fixed << std::setprecision(0) << item.getPrice()
                         << ", Stok: " << item.getStock() 
                         << " (Penjual ID: " << item.getSellerId() << ")" << std::endl;
                }
            }
        } else if (choice == 2) {
            int itemId, quantity;
            std::cout << "Masukkan ID item: ";
            std::cin >> itemId;
            std::cout << "Masukkan jumlah: ";
            std::cin >> quantity;
            
            if (store->purchaseItem(user->getId(), itemId, quantity)) {
              
            } else {
                std::cout << "Pembelian gagal!" << std::endl;
            }
        } else if (choice == 3) {
            auto orders = store->getUserOrders(user->getId());
            std::cout << "\n--- Pesanan Saya ---" << std::endl;
            if(orders.empty()){
                std::cout << "Anda belum memiliki pesanan." << std::endl;
            }
            for (const auto& order : orders) {
                std::cout << "ID: " << order.getId()
                     << ", Item: " << order.getItemId()
                     << ", Jumlah: " << order.getQuantity()
                     << ", Total: Rp " << std::fixed << std::setprecision(0) << order.getTotalAmount()
                     << ", Status: " << order.getStatus() << std::endl;
            }
        } else if (choice == 4) {
            if (user->getBankAccountId() == -1) {
                int accountId = store->getBank()->createAccount(user->getId());
                user->setBankAccountId(accountId);
                store->saveUsers(); // Simpan bank account ID ke user
                std::cout << "Akun bank berhasil dibuat dengan ID: " << accountId << std::endl;
            } else {
                std::cout << "Anda sudah memiliki akun bank dengan ID: " << user->getBankAccountId() << std::endl;
            }
        } else if (choice == 5) {
            if (user->getBankAccountId() != -1) {
                double amount;
                std::cout << "Masukkan jumlah top up: Rp ";
                std::cin >> amount;
                if (store->getBank()->topup(user->getBankAccountId(), amount)) {
                    std::cout << "Top up berhasil!" << std::endl;
                } else {
                    std::cout << "Top up gagal!" << std::endl;
                }
            } else {
                std::cout << "Anda belum memiliki akun bank! Silakan buat akun terlebih dahulu." << std::endl;
            }
        } else if (choice == 6) {
            if (user->getBankAccountId() != -1) {
                BankAccount* acc = store->getBank()->getAccount(user->getBankAccountId());
                if (acc) {
                    std::cout << "Saldo Anda: Rp " << std::fixed << std::setprecision(0) << acc->getBalance() << std::endl;
                }
            } else {
                std::cout << "Anda belum memiliki akun bank!" << std::endl;
            }
        } else if (choice == 7) {
            if (user->getBankAccountId() != -1) {
                auto transactions = store->getBank()->getAccountTransactions(user->getBankAccountId());
                std::cout << "\n--- Transaksi Bank Anda ---" << std::endl;
                if(transactions.empty()){
                    std::cout << "Belum ada riwayat transaksi bank." << std::endl;
                }
                for (const auto& trans : transactions) {
                    std::cout << "Tipe: " << trans.getType()
                         << ", Jumlah: Rp " << std::fixed << std::setprecision(0) << trans.getAmount()
                         << ", Deskripsi: " << trans.getDescription()
                         << ", Tanggal: " << trans.getDate() << std::endl;
                }
            } else {
                std::cout << "Anda belum memiliki akun bank!" << std::endl;
            }
        
        // --- PERUBAHAN DIMULAI ---
        } else if (choice == 8) { // Opsi baru: Tarik Saldo
            if (user->getBankAccountId() != -1) {
                double amount;
                std::cout << "Masukkan jumlah yang akan ditarik: Rp ";
                std::cin >> amount;
                
                // Panggil fungsi withdraw dari Bank
                if (store->getBank()->withdraw(user->getBankAccountId(), amount)) {
                    // Jika berhasil, cetak pesan yang Anda minta
                    std::cout << "Saldo anda sudah ditarik Rp " 
                              << std::fixed << std::setprecision(0) << amount << std::endl;
                } else {
                    // Jika gagal (misal, saldo tidak cukup)
                    std::cout << "Penarikan gagal! Saldo tidak mencukupi atau jumlah tidak valid." << std::endl;
                }
            } else {
                std::cout << "Anda belum memiliki akun bank!" << std::endl;
            }
        } else if (choice == 9) { // Opsi Logout, digeser dari 8 ke 9
            break;
        }
    }
}

void sellerMenu(OnlineStore* store, User* user) {
    while (true) {
        std::cout << "\n===== Menu Penjual: " << user->getName() << " =====" << std::endl;
        std::cout << "1. Lihat Stok Barang Saya" << std::endl;
        std::cout << "2. Tambah Item Baru" << std::endl;
        std::cout << "3. Isi Ulang Stok Item" << std::endl;
        std::cout << "4. Hapus Item dari Etalase Penjualan (Kurangi Stok)" << std::endl;
        std::cout << "5. Set Harga Item" << std::endl;
        std::cout << "6. Lihat Laporan Penjualan" << std::endl;
        std::cout << "7. Menu Pembeli (Jika ingin membeli)" << std::endl;
        std::cout << "8. Logout" << std::endl;
        std::cout << "Pilih opsi: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            std::vector<Item> items = store->getSellerItems(user->getId());
            std::cout << "\n--- Stok Milik Penjual: " << user->getName() << " ---" << std::endl;
            if(items.empty()){
                std::cout << "Anda belum memiliki item untuk dijual." << std::endl;
            }
            for (const auto& item : items) {
                std::cout << "ID: " << item.getId()
                     << ", Nama: " << item.getName()
                     << ", Harga: Rp " << std::fixed << std::setprecision(0) << item.getPrice()
                     << ", Stok: " << item.getStock() << std::endl;
            }
        } else if (choice == 2) {
            std::string name;
            double price;
            int stock;
            
            std::cout << "Nama item: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Harga: Rp ";
            std::cin >> price;
            std::cout << "Stok awal: ";
            std::cin >> stock;
            
            int itemId = store->addItem(user->getId(), name, price, stock);
            std::cout << "Item berhasil ditambahkan dengan ID: " << itemId << std::endl;
        } else if (choice == 3) {
            int itemId, addStock;
            std::cout << "Masukkan ID item: ";
            std::cin >> itemId;
            std::cout << "Jumlah stok yang ditambahkan: ";
            std::cin >> addStock;
            
            Item* item = store->getItem(itemId);
            if (item && item->getSellerId() == user->getId()) {
                item->addStock(addStock);
                store->saveItems(); // Simpan perubahan stok
                std::cout << "Stok berhasil ditambahkan!" << std::endl;
            } else {
                std::cout << "Item tidak ditemukan atau bukan milik Anda!" << std::endl;
            }
        } else if (choice == 4) {
            int itemId, removeStock;
            std::cout << "Masukkan ID item: ";
            std::cin >> itemId;
            std::cout << "Jumlah stok yang dikurangi: ";
            std::cin >> removeStock;
            
            Item* item = store->getItem(itemId);
            if (item && item->getSellerId() == user->getId()) {
                if (item->removeStock(removeStock)) {
                    store->saveItems(); // Simpan perubahan stok
                    std::cout << "Stok berhasil dikurangi!" << std::endl;
                } else {
                    std::cout << "Stok tidak mencukupi!" << std::endl;
                }
            } else {
                std::cout << "Item tidak ditemukan atau bukan milik Anda!" << std::endl;
            }
        } else if (choice == 5) {
            int itemId;
            double newPrice;
            std::cout << "Masukkan ID item: ";
            std::cin >> itemId;
            std::cout << "Harga baru: Rp ";
            std::cin >> newPrice;
            
            Item* item = store->getItem(itemId);
            if (item && item->getSellerId() == user->getId()) {
                item->setPrice(newPrice);
                store->saveItems(); // Simpan perubahan harga
                std::cout << "Harga berhasil diubah!" << std::endl;
            } else {
                std::cout << "Item tidak ditemukan atau bukan milik Anda!" << std::endl;
            }
        } else if (choice == 6) {
            std::cout << "\n===== Laporan Penjualan =====" << std::endl;
            std::cout << "1. Top K Item Populer" << std::endl;
            std::cout << "2. Customer Loyal" << std::endl;
            std::cout << "Pilih: ";
            int reportChoice;
            std::cin >> reportChoice;
            
            if (reportChoice == 1) {
                int k;
                std::cout << "Masukkan nilai K: ";
                std::cin >> k;
                
                std::map<int, int> itemSales;
                std::ifstream transFile("data/transaction_items_1001.csv");
                if (transFile.is_open()) {
                    std::string line;
                    while (std::getline(transFile, line)) {
                        if (!line.empty()) {
                            Transaction trans = Transaction::deserialize(line);
                            if (trans.getSellerId() == user->getId()) {
                                itemSales[trans.getItemId()] += trans.getQuantity();
                            }
                        }
                    }
                    transFile.close();
                }
                
                std::vector<std::pair<int, int>> sortedItems(itemSales.begin(), itemSales.end());
                std::sort(sortedItems.begin(), sortedItems.end(),
                     [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                         return a.second > b.second;
                     });
                
                std::cout << "\nTop " << k << " Item Populer:" << std::endl;
                int count = 0;
                for (const auto& pair : sortedItems) {
                    if (count >= k) break;
                    Item* item = store->getItem(pair.first);
                    if (item) {
                        std::cout << "Item: " << item->getName()
                             << ", Terjual: " << pair.second << std::endl;
                    }
                    count++;
                }
            } else if (reportChoice == 2) {
                std::map<int, int> buyerCount;
                
                std::ifstream transFile("data/transaction_items_1001.csv");
                if (transFile.is_open()) {
                    std::string line;
                    while (std::getline(transFile, line)) {
                        if (!line.empty()) {
                            Transaction trans = Transaction::deserialize(line);
                            if (trans.getSellerId() == user->getId()) {
                                buyerCount[trans.getBuyerId()]++;
                            }
                        }
                    }
                    transFile.close();
                }
                
                std::vector<std::pair<int, int>> sortedBuyers(buyerCount.begin(), buyerCount.end());
                std::sort(sortedBuyers.begin(), sortedBuyers.end(),
                     [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                         return a.second > b.second;
                     });
                
                std::cout << "\nCustomer Loyal:" << std::endl;
                for (const auto& pair : sortedBuyers) {
                    User* buyer = store->login(pair.first);
                    if (buyer) {
                        std::cout << "Buyer: " << buyer->getName()
                             << ", Transaksi: " << pair.second << std::endl;
                    }
                }
            }
        } else if (choice == 7) {
            buyerMenu(store, user);
        } else if (choice == 8) {
            break;
        }
    }
}

void adminMenu(OnlineStore* store) {
    while (true) {
        std::cout << "\n===== MENU ADMIN =====" << std::endl;
        std::cout << "1. Laporan Item Terlaris (Global)" << std::endl;
        std::cout << "2. Laporan Pembeli Teraktif (Global)" << std::endl;
        std::cout << "3. Laporan Penjual Teraktif (Global)" << std::endl;
        std::cout << "4. Laporan Akun Tidak Aktif (Dormant)" << std::endl;
        std::cout << "5. Top N User Bank (Global)" << std::endl;
        std::cout << "6. Kembali ke Menu Utama" << std::endl;
        std::cout << "Pilih opsi: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            int m;
            std::cout << "Masukkan jumlah top item: ";
            std::cin >> m;
            store->listTopMItems(m);
        } else if (choice == 2) {
            store->listActiveBuyers();
        } else if (choice == 3) {
            store->listActiveSellers();
        } else if (choice == 4) {
            store->getBank()->listDormantAccounts();
        } else if (choice == 5) {
            int n;
            std::cout << "Masukkan nilai N: ";
            std::cin >> n;
            store->getBank()->listTopUsers(n);
        } else if (choice == 6) {
            break;
        }
    }
}

void mainMenu(OnlineStore* store) {
    while (true) {
        std::cout << "\n===== MENU UTAMA =====" << std::endl;
        std::cout << "1. Daftar (Register)" << std::endl;
        std::cout << "2. Tampilkan Semua Pengguna (Show All Users)" << std::endl;
        std::cout << "3. Masuk (Login)" << std::endl;
        std::cout << "4. Masuk Sebagai Admin (Admin Login)" << std::endl;
        std::cout << "5. Keluar (Exit)" << std::endl;
        std::cout << "Pilih opsi: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            std::string name;
            int type;
            
            std::cout << "Nama: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Tipe (1=Pembeli, 2=Penjual): ";
            std::cin >> type;
            
            std::string userType = (type == 1) ? "Pembeli" : "Penjual";
            store->registerUser(name, userType);
        } else if (choice == 2) {
            store->showAllUsers();
        } else if (choice == 3) {
            int userId;
            std::cout << "Masukkan ID User: ";
            std::cin >> userId;
            
            User* user = store->login(userId);
            if (user) {
                std::cout << "Login berhasil! Selamat datang, " << user->getName() << std::endl;
                if (user->getType() == "Pembeli") {
                    buyerMenu(store, user);
                } else if (user->getType() == "Penjual") {
                    sellerMenu(store, user);
                }
            } else {
                std::cout << "User tidak ditemukan!" << std::endl;
            }
        } else if (choice == 4) {
            adminMenu(store);
        } else if (choice == 5) {
            std::cout << "Terima kasih! Program selesai." << std::endl;
            break;
        }
    }
}