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
        ostringstream oss;
        oss << "INSERT INTO manufacturers(name, country_id) VALUES('" << name << "', " << countryId << ");";
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

    const string &getName() const {
        return name;
    }

    int getCountryId() const {
        return countryId;
    }

    int getId() const {
        return id;
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

};


#endif //LAB1_MANUFACTURER_H
