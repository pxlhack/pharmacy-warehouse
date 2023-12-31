#ifndef LAB1_SQL_EXECUTOR_H
#define LAB1_SQL_EXECUTOR_H

#ifdef __unix__

#include <unistd.h>

#endif

#include <vector>
#include <string>
#include <sql.h>
#include <sqlext.h>
#include <sstream>
#include <stdexcept>

using namespace std;

class SqlExecutor {
public:
    static vector<vector<string>> executeSql(SQLHDBC hDbc, const string &sql) {
        printSql(sql);

        vector<vector<string>> results;

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

            ostringstream errorStream;
            errorStream << "SQL Error (" << sqlState << "): " << errorMsg;

            throw runtime_error(errorStream.str());

        } else {
            SQLCHAR columnName[256];
            SQLLEN columnNameLen;
            SQLSMALLINT columnCount = 0;

            SQLNumResultCols(hStmt, &columnCount);

            if (columnCount > 0) {
                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    vector<string> row;
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

private:
    static void printSql(const string &sql) {
#ifdef __unix__
        if (isatty(fileno(stdout))) {
            std::cout << "\x1b[32m" << sql << "\x1b[0m" << std::endl;
        } else {
            std::cout << sql << std::endl;
        }
#else
        std::cout << "Текст без изменения цвета" << std::endl;
#endif
    }

};


#endif //LAB1_SQL_EXECUTOR_H
