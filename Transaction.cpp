#include <iostream>
#include <iomanip> // https://en.cppreference.com/w/cpp/locale/money_put.html
#include <locale>
#include "Transaction.h"

using namespace std;

/** Create new transaction with today's date */
Transaction::Transaction() {
    time(&timestamp_); // Get the timestamp for the current date and time
    datetime_ = *localtime(&timestamp_);
}
/** Create new transaction with year, month and day */
Transaction::Transaction(int year, int month, int day) {
    setDate(year, month, day);
}
/** Set transaction date with year, month and day */
void Transaction::setDate(int year, int month, int day) {
    datetime_.tm_year = year - 1900; // Number of years since 1900
    datetime_.tm_mon = month - 1; // Number of months since January
    datetime_.tm_mday = day;
    datetime_.tm_hour = 0; datetime_.tm_min = 0; datetime_.tm_sec = 0;
    datetime_.tm_isdst = -1; // Daylight Savings - use computer's timezone setting
    timestamp_ = mktime(&datetime_);
}
/** Get date in yy/mm/dd format */
string Transaction::getDateString() {
    char output[11];
    strftime(output, 11, "%Y/%m/%d", &datetime_);
    return output;
}

// testing
int main() {
    Transaction t(2020,10,5);
    cout << t.getDateString() << endl;
    return 0;
}