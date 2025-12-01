#include "Transaction.h"

/** Collection of Transactions with supported operations */
class Spreadsheet { 
private:
    set<unique_ptr<Transaction>, TransactionComparator> entries_; // set is like an ordered list with unique values
    bool addEntry(unique_ptr<Transaction> entry);
    Transaction* getEntry();
    unique_ptr<Transaction> parseRecord(char* record);
    vector<Transaction*> filterMonth(int year, int month);
public:
    bool addEntryFromUser();
    bool deleteEntry();
    bool updateEntry();
    vector<Transaction*> printEntriesFromDate(int year, int month, int day);
    void display();
    void importFile(string filename);
    void exportFile(string filename);
    void calculateStats();
};