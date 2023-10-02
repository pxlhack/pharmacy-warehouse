#ifndef LAB1_MANUFACTURER_H
#define LAB1_MANUFACTURER_H

#include <string>
#include "../utils/sql_executor.h"

using namespace std;


class Manufacturer {
public:
    Manufacturer(const string &name, int countryId) :
            name(name), countryId(countryId) {}

    void save(SQLHDBC sqlhdbc) {
        ostringstream oss;
        oss << "INSERT INTO manufacturers(name, country_id) VALUES('" << name << "', " << countryId << ");";
        SqlExecutor::executeSql(sqlhdbc, oss.str());
    }
private:
    int id;
    string name;
    int countryId;
};


#endif //LAB1_MANUFACTURER_H
