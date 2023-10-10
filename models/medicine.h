#ifndef LAB1_MEDICINE_H
#define LAB1_MEDICINE_H

#include <string>
#include <utility>
#include "../utils/sql_executor.h"

using namespace std;

class Medicine {

public:
    Medicine(string name, int manufacturerId, int price) :
            name(std::move(name)), manufacturerId(manufacturerId),
            price(price) {}


    void save(SQLHDBC sqlhdbc) {
        try {
            checkManufacturerExistence(sqlhdbc);
        }
        catch (const runtime_error &e) {
            throw;
        }
        ostringstream oss;
        oss << "INSERT INTO medicines(name, manufacturer_id, price) VALUES ('"
            << name << "', " << manufacturerId << ", '" << price << "') RETURNING id;";

        string insertSql = oss.str();

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
    }

    void update(SQLHDBC sqlhdbc) {
        try {
            checkManufacturerExistence(sqlhdbc);
        }
        catch (const runtime_error &e) {
            throw e;
        }
        ostringstream oss;
        oss << "UPDATE medicines "
               "SET name = '" << name << "', " <<
            "manufacturer_id = " << manufacturerId << ", " <<
            "price = " << price <<
            " WHERE id = " << id << ";";

        SqlExecutor::executeSql(sqlhdbc, oss.str());
    }

    static vector<Medicine> findAll(SQLHDBC sqlhdbc) {
        string selectSql = "SELECT * FROM medicines;";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, selectSql);

        vector<Medicine> medicines;
        for (const auto &row: results) {
            Medicine medicine = parseFromVector(row);
            medicines.push_back(medicine);
        }
        return medicines;
    }

    static Medicine findById(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT * FROM medicines WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty()) {
            throw runtime_error("Medicine not found");
        }

        return parseFromVector(results[0]);
    }

    static void deleteById(SQLHDBC sqlhdbc, int id) {
        if (isMedicineExists(sqlhdbc, id)) {
            if (!doesAnyMedicineBuyingReferToMedicine(sqlhdbc, id)) {
                ostringstream oss;
                oss << "DELETE FROM medicines "
                       "WHERE id = " << id << ";";
                vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
            } else {
                throw runtime_error("The medicine is present in the medicine buyings, it cannot be deleted");
            }

        } else {
            throw runtime_error("Medicine not found");
        }
    }


    int getId() const {
        return id;
    }

    const string &getName() const {
        return name;
    }

    int getManufacturerId() const {
        return manufacturerId;
    }

    int getPrice() const {
        return price;
    }

    void setName(const string &name) {
        Medicine::name = name;
    }

    void setManufacturerId(int manufacturerId) {
        Medicine::manufacturerId = manufacturerId;
    }

    void setPrice(int price) {
        Medicine::price = price;
    }

private:
    int id;
    string name;
    int manufacturerId;
    int price;

    Medicine(int id, string name, int manufacturerId, int price) :
            id(id), name(std::move(name)),
            manufacturerId(manufacturerId),
            price(price) {}

    void checkManufacturerExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM manufacturers WHERE id = " << manufacturerId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Manufacturer not found");
        }
    }

    static bool isMedicineExists(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicines WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static bool doesAnyMedicineBuyingReferToMedicine(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicine_buyings WHERE medicine_id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static Medicine parseFromVector(vector<string> vector) {
        Medicine medicine(stoi(vector[0]), vector[1], stoi(vector[2]), stoi(vector[3]));
        return medicine;
    }
};


#endif //LAB1_MEDICINE_H
