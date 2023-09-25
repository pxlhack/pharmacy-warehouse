#ifndef LAB1_MENU_H
#define LAB1_MENU_H

#include <string>
#include <iostream>
#include <regex>
#include <iomanip>
#include <limits>
#include "../models/pharmacy.h"
#include "../models/medicine.h"
#include "table_printer.h"
#include "../models/date.h"
#include "../models/request.h"
#include "../models/medicine_buying.h"

using namespace std;

#define HELP "Menu:\n\
1) Add pharmacy\n\
2) Get pharmacies list\n\
\n\
3) Add medicine\n\
4) Get medicines list \n\
\n\
5) Add request\n\
\n\
6) Add medicine buying\n\
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

                    case 3: {
                        string name, manufacturer, price;

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        cout << "Enter name:\n>";
                        getline(cin, name);

                        cout << "Enter manufacturer:\n>";
                        getline(cin, manufacturer);

                        cout << "Enter price:\n>";
                        getline(cin, price);


                        Medicine medicine(name, manufacturer, stoi(price));

                        int id = medicine.save(hdbc);

                        cout << "Medicine with id=" << id << " created\n";

                        break;
                    }
                    case 4: {
                        vector<Medicine> medicines = Medicine::findAll(hdbc);

                        if (!medicines.empty()) {
                            vector<string> headers = {"№", "Name", "Manufacturer", "Price"};

                            vector<vector<string>> data;

                            int i = 1;
                            for (Medicine medicine: medicines) {
                                vector<string> tmp = medicine.toStringVector();
                                tmp[0] = to_string(i++);
                                data.push_back(tmp);
                            }

                            TablePrinter::printTable(headers, data);

                        } else {
                            cout << "No medicines found." << endl;
                        }
                        break;
                    }

                    case 5: {
                        Date date1(1, 2, 2005);
                        Date date2(4, 2, 2005);

                        Request request(date1, date2, 30);

                        try {
                            request.save(hdbc);
                        }
                        catch (const runtime_error &e) {
                            cout << "Error: " << e.what() << endl;
                        }

                        break;
                    }

                    case 6: {
                        MedicineBuying medicineBuying(30, 20, 20);

                        try {
                            medicineBuying.save(hdbc);
                        }
                        catch (const runtime_error &e) {
                            cout << "Error: " << e.what() << endl;
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
