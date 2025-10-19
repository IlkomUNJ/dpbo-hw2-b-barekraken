#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <vector>

class User {
private:
    int id;
    std::string name;
    std::string type;
    int bankAccountId;

public:
    User();
    User(int id, std::string name, std::string type);

    int getId() const;
    std::string getName() const;
    std::string getType() const;
    int getBankAccountId() const;
    void setBankAccountId(int accId);

    std::string serialize() const;
    static User deserialize(const std::string& data);
};

#endif