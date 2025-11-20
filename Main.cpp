#include <iostream>
#include "Spreadsheet.h"

using namespace std;

// testing
int main() {
    Income i(2020,5,3,50,"tutoring");
    Expense e(2020,5,4, 40, "food");
    Spreadsheet s;
    //s.addEntry();
    s.addEntry(&i);
    s.addEntry(new Expense(30, "rent"));
    s.addEntry(&e);
    s.addEntry(new Income(20, "tutoring"));
    s.addEntry(new Income(2022,5,3,100000, "big moneys"));
    cout << "initial: " << endl;
    s.display();
    s.updateEntry();
    cout << "updated: " << endl;
    s.display();
    s.deleteEntry();
    cout << "after deletion: " << endl;
    s.display();
    return 0;
}