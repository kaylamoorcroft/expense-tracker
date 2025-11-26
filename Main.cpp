#include <iostream>
#include "Spreadsheet.h"

using namespace std;

// main will include summary, import, export functions
// https://www.geeksforgeeks.org/cpp/csv-file-management-using-c/


// create new transaction by parsing record
Transaction* parseRecord(char* record) {
    char *attribute = strtok(record, ",");
    int counter = 0;
    struct tm datetime;
    double amount;
    string category;
    while (attribute != nullptr) {
        // set attribute value
        switch(counter) {
            case 0: // date
                cout << "datestring: " << attribute << endl;
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
    cout << "year: " << datetime.tm_year << endl;
    if (amount < 0) {
        return new Expense(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, amount, category);
    }
    else {
        return new Income(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, amount, category);
    }
}

// testing
int main() {
    Income i(2020,5,3,50,"tutoring");
    Expense e(2020,5,4, 40, "food");
    string record1 = i.toString();
    char testRecord1[30];
    strcpy(testRecord1, record1.c_str()); 
    Spreadsheet s;
    s.addEntry(parseRecord(testRecord1));
    //s.addEntry(&i);
    s.addEntry(new Expense(30, "rent"));
    s.addEntry(&e);
    s.addEntry(new Income(20, "tutoring"));
    s.addEntry(new Income(2022,5,3,100000, "big moneys"));
    string r = "2022/01/01,50.00,hewwo";
    char testRecord[30];
    strcpy(testRecord, r.c_str()); 
    s.addEntry(parseRecord(testRecord));
    cout << "initial: " << endl;
    s.display();
    // cout << "\n--from nov 2025: " << endl;
    // for (Transaction* t : s.filterMonth(2025, 11)) {
    //     t->display();
    // }
    // s.updateEntry();
    // cout << "updated: " << endl;
    // s.display();
    // s.deleteEntry();
    // cout << "after deletion: " << endl;
    // s.display();
    return 0;
}