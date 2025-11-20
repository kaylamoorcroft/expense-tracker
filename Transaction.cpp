#include <iostream>
#include <sstream>
#include <iomanip> // https://en.cppreference.com/w/cpp/locale/money_put.html
#include <locale>
#include <iomanip>
#include "Transaction.h"

using namespace std;

/** Create new transaction with today's date */
Transaction::Transaction() {
    time(&timestamp_); // Get the timestamp for the current date and time
    datetime_ = *localtime(&timestamp_);
}
Transaction::Transaction(const 
    Transaction &t) {
    timestamp_ = t.timestamp_;
    datetime_ = t.datetime_;
    amount_ = t.amount_;
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
bool Transaction::datesAreEqual(int year, int month, int day) {
    return (datetime_.tm_year == year - 1900) 
    && (datetime_.tm_mon == month - 1)
    && (datetime_.tm_mday == day);
}
/** Add support for comparing / sorting Transactions by date */
bool Transaction::operator<(const Transaction& other) const {
    return timestamp_ < other.timestamp_;
}

Income::Income(const Income &i) : Transaction(i) {
    category_ = i.category_;
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
/** Display the amount, date and source of the income */
void Income::display() {
    cout << getDateString() << " | " << setw(11) << getAmountString() << " | " << category_ << endl;
}

Expense::Expense(const Expense &e) : Transaction(e) {
    category_ = e.category_;
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
/** Display the amount, date and category of the expense */
void Expense::display() {
    cout << getDateString() << " | " << setw(11) << getAmountString() << " | " << category_ << endl;
}

bool Spreadsheet::addEntry(Transaction* entry) {
    return entries_.insert(entry).second;
}
Transaction* Spreadsheet::deleteEntry() {
    Transaction* entry = getEntry();
    char option;
    cout << "Are you sure you want to delete this entry (y/n)? > "; cin >> option;
    if (option == 'y') {
        entries_.erase(entry);
        return entry;
    }
    return nullptr;
}
Transaction* Spreadsheet::updateEntry() {
    Transaction* entry = getEntry();
    cout << "Select what you want to edit:" << endl;
    cout << "\t(1) Date \n\t(2) Amount \n\t(3) Category" << endl;
    int option; cin >> option;
    string category;
    switch (option) {
        case 1:
            int year, month, day;
            cout << "Please enter new date: \n\tYear > "; cin >> year;
            cout << "\tMonth > "; cin >> month;
            cout << "\tDay > "; cin >> day;
            entry->setDate(year,month,day);
            break;
        case 2:
            double amount; 
            cout << "Please enter new amount > "; cin >> amount;
            entry->setAmount(amount);
            break;
        case 3:
            cout << "Please enter new category > "; cin >> category;
            entry->setCategory(category);
            break;
        default:
            cout << "Invalid option..." << endl;
    }
    // update value in set
    entries_.erase(entry);
    entries_.insert(entry);
    return entry;
}
Transaction* Spreadsheet::getEntry() {
    int year, month, day;
    cout << "Please enter date: \n\tYear > "; cin >> year;
    cout << "\tMonth > "; cin >> month;
    cout << "\tDay > "; cin >> day;
    set<Transaction*, greater<Transaction*>> filtered = printEntriesFromDate(year, month, day);
    int num; 
    cout << "Select entry number > "; cin >> num;
    int i = 1;
    for (Transaction* t : filtered) {
        if (i == num) {
            return t;
        }
        i++;
    }
    return nullptr;
}
set<Transaction*, greater<Transaction*>> Spreadsheet::printEntriesFromDate(int year, int month, int day) {
    set<Transaction*, greater<Transaction*>> filtered;
    int counter = 1;
    for (Transaction* t : entries_) {
        if (t->datesAreEqual(year, month, day)) {
            cout << "(" << counter << ") ";
            t->display();
            filtered.insert(t);
            counter++;
        }
    }
    return filtered;
}
set<Transaction*, greater<Transaction*>> Spreadsheet::getAllEntries() {
    return entries_;
}

void Spreadsheet::display() {
    cout << "   Date    | Amount      |  Tag" << endl;
    cout << "-----------|-------------|---------" << endl;
    for (Transaction* t : entries_) {
        t->display();
    }
}

// testing
int main() {
    Income i(2020,5,3,50,"tutoring");
    Expense e(2020,5,4, 40, "food");
    Spreadsheet s;
    s.addEntry(&i);
    s.addEntry(new Expense(30, "rent"));
    s.addEntry(&e);
    s.addEntry(new Income(20, "tutoring"));
    s.addEntry(new Income(2022,5,3,100000, "big moneys"));
    cout << "initial: " << endl;
    s.display();
    // s.updateEntry();
    // cout << "updated: " << endl;
    // s.display();
    // s.deleteEntry();
    // cout << "after deletion: " << endl;
    // s.display();
    return 0;
}