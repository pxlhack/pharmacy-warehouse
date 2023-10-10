#ifndef LAB1_REQUEST_H
#define LAB1_REQUEST_H


#include "date.h"
#include <chrono>
#include "../utils/sql_executor.h"

class Request {
public:
    Request() {}

    Request(const Date &creationDate, const Date &completionDate, int pharmacyId) :
            creationDate(creationDate),
            completionDate(completionDate),
            pharmacyId(pharmacyId) {}

    void save(SQLHDBC sqlhdbc) {
        try {
            checkPharmacyExistence(sqlhdbc, pharmacyId);
        } catch (runtime_error const &e) {
            throw;
        }

        ostringstream oss;
        oss << "INSERT INTO requests(creation_date, completion_date, pharmacy_id) VALUES ('"
            << creationDate.toString() << "', '1900-01-01', " << pharmacyId << ");";

        string insertSql = oss.str();

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
    }

    void update(SQLHDBC sqlhdbc) {
        try {
            checkPharmacyExistence(sqlhdbc, pharmacyId);
        }
        catch (const runtime_error &e) {
            throw e;
        }
        ostringstream oss;
        oss << "UPDATE requests "
               "SET creation_date = '" << creationDate.toString() << "', " <<
            "completion_date = '" << completionDate.toString() << "', " <<
            "pharmacy_id = " << pharmacyId <<
            " WHERE id = " << id << ";";

        SqlExecutor::executeSql(sqlhdbc, oss.str());
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

    static Request findById(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT * FROM requests WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty()) {
            throw runtime_error("Request not found");
        }

        return parseFromVector(results[0]);
    }

    static void deleteById(SQLHDBC sqlhdbc, int id) {
        if (isRequestExists(sqlhdbc, id)) {
            if (!doesAnyMedicineBuyingReferToRequest(sqlhdbc, id)) {
                ostringstream oss;
                oss << "DELETE FROM requests "
                       "WHERE id = " << id << ";";
                vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
            } else {
                throw runtime_error("The request is present in the medicine buyings, it cannot be deleted");
            }

        } else {
            throw runtime_error("request not found");
        }
    }


    void complete(SQLHDBC sqlhdbc) {
        completionDate = Date::getCurrentDate();
        update(sqlhdbc);
    }

    static Request create(SQLHDBC sqlhdbc, int pharmacyId) {
        Request request(Date::getCurrentDate(), pharmacyId);
        request.save(sqlhdbc);
        return request;
    }


    int getId() const {
        return id;
    }

    int getPharmacyId() const {
        return pharmacyId;
    }

    const Date &getCreationDate() const {
        return creationDate;
    }

    const Date &getCompletionDate() const {
        return completionDate;
    }

    void setPharmacyId(int pharmacyId) {
        Request::pharmacyId = pharmacyId;
    }

    void setCreationDate(const Date &creationDate) {
        Request::creationDate = creationDate;
    }

    void setCompletionDate(const Date &completionDate) {
        Request::completionDate = completionDate;
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

    Request(const Date &creationDate, int pharmacyId) :
            creationDate(creationDate), pharmacyId(pharmacyId) {}


    static bool isRequestExists(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM requests WHERE id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static void checkPharmacyExistence(SQLHDBC sqlhdbc, int pharmacyId) {
        ostringstream oss;
        oss << "SELECT 1 FROM pharmacies WHERE id = " << pharmacyId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Pharmacy not found");
        }
    }

    static bool doesAnyMedicineBuyingReferToRequest(SQLHDBC sqlhdbc, int id) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicine_buyings WHERE request_id = " << id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static Request parseFromVector(const vector<string> &vector) {
        int id = stoi(vector[0]);
        Date creationDate = Date::parseFromStringFromDB(vector[1]);
        Date completionDate = Date::parseFromStringFromDB(vector[2]);
        int pharmacyId = stoi(vector[3]);

        return {id, creationDate, completionDate, pharmacyId};
    }
};


#endif //LAB1_REQUEST_H
