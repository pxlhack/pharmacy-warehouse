#ifndef LAB1_DATE_H
#define LAB1_DATE_H

#include <string>
#include <sstream>

using namespace std;

class Date {
public:
    Date() {}

    Date(int day, int month, int year) :
            day(day), month(month), year(year) {}

    string toString() const {
        ostringstream oss;
        string dayStr = (day < 10) ? "0" + to_string(day) : to_string(day);
        string monthStr = (month < 10) ? "0" + to_string(month) : to_string(month);
        oss << dayStr << "/" << monthStr << "/" << year;
        return oss.str();
    }

    static Date parseFromString(const string &str) {
        int day = stoi(str.substr(0, 2));
        int month = stoi(str.substr(3, 2));
        int year = stoi(str.substr(6, 4));

        return {day, month, year};
    }

    static Date parseFromStringFromDB(const string &str) {
        int year = stoi(str.substr(0, 4));
        int month = stoi(str.substr(5, 2));
        int day = stoi(str.substr(8, 2));

        return {day, month, year};
    }

    static Date getCurrentDate() {
        const time_t tm = time(nullptr);

        char buf[64];
        strftime(buf, std::size(buf), "%d/%m/%Y", localtime(&tm));
        string dateString(buf);

        return parseFromString(dateString);
    }

private:
    int day;
    int month;
    int year;
};

#endif //LAB1_DATE_H
