#ifndef LAB1_REQUEST_H
#define LAB1_REQUEST_H


#include "date.h"
#include "../utils/sql_executor.h"

class Request {
public:
    Request(const Date &creationDate, const Date &completionDate, int pharmacyId) :
            creationDate(creationDate),
            completionDate(completionDate),
            pharmacyId(pharmacyId) {}

    int save(SQLHDBC sqlhdbc) {
        try {
            checkPharmacyExistence(sqlhdbc);
        } catch (runtime_error const &e) {
            throw;
        }

        ostringstream oss;
        oss << "INSERT INTO requests(creation_date, completion_date, pharmacy_id) VALUES ('"
            << creationDate.toString() << "', '" << completionDate.toString() << "', " << pharmacyId
            << ") RETURNING id;";

        string insertSql = oss.str();

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
        this->id = stoi(results[0][0]);

        return this->id;
    }


    static vector<Request> findAll(SQLHDBC sqlhdbc) {
        string selectSql = "SELECT * FROM requests;";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, selectSql);

        vector<Request> requests;
        for (const auto &row: results) {
            Request request = parseFromVector(row);
            requests.push_back(request);
        }
        return requests;
    }

    vector<string> toStringVector(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT name FROM pharmacies WHERE id = " << pharmacyId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
        if (results.empty()) {
            throw runtime_error("Pharmacy not found");
        }

        return {to_string(id), creationDate.toString(), completionDate.toString(), results[0][0]};
    }

    static Request findById(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT * FROM requests WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty()) {
            throw runtime_error("Request not found");
        }

        return parseFromVector(results[0]);
    }

    string toString() {
        ostringstream oss;
        oss << "{id = " << id <<
            "; creationDate = " << creationDate.toString() <<
            "; completionDate = " << completionDate.toString() <<
            "; pharmacyId = " << pharmacyId << "}\n";
        return oss.str();
    }

    int getPharmacyId() const {
        return pharmacyId;
    }

    int getId() const {
        return id;
    }

    const Date &getCreationDate() const {
        return creationDate;
    }

    const Date &getCompletionDate() const {
        return completionDate;
    }


private:
    int id;
    Date creationDate;
    Date completionDate;
    int pharmacyId;

    Request(int id, const Date &creationDate, const Date &completionDate, int pharmacyId) :
            id(id),
            creationDate(creationDate),
            completionDate(completionDate),
            pharmacyId(pharmacyId) {}


    static Request parseFromVector(const vector<string> &vector) {
        int id = stoi(vector[0]);
        Date creationDate = Date::parseFromStringFromDB(vector[1]);
        Date completionDate = Date::parseFromStringFromDB(vector[2]);
        int pharmacyId = stoi(vector[3]);

        return {id, creationDate, completionDate, pharmacyId};
    }

    void checkPharmacyExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM pharmacies WHERE id = " << pharmacyId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Pharmacy not found");
        }
    }

};


#endif //LAB1_REQUEST_H
