#include "AllPurchases.h"
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

//Constructors / Destructor
AllPurchases::AllPurchases() = default;

AllPurchases::AllPurchases(const AllPurchases& other)
{
    purchases = other.purchases;
}

AllPurchases& AllPurchases::operator=(const AllPurchases& other)
{
    if (this != &other) purchases = other.purchases;
    return *this;
}

AllPurchases::~AllPurchases() = default;

// File I/O
bool AllPurchases::loadFromFile(const string& filename)
{
    ifstream in(filename);
    if (!in) return false;
    purchases.clear();
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        // expecting acct,item,brand,color,date,amount
        istringstream ss(line);
        string token;
        vector<string> fields;
        while (getline(ss, token, ',')) fields.push_back(token);
        if (fields.size() < 6) continue;
        Purchase p;
        p.accountNumber = stoi(fields[0]);
        p.item = fields[1];
        p.brand = fields[2];
        p.color = fields[3];
        p.date = fields[4];
        p.amount = stod(fields[5]);
        purchases.push_back(p);
    }
    return true;
}

bool AllPurchases::saveToFile(const string& filename) const
{
    ofstream out(filename);
    if (!out) return false;
    for (const auto& p : purchases) {
        out << p.accountNumber << ','
            << p.item << ","
            << p.brand << ","
            << p.color << ","
            << p.date << ","
            << p.amount << endl;
    }
    return true;
}

// Print 
void AllPurchases::printCustomerPurchases(int acct) const
{
    double total = 0.0;
    bool any = false;
    cout << left << setw(5) << "#"
        << setw(18) << "Model"
        << setw(15) << "Brand"
        << setw(12) << "Color"
        << setw(12) << "Date"
        << right << setw(10) << "Amount" << endl;
    cout << string(72, '-') << endl;
    size_t idx = 1;
    for (const auto& p : purchases) {
        if (p.accountNumber == acct) {
            any = true;
            cout << left << setw(5) << idx++
                << setw(18) << p.item
                << setw(15) << p.brand
                << setw(12) << p.color
                << setw(12) << p.date
                << right << setw(10) << fixed << setprecision(2) << p.amount << endl;
            total += p.amount;
        }
    }
    if (!any) {
        cout << "No purchases found for account " << acct << endl;
    }
    else {
        cout << string(72, '-') << endl;
        cout << setw(62) << "Total:" << right << setw(12) << fixed << setprecision(2) << total << endl;
    }
}

double AllPurchases::totalCustomerSpend(int acct) const
{
    double total = 0.0;
    for (const auto& p : purchases) if (p.accountNumber == acct) total += p.amount;
    return total;
}

// Add / Delete 
void AllPurchases::addPurchaseInteractive()
{
    string tmp;
    Purchase p;
    // account
    while (true) {
        cout << "Enter account number for purchase: ";
        getline(cin, tmp);
        if (tmp.empty()) { cout << "Account number required." << endl; continue; }
        bool allDigits = true;
        for (char ch : tmp) if (!isdigit(static_cast<unsigned char>(ch))) { allDigits = false; break; }
        if (!allDigits) { cout << "Account must be numeric."<< endl; continue; }
        p.accountNumber = stoi(tmp);
        break;
    }
    cout << "Car model (item): ";
    getline(cin, p.item);

    cout << "Brand: ";
    getline(cin, p.brand);

    cout << "Color: ";
    getline(cin, p.color);

    cout << "Date (YYYY-MM-DD): ";
    getline(cin, p.date);

    while (true) {
        cout << "Price amount: ";
        getline(cin, tmp);
        if (!validAmountString(tmp)) { cout << "Invalid amount format.\n"; continue; }
        p.amount = stod(tmp);
        break;
    }
    purchases.push_back(p);
    cout << "Purchase added." << endl;
}

void AllPurchases::addMultiplePurchasesRecursive(int remaining)
{
    if (remaining <= 0) return;
    cout << "Adding purchase (" << remaining << " remaining):" << endl;
    addPurchaseInteractive();
    addMultiplePurchasesRecursive(remaining - 1);
}

void AllPurchases::deletePurchasesForCustomer(int acct)
{
    purchases.erase(std::remove_if(purchases.begin(), purchases.end(),
        [acct](const Purchase& p) { return p.accountNumber == acct; }), purchases.end());
}

//  Utilities
bool AllPurchases::validAmountString(const string& s)
{
    if (s.empty()) return false;
    bool dotSeen = false;
    for (char ch : s) {
        if (ch == '.') {
            if (dotSeen) return false;
            dotSeen = true;
            continue;
        }
        if (!isdigit(static_cast<unsigned char>(ch))) return false;
    }
    return true;
}