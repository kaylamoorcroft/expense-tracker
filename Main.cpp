#include <iostream>
#include <fstream>
#include "Spreadsheet.h"

using namespace std;

// main will include summary, import, export functions
//https://www.geeksforgeeks.org/cpp/convert-string-char-array-cpp/
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
        return new Expense(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, amount, category);
    }
    else {
        return new Income(datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday, amount, category);
    }
}

void displayMenu() {
    cout << "\n-------- MENU --------" << endl;
    cout << "1) Load file" << endl;
    cout << "2) Save file" << endl;
    cout << "3) Add transaction" << endl;
    cout << "4) Delete transaction" << endl;
    cout << "5) Edit transaction" << endl;
    cout << "6) Get transaction" << endl;
    cout << "7) Display spreadsheet" << endl;
    cout << "8) Get stats" << endl;
    cout << "\nPlease select an operation (enter 0 to quit) > ";
}

//read csv file, convert string records into Transaction objects and add Entries
void importFile(string filename){
    ifstream readFile(filename);
    string record;

    //loop to get all lines/records from csv file
    while (getline(readFile, record)){
        //convert (string) record to array of chars to be used in parseRecord()
        int arrayLength = record.length();
        char recordArray[arrayLength + 1];
        strcpy(recordArray, record.c_str());

        //pass new char array to be converted from csv data
        Transaction* newRecord = parseRecord(recordArray);
        newRecord->display();
        
        //add converted entry from csv file
        Spreadsheet spreadsheet;
        spreadsheet.addEntry(newRecord);
    }
}

int main() {
    cout << "~~ WELCOME TO EXPENSE TRACKER ~~" << endl;
    Spreadsheet sheet;
    int option;
    string filename;
    do {
        displayMenu();
        cin >> option;

        switch(option) {
            case 0: // quit
                cout << "\nGoodbye!" << endl;
                break;
            case 1: // load file
                cout << "\tEnter filename > ";
                cin >> filename;
                // code to load filename
                importFile(filename);
                break;
            case 2: // save file
                cout << "\tEnter filename > ";
                cin >> filename;
                // code to save to file
                cout << "\nSaving " << filename << "..." << endl; // replace with function
                break;
            case 3: // add
                // call function to get input and add transaction
                sheet.addEntryFromUser();
                break;
            case 4: // delete
                sheet.deleteEntry();
                break;
            case 5: // edit 
                sheet.updateEntry();
                break;
            case 6: // get
                sheet.getEntry();
                break;
            case 7: // display
                sheet.display();
                break;
            case 8: // stats
                // call stats summary function
                cout << "\nSummary..." << endl; // replace with function
                break;
            default: // invalid option
                cout << "\nInvalid selection... please try again" << endl;
        }
    } while (option != 0);







    // testing

    // Income i(2020,5,3,50,"tutoring");
    // Expense e(2020,5,4, 40, "food");
    // string record1 = i.toString();
    // char testRecord1[30];
    // strcpy(testRecord1, record1.c_str()); 
    // Spreadsheet s;
    // s.addEntry(parseRecord(testRecord1));
    // //s.addEntry(&i);
    // s.addEntry(new Expense(30, "rent"));
    // s.addEntry(&e);
    // s.addEntry(new Income(20, "tutoring"));
    // s.addEntry(new Income(2022,5,3,100000, "big moneys"));
    // string r = "2022/01/01,50.00,hewwo";
    // char testRecord[30];
    // strcpy(testRecord, r.c_str()); 
    // s.addEntry(parseRecord(testRecord));
    // cout << "initial: " << endl;
    // s.display();
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