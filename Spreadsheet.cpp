#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include "Spreadsheet.h"

using namespace std;


// Move Constructor
Spreadsheet::Spreadsheet(Spreadsheet&& other) noexcept
    : entries_(std::move(other.entries_)) { cout << "called move constructor" << endl;}

// Move Assignment Operator
Spreadsheet& Spreadsheet::operator=(Spreadsheet&& other) noexcept {
    if (this != &other) {
        entries_ = std::move(other.entries_);
    }
    cout << "Move Assignment for spreadsheet " << endl;
    return *this;
}
// create new entry
bool Spreadsheet::addEntry(unique_ptr<Transaction> entry) {
    return entries_.insert(std::move(entry)).second;
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
        cout << "Invalid input... please enter a positive amount > "; cin >> amount;
    }
    cout << "Please enter category > "; cin >> category;
    if (type == 'e') {
        return addEntry(make_unique<Expense>(year, month, day, -amount, category));
    }
    else {
        return addEntry(make_unique<Income>(year, month, day, amount, category));
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
        auto it = std::find_if(entries_.begin(), entries_.end(),
            [&](const std::unique_ptr<Transaction>& p) { return p.get() == entry; });
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
    cout << "Select what you want to edit:" << endl;
    cout << "\t(1) Date \n\t(2) Amount \n\t(3) Category" << endl;
    int option; cin >> option;
    string category;
    if (entry) {
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
                cout << "Please enter new date: \n\tYear > "; cin >> year;
                cout << "\tMonth > "; cin >> month;
                cout << "\tDay > "; cin >> day;
                entry->setDate(year,month,day);
                break;
            case 2:
                double amount; 
                cout << "Please enter new amount > "; cin >> amount;
                while (amount < 0) {
                    cout << "Invalid input... please enter a positive amount > "; cin >> amount;
                }
                entry->setAmount(amount);
                break;
            case 3:
                cout << "Please enter new category > "; cin >> category;
                entry->setCategory(category);
                break;
            default:
                cout << "Invalid option..." << endl;
        }
        return entries_.insert(std::move(temp)).second;
    }
    return false;
}
// select entry
Transaction* Spreadsheet::getEntry() {
    int year, month, day;
    cout << "Please enter date: \n\tYear > "; cin >> year;
    cout << "\tMonth > "; cin >> month;
    cout << "\tDay > "; cin >> day;
    set<Transaction*, TransactionRawPtrComparator> filtered = printEntriesFromDate(year, month, day);
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
    cout << "   Date    | Amount      |  Tag" << endl;
    cout << "-----------|-------------|---------" << endl;
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        (*it)->display(); 
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

        //add converted entry from csv file
        addEntry(parseRecord(recordArray));

        // deallocate char pointer memory
        free(recordArray);
        recordArray = NULL;
    }
    readFile.close();
}

//save updated spreadsheet or transactions to file
void Spreadsheet::exportFile(string filename){
    ofstream saveFile(filename);

    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        string record = (*it)->toString();
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
    set<Transaction*, TransactionRawPtrComparator> monthEntries;
    monthEntries = filterMonth(year, month);

    for (auto it = monthEntries.begin(); it != monthEntries.end(); ++it) {
        (*it)->display(); 
    }

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
