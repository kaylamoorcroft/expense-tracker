#include <ctime> // https://www.w3schools.com/cpp/cpp_date.asp
#include <string>
#include <set> // https://www.w3schools.com/cpp/cpp_sets.asp

using namespace std;

/** Either an Income or Expense, with a date */
class Transaction {
protected:
    time_t timestamp_;
    struct tm datetime_;
    double amount_;
public:
    Transaction();
    // Copy Constructor 
    Transaction(const Transaction &Transaction);
    Transaction(int year, int month, int day);
    void setDate(int year, int month, int day);
    string getDateString();
    bool datesAreEqual(int year, int month, int day);
    bool operator <(const Transaction& other) const;
    virtual void setAmount(double amount) = 0;
    virtual void setCategory(string category) = 0;
    virtual void display() = 0;
};

/** Positive balance transaction */
class Income : public Transaction {
private:
    string category_;
public:
    Income(const Income &i); // copy constructor
    Income(double amount = 0, string source = "undefined");
    Income(int year, int month, int day, double amount = 0, string source = "undefined");
    void setAmount(double amount) override;
    void setCategory(string category) override;
    string getAmountString();
    void display() override;
};

/** Negative balance transaction */
class Expense : public Transaction {
private:
    string category_; // could change to enum
public:
    Expense(const Expense &e); // copy constructor
    Expense(double amount = 0, string category = "undefined");
    Expense(int year, int month, int day, double amount = 0, string source = "undefined");
    void setAmount(double amount) override;
    void setCategory(string category) override;
    string getAmountString();
    void display() override;
};