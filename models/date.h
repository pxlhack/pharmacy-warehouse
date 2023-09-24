#ifndef LAB1_DATE_H
#define LAB1_DATE_H

#include <string>
#include <sstream>

using namespace std;

class Date {
public:
    Date(int day, int month, int year) :
            day(day), month(month), year(year) {}

    string toString() {
        ostringstream oss;
        oss << day << "/" << month << "/" << year;
        return oss.str();
    }

private:
    int day;
    int month;
    int year;
};

#endif //LAB1_DATE_H
