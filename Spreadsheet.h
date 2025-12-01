#include "Transaction.h"

/** Collection of Transactions with supported operations */
class Spreadsheet { 
private:
    set<unique_ptr<Transaction>, TransactionComparator> entries_; // set is like an ordered list with unique values
public:
    bool addEntry(unique_ptr<Transaction> entry);
    bool addEntryFromUser();
    bool deleteEntry();
    bool updateEntry();
    Transaction* getEntry();
    vector<Transaction*> printEntriesFromDate(int year, int month, int day);
    vector<Transaction*> filterMonth(int year, int month);
    void display();
    unique_ptr<Transaction> parseRecord(char* record);
    void importFile(string filename);
    void exportFile(string filename);
    void calculateStats();
};