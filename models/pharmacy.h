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
            name(move(name)),
            address(move(address)),
            phoneNumber(move(phoneNumber)) {}

    int save(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "INSERT INTO pharmacies(name, address, phone_number) VALUES ('"
            << name << "', '" << address << "', '" << phoneNumber << "') RETURNING id;";

        string insertSql = oss.str();
        cout << insertSql << endl;

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
        this->id = stoi(results[0][0]);

        return this->id;
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
