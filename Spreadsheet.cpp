#include <iostream>
#include "Spreadsheet.h"

using namespace std;

void Spreadsheet::test() {
    cout << "test" << endl;
}

// add entry
bool Spreadsheet::addEntry(Transaction* entry) {
    // Income i(2020,5,3,50,"tutoring");
    // Expense e(2020,5,4, 40, "food");
    //entries_.insert(&i);
    // entries_.insert(&e)
    return entries_.insert(entry).second;
}
// delete entry
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
// update entry
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
// select entry
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
// filter by date
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
// return entries
set<Transaction*, greater<Transaction*>> Spreadsheet::getAllEntries() {
    return entries_;
}
// display all entries in a table format
void Spreadsheet::display() {
    cout << "   Date    | Amount      |  Tag" << endl;
    cout << "-----------|-------------|---------" << endl;
    for (Transaction* t : entries_) {
        t->display();
    }
}