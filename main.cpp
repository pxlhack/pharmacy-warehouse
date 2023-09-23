#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include "utils/config_reader.h"
#include "utils/sql_executor.h"
#include "utils/menu.h"

using namespace std;


void printTables(SQLHDBC hDbc) {
    std::string listTablesQuery = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public' AND table_type = 'BASE TABLE';";
    std::vector<std::vector<std::string>> tableList = SqlExecutor::executeSql(hDbc, listTablesQuery);

    for (const auto &row: tableList) {
        for (const std::string &cell: row) {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }
}

int main() {
    try {
        ConfigReader reader("../config.properties");

        std::string username = reader.get("username");
        std::string password = reader.get("password");
        std::string driver = reader.get("driver");
        std::string database = reader.get("database");
        std::string server = reader.get("server");
        std::string port = reader.get("port");


        std::string connectionString = "DRIVER=" + driver + ";" +
                                       "DATABASE=" + database + ";" +
                                       "SERVER=" + server + ";" +
                                       "PORT=" + port + ";" +
                                       "UID=" + username + ";" +
                                       "PWD=" + password + ";";

        SQLHENV hEnv;
        SQLHDBC hDbc;


        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);

        SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
        auto *connStr = (SQLCHAR *) connectionString.c_str();
        SQLRETURN sqlreturn = SQLDriverConnect(hDbc, nullptr, connStr, SQL_NTS, nullptr,
                                               0, nullptr, SQL_DRIVER_COMPLETE);

        if (sqlreturn == SQL_SUCCESS || sqlreturn == SQL_SUCCESS_WITH_INFO) {

            Menu menu(hDbc);
            menu.testMenu();

            SQLDisconnect(hDbc);
        } else {
            std::cerr << "Connection failed" << std::endl;
        }

        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }


    return 0;
}
