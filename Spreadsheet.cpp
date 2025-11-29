#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include "Spreadsheet.h"

using namespace std;

// create new entry
bool Spreadsheet::addEntry(Transaction* entry) {
    return entries_.insert(entry).second;
}
// add new entry through user input
bool Spreadsheet::addEntryFromUser() {
    int year, month, day;
    double amount; 
    string category;
    char type;
    
    cout << "\nChoose Income (i) or Expense (e) > "; cin >> type;
    while (type != 'i' && type != 'e') {
        cout << "Invalid input... please try again > "; cin >> type;
    }
    cout << "Please enter  date: \n\tYear > "; cin >> year;
    cout << "\tMonth > "; cin >> month;
    cout << "\tDay > "; cin >> day;
    cout << "Please enter amount spent / received > "; cin >> amount;
    while (amount < 0) {
        cout << "Invalid input... please enter a positive amount > "; cin >> type;
    }
    cout << "Please enter category > "; cin >> category;
    if (type == 'e') {
        return addEntry(new Expense(year, month, day, amount, category));
    }
    else {
        return addEntry(new Income(year, month, day, amount, category));
    }
}
// remove entry
bool Spreadsheet::deleteEntry() {
    Transaction* entry = getEntry();
    if (entry == nullptr) {
        cout << "entry does not exist..." << endl;
        return false;
    }
    char option;
    cout << "Are you sure you want to delete this entry (y/n)? > "; cin >> option;
    if (option == 'y') {
        entries_.erase(entry);
        return true;
    }
    return false;
}
// change entry info
bool Spreadsheet::updateEntry() {
    Transaction* entry = getEntry();
    if (entry == nullptr) {
        return false;
    }
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
    return addEntry(entry);
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
// filter by date and print
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

// filter by date
set<Transaction*, greater<Transaction*>> Spreadsheet::filterMonth(int year, int month) {
    set<Transaction*, greater<Transaction*>> filtered;
    for (Transaction* t : entries_) {
        if (t->monthsAreEqual(year, month)) {
            filtered.insert(t);
        }
    }
    return filtered;
}
/** display all entries in a table format */
void Spreadsheet::display() {
    cout << "   Date    | Amount      |  Tag" << endl;
    cout << "-----------|-------------|---------" << endl;
    for (Transaction* t : entries_) {
        t->display();
    }
}

// create new transaction by parsing record
Transaction* Spreadsheet::parseRecord(char* record) {
    char *attribute = strtok(record, ",");
    int counter = 0;
    struct tm datetime;
    double amount;
    string category;
    while (attribute != nullptr) {
        // set attribute value
        switch(counter) {
            case 0: // date
                strptime(attribute, "%Y/%m/%d", &datetime);
                datetime.tm_hour = 0; datetime.tm_min = 0; datetime.tm_sec = 0;
                datetime.tm_isdst = -1; // Daylight Savings - use computer's timezone setting
                mktime(&datetime);
            case 1: // amount
                amount = stod(attribute);
                break;
            case 2: // category
                category = attribute;
        }
        // Get the next substring
        attribute = strtok(nullptr, ",");
        counter++;
    }
    if (amount < 0) {
        return new Expense(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, -amount, category);
    }
    else {
        return new Income(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, amount, category);
    }
}

//read csv file, convert string records into Transaction objects and add Entries
void Spreadsheet::importFile(string filename){
    ifstream readFile(filename);
    string record;

    //loop to get all lines/records from csv file
    while (getline(readFile, record)){
        //convert (string) record to array of chars to be used in parseRecord()
        int arrayLength = record.length();
        char* recordArray = (char*)malloc(arrayLength * sizeof(char));
        if (recordArray == NULL) {  // Handle allocation failure
            cout << "Unable to read file..." << endl;
            return;
        }
        strcpy(recordArray, record.c_str());

        //pass new char array to be converted from csv data
        Transaction* newRecord = parseRecord(recordArray);

        // deallocate char pointer memory
        free(recordArray);
        recordArray = NULL;

        //add converted entry from csv file
        addEntry(newRecord);
    }
    readFile.close();
}

//save updated spreadsheet or transactions to file
void Spreadsheet::exportFile(string filename){
    ofstream saveFile(filename);

    // for each entry in spreadsheet, convert literal values to string type before saving in csv
    for (Transaction* t: entries_){
        //convert literal values back to strings before saving to file:
        string record = t->toString(); //convert record Transaction* type to string
        saveFile << record << "\n";
    }
    saveFile.close();
}

//calculate total income and total expense for a given month
void Spreadsheet::calculateStats(){
    int year, month;
    cout << "Please enter date: \n\tYear > "; cin >> year;
    cout << "\tMonth > "; cin >> month;

    //filter entries by month
    set<Transaction*, greater<Transaction*>> monthEntries;
    monthEntries = filterMonth(year, month);

    //display entries for selected month
    // for (Transaction* t : monthEntries) {
    //     t->display();
    // }

    double totalIncome, totalExpense, cashflow;
    totalIncome = 0;
    totalExpense = 0;

    //determine if entry is income or expense and record total for month
    for(Transaction* t : monthEntries){
        if(t->getAmount() >=0){
            totalIncome += t->getAmount();
        }
        else{
            totalExpense += t->getAmount();
        }
    }

    //net profit or loss of a given month
    cashflow = totalIncome - (totalExpense * -1);

    cout << "\n" << setw(36) << "MONTHLY SUMMARY\n" << endl;
    cout << "   Total Income  |  Total Expense  |  Cashflow" << endl;
    cout << "-----------------|-----------------|------------" << endl;

    cout.imbue(locale("en_CA.UTF-8"));//format total value into money type string

    //display formatted totalIncome, totalExpense, and cashflow (with spacing)
    cout << setw(12)<< showbase << put_money(totalIncome*100) << setw(6) << "|"
        << setw(12) << showbase << put_money(totalExpense*100) << setw(6) << "|"
        << setw(10) << showbase << put_money(cashflow*100) << endl;
}
