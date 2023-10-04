#ifndef LAB1_MEDICINE_BUYING_H
#define LAB1_MEDICINE_BUYING_H

#include "../utils/sql_executor.h"

class MedicineBuying {
public:
    MedicineBuying(int requestId, int medicineId, int medicineNumber) :
            requestId(requestId), medicineId(medicineId),
            medicineNumber(medicineNumber) {}


    void save(SQLHDBC sqlhdbc) {
        try {
            checkMedicineExistence(sqlhdbc);
            checkRequestExistence(sqlhdbc);
            checkMedicineBuyingExistence(sqlhdbc);
        }
        catch (const runtime_error &e) {
            throw;
        }

        ostringstream oss;
        oss << "INSERT INTO medicine_buyings(medicineId, requestId, medicineNumber) VALUES ("
            << medicineId << ", " << requestId << ", " << medicineNumber << ");";
        SqlExecutor::executeSql(sqlhdbc, oss.str());
    }


    static vector<MedicineBuying> findAll(SQLHDBC sqlhdbc) {
        string selectSql = "SELECT * FROM medicine_buyings;";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, selectSql);

        vector<MedicineBuying> medicine_buyings;
        for (const auto &row: results) {
            MedicineBuying medicineBuying = parseFromVector(row);
            medicine_buyings.push_back(medicineBuying);
        }
        return medicine_buyings;
    }

    int getMedicineId() const {
        return medicineId;
    }

    int getRequestId() const {
        return requestId;
    }

    int getMedicineNumber() const {
        return medicineNumber;
    }


    static void deleteByRequestIdAndMedicineId(SQLHDBC sqlhdbc, int requestId, int medicineId) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicine_buyings WHERE request_id = " << requestId << " AND medicine_id = "
            << medicineId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (!results.empty() && results[0][0] == "1") {
            throw runtime_error("Medicine buying already exists");
        }
    }

private:
    int requestId;
    int medicineId;
    int medicineNumber;

    static MedicineBuying parseFromVector(const vector<string> &vector) {
        return {stoi(vector[0]), stoi(vector[1]), stoi(vector[2])};
    }

    void checkMedicineExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicines WHERE id = " << medicineId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Medicine not found");
        }
    }

    void checkRequestExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM requests WHERE id = " << requestId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Request not found");
        }
    }

    void checkMedicineBuyingExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicine_buyings WHERE medicineId = " << medicineId << " AND requestId = "
            << requestId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (!results.empty() && results[0][0] == "1") {
            throw runtime_error("Medicine buying already exists");
        }
    }

};


#endif //LAB1_MEDICINE_BUYING_H
