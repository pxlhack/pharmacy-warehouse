#ifndef LAB1_MANUFACTURER_H
#define LAB1_MANUFACTURER_H

#include <string>
#include <utility>
#include "../utils/sql_executor.h"

using namespace std;


class Manufacturer {
public:
    Manufacturer(string name, int countryId) :
            name(std::move(name)), countryId(countryId) {}


    void save(SQLHDBC sqlhdbc) {
        try {
            checkCountryExistence(sqlhdbc);
        }
        catch (const runtime_error &e) {
            throw;
        }

        ostringstream oss;
        oss << "INSERT INTO manufacturers(name, country_id) "
               "VALUES('" << name << "', " << countryId << ");";
        SqlExecutor::executeSql(sqlhdbc, oss.str());
    }

    void update(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "UPDATE manufacturers "
               "SET name = '" << name <<
            "', country_id = '" << countryId <<
            "' WHERE id = " << id << ";";

        SqlExecutor::executeSql(sqlhdbc, oss.str());
    }

    static vector<Manufacturer> findAll(SQLHDBC sqlhdbc) {
        string selectSql = "SELECT * FROM manufacturers;";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, selectSql);

        vector<Manufacturer> countries;
        for (const vector<string> &row: results) {
            countries.push_back(parseFromVector(row));
        }

        return countries;
    }

    static Manufacturer findById(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT * FROM manufacturers WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty()) {
            throw runtime_error("Request not found");
        }

        return parseFromVector(results[0]);
    }

    static void deleteById(SQLHDBC sqlhdbc, int id) {
        if (isManufacturerExists(sqlhdbc, id)) {
            if (!doesAnyMedicineToManufacturer(sqlhdbc, id)) {
                ostringstream oss;
                oss << "DELETE FROM manufacturers "
                       "WHERE id = " << id << ";";
                vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
            } else {
                throw runtime_error("The manufacturer is present in the medicines, it cannot be deleted");
            }

        } else {
            throw runtime_error("Medicine buying not found");
        }
    }


    const string &getName() const {
        return name;
    }

    int getCountryId() const {
        return countryId;
    }

    int getId() const {
        return id;
    }

    void setName(const string &name) {
        Manufacturer::name = name;
    }

    void setCountryId(int countryId) {
        Manufacturer::countryId = countryId;
    }

private:
    int id;
    string name;
    int countryId;

    Manufacturer(int id, string name, int countryId) :
            id(id), name(std::move(name)), countryId(countryId) {}


    static Manufacturer parseFromVector(const vector<string> &vector) {
        return {stoi(vector[0]), vector[1], stoi(vector[2])};
    }

    static bool doesAnyMedicineToManufacturer(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicines WHERE manufacturer_id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static bool isManufacturerExists(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM manufacturers WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    void checkCountryExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM countries WHERE id = " << countryId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Country not found");
        }
    }
};


#endif //LAB1_MANUFACTURER_H
