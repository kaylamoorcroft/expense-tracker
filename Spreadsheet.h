#include "Transaction.h"

/** Collection of Transactions with supported operations */
class Spreadsheet { 
private:
    set<Transaction*, greater<Transaction*>> entries_; // set is like an ordered list with unique values
public:
    bool addEntry(Transaction* entry);
    bool addEntryFromUser();
    bool deleteEntry();
    bool updateEntry();
    Transaction* getEntry();
    set<Transaction*, greater<Transaction*>> printEntriesFromDate(int year, int month, int day);
    set<Transaction*, greater<Transaction*>> filterMonth(int year, int month);
    void display();
    Transaction* parseRecord(char* record);
    void importFile(string filename);
    void exportFile(string filename);
    void calculateStats();
};