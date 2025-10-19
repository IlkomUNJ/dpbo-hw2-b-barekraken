#ifndef MENUS_H
#define MENUS_H

#include "OnlineStore.h"
#include "User.h"

void buyerMenu(OnlineStore* store, User* user);
void sellerMenu(OnlineStore* store, User* user);
void adminMenu(OnlineStore* store);
void mainMenu(OnlineStore* store);

#endif