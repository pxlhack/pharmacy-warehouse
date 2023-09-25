#ifndef LAB1_MEDICINE_BUYING_H
#define LAB1_MEDICINE_BUYING_H

#include "../utils/sql_executor.h"

class MedicineBuying {
public:
    MedicineBuying(int medicineId, int requestId, int medicineNumber) :
            medicine_id(medicineId),
            request_id(requestId),
            medicine_number(medicineNumber) {}


    void save(SQLHDBC sqlhdbc) {
        try {
            checkMedicineExistence(sqlhdbc);
            checkRequestExistence(sqlhdbc);

            ostringstream oss;
            oss << "INSERT INTO medicine_buyings(medicine_id, request_id, medicine_number) VALUES ("
                << medicine_id << ", " << request_id << ", " << medicine_number << ");";
            SqlExecutor::executeSql(sqlhdbc, oss.str());
        } catch (const runtime_error &e) {
            cerr << "Error: " << e.what() << endl;
            throw;
        }
    }

private:
    int medicine_id;
    int request_id;
    int medicine_number;

    void checkMedicineExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM medicines WHERE id = " << medicine_id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Medicine not found");
        }
    }

    void checkRequestExistence(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "SELECT 1 FROM requests WHERE id = " << request_id << ";";
        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, oss.str());

        if (results.empty() || results[0][0] != "1") {
            throw runtime_error("Request not found");
        }
    }

};


#endif //LAB1_MEDICINE_BUYING_H
