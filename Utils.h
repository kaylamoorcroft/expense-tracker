#include <iostream>
#include <limits>

using namespace std;

class Utils {
private:
    // Array of month names (adjusting for 0-based indexing if monthNumber is 1-12)
    const static string monthNames[];
public:
    // helper function to validate int/double input 
    // https://www.geeksforgeeks.org/cpp/how-to-handle-wrong-data-type-input-in-cpp/
    template <typename T>
    static void getValidNumInput(T& val, string prompt, T lower = -1, T upper = -1) {
        bool validInput = false;
        do {
            cout << prompt;
            cin >> val;
            if (cin.fail()) {
                cout << "\n--Invalid input! Expected an number... try again:" << endl;
                // Clear the failbit and ignore the remaining input
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                if (lower == upper && upper == -1) { // no upper / lower bounds
                    validInput = true;
                }
                else if (val <= upper && val >= lower) {
                    validInput = true;
                }
                else if (val >= lower && upper == -1) { // only lower bound set 
                    validInput = true;
                }
                else if (val < lower && lower == 0 && upper == -1) { // no negative numbers
                    cout << "\n--Invalid input! Please enter positive value... try again:" << endl;
                }
                else {
                    cout << "\n--Invalid input! Value out of range... try again:" << endl;
                }
            }
        }  while (!validInput);
    }

    /** Get month name (January-December) from number (1-12) */
    static string getMonthName(int monthNum);

    /** format a double (50.000000 -> $50.00) */
    static string formatMoney(double amount);
};