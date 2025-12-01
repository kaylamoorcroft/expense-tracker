#include "Utils.h"
#include <iomanip>
#include <locale>
#include <sstream>

using namespace std;

// Array of month names (adjusting for 0-based indexing if monthNumber is 1-12)
const string Utils::monthNames[] = {
    "Invalid Month", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

string Utils::getMonthName(int monthNum) {
    if (monthNum >= 1 && monthNum <= 12) {
        return monthNames[monthNum];
    } else {
        return monthNames[0]; // Return "Invalid Month" for out-of-range input
    }
}

string Utils::formatMoney(double amount) {
    stringstream ss;
    // use the system's native locale settings.
    ss.imbue(locale("en_CA.UTF-8"));
    ss << showbase << put_money(amount * 100); // amount in cents
    return ss.str();
}