#include "AllCustomers.h"
#include <sstream>
#include <limits>

using namespace std;

// --------------------- Constructors / Destructor -----------------------
AllCustomers::AllCustomers() = default;

AllCustomers::AllCustomers(const AllCustomers& other)
{
    // deep copy of vector
    customers = other.customers;
}

AllCustomers& AllCustomers::operator=(const AllCustomers& other)
{
    if (this != &other) {
        customers = other.customers; // vector does deep copy of contained strings
    }
    return *this;
}

AllCustomers::~AllCustomers() = default;

// --------------------- File I/O -----------------------
bool AllCustomers::loadFromFile(const string& filename)
{
    ifstream in(filename);
    if (!in) return false;

    customers.clear();
    string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        // expected CSV: First,Last,Acct,Street,City,State,Zip,Phone
        istringstream ss(line);
        string token;
        vector<string> fields;
        while (std::getline(ss, token, ',')) fields.push_back(token);
        if (fields.size() < 8) continue; // skip malformed
        Customer c;
        c.firstName = fields[0];
        c.lastName = fields[1];
        c.accountNumber = std::stoi(fields[2]);
        c.street = fields[3];
        c.city = fields[4];
        c.state = fields[5];
        c.zip = fields[6];
        c.phone = fields[7];
        customers.push_back(c);
    }
    return true;
}

bool AllCustomers::saveToFile(const string& filename) const
{
    ofstream out(filename);
    if (!out) return false;
    // CSV
    for (const auto& c : customers) {
        out << c.firstName << ','
            << c.lastName << ','
            << c.accountNumber << ','
            << c.street << ','
            << c.city << ','
            << c.state << ','
            << c.zip << ','
            << c.phone << '\n';
    }
    return true;
}

// --------------------- Printing & UI helpers -----------------------
void AllCustomers::printAllCustomers() const
{
    if (customers.empty()) {
        cout << "No customers to display.\n";
        return;
    }
    cout << left << setw(4) << "#"
        << setw(15) << "Last Name"
        << setw(15) << "First Name"
        << setw(10) << "Account"
        << setw(25) << "City"
        << setw(8) << "State"
        << setw(12) << "Phone" << '\n';
    cout << std::string(79, '-') << '\n';
    size_t idx = 1;
    for (const auto& c : customers) {
        cout << setw(4) << idx++
            << setw(15) << c.lastName
            << setw(15) << c.firstName
            << setw(10) << c.accountNumber
            << setw(25) << c.city
            << setw(8) << c.state
            << setw(12) << c.phone << '\n';
    }
}

void AllCustomers::printCustomerByIndex(size_t index) const
{
    if (index >= customers.size()) {
        std::cout << "Invalid index.\n";
        return;
    }
    printCustomerByAccount(customers[index].accountNumber);
}

void AllCustomers::printCustomerByAccount(int acct, std::ostream& out) const
{
    int idx = findIndexByAccount(acct);
    if (idx < 0) {
        out << "Customer not found (acct " << acct << ").\n";
        return;
    }
    const Customer& c = customers[idx];
    out << "Account #: " << c.accountNumber << '\n';
    out << "Name      : " << c.firstName << " " << c.lastName << '\n';
    out << "Address   : " << c.street << ", " << c.city << ", " << c.state << " " << c.zip << '\n';
    out << "Phone     : " << c.phone << '\n';
}

// --------------------- Sorting -----------------------
void AllCustomers::sortAscending()
{
    std::sort(customers.begin(), customers.end(),
        [](const Customer& a, const Customer& b) {
            if (a.lastName != b.lastName) return a.lastName < b.lastName;
            return a.firstName < b.firstName;
        });
}

void AllCustomers::sortDescending()
{
    std::sort(customers.begin(), customers.end(),
        [](const Customer& a, const Customer& b) {
            if (a.lastName != b.lastName) return a.lastName > b.lastName;
            return a.firstName > b.firstName;
        });
}

// --------------------- Search -----------------------
int AllCustomers::findIndexByAccount(int acct) const
{
    for (size_t i = 0; i < customers.size(); ++i)
        if (customers[i].accountNumber == acct) return static_cast<int>(i);
    return -1;
}

