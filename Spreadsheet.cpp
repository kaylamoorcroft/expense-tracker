#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include "Spreadsheet.h"
#include "Utils.h"

using namespace std;

// create new entry
bool Spreadsheet::addEntry(unique_ptr<Transaction> entry) {
    return entries_.insert(std::move(entry)).second;
}
// add new entry through user input
bool Spreadsheet::addEntryFromUser() {
    int year, month, day;
    double amount; 
    string category;
    char type, dateType;
    
    cout << "\nChoose Income (i) or Expense (e) > "; cin >> type;
    while (type != 'i' && type != 'e') { // income or expense
        cout << "Invalid input... please try again > "; cin >> type;
    }
    cout << "Choose to use today's date (t) or enter custom date (c) > "; 
    cin >> dateType;
    while (dateType != 't' && dateType != 'c') { // custom to default date
        cout << "Invalid input... please try again > "; cin >> dateType;
    }
    // custom date
    if (dateType == 'c') {
        Utils::getValidNumInput(year, "\tYear > ", 1900);
        Utils::getValidNumInput(month, "\tMonth > ", 1, 12);
        Utils::getValidNumInput(day, "\tDay > ", 1, 31);
    }
    Utils::getValidNumInput(amount, "Please enter amount spent / received > ", 0.0);
    cout << "Please enter category > "; getline(cin >> ws, category); // read line with spaces and skip leading whitespace

    if (type == 'e') { // expense
        if (dateType == 't') // default date
            return addEntry(make_unique<Expense>(amount, category));
        else // custom date
            return addEntry(make_unique<Expense>(year, month, day, amount, category));
    }
    else { // income
        if (dateType == 't') // default date
            return addEntry(make_unique<Income>(amount, category));
        else // custom date
            return addEntry(make_unique<Income>(year, month, day, amount, category));
    }
}
// remove entry
bool Spreadsheet::deleteEntry() {
    Transaction* entry = getEntry();
    if (entry == nullptr) {
        return false;
    }
    char option = '0';
    cout << "Are you sure you want to delete this entry (y/n)? > "; cin >> option;
    while (!(option == 'y' || option == 'n')) {
        cout << "Invalid selection... please enter 'y' or 'n' > "; cin >> option;
    }
    if (option == 'y') {
        auto it = entries_.begin();
        for (; it != entries_.end(); ++it) {
            if ((*it).get() == entry) {
                break;
            }
        }
        entries_.erase(it);
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
    int option; Utils::getValidNumInput(option,"Select what you want to edit:\n\t(1) Date \n\t(2) Amount \n\t(3) Category\n\t>");
    string category;

    auto it = entries_.begin();
    for (; it != entries_.end(); ++it) {
        if ((*it).get() == entry) {
            break;
        }
    }
    unique_ptr<Transaction> temp;
    temp.swap(const_cast<std::unique_ptr<Transaction>&>(*it));
    entries_.erase(it); 
    entry = temp.get();

    // modify
    switch (option) {
        case 1:
            int year, month, day;
            cout << "Please enter new date: \n"; 
            Utils::getValidNumInput(year, "\tYear > ", 1900);
            Utils::getValidNumInput(month, "\tMonth > ", 1, 12);
            Utils::getValidNumInput(day, "\tDay > ", 1, 31);
            entry->setDate(year,month,day);
            break;
        case 2:
            double amount; 
            Utils::getValidNumInput(amount, "Please enter new amount > ", 0.0);
            entry->setAmount(amount);
            break;
        case 3:
            cout << "Please enter new category > "; getline(cin >> ws, category); // read line with spaces and skip leading whitespace
            entry->setCategory(category);
            break;
        default:
            cout << "Invalid option..." << endl;
            return false;
    }
    return entries_.insert(std::move(temp)).second;
}
// select entry
Transaction* Spreadsheet::getEntry() {
    int year, month, day;
    cout << "Please enter date: \n"; 
    Utils::getValidNumInput(year, "\tYear > ", 1900);
    Utils::getValidNumInput(month, "\tMonth > ", 1, 12);
    Utils::getValidNumInput(day, "\tDay > ", 1, 31);
    set<Transaction*, TransactionRawPtrComparator> filtered = printEntriesFromDate(year, month, day);
    if (filtered.size() == 0) { // no entries - exit
        cout << "No entries found for " << year << "/" << month << "/" << day << endl;
        return nullptr;
    }
    if (filtered.size() == 1) { // get only entry
        return *filtered.begin(); 
    }
    int num; 
    Utils::getValidNumInput(num, "Select entry number > ");
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
set<Transaction*, TransactionRawPtrComparator> Spreadsheet::printEntriesFromDate(int year, int month, int day) {
    set<Transaction*, TransactionRawPtrComparator> filtered;
    int counter = 1;
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if ((*it)->datesAreEqual(year, month, day)) {
            cout << "(" << counter << ") ";
            (*it)->display();
            filtered.insert(it->get()); // insert raw pointer (not unique ptr)
            counter++;
        }
    }
    return filtered;
}

// filter by date
set<Transaction*, TransactionRawPtrComparator> Spreadsheet::filterMonth(int year, int month) {
    set<Transaction*, TransactionRawPtrComparator> filtered;
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if ((*it)->monthsAreEqual(year, month)) {
            filtered.insert(it->get()); // insert raw pointer (not unique ptr)
        }
    }
    return filtered;
}
/** display all entries in a table format */
void Spreadsheet::display() {
    int prevYear = 1900; int curYear = 1900;
    string prevMonth = ""; string curMonth = "";
    bool yearJump = true;
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        curMonth = (*it)->getMonthName();
        curYear = (*it)->getYear();
        yearJump = curYear != prevYear;

        // display year header
        if (yearJump) {
            prevYear = curYear;
            cout << endl;
            cout << "-----------------------------------" << endl;
            cout << setw(20) << curYear << endl;
        }
        // display month header
        if ((curMonth != prevMonth) || yearJump) {
            prevMonth = curMonth;
            cout << "-----------------------------------" << endl;
            cout << setw(35) << curMonth << endl;
            cout << "-----------------------------------" << endl;
            cout << " Day |    Amount    |  Tag " << endl;
            cout << "-----|--------------|--------------" << endl;
        }
        // display transaction
        (*it)->displayInTable(); 
    }
}

// create new transaction by parsing record
unique_ptr<Transaction> Spreadsheet::parseRecord(char* record) {
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
        return make_unique<Expense>(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, -amount, category);
    }
    else {
        return make_unique<Income>(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, amount, category);
    }
}

//read csv file, convert string records into Transaction objects and add Entries
void Spreadsheet::importFile(string filename){
    entries_.clear(); // clear existing file load
    ifstream readFile(filename);
    string record;

    // Check if the file is opened
    if (!readFile.is_open()) {
        cout << "\nError: Unable to open file!" << endl;
        return;
    }

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

        //add converted entry from csv file
        addEntry(parseRecord(recordArray));

        // deallocate char pointer memory
        free(recordArray);
        recordArray = NULL;
    }
    readFile.close();
    cout << "\n" << filename << " loaded successfully" << endl;
}

//save updated spreadsheet or transactions to file
void Spreadsheet::exportFile(string filename){
    ofstream saveFile(filename);

    // Check if the file is opened
    if (!saveFile.is_open()) {
        cout << "\nError: Unable to save file!" << endl;
        return;
    }

    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        string record = (*it)->toString();
        saveFile << record << "\n";
    }
    saveFile.close();
    cout << "\nSaving " << filename << "..." << endl;
}

//calculate total income and total expense for a given month
void Spreadsheet::calculateStats(){
    int year, month;
    cout << "Please enter year and month: \n"; 
    Utils::getValidNumInput(year, "\tYear > ", 1900);
    Utils::getValidNumInput(month, "\tMonth > ", 1, 12);

    //filter entries by month
    set<Transaction*, TransactionRawPtrComparator> monthEntries;
    monthEntries = filterMonth(year, month);

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

    cout << "\n" << setw(24) << "SUMMARY for " << Utils::getMonthName(month) << " " << year << "\n" << endl;
    cout << "   Total Income  |  Total Expense  |   Cashflow" << endl;
    cout << "-----------------|-----------------|--------------" << endl;

    //display formatted totalIncome, totalExpense, and cashflow (with spacing)
    cout << setw(12)<< Utils::formatMoney(totalIncome) << setw(6) << "|"
        << setw(12) << Utils::formatMoney(totalExpense) << setw(6) << "|"
        << setw(12) << Utils::formatMoney(cashflow) << endl;
}
