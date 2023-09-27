#ifndef LAB1_MEDICINE_H
#define LAB1_MEDICINE_H

#include <string>
#include <utility>
#include "../utils/sql_executor.h"

using namespace std;

class Medicine {

public:
    Medicine(string name, string manufacturer, int price) :
            name(std::move(name)),
            manufacturer(std::move(manufacturer)),
            price(price) {}

    int save(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "INSERT INTO medicines(name, manufacturer, price) VALUES ('"
            << name << "', '" << manufacturer << "', '" << price << "') RETURNING id;";

        string insertSql = oss.str();

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
        this->id = stoi(results[0][0]);

        return this->id;
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

    vector<string> toStringVector() {
        return {to_string(this->id), this->name, this->manufacturer, to_string(this->price)};
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

    const string &getName() const {
        return name;
    }

    string toString() {
        ostringstream oss;
        oss << "{id = " << id <<
            "; name = " << name <<
            "; manufacturer = " << manufacturer <<
            "; price = " << price << "}\n";
        return oss.str();
    }

private:
    int id;
    string name;
    string manufacturer;
    int price;

public:
    Medicine(int id, string name, string manufacturer, int price) :
            id(id), name(std::move(name)),
            manufacturer(std::move(manufacturer)),
            price(price) {}

private:

    static Medicine parseFromVector(vector<string> vector) {
        Medicine medicine(stoi(vector[0]), vector[1], vector[2], stoi(vector[3]));
        return medicine;
    }
};


#endif //LAB1_MEDICINE_H
