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
    Transaction(int year, int month, int day);
    Transaction(const Transaction& t);
    virtual ~Transaction() {}
    bool operator <(const Transaction& other) const;
    void setDate(int year, int month, int day);
    string getDateString();
    bool datesAreEqual(int year, int month, int day);
    bool monthsAreEqual(int year, int month);
    string toString();
    void display();
    double getAmount();
    virtual void setAmount(double amount) = 0;
    virtual void setCategory(string category) = 0;
    virtual string getAmountString() = 0;
    virtual string getCategory() = 0;
};

/** Positive balance transaction */
class Income : public Transaction {
private:
    string category_;
public:
    Income(double amount = 0, string source = "undefined");
    Income(int year, int month, int day, double amount = 0, string source = "undefined");
    Income(const Income& t);
    ~Income() {}
    void setAmount(double amount) override;
    void setCategory(string category) override;
    string getAmountString() override;
    string getCategory() override;
};

/** Negative balance transaction */
class Expense : public Transaction {
private:
    string category_; // could change to enum
public:
    Expense(double amount = 0, string category = "undefined");
    Expense(int year, int month, int day, double amount = 0, string source = "undefined");
    Expense(const Expense& t);
    ~Expense() {}
    void setAmount(double amount) override;
    void setCategory(string category) override;
    string getAmountString() override;
    string getCategory() override;
};

struct TransactionComparator {
    bool operator()(const unique_ptr<Transaction>& t1, const unique_ptr<Transaction>& t2) const;
};
struct TransactionRawPtrComparator {
    bool operator()(const Transaction* t1, const Transaction* t2) const;
};