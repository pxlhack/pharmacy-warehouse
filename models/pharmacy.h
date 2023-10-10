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

    void update(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "UPDATE pharmacies "
               "SET name = '" << name << "', " <<
            "address = '" << address << "', " <<
            "phone_number = '" << phoneNumber <<
            "' WHERE id = " << id << ";";

        SqlExecutor::executeSql(sqlhdbc, oss.str());
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

    static Pharmacy findById(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT * FROM pharmacies WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty()) {
            throw runtime_error("Pharmacy not found");
        }

        return parseFromVector(results[0]);
    }

    static void deleteById(SQLHDBC sqlhdbc, int id) {
        if (isPharmacyExists(sqlhdbc, id)) {
            if (!doesAnyRequestReferToPharmacy(sqlhdbc, id)) {
                ostringstream oss;
                oss << "DELETE FROM pharmacies "
                       "WHERE id = " << id << ";";
                vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
            } else {
                throw runtime_error("The pharmacy is present in the requests, it cannot be deleted");
            }

        } else {
            throw runtime_error("Pharmacy not found");
        }
    }


    int getId() const {
        return id;
    }

    const string &getName() const {
        return name;
    }

    const string &getAddress() const {
        return address;
    }

    const string &getPhoneNumber() const {
        return phoneNumber;
    }

    void setName(const string &name) {
        Pharmacy::name = name;
    }

    void setAddress(const string &address) {
        Pharmacy::address = address;
    }

    void setPhoneNumber(const string &phoneNumber) {
        Pharmacy::phoneNumber = phoneNumber;
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


    static bool isPharmacyExists(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM pharmacies WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static bool doesAnyRequestReferToPharmacy(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM requests WHERE pharmacy_id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static Pharmacy parseFromVector(vector<string> vector) {
        return {stoi(vector[0]), vector[1], vector[2], vector[3]};
    }
};


#endif //LAB1_PHARMACY_H
