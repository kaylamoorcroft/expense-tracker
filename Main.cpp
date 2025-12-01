#include <iostream>
#include "Spreadsheet.h"
#include "Utils.h"

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
    cout << "6) Get transactions from date" << endl;
    cout << "7) Display spreadsheet" << endl;
    cout << "8) Get stats" << endl;
    cout << "\nPlease select an operation (enter 0 to quit) > ";
}

int main() {
    cout << "~~ WELCOME TO EXPENSE TRACKER ~~" << endl;
    Spreadsheet sheet;
    int option;
    string filename;
    int year, month, day;
    do {
        displayMenu();
        Utils::getValidNumInput(option, "");

        switch(option) {
            case 0: // quit
                cout << "\nGoodbye!" << endl;
                break;
            case 1: // load file
                cout << "\tEnter filename > ";
                cin >> filename;
                // code to load filename
                sheet.importFile(filename);
                break;
            case 2: // save file
                cout << "\tEnter filename > ";
                cin >> filename;
                // code to save to file
                sheet.exportFile(filename);
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
            case 6: // get by date
                cout << "Please enter date: \n"; 
                Utils::getValidNumInput(year, "\tYear > ", 1900);
                Utils::getValidNumInput(month, "\tMonth > ", 1, 12);
                Utils::getValidNumInput(day, "\tDay > ", 1, 31);
                sheet.printEntriesFromDate(year, month, day);
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

    return 0;
}