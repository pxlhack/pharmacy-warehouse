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
8) Delete medicine buyings list\n\
\n\
10) Add manufacturer\n\
11) Get manufacturers list\n\
\n\
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
                        pharmacy.save(hdbc);

                        break;
                    }
                    case 2: {
                        vector<Pharmacy> pharmacies = Pharmacy::findAll(hdbc);

                        if (!pharmacies.empty()) {

                            vector<vector<string>> data;

                            for (int i = 0; i < pharmacies.size(); i++) {
                                data.push_back(
                                        {
                                                to_string(i + 1),
                                                pharmacies[i].getName(),
                                                pharmacies[i].getAddress(),
                                                pharmacies[i].getPhoneNumber()
                                        }
                                );
                            }

                            vector<string> headers = {"№", "Pharmacy", "Address", "Phone number"};

                            TablePrinter::printTable(headers, data);

                            break;
                        }

                        cout << "No pharmacies found." << endl;

                        break;
                    }

                    case 3: {
                        vector<Manufacturer> manufacturers = Manufacturer::findAll(hdbc);

                        if (!manufacturers.empty()) {
                            string name, manufacturerNumber, price;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            cout << "Enter name:\n>";
                            getline(cin, name);

                            cout << "Select manufacturer [1, " << manufacturers.size() << "]:\n>";
                            getline(cin, manufacturerNumber);

                            cout << "Enter price:\n>";
                            getline(cin, price);

                            int manufacturerId = manufacturers[stoi(manufacturerNumber) - 1].getId();
                            Medicine medicine(name, manufacturerId, stoi(price));

                            try {
                                medicine.save(hdbc);
                            }
                            catch (const runtime_error &e) {
                                cout << "Error: " << e.what() << endl;
                            }

                            break;
                        }

                        cout << "No manufacturers found. You cannot add a medicine\n";

                        break;
                    }
                    case 4: {
                        vector<Medicine> medicines = Medicine::findAll(hdbc);

                        if (!medicines.empty()) {
                            vector<Manufacturer> manufacturers;
                            for (const Medicine &medicine: medicines) {
                                manufacturers.push_back(Manufacturer::findById(hdbc, medicine.getManufacturerId()));
                            }

                            vector<vector<string>> data;

                            for (int i = 0; i < medicines.size(); ++i) {
                                data.push_back(
                                        {
                                                to_string(i + 1),
                                                medicines[i].getName(),
                                                manufacturers[i].getName(),
                                                to_string(medicines[i].getPrice())
                                        }
                                );
                            }

                            vector<string> headers = {"№", "Medicine", "Manufacturer", "Price"};

                            TablePrinter::printTable(headers, data);

                            break;
                        }

                        cout << "No medicines found." << endl;
                        break;
                    }

                    case 5: {
                        vector<Pharmacy> pharmacies = Pharmacy::findAll(hdbc);

                        if (!pharmacies.empty()) {
                            string creationDateString, completionDateString, pharmacyNumber;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            cout << "Select pharmacy [1, " << pharmacies.size() << "]:\n>";
                            getline(cin, pharmacyNumber);

                            cout << "Enter creation date in format dd/mm/yyyy:\n>";
                            getline(cin, creationDateString);

                            cout << "Enter completion date in format dd/mm/yyyy:\n>";
                            getline(cin, completionDateString);

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

                        cout << "No pharmacies found. You cannot add a request\n";

                        break;
                    }
                    case 6: {
                        vector<Request> requests = Request::findAll(hdbc);

                        if (!requests.empty()) {
                            vector<Pharmacy> pharmacies;
                            for (Request request: requests) {
                                pharmacies.push_back(Pharmacy::findById(hdbc, request.getPharmacyId()));
                            }

                            vector<vector<string>> data;

                            for (int i = 0; i < requests.size(); ++i) {
                                data.push_back(
                                        {
                                                to_string(i + 1),
                                                requests[i].getCreationDate().toString(),
                                                requests[i].getCompletionDate().toString(),
                                                pharmacies[i].getName()
                                        }
                                );
                            }

                            vector<string> headers = {"№", "Creation date", "Completion date", "Pharmacy"};

                            TablePrinter::printTable(headers, data);

                            break;
                        }

                        cout << "No requests found." << endl;

                        break;
                    }

                    case 7: {
                        vector<Medicine> medicines = Medicine::findAll(hdbc);

                        if (!medicines.empty()) {
                            vector<Request> requests = Request::findAll(hdbc);

                            if (!requests.empty()) {
                                string medicineString, requestString, medicineNumberString;

                                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                cout << "Select medicine [1, " << medicines.size() << "]:\n>";
                                getline(cin, medicineString);

                                cout << "Select request [1, " << requests.size() << "]:\n>";
                                getline(cin, requestString);

                                cout << "Enter medicine number:\n>";
                                getline(cin, medicineNumberString);

                                int medicineId = medicines[stoi(medicineString) - 1].getId();
                                int requestId = requests[stoi(requestString) - 1].getId();

                                MedicineBuying medicineBuying(requestId, medicineId, stoi(medicineNumberString));

                                try {
                                    medicineBuying.save(hdbc);
                                }
                                catch (const runtime_error &e) {
                                    cout << "Error: " << e.what() << endl;
                                }
                                break;
                            }

                            cout << "No requests found. You cannot add a medicine buying\n";

                            break;
                        }

                        cout << "No medicines found. You cannot add a medicine buying\n";

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

                            vector<vector<string>> data;
                            for (int i = 0; i < medicineBuyings.size(); ++i) {
                                data.push_back(
                                        {
                                                to_string(i + 1),
                                                medicines[i].getName(),
                                                pharmacies[i].getName(),
                                                to_string(medicineBuyings[i].getMedicineNumber())

                                        }
                                );
                            }

                            vector<string> headers = {"№", "Medicine", "Pharmacy", "Medicine number"};

                            TablePrinter::printTable(headers, data);

                            break;
                        }

                        cout << "No medicine buyings found." << endl;

                        break;
                    }
                    case 9: {
                        vector<MedicineBuying> medicineBuyings = MedicineBuying::findAll(hdbc);

                        if (!medicineBuyings.empty()) {
                            string medicineBuyingString;

                            cout << "Select medicine buying [1, " << medicineBuyings.size() << "]:\n>";
                            getline(cin, medicineBuyingString);

                            int requestId = medicineBuyings[stoi(medicineBuyingString) - 1].getRequestId();
                            int medicineId = medicineBuyings[stoi(medicineBuyingString) - 1].getMedicineId();

                            MedicineBuying::deleteByRequestIdAndMedicineId(hdbc, requestId, medicineId);

                        }

                        cout << "No medicine buyings found." << endl;

                        break;
                    }

                        //add manufacturer
                    case 10: {
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
                    case 11: {
                        vector<Manufacturer> manufacturers = Manufacturer::findAll(hdbc);

                        if (!manufacturers.empty()) {
                            vector<Country> countries;
                            for (const Manufacturer &manufacturer: manufacturers) {
                                countries.push_back(Country::findById(hdbc, manufacturer.getCountryId()));
                            }

                            vector<vector<string>> data;
                            for (int i = 0; i < manufacturers.size(); ++i) {
                                data.push_back(
                                        {
                                                to_string(i + 1),
                                                manufacturers[i].getName(),
                                                countries[i].getName()
                                        }
                                );
                            }

                            vector<string> headers = {"№", "Manufacturer", "Country"};

                            TablePrinter::printTable(headers, data);

                            break;
                        }

                        cout << "No manufacturers found. You can add a manufacturer\n";

                        break;
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
    SQLHDBC hdbc;

    static bool isValidCommand(const string &command) {
        return isPositiveValue(string_view(command));
    }

    static bool isPositiveValue(string_view s) {
        static const regex r(R"(\d{1,})");
        return regex_match(s.data(), r);
    }


};


#endif //LAB1_MENU_H
