#ifndef LAB1_SQL_EXECUTOR_H
#define LAB1_SQL_EXECUTOR_H


#include <vector>
#include <string>
#include <sql.h>
#include <sqlext.h>
#include <sstream>
#include <stdexcept>


class SqlExecutor {
public:
    static std::vector<std::vector<std::string>> executeSql(SQLHDBC hDbc, const std::string &sql) {
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
};


#endif //LAB1_SQL_EXECUTOR_H