Customer* AllCustomers::findCustomerPtrByAccount(int acct)
{
    int idx = findIndexByAccount(acct);
    if (idx == -1) return nullptr;
    return &customers[idx];
}

// --------------------- Add / Update / Delete -----------------------
Customer AllCustomers::promptForCustomer(int suggestedAcct) const
{
    Customer c;
    string temp;
    cout << "Enter first name: ";
    getline(cin, c.firstName);
    cout << "Enter last name: ";
    getline(cin, c.lastName);

    if (suggestedAcct != 0) {
        c.accountNumber = suggestedAcct;
        cout << "Assigned account number: " << c.accountNumber << '\n';
    }
    else {
        while (true) {
            cout << "Enter account number (numeric, unique): ";
            getline(cin, temp);
            if (!isDigits(temp)) { cout << "Account must be digits only.\n"; continue; }
            c.accountNumber = stoi(temp);
            if (accountExists(c.accountNumber)) { cout << "That account already exists.\n"; continue; }
            break;
        }
    }

    cout << "Street address: ";
    getline(cin, c.street);
    cout << "City: ";
    getline(cin, c.city);
    cout << "State (2-letter): ";
    getline(cin, c.state);
    cout << "ZIP: ";
    getline(cin, c.zip);
    cout << "Phone: ";
    getline(cin, c.phone);
    return c;
}

void AllCustomers::addCustomer()
{
    int suggested = generateUniqueAccountNumber();
    Customer c = promptForCustomer(suggested);
    customers.push_back(c);
    cout << "Customer added (Acct " << c.accountNumber << ").\n";
}

void AllCustomers::addMultipleCustomersRecursive(int remaining)
{
    if (remaining <= 0) return;
    cout << "\nAdding customer (" << remaining << " remaining):\n";
    addCustomer();
    addMultipleCustomersRecursive(remaining - 1);
}

bool AllCustomers::updateCustomer(int acct)
{
    int idx = findIndexByAccount(acct);
    if (idx < 0) return false;
    Customer& c = customers[idx];
    cout << "Updating customer (leave blank to keep current)\n";
    string temp;
    cout << "First name [" << c.firstName << "]: ";
    getline(cin, temp);
    if (!temp.empty()) c.firstName = temp;

    cout << "Last name [" << c.lastName << "]: ";
    getline(cin, temp);
    if (!temp.empty()) c.lastName = temp;

    cout << "Street [" << c.street << "]: ";
    getline(cin, temp);
    if (!temp.empty()) c.street = temp;

    cout << "City [" << c.city << "]: ";
    getline(cin, temp);
    if (!temp.empty()) c.city = temp;

    cout << "State [" << c.state << "]: ";
    getline(cin, temp);
    if (!temp.empty()) c.state = temp;

    cout << "ZIP [" << c.zip << "]: ";
    getline(cin, temp);
    if (!temp.empty()) c.zip = temp;

    cout << "Phone [" << c.phone << "]: ";
    getline(cin, temp);
    if (!temp.empty()) c.phone = temp;

    cout << "Customer updated.\n";
    return true;
}

bool AllCustomers::deleteCustomer(int acct)
{
    int idx = findIndexByAccount(acct);
    if (idx < 0) return false;
    customers.erase(customers.begin() + idx);
    return true;
}

// --------------------- Utilities -----------------------
int AllCustomers::generateUniqueAccountNumber() const
{
    // generate a simple unique account number: max existing + 1, or 1000 if none
    int maxAcct = 999;
    for (const auto& c : customers) if (c.accountNumber > maxAcct) maxAcct = c.accountNumber;
    return maxAcct + 1;
}

bool AllCustomers::isDigits(const string& s)
{
    if (s.empty()) return false;
    for (char ch : s) if (!std::isdigit(static_cast<unsigned char>(ch))) return false;
    return true;
}

bool AllCustomers::accountExists(int acct) const
{
    return findIndexByAccount(acct) != -1;
}