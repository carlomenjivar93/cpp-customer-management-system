#include <iostream>
#include <limits>
#include "AllCustomers.h"
#include "AllPurchases.h"

using namespace std;

void pause() {
    cout << "Press Enter to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper: prompt for integer with validation
int promptInt(const string& msg) {
    string s;
    while (true) {
        cout << msg;
        getline(cin, s);
        if (s.empty()) return -1;
        bool ok = true;
        for (char ch : s) if (!isdigit(static_cast<unsigned char>(ch))) { ok = false; break; }
        if (!ok) { cout << "Please enter digits only." << endl; continue; }
        return stoi(s);
    }
}

int main() {
    AllCustomers customers;
    AllPurchases purchases;

    const string defaultCustFile = "customers.txt";
    const string defaultPurchFile = "purchases.txt";
    ;
    cout << "   Welcome to Car World Inventory  " << endl;
    cout << "  Manage customers and purchases easily  " << endl;
    cout << "=========================================" << endl << endl;

    // Load data
    if (customers.loadFromFile(defaultCustFile)) {
        cout << "Customer data found." << endl;
    }
    else {
        cout << "No customer file found . Starting with empty database." << endl;
    }
    if (purchases.loadFromFile(defaultPurchFile)) {
        cout << "Purchase data found." << endl;
    }
    else {
        cout << "No purchases file found. Starting with no data." << endl;
    }

    while (true) {
        cout << "========== MAIN MENU ==========" << endl
            << "1) Print all customers" << endl
            << "2) Sort customers A -> Z" << endl
            << "3) Sort customers Z -> A" << endl
            << "4) View customer + purchases" << endl
            << "5) View customer total spend" << endl
            << "6) Add a new customer" << endl
            << "7) Add multiple customers" << endl
            << "8) Update a customer" << endl
            << "9) Delete a customer" << endl
            << "10) Add a purchase" << endl
            << "11) Add multiple purchases" << endl
            << "12) Save data (overwrite)" << endl
            << "13) Export data" << endl
            << "14) Exit" << endl
            << "Choose an option: ";
        string choice;
        getline(cin, choice);
        if (choice.empty()) continue;

        if (choice == "1") {
            customers.printAllCustomers();
            pause();
        }
        else if (choice == "2") {
            customers.sortAscending();
            cout << "Sorted ascending." << endl;
            pause();
        }
        else if (choice == "3") {
            customers.sortDescending();
            cout << "Sorted descending." << endl;
            pause();
        }
        else if (choice == "4") {
            customers.printAllCustomers();
            int idx = promptInt("Select customer by number (or 0 to cancel): ");
            if (idx <= 0) continue;
            if (static_cast<size_t>(idx) > customers.size()) {
                std::cout << "Invalid selection." << endl; continue;
            }
            int acct = customers.at(idx - 1).accountNumber;
            cout << "Customer Info" << endl;
            customers.printCustomerByIndex(idx - 1);
            cout << "Purchases" << endl;
            purchases.printCustomerPurchases(acct);
            pause();
        }
        else if (choice == "5") {
            customers.printAllCustomers();
            int idx = promptInt("Select customer by number to view total spend (or 0 to cancel): ");
            if (idx <= 0) continue;
            if (static_cast<size_t>(idx) > customers.size()) { cout << "Invalid selection." << endl; continue; }
            int acct = customers.at(idx - 1).accountNumber;
            double total = purchases.totalCustomerSpend(acct);
            cout << "Total spend for account " << acct << ": $" << fixed << setprecision(2) << total << endl;
            pause();
        }
        else if (choice == "6") {
            customers.addCustomer();
            pause();
        }
        else if (choice == "7") {
            int n = promptInt("How many customers to add (or 0 to cancel): ");
            if (n > 0) customers.addMultipleCustomersRecursive(n);
            pause();
        }
        else if (choice == "8") {
            customers.printAllCustomers();
            int idx = promptInt("Select customer number to update (or 0 to cancel): ");
            if (idx <= 0) continue;
            if (static_cast<size_t>(idx) > customers.size()) { std::cout << "Invalid selection." << endl; continue; }
            int acct = customers.at(idx - 1).accountNumber;
            if (customers.updateCustomer(acct)) cout << "Updated.\n";
            else cout << "Failed to update." << endl;
            pause();
        }
        else if (choice == "9") {
            customers.printAllCustomers();
            int idx = promptInt("Select customer number to delete (or 0 to cancel): ");
            if (idx <= 0) continue;
            if (static_cast<size_t>(idx) > customers.size()) { cout << "Invalid selection." << endl; continue; }
            int acct = customers.at(idx - 1).accountNumber;
            cout << "Are you sure you want to delete account " << acct << "? (y/n): ";
            string y; getline(cin, y);
            if (!y.empty() && (y[0] == 'y' || y[0] == 'Y')) {
                if (customers.deleteCustomer(acct)) {
                    purchases.deletePurchasesForCustomer(acct); // remove linked purchases
                    cout << "Deleted customer and their purchases." << endl;
                }
                else cout << "Delete failed." << endl;
            }
            else cout << "Delete canceled." << endl;
            pause();
        }
        else if (choice == "10") {
            purchases.addPurchaseInteractive();
            pause();
        }
        else if (choice == "11") {
            int n = promptInt("How many purchases to add (or 0 to cancel): ");
            if (n > 0) purchases.addMultiplePurchasesRecursive(n);
            pause();
        }
        else if (choice == "12") {
            if (customers.saveToFile(defaultCustFile) && purchases.saveToFile(defaultPurchFile)) {
                cout << "Saved to default files." << endl;
            }
            else cout << "Failed to save data." << endl;
            pause();
        }
        else if (choice == "13") {
            string exportFile = "output.txt";
            ofstream out(exportFile);

            if (!out) {
                cout << "ERROR: Could not open " << exportFile << " for writing." << endl;
                pause();
                continue;
            }

            // CUSTOMERS
            for (size_t i = 0; i < customers.size(); i++) {
                const Customer& c = customers.at(i);
                out << "Customer #" << (i + 1) << endl;
                out << "Name: " << c.firstName << " " << c.lastName << endl;
                out << "Account: " << c.accountNumber << endl;
                out << "Address: " << c.street << ", " << c.city << ", "
                    << c.state << " " << c.zip << endl;
                out << "Phone: " << c.phone << endl << endl;
            }

            // PURCHASES
            for (size_t i = 0; i < purchases.size(); i++) {
                const Purchase& p = purchases.get(i);
                out << "Purchase #" << (i + 1) << endl;
                out << "Account: " << p.accountNumber << endl;
                out << "Model: " << p.item << endl;
                out << "Brand: " << p.brand << endl;
                out << "Color: " << p.color << endl;
                out << "Date: " << p.date << endl;
                out << "Price: $" << fixed << setprecision(2) << p.amount << endl << endl;
            }

            // TOTALS
            out << "Total Spent By Customers" << endl << endl;
            for (size_t i = 0; i < customers.size(); i++) {
                const Customer& c = customers.at(i);
                double total = purchases.totalCustomerSpend(c.accountNumber);
                out << c.firstName << " " << c.lastName
                    << " (Acct " << c.accountNumber << "): $"
                    << fixed << setprecision(2) << total << endl;
            }

            out.close();
            cout << "Data successfully exported to " << exportFile << endl;
            pause();
        }
        else if (choice == "14") {
            cout << "Exiting. Would you like to save changes? (y/n): ";
            string s; getline(cin, s);
            if (!s.empty() && (s[0] == 'y' || s[0] == 'Y')) {
                customers.saveToFile(defaultCustFile);
                purchases.saveToFile(defaultPurchFile);
                cout << "Saved." << endl;
            }
            cout << "Goodbye! And thank you for the 100!" << endl;
            break;
        }
            else {
                cout << "Invalid selection." << endl;
            }
        }
        return 0;
    }

