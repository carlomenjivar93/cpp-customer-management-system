#ifndef ALLCUSTOMERS_H
#define ALLCUSTOMERS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Customer {
    string firstName;
    string lastName;
    int accountNumber{ 0 };
    string street;
    string city;
    string state;
    string zip;
    string phone;

    Customer() = default;
    Customer(const string& f, const string& l, int acct,
        const string& st, const string& c, const string& s,
        const string& z, const string& p)
        : firstName(f), lastName(l), accountNumber(acct),
        street(st), city(c), state(s), zip(z), phone(p) {
    }
};

class AllCustomers {
public:
    // Constructors / destructors
    AllCustomers();
    AllCustomers(const AllCustomers& other);            // copy ctor
    AllCustomers& operator=(const AllCustomers& other); // overloaded assignment
    ~AllCustomers();

    // File I/O
    bool loadFromFile(const string& filename);
    bool saveToFile(const string& filename) const;

    // Printing
    void printAllCustomers() const;
    void printCustomerByIndex(size_t index) const;
    void printCustomerByAccount(int acct, std::ostream& out = std::cout) const;

    // Sorting
    void sortAscending();   // by lastName, firstName
    void sortDescending();

    // Search helpers
    int findIndexByAccount(int acct) const; // returns -1 if not found
    Customer* findCustomerPtrByAccount(int acct); // returns nullptr if not found

    // Add / Update / Delete
    void addCustomer();                 // interactive - add one
    void addMultipleCustomersRecursive(int remaining); // recursive add
    bool updateCustomer(int acct);      // interactive update
    bool deleteCustomer(int acct);      // delete by account

    // Utilities
    int generateUniqueAccountNumber() const;
    size_t size() const { return customers.size(); }
    const Customer& at(size_t idx) const { return customers.at(idx); }

private:
    vector<Customer> customers;

    // Validation helpers
    static bool isDigits(const string& s);
    bool accountExists(int acct) const;
    // Internal helper for interactive input
    Customer promptForCustomer(int suggestedAcct = 0) const;
};

#endif // ALLCUSTOMERS_H