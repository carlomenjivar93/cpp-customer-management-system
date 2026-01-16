#ifndef ALLPURCHASES_H
#define ALLPURCHASES_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct Purchase {
    int accountNumber{ 0 };
    string item; // car model
    string brand; // car brand
    string color;
    string date; // store as YYYY-MM-DD string for simplicity
    double amount{ 0.0 };

    Purchase() = default;
    Purchase(int acct, const string& it, const string& br, const string& col,
        const string& d, double a)
        : accountNumber(acct), item(it), brand(br), color(col),
        date(d), amount(a) {
    }
};

class AllPurchases {
public:
    AllPurchases();
    AllPurchases(const AllPurchases& other);
    AllPurchases& operator=(const AllPurchases& other);
    ~AllPurchases();

    // File I/O
    bool loadFromFile(const string& filename);
    bool saveToFile(const string& filename) const;

    // Print / Query
    void printCustomerPurchases(int acct) const;
    double totalCustomerSpend(int acct) const;

    // Add / Delete
    void addPurchaseInteractive();
    void addMultiplePurchasesRecursive(int remaining);
    void deletePurchasesForCustomer(int acct);

    // Utilities
    size_t size() const { return purchases.size(); }
    const Purchase& get(size_t index) const { return purchases[index]; }

private:
    vector<Purchase> purchases;
    static bool validAmountString(const string& s);
};

#endif // ALLPURCHASES_H
