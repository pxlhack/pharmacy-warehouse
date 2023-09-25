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

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
        this->id = stoi(results[0][0]);

        return this->id;
    }

    static vector<Pharmacy> findAll(SQLHDBC sqlhdbc) {
        string selectSql = "SELECT * FROM pharmacies;";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, selectSql);

        vector<Pharmacy> pharmacies;
        for (const auto &row: results) {
            Pharmacy pharmacy = parseFromVector(row);
            pharmacies.push_back(pharmacy);
        }
        return pharmacies;
    }

    vector<string> toStringVector() {
        return {to_string(this->id), this->name, this->address, this->phoneNumber};
    }

private:
    Pharmacy() = default;

    Pharmacy(int id, string name, string address, string phoneNumber) :
            id(id),
            name(move(name)),
            address(move(address)),
            phoneNumber(move(phoneNumber)) {}

    int id;
    string name;
    string address;
    string phoneNumber;

    static Pharmacy parseFromVector(vector<string> vector) {
        return {stoi(vector[0]), vector[1], vector[2], vector[3]};
    }
};


#endif //LAB1_PHARMACY_H
