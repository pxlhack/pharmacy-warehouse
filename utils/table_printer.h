#ifndef LAB1_TABLE_PRINTER_H
#define LAB1_TABLE_PRINTER_H


#include <vector>
#include <string>
#include <iostream>

using namespace std;

class TablePrinter {

public:

    static void printTable(const vector<string> &headers, const vector<vector<string>> &data) {
        vector<int> columnWidths(headers.size());
        for (int i = 0; i < headers.size(); i++) {
            columnWidths[i] = headers[i].size();
            for (const vector<string> &row: data) {
                if (i < row.size() && row[i].size() > columnWidths[i]) {
                    columnWidths[i] = row[i].size();
                }
            }
        }

        printHeader(headers, columnWidths);
        printSeparator(columnWidths);

        for (const vector<string> &row: data) {
            printRow(row, columnWidths);
        }
    }

private:

    static void printHeader(const vector<string> &headers, const vector<int> &columnWidths) {
        for (int i = 0; i < headers.size(); i++) {
            cout << headers[i];
            int spaces = columnWidths[i] - headers[i].size();
            for (int j = 0; j < spaces; j++) {
                cout << " ";
            }
            cout << "\t| ";
        }
        cout << endl;
    }

    static void printSeparator(const vector<int> &columnWidths) {
        for (int columnWidth: columnWidths) {
            for (int j = 0; j < columnWidth; j++) {
                cout << "-";
            }
            cout << "\t+ ";
        }
        cout << endl;
    }

    static void printRow(const vector<string> &row, const vector<int> &columnWidths) {
        for (int i = 0; i < row.size(); i++) {
            cout << row[i];
            int spaces = columnWidths[i] - row[i].size();
            for (int j = 0; j < spaces; j++) {
                cout << " ";
            }
            cout << "\t| ";
        }
        cout << endl;
    }
};


#endif //LAB1_TABLE_PRINTER_H
