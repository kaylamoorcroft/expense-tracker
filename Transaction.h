#include <ctime> // https://www.w3schools.com/cpp/cpp_date.asp
#include <string>
#include <vector> // https://www.w3schools.com/cpp/cpp_vectors.asp

using namespace std;

/** Either an Income or Expense, with a date */
class Transaction {
private:
    time_t timestamp_;
    struct tm datetime_;
public:
    Transaction();
    Transaction(int year, int month, int day);
    void setDate(int year, int month, int day);
    string getDateString();
    virtual void display() = 0;
};

/** Positive balance transaction */
class Income : public Transaction {
private:
    double amount_;
    string source_;
public:
    Income(double amount = 0, string source = "undefined");
    Income(int year, int month, int day, double amount = 0, string source = "undefined");
    void setAmount(double amount);
    void setSource(string source);
    string getAmountString();
    void display() override;
};

/** Negative balance transaction */
class Expense : public Transaction {
private:
    double amount_;
    string category_; // could change to enum
public:
    Expense() : Transaction() {}
    Expense(double amount);
    Expense(double amount, string category);
    Expense(int year, int month, int day) : Transaction(year, month, day) {}
    Expense(int year, int month, int day, double amount) : Transaction(year, month, day) {}
    Expense(int year, int month, int day, double amount, string source) : Transaction(year, month, day) {}
};

/** Collection of Transactions with supported operations */
class Spreadsheet { 
private:
    vector<Transaction> entries_; // vector is like a resizable array
public:
    // CRUD operations
    bool addEntry();
    Transaction deleteEntry();
    Transaction updateEntry();
    Transaction getEntry();
    vector<Transaction> getAllEntries();
};

// main will include summary, import, export