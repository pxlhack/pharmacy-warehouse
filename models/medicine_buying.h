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
            checkMedicineBuyingExistence(sqlhdbc, requestId, medicineId);
        }
        catch (const runtime_error &e) {
            throw;
        }

        ostringstream oss;
        oss << "INSERT INTO medicine_buyings(request_id, medicine_id, medicine_number) VALUES ("
            << requestId << ", " << medicineId << ", " << medicineNumber << ");";
        SqlExecutor::executeSql(sqlhdbc, oss.str());
    }

    void update(SQLHDBC sqlhdbc) const {
        ostringstream oss;
        oss << "UPDATE medicine_buyings "
               "SET medicine_number = " << medicineNumber <<
            " WHERE request_id = " << requestId <<
            " AND medicine_id =" << medicineId << ";";

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

    static void deleteByRequestIdAndMedicineId(SQLHDBC sqlhdbc, int requestId, int medicineId) {
        if (isMedicineBuyingExists(sqlhdbc, requestId, medicineId)) {
            ostringstream oss;
            oss << "DELETE FROM medicine_buyings "
                   "WHERE request_id = " << requestId <<
                " AND medicine_id =" << medicineId << ";";
            vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());
        } else {
            throw runtime_error("Medicine buying not found");
        }
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

    void setMedicineNumber(int medicineNumber) {
        MedicineBuying::medicineNumber = medicineNumber;
    }

private:
    int requestId;
    int medicineId;
    int medicineNumber;

    void checkMedicineExistence(SQLHDBC sqlhdbc) const {
        ostringstream oss;
        oss << "SELECT 1 FROM medicines WHERE id = " << medicineId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Medicine not found");
        }
    }

    void checkRequestExistence(SQLHDBC sqlhdbc) const {
        ostringstream oss;
        oss << "SELECT 1 FROM requests WHERE id = " << requestId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Request not found");
        }
    }

    static void checkMedicineBuyingExistence(SQLHDBC sqlhdbc, int requestId, int medicineId) {
        if (isMedicineBuyingExists(sqlhdbc, requestId, medicineId))
            throw runtime_error("Medicine buying already exists");
    }

    static bool isMedicineBuyingExists(SQLHDBC sqlhdbc, int requestId, int medicineId) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicine_buyings WHERE request_id = " << requestId << " AND medicine_id = "
            << medicineId << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (!results.empty() && results[0][0] == "1") {
            return true;
        }
        return false;
    }

    static MedicineBuying parseFromVector(const vector<string> &vector) {
        return {stoi(vector[0]), stoi(vector[1]), stoi(vector[2])};
    }
};


#endif //LAB1_MEDICINE_BUYING_H
