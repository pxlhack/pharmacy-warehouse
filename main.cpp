#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <sstream>
#include "config_reader.h"

using namespace std;

std::vector<std::vector<std::string>> executeSql(SQLHDBC hDbc, const std::string &sql) {
    std::vector<std::vector<std::string>> results;

    SQLHSTMT hStmt;

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLRETURN sqlreturn = SQLExecDirect(hStmt, (SQLCHAR *) sql.c_str(), SQL_NTS);

    if (sqlreturn != SQL_SUCCESS && sqlreturn != SQL_SUCCESS_WITH_INFO) {
        SQLCHAR sqlState[6];
        SQLINTEGER errorCode;
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH + 1];
        SQLSMALLINT errorMsgLen;
        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState,
                      &errorCode, errorMsg,
                      SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        std::ostringstream errorStream;
        errorStream << "SQL Error (" << sqlState << "): " << errorMsg;

        throw std::runtime_error(errorStream.str());

    } else {
        SQLCHAR columnName[256];
        SQLLEN columnNameLen;
        SQLSMALLINT columnCount = 0;

        SQLNumResultCols(hStmt, &columnCount);

        if (columnCount > 0) {
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                std::vector<std::string> row;
                for (int i = 1; i <= columnCount; i++) {
                    SQLGetData(hStmt, i,
                               SQL_C_CHAR, columnName, sizeof(columnName), &columnNameLen);
                    row.emplace_back(reinterpret_cast<char *>(columnName));
                }
                results.push_back(row);
            }
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    return results;
}

void printTables(SQLHDBC hDbc) {
    std::string listTablesQuery = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public' AND table_type = 'BASE TABLE';";
    std::vector<std::vector<std::string>> tableList = executeSql(hDbc, listTablesQuery);

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

            printTables(hDbc);

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
