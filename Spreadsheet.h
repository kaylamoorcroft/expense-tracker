#include "Transaction.h"

/** Collection of Transactions with supported operations */
class Spreadsheet { 
private:
    set<Transaction*, greater<Transaction*>> entries_; // set is like an ordered list with unique values
public:
    bool addEntry(Transaction* entry);
    bool addEntryFromUser();
    Transaction* deleteEntry();
    Transaction* updateEntry();
    Transaction* getEntry();
    set<Transaction*, greater<Transaction*>> printEntriesFromDate(int year, int month, int day);
    set<Transaction*, greater<Transaction*>> filterMonth(int year, int month);
    set<Transaction*, greater<Transaction*>> getAllEntries();
    void display();
};