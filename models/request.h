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
        ostringstream oss;
        oss << "INSERT INTO requests(creation_date, completion_date, pharmacy_id) VALUES ('"
            << creationDate.toString() << "', '" << completionDate.toString() << "', " << pharmacyId
            << ") RETURNING id;";

        string insertSql = oss.str();

        vector<vector<string>> results = SqlExecutor::executeSql(sqlhdbc, insertSql);
        this->id = stoi(results[0][0]);

        return this->id;
    }

private:
    int id;
    Date creationDate;
    Date completionDate;
    int pharmacyId;

};


#endif //LAB1_REQUEST_H
