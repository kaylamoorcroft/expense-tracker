#include "Transaction.h"

/** Collection of Transactions with supported operations */
class Spreadsheet { 
private:
    set<unique_ptr<Transaction>, TransactionComparator> entries_; // set is like an ordered list with unique values
public:
    Spreadsheet() {}
    Spreadsheet(Spreadsheet&& other) noexcept;
    Spreadsheet& operator=(Spreadsheet&& other) noexcept;
    bool addEntry(unique_ptr<Transaction> entry);
    bool addEntryFromUser();
    bool deleteEntry();
    bool updateEntry();
    Transaction* getEntry();
    set<Transaction*, TransactionRawPtrComparator> printEntriesFromDate(int year, int month, int day);
    set<Transaction*, TransactionRawPtrComparator> filterMonth(int year, int month);
    void display();
    unique_ptr<Transaction> parseRecord(char* record);
    void importFile(string filename);
    void exportFile(string filename);
    void calculateStats();
};