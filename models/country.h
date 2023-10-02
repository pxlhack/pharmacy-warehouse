#ifndef LAB1_COUNTRY_H
#define LAB1_COUNTRY_H

#include <string>
#include "../utils/sql_executor.h"

using namespace std;


class Country {
public:
    static vector<Country> findAll(SQLHDBC sqlhdbc) {
        string selectSql = "SELECT * FROM countries;";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, selectSql);

        vector<Country> countries;
        for (const vector<string> &row: results) {
            countries.push_back(parseFromVector(row));
        }

        return countries;
    }

    static Country findById(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT * FROM countries WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty()) {
            throw runtime_error("Country not found");
        }

        return parseFromVector(results[0]);
    }

    int getId() const {
        return id;
    }

    const string &getName() const {
        return name;
    }

    string toString() {
        ostringstream oss;
        oss << "{id = " << id << "; name = " << name << "}";
        return oss.str();
    }

private:
    int id;
    string name;

    Country(int id, string name) :
            id(id), name(std::move(name)) {}

    explicit Country(const string &name) :
            name(name) {}

    void save(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "INSERT INTO countries(name) VALUES(" << name << ");";
        SqlExecutor::executeSql(sqlhdbc, oss.str());
    }


    static Country parseFromVector(vector<string> row) {
        return {stoi(row[0]), row[1]};
    }
};


#endif //LAB1_COUNTRY_H
