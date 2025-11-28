#include <iostream>
#include "Spreadsheet.h"

using namespace std;

// main will include summary, import, export functions
//https://www.geeksforgeeks.org/cpp/convert-string-char-array-cpp/
// https://www.geeksforgeeks.org/cpp/csv-file-management-using-c/

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

int main() {
    cout << "~~ WELCOME TO EXPENSE TRACKER ~~" << endl;
    Spreadsheet sheet;
    int option;
    string filename;

    char str1[30] = "2020/06/15,-50.900000, eggs";
    char str2[30] = "2021/07/28,-17.990000, candy";
    Transaction* t1 = sheet.parseRecord(str1);
    Transaction* t2 = sheet.parseRecord(str2);
    t1->display();
    t2->display();
    cout << "t1 < t2: " << (t1 < t2) << endl;
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
                sheet.importFile(filename);
                cout << "\n" << filename << " loaded successfully" << endl;
                break;
            case 2: // save file
                cout << "\tEnter filename > ";
                cin >> filename;
                // code to save to file
                sheet.exportFile(filename);
                cout << "\nSaving " << filename << "..." << endl; // replace with function
                break;
            case 3: // add
                if (sheet.addEntryFromUser()) {
                    cout << "\nSuccessfully inserted new entry!" << endl;
                }
                else {
                    cout << "\nError inserting new entry..." << endl;
                }
                break;
            case 4: // delete
                if (sheet.deleteEntry()) {
                    cout << "\nEntry deleted" << endl;
                }
                else {
                    cout << "\nEntry not deleted..." << endl;
                }
                break;
            case 5: // edit 
                if (sheet.updateEntry()) {
                    cout << "\nEntry successfully updated!" << endl;
                }
                else {
                    cout << "\nError updating entry..." << endl;
                }
                break;
            case 6: // get
                sheet.getEntry();
                break;
            case 7: // display
                sheet.display();
                break;
            case 8: // stats
                // call stats summary function
                sheet.calculateStats();
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