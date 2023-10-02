#ifndef LAB1_MENU_H
#define LAB1_MENU_H

#include <string>
#include <iostream>
#include <regex>
#include <iomanip>
#include <limits>
#include "../models/pharmacy.h"
#include "../models/medicine.h"
#include "../models/date.h"
#include "../models/request.h"
#include "../models/medicine_buying.h"
#include "../models/manufacturer.h"
#include "table_printer.h"
#include "../models/country.h"


using namespace std;

#define HELP "Menu:\n\
1) Add pharmacy\n\
2) Get pharmacies list\n\
\n\
3) Add medicine\n\
4) Get medicines list \n\
\n\
5) Add request\n\
6) Get requests list\n\
\n\
7) Add medicine buying\n\
8) Get medicine buyings list\n\
\n\
9) Add manufacturer\n\
0) Exit\n\
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
                        string creationDateString, completionDateString, pharmacyNumber;

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        cout << "Enter pharmacy number:\n>";
                        getline(cin, pharmacyNumber);

                        cout << "Enter creation date in format dd/mm/yyyy:\n>";
                        getline(cin, creationDateString);

                        cout << "Enter completion date in format dd/mm/yyyy:\n>";
                        getline(cin, completionDateString);

                        vector<Pharmacy> pharmacies = Pharmacy::findAll(hdbc);

                        Date creationDate = Date::parseFromString(creationDateString);
                        Date completionDate = Date::parseFromString(completionDateString);

                        int pharmacyId = pharmacies[stoi(pharmacyNumber) - 1].getId();
                        Request request(creationDate, completionDate, pharmacyId);

                        try {
                            request.save(hdbc);
                        }
                        catch (const runtime_error &e) {
                            cout << "Error: " << e.what() << endl;
                        }

                        break;
                    }
                    case 6: {
                        vector<Request> requests = Request::findAll(hdbc);

                        if (!requests.empty()) {
                            vector<string> headers = {"№", "Creation date", "Completion date", "Pharmacy"};

                            vector<vector<string>> data;

                            int i = 1;
                            for (Request request: requests) {
                                vector<string> tmp = request.toStringVector(hdbc);
                                tmp[0] = to_string(i++);
                                data.push_back(tmp);
                            }

                            TablePrinter::printTable(headers, data);

                        } else {
                            cout << "No requests found." << endl;
                        }
                        break;
                    }

                    case 7: {
                        string medicineString, requestString, medicineNumberString;


                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        cout << "Enter medicine :\n>";
                        getline(cin, medicineString);

                        cout << "Enter request:\n>";
                        getline(cin, requestString);

                        cout << "Enter medicine number:\n>";
                        getline(cin, medicineNumberString);

                        vector<Medicine> medicines = Medicine::findAll(hdbc);
                        int medicineId = medicines[stoi(medicineString) - 1].getId();

                        vector<Request> requests = Request::findAll(hdbc);
                        int requestId = requests[stoi(requestString) - 1].getId();

                        MedicineBuying medicineBuying(medicineId, requestId, stoi(medicineNumberString));

                        try {
                            medicineBuying.save(hdbc);
                        }
                        catch (const runtime_error &e) {
                            cout << "Error: " << e.what() << endl;
                        }

                        break;
                    }
                    case 8: {
                        vector<MedicineBuying> medicineBuyings = MedicineBuying::findAll(hdbc);

                        if (!medicineBuyings.empty()) {
                            vector<Medicine> medicines;
                            for (MedicineBuying medicineBuying: medicineBuyings) {
                                Medicine medicine = Medicine::findById(hdbc, medicineBuying.getMedicineId());
                                medicines.push_back(medicine);
                            }

                            vector<Request> requests;
                            for (MedicineBuying medicineBuying: medicineBuyings) {
                                Request request = Request::findById(hdbc, medicineBuying.getRequestId());
                                requests.push_back(request);
                            }

                            vector<Pharmacy> pharmacies;
                            for (Request request: requests) {
                                Pharmacy pharmacy = Pharmacy::findById(hdbc, request.getPharmacyId());
                                pharmacies.push_back(pharmacy);
                            }

                            vector<int> numbers;
                            for (int i = 0; i < medicineBuyings.size(); i++) {
                                numbers.push_back(i + 1);
                            }

                            vector<string> headers = {"№", "Medicine", "Pharmacy", "Medicine number"};

                            vector<vector<string>> data;
                            for (int i = 0; i < medicineBuyings.size(); ++i) {
                                vector<string> data_i;

                                data_i.push_back(to_string(numbers[i]));
                                data_i.push_back(medicines[i].getName());
                                data_i.push_back(pharmacies[i].getName());
                                data_i.push_back(to_string(medicineBuyings[i].getMedicineNumber()));

                                data.push_back(data_i);
                            }

                            TablePrinter::printTable(headers, data);
                        } else {
                            cout << "No medicine buyings found." << endl;
                        }
                        break;

                    }

                        //add manufacturer
                    case 9: {
                        vector<Country> countries = Country::findAll(hdbc);

                        if (!countries.empty()) {
                            string name, countryNumber;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            cout << "Select country [1, " << countries.size() << "]:\n>";
                            getline(cin, countryNumber);

                            cout << "Enter name:\n>";
                            getline(cin, name);

                            int countryId = countries[stoi(countryNumber) - 1].getId();
                            Manufacturer manufacturer(name, countryId);
                            manufacturer.save(hdbc);
                            break;
                        }

                        cout << "No countries found. You cannot add a manufacturer\n";
                        break;
                    }

                        // get manufacturers list
                    case 10: {

                    }

                    case 0: {
                        isWorked = false;
                        break;
                    }
                    default: {
                        cout << "Error" << endl;
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
