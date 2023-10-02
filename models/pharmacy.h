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

    void save(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "INSERT INTO pharmacies(name, address, phone_number) VALUES ('"
            << name << "', '" << address << "', '" << phoneNumber << "');";

        string insertSql = oss.str();

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
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


    static Pharmacy findById(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT * FROM pharmacies WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty()) {
            throw runtime_error("Pharmacy not found");
        }

        return parseFromVector(results[0]);
    }

    string getName() {
        return name;
    }

    int getId() const {
        return id;
    }

    const string &getAddress() const {
        return address;
    }

    const string &getPhoneNumber() const {
        return phoneNumber;
    }

private:
    int id;
    string name;
    string address;
    string phoneNumber;

    Pharmacy() = default;

    Pharmacy(int id, string name, string address, string phoneNumber) :
            id(id),
            name(move(name)),
            address(move(address)),
            phoneNumber(move(phoneNumber)) {}

    static Pharmacy parseFromVector(vector<string> vector) {
        return {stoi(vector[0]), vector[1], vector[2], vector[3]};
    }
};


#endif //LAB1_PHARMACY_H
