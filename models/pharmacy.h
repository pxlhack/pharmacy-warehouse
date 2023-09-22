#ifndef LAB1_PHARMACY_H
#define LAB1_PHARMACY_H

#include <string>
#include <utility>
#include <sstream>
#include <iostream>

#include "../utils/sql_executor.h"

using namespace std;


class Pharmacy {

public:
    Pharmacy(string name, string address, string phoneNumber) :
            name(std::move(name)),
            address(std::move(address)),
            phoneNumber(std::move(phoneNumber)) {}

    int save(SQLHDBC hDbc) {
        std::ostringstream oss;
        oss << "INSERT INTO pharmacies(name, address, phone_number) VALUES ('"
            << name << "', '" << address << "', '" << phoneNumber << "') RETURNING id;";

        std::string insertSql = oss.str();
        cout << insertSql << endl;

        std::vector<std::vector<std::string>> results = SqlExecutor::executeSql(hDbc, insertSql);
        this->id = stoi(results[0][0]);
    }

private:

public:
    Pharmacy() = default;

private:
    int id;
    string name;
    string address;
    string phoneNumber;
};


#endif //LAB1_PHARMACY_H
