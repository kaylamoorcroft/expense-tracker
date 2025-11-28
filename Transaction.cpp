#include <iostream>
#include <sstream>
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
// Copy Constructor
Transaction::Transaction(const Transaction& t) {
    timestamp_ = t.timestamp_;
    datetime_ = t.datetime_;
    amount_ = t.amount_;
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
/** returns true if Transaction date is same as date passed in */
bool Transaction::datesAreEqual(int year, int month, int day) {
    return (datetime_.tm_year == year - 1900) 
    && (datetime_.tm_mon == month - 1)
    && (datetime_.tm_mday == day);
}
/** returns true if Transaction month and year is same as info passed in*/
bool Transaction::monthsAreEqual(int year, int month) {
    return (datetime_.tm_year == year - 1900) 
    && (datetime_.tm_mon == month - 1);
}
/** Add support for comparing / sorting Transactions by date */
// bool Transaction::operator<(const Transaction& other) const {
//     if (timestamp_ != other.timestamp_) {
//         cout << timestamp_ << " < " << other.timestamp_ << " = " << (timestamp_ < other.timestamp_) << endl;
//         return timestamp_ < other.timestamp_;
//     }
//     return amount_ < other.amount_; // Secondary sort by name if dates are equal
// }
/** get the string representation for csv file */
string Transaction::toString() {
    return getDateString() + "," + to_string(amount_) + "," + getCategory();
}
/** Display the amount, date and category of the expense */
void Transaction::display() {
    cout << getDateString() << " | " << timestamp_ << " | " << setw(11) << getAmountString() << " | " << getCategory() << endl;
}

/** Create new income of amount dollars from a set source with today's date 
or a default value of $0 and source = "undefined"*/
Income::Income(double amount, string category) : Transaction() {
    amount_ = amount;
    category_ = category;
}
/** Create new income of amount dollars from a set source with specified date 
or a default value of $0 and source = "undefined"*/
Income::Income(int year, int month, int day, double amount, string category) : Transaction(year, month, day) {
    amount_ = amount;
    category_ = category;
}
// Copy Constructor
Income::Income(const Income& t) : Transaction(t) {
    category_ = t.category_;
}
bool Income::operator <(const Income& other) const {
    if (timestamp_ != other.timestamp_) {
        cout << timestamp_ << " < " << other.timestamp_ << " = " << (timestamp_ < other.timestamp_) << endl;
        return timestamp_ < other.timestamp_;
    }
    return amount_ < other.amount_; // Secondary sort by name if dates are equal
}
/** Set income amount in dollars */
void Income::setAmount(double amount) {
    amount_ = amount;
}
/** Set income source */
void Income::setCategory(string category) {
    category_ = category;
}
/** Get a string representation of income amount */
string Income::getAmountString() {
    stringstream ss;
    ss.imbue(locale("en_CA.UTF-8"));
    ss << showbase << put_money(amount_ * 100); // amount in cents
    return ss.str();
}
/** return category string */
string Income::getCategory() {
    return category_;
}

/** Create new expense of amount dollars from a set category with today's date 
or a default value of $0 and category = "undefined"*/
Expense::Expense(double amount, string category) : Transaction() {
    amount_ = -amount;
    category_ = category;
}
/** Create new expense of amount dollars from a set category with specified date 
or a default value of $0 and category = "undefined"*/
Expense::Expense(int year, int month, int day, double amount, string category) : Transaction(year, month, day) {
    amount_ = -amount;
    category_ = category;
}
// Copy Constructor
Expense::Expense(const Expense& t) : Transaction(t) {
    category_ = t.category_;
}
bool Expense::operator <(const Expense& other) const {
    if (timestamp_ != other.timestamp_) {
        cout << timestamp_ << " < " << other.timestamp_ << " = " << (timestamp_ < other.timestamp_) << endl;
        return timestamp_ < other.timestamp_;
    }
    return amount_ < other.amount_; // Secondary sort by name if dates are equal
}
/** Set expense amount in dollars */
void Expense::setAmount(double amount) {
    amount_ = -amount;
}
/** Set expense category */
void Expense::setCategory(string category) {
    category_ = category;
}
/** Get a string representation of expense amount */
string Expense::getAmountString() {
    stringstream ss;
    ss.imbue(locale("en_CA.UTF-8"));
    ss << showbase << put_money(amount_ * 100); // amount in cents
    return ss.str();
}
/** return category string */
string Expense::getCategory() {
    return category_;
}

//get amount_ (protected data member)
double Transaction::getAmount(){
    return amount_;
}