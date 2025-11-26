#include "Transaction.h"

/** Collection of Transactions with supported operations */
class Spreadsheet { 
private:
    set<Transaction*, greater<Transaction*>> entries_; // set is like an ordered list with unique values
public:
void test();
    bool addEntry(Transaction* entry);
    Transaction* deleteEntry();
    Transaction* updateEntry();
    Transaction* getEntry();
    set<Transaction*, greater<Transaction*>> printEntriesFromDate(int year, int month, int day);
    set<Transaction*, greater<Transaction*>> filterMonth(int year, int month, int day);
    set<Transaction*, greater<Transaction*>> getAllEntries();
    void display();
};