#ifndef LAB1_MENU_H
#define LAB1_MENU_H

#include <string>
#include <iostream>
#include <regex>
#include <iomanip>
#include <limits>
#include "../models/pharmacy.h"
#include "table_printer.h"

using namespace std;

#define HELP "Menu:\n\
1) Add pharmacy\n\
2) Get pharmacies list\n\
"

class Menu {

public:

    explicit Menu(SQLHDBC hdbc) : hdbc(hdbc) {}

    void testMenu() {
        bool isWorked = true;
        string command;

        do {
            cout << HELP;
            cout << ">";
            cin >> command;

            if (isValidCommand(command)) {
                int num = stoi(command);

                switch (num) {
                    case 1: {
                        string name, address, phoneNumber;

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        cout << "Enter name:\n>";
                        getline(cin, name);

                        cout << "Enter address:\n>";
                        getline(cin, address);

                        cout << "Enter phone number:\n>";
                        getline(cin, phoneNumber);


                        Pharmacy pharmacy(name, address, phoneNumber);

                        int id = pharmacy.save(hdbc);

                        cout << "Pharmacy with id=" << id << " created\n";

                        break;
                    }
                    case 2: {
                        vector<Pharmacy> pharmacies = Pharmacy::findAll(hdbc);

                        if (!pharmacies.empty()) {
                            vector<string> headers = {"№", "Name", "Address", "Phone number"};

                            vector<vector<string>> data;

                            int i = 1;
                            for (Pharmacy pharmacy: pharmacies) {
                                vector<string> tmp = pharmacy.toStringVector();
                                tmp[0] = to_string(i++);
                                data.push_back(tmp);
                            }

                            TablePrinter::printTable(headers, data);

                        } else {
                            cout << "No pharmacies found." << endl;
                        }
                        break;
                    }

                }
            }

        } while (isWorked);
    }

private:
    static bool isValidCommand(const string &command) {
        return isPositiveValue(string_view(command));
    }

    static bool isPositiveValue(string_view s) {
        static const regex r(R"(\d{1,})");
        return regex_match(s.data(), r);
    }

    SQLHDBC hdbc;

};


#endif //LAB1_MENU_H
