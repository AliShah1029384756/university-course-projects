// Group Roll Numbers: <ROLL_NO_1>, <ROLL_NO_2>
#include "IMS.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

int Shopkeeper::nextSerial = 1;
int Customer::nextSerial = 1;
int Product::globalSerial = 1;

const string IMS::shopkeeperFile = "shopkeeper.txt";
const string IMS::customerFile = "customer.txt";
const string IMS::productFile = "products.txt";
const string IMS::cartFile = "addToCart.txt";

Person::Person() {
    firstName = "";
    lastName = "";
    id = "";
    username = "";
    email = "";
    password = "";
    registrationDate = "";
    gender = "";
    bloodGroup = "";
    contactNo = "";
    address = "";
}

const string& Person::getId() const { return id; }
const string& Person::getUsername() const { return username; }
const string& Person::getPassword() const { return password; }

void Person::setBasicEditableFields(
    const string& first,
    const string& last,
    const string& addr,
    const string& contact,
    const string& user,
    const string& mail,
    const string& pass
) {
    firstName = first;
    lastName = last;
    address = addr;
    contactNo = contact;
    username = user;
    email = mail;
    password = pass;
}

void Shopkeeper::seedCounterFromExisting(int maxSerial) {
    nextSerial = max(1, maxSerial + 1);
}

string Shopkeeper::generateId() {
    ostringstream out;
    out << setw(4) << setfill('0') << nextSerial++;
    return out.str();
}

Shopkeeper::Shopkeeper() : Person() {}

string Shopkeeper::toStorageRow() const {
    ostringstream out;
    out << firstName << '|'
        << lastName << '|'
        << id << '|'
        << username << '|'
        << email << '|'
        << password << '|'
        << registrationDate << '|'
        << gender << '|'
        << bloodGroup << '|'
        << contactNo << '|'
        << address;
    return out.str();
}

string Shopkeeper::roleName() const {
    return "Shopkeeper";
}

Shopkeeper Shopkeeper::fromStorageRow(const string& line) {
    auto cols = IMS::split(line, '|');
    Shopkeeper s;
    if (cols.size() < 11) {
        return s;
    }
    s.firstName = cols[0];
    s.lastName = cols[1];
    s.id = cols[2];
    s.username = cols[3];
    s.email = cols[4];
    s.password = cols[5];
    s.registrationDate = cols[6];
    s.gender = cols[7];
    s.bloodGroup = cols[8];
    s.contactNo = cols[9];
    s.address = cols[10];
    return s;
}

Shopkeeper Shopkeeper::createFromInput() {
    Shopkeeper s;
    s.firstName = IMS::promptLine("First Name: ");
    s.lastName = IMS::promptLine("Last Name: ");
    s.id = generateId();
    s.username = IMS::promptLine("Username: ");
    s.email = IMS::promptLine("Email: ");
    s.password = IMS::promptLine("Password: ");
    s.registrationDate = IMS::todayDate();
    s.gender = IMS::promptLine("Gender: ");
    s.bloodGroup = IMS::promptLine("Blood Group: ");
    s.contactNo = IMS::promptLine("Contact No: ");
    s.address = IMS::promptLine("Address: ");
    return s;
}

void Customer::seedCounterFromExisting(int maxSerial) {
    nextSerial = max(1, maxSerial + 1);
}

string Customer::generateId() {
    ostringstream out;
    out << setw(4) << setfill('0') << nextSerial++;
    return out.str();
}

Customer::Customer() : Person() {}

string Customer::toStorageRow() const {
    ostringstream out;
    out << firstName << '|'
        << lastName << '|'
        << id << '|'
        << username << '|'
        << email << '|'
        << password << '|'
        << registrationDate << '|'
        << gender << '|'
        << bloodGroup << '|'
        << contactNo << '|'
        << address;
    return out.str();
}

string Customer::roleName() const {
    return "Customer";
}

Customer Customer::fromStorageRow(const string& line) {
    auto cols = IMS::split(line, '|');
    Customer c;
    if (cols.size() < 11) {
        return c;
    }
    c.firstName = cols[0];
    c.lastName = cols[1];
    c.id = cols[2];
    c.username = cols[3];
    c.email = cols[4];
    c.password = cols[5];
    c.registrationDate = cols[6];
    c.gender = cols[7];
    c.bloodGroup = cols[8];
    c.contactNo = cols[9];
    c.address = cols[10];
    return c;
}

Customer Customer::createFromInput() {
    Customer c;
    c.firstName = IMS::promptLine("First Name: ");
    c.lastName = IMS::promptLine("Last Name: ");
    c.id = generateId();
    c.username = IMS::promptLine("Username: ");
    c.email = IMS::promptLine("Email: ");
    c.password = IMS::promptLine("Password: ");
    c.registrationDate = IMS::todayDate();
    c.gender = IMS::promptLine("Gender: ");
    c.bloodGroup = IMS::promptLine("Blood Group: ");
    c.contactNo = IMS::promptLine("Contact No: ");
    c.address = IMS::promptLine("Address: ");
    return c;
}

Product::Product() {
    productId = "";
    ownerShopkeeperId = "";
    categoryName = "";
    productName = "";
    description = "";
    size = "";
    color = "";
    quantity = 0;
    price = 0.0;
}

void Product::seedCounterFromExisting(int maxSerial) {
    globalSerial = max(1, maxSerial + 1);
}

string Product::generateProductId(int categoryCode) {
    ostringstream out;
    out << setw(2) << setfill('0') << categoryCode
        << setw(4) << setfill('0') << globalSerial++;
    return out.str();
}

bool Product::isOutOfStock() const {
    return quantity <= 0;
}

string Product::toStorageRow() const {
    ostringstream out;
    out << productId << '|'
        << ownerShopkeeperId << '|'
        << categoryName << '|'
        << productName << '|'
        << description << '|'
        << size << '|'
        << color << '|'
        << quantity << '|'
        << fixed << setprecision(2) << price;
    return out.str();
}

Product Product::fromStorageRow(const string& line) {
    auto cols = IMS::split(line, '|');
    Product p;
    if (cols.size() < 9) {
        return p;
    }

    p.productId = cols[0];
    p.ownerShopkeeperId = cols[1];
    p.categoryName = cols[2];
    p.productName = cols[3];
    p.description = cols[4];
    p.size = cols[5];
    p.color = cols[6];
    p.quantity = stoi(cols[7]);
    p.price = stod(cols[8]);
    return p;
}

Product& Product::operator+=(int deltaQuantity) {
    quantity += deltaQuantity;
    if (quantity < 0) {
        quantity = 0;
    }
    return *this;
}

ostream& operator<<(ostream& os, const Product& p) {
    os << "ID: " << p.productId
       << " | Name: " << p.productName
       << " | Category: " << p.categoryName
       << " | Qty: " << p.quantity
       << " | Price: " << fixed << setprecision(2) << p.price;
    if (!p.size.empty()) {
        os << " | Size: " << p.size;
    }
    if (!p.color.empty()) {
        os << " | Color: " << p.color;
    }
    return os;
}

CartItem::CartItem() {
    customerId = "";
    productId = "";
    quantity = 0;
}

CartItem::CartItem(const string& cId, const string& pId, int qty) {
    customerId = cId;
    productId = pId;
    quantity = qty;
}

string CartItem::toStorageRow() const {
    ostringstream out;
    out << customerId << '|' << productId << '|' << quantity;
    return out.str();
}

CartItem CartItem::fromStorageRow(const string& line) {
    auto cols = IMS::split(line, '|');
    CartItem item;
    if (cols.size() < 3) {
        return item;
    }
    item.customerId = cols[0];
    item.productId = cols[1];
    item.quantity = stoi(cols[2]);
    return item;
}

IMS::IMS() {
    loadAll();
}

void IMS::run() {
    while (true) {
        cout << "\n===== INVENTORY MANAGEMENT SYSTEM =====\n";
        cout << "1. Administrator\n";
        cout << "2. Shopkeeper\n";
        cout << "3. Customer\n";
        cout << "0. Exit\n";

        int choice = promptInt("Select option: ", 0, 3);

        if (choice == 0) {
            cout << "Exiting IMS...\n";
            return;
        }

        switch (choice) {
        case 1:
            adminMenu();
            break;
        case 2: {
            cout << "\n1. Register\n2. Sign In\n0. Back\n";
            int skChoice = promptInt("Select option: ", 0, 2);
            if (skChoice == 1) {
                shopkeeperRegister();
            } else if (skChoice == 2) {
                shopkeeperSignInMenu();
            }
            break;
        }
        case 3: {
            cout << "\n1. Register\n2. Sign In\n0. Back\n";
            int cChoice = promptInt("Select option: ", 0, 2);
            if (cChoice == 1) {
                customerRegister();
            } else if (cChoice == 2) {
                customerSignInMenu();
            }
            break;
        }
        default:
            break;
        }
    }
}

void IMS::loadAll() {
    shopkeepers.clear();
    customers.clear();
    products.clear();
    cartItems.clear();

    {
        ifstream in(shopkeeperFile);
        string line;
        int maxId = 0;
        while (getline(in, line)) {
            line = trim(line);
            if (line.empty()) {
                continue;
            }
            Shopkeeper s = Shopkeeper::fromStorageRow(line);
            if (!s.getId().empty()) {
                shopkeepers.push_back(s);
                maxId = max(maxId, parseNumericSuffix(s.getId()));
            }
        }
        Shopkeeper::seedCounterFromExisting(maxId);
    }

    {
        ifstream in(customerFile);
        string line;
        int maxId = 0;
        while (getline(in, line)) {
            line = trim(line);
            if (line.empty()) {
                continue;
            }
            Customer c = Customer::fromStorageRow(line);
            if (!c.getId().empty()) {
                customers.push_back(c);
                maxId = max(maxId, parseNumericSuffix(c.getId()));
            }
        }
        Customer::seedCounterFromExisting(maxId);
    }

    {
        ifstream in(productFile);
        string line;
        int maxId = 0;
        while (getline(in, line)) {
            line = trim(line);
            if (line.empty()) {
                continue;
            }
            Product p = Product::fromStorageRow(line);
            if (!p.productId.empty()) {
                products.push_back(p);
                maxId = max(maxId, parseNumericSuffix(p.productId));
            }
        }
        Product::seedCounterFromExisting(maxId);
    }

    {
        ifstream in(cartFile);
        string line;
        while (getline(in, line)) {
            line = trim(line);
            if (line.empty()) {
                continue;
            }
            CartItem item = CartItem::fromStorageRow(line);
            if (!item.customerId.empty() && !item.productId.empty() && item.quantity > 0) {
                cartItems.push_back(item);
            }
        }
    }
}

void IMS::saveShopkeepers() const {
    ofstream out(shopkeeperFile, ios::trunc);
    for (const auto& s : shopkeepers) {
        out << s.toStorageRow() << '\n';
    }
}

void IMS::saveCustomers() const {
    ofstream out(customerFile, ios::trunc);
    for (const auto& c : customers) {
        out << c.toStorageRow() << '\n';
    }
}

void IMS::saveProducts() const {
    ofstream out(productFile, ios::trunc);
    for (const auto& p : products) {
        out << p.toStorageRow() << '\n';
    }
}

void IMS::saveCart() const {
    ofstream out(cartFile, ios::trunc);
    for (const auto& ci : cartItems) {
        out << ci.toStorageRow() << '\n';
    }
}

int IMS::findShopkeeperByUsername(const string& user) const {
    for (int i = 0; i < static_cast<int>(shopkeepers.size()); ++i) {
        if (shopkeepers[i].getUsername() == user) {
            return i;
        }
    }
    return -1;
}

int IMS::findCustomerByUsername(const string& user) const {
    for (int i = 0; i < static_cast<int>(customers.size()); ++i) {
        if (customers[i].getUsername() == user) {
            return i;
        }
    }
    return -1;
}

int IMS::findProductById(const string& productId) const {
    for (int i = 0; i < static_cast<int>(products.size()); ++i) {
        if (products[i].productId == productId) {
            return i;
        }
    }
    return -1;
}

string IMS::trim(const string& s) {
    size_t left = 0;
    while (left < s.size() && isspace(static_cast<unsigned char>(s[left]))) {
        ++left;
    }
    size_t right = s.size();
    while (right > left && isspace(static_cast<unsigned char>(s[right - 1]))) {
        --right;
    }
    return s.substr(left, right - left);
}

vector<string> IMS::split(const string& s, char delim) {
    vector<string> out;
    string token;
    istringstream in(s);
    while (getline(in, token, delim)) {
        out.push_back(token);
    }
    return out;
}

string IMS::promptLine(const string& label) {
    while (true) {
        cout << label;
        string value;
        getline(cin, value);
        value = trim(value);
        if (!value.empty()) {
            return value;
        }
        cout << "Value cannot be empty. Try again.\n";
    }
}

int IMS::promptInt(const string& label, int minValue, int maxValue) {
    while (true) {
        cout << label;
        int value;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value >= minValue && value <= maxValue) {
                return value;
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Invalid number. Enter value between " << minValue << " and " << maxValue << ".\n";
    }
}

double IMS::promptDouble(const string& label, double minValue) {
    while (true) {
        cout << label;
        double value;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value >= minValue) {
                return value;
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Invalid amount. Enter value >= " << minValue << ".\n";
    }
}

bool IMS::promptBool(const string& label) {
    while (true) {
        string s = promptLine(label + " (true/false): ");
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s == "true") {
            return true;
        }
        if (s == "false") {
            return false;
        }
        cout << "Please type true or false.\n";
    }
}

string IMS::todayDate() {
    auto now = chrono::system_clock::now();
    time_t tt = chrono::system_clock::to_time_t(now);
    tm localTm{};
#if defined(_WIN32)
    localtime_s(&localTm, &tt);
#else
    localTm = *localtime(&tt);
#endif
    ostringstream out;
    out << setw(2) << setfill('0') << localTm.tm_mday << '-'
        << setw(2) << setfill('0') << (localTm.tm_mon + 1) << '-'
        << (localTm.tm_year + 1900);
    return out.str();
}

int IMS::parseNumericSuffix(const string& idStr) {
    string digits;
    for (char c : idStr) {
        if (isdigit(static_cast<unsigned char>(c))) {
            digits.push_back(c);
        }
    }
    if (digits.empty()) {
        return 0;
    }
    return stoi(digits);
}

int IMS::selectCategoryCode() const {
    cout << "1. Sports\n";
    cout << "2. Garments\n";
    cout << "3. Eatables\n";
    cout << "4. Medicines\n";
    cout << "5. Fashion\n";
    return promptInt("Select category: ", 1, 5);
}

string IMS::categoryByCode(int code) const {
    switch (code) {
    case 1: return "Sports";
    case 2: return "Garments";
    case 3: return "Eatables";
    case 4: return "Medicines";
    case 5: return "Fashion";
    default: return "Unknown";
    }
}

void IMS::listShopkeepers() const {
    cout << "\n--- Shopkeepers ---\n";
    if (shopkeepers.empty()) {
        cout << "No shopkeepers found.\n";
        return;
    }
    for (const auto& s : shopkeepers) {
        cout << "ID: " << s.getId() << " | Username: " << s.getUsername() << '\n';
    }
}

void IMS::listCustomers() const {
    cout << "\n--- Customers ---\n";
    if (customers.empty()) {
        cout << "No customers found.\n";
        return;
    }
    for (const auto& c : customers) {
        cout << "ID: " << c.getId() << " | Username: " << c.getUsername() << '\n';
    }
}

void IMS::listProducts(bool onlyInStock) const {
    cout << "\n--- Products ---\n";
    bool any = false;
    for (const auto& p : products) {
        if (onlyInStock && p.isOutOfStock()) {
            continue;
        }
        cout << p;
        if (p.isOutOfStock()) {
            cout << " | OUT OF STOCK";
        }
        cout << '\n';
        any = true;
    }
    if (!any) {
        cout << "No products found.\n";
    }
}

void IMS::listProductsByOwner(const string& ownerId) const {
    cout << "\n--- Your Products ---\n";
    bool any = false;
    for (const auto& p : products) {
        if (p.ownerShopkeeperId == ownerId) {
            cout << p;
            if (p.isOutOfStock()) {
                cout << " | OUT OF STOCK";
            }
            cout << '\n';
            any = true;
        }
    }
    if (!any) {
        cout << "No products found for this shopkeeper.\n";
    }
}

void IMS::addShopkeeper() {
    Shopkeeper s = Shopkeeper::createFromInput();
    if (findShopkeeperByUsername(s.getUsername()) != -1 || findCustomerByUsername(s.getUsername()) != -1) {
        cout << "Username already exists. Operation cancelled.\n";
        return;
    }
    shopkeepers.push_back(s);
    saveShopkeepers();
    cout << "Shopkeeper added with ID: " << s.getId() << '\n';
}

void IMS::addCustomer() {
    Customer c = Customer::createFromInput();
    if (findCustomerByUsername(c.getUsername()) != -1 || findShopkeeperByUsername(c.getUsername()) != -1) {
        cout << "Username already exists. Operation cancelled.\n";
        return;
    }
    customers.push_back(c);
    saveCustomers();
    cout << "Customer added with ID: " << c.getId() << '\n';
}

void IMS::addProductByAdmin() {
    if (shopkeepers.empty()) {
        cout << "No shopkeeper exists. Add shopkeeper first.\n";
        return;
    }

    listShopkeepers();
    string ownerId = promptLine("Owner Shopkeeper ID: ");

    bool found = false;
    for (const auto& s : shopkeepers) {
        if (s.getId() == ownerId) {
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Invalid owner ID.\n";
        return;
    }

    int code = selectCategoryCode();
    Product p;
    p.ownerShopkeeperId = ownerId;
    p.categoryName = categoryByCode(code);
    p.productName = promptLine("Product Name: ");
    p.description = promptLine("Description: ");
    p.productId = Product::generateProductId(code);
    p.size = promptLine("Size (optional, type NA if not applicable): ");
    if (p.size == "NA" || p.size == "na") {
        p.size = "";
    }
    p.color = promptLine("Color (optional, type NA if not applicable): ");
    if (p.color == "NA" || p.color == "na") {
        p.color = "";
    }
    p.quantity = promptInt("Quantity: ", 0, 1000000);
    p.price = promptDouble("Price: ", 0.0);

    products.push_back(p);
    saveProducts();

    cout << "Product added with ID: " << p.productId;
    if (p.quantity == 0) {
        cout << " (OUT OF STOCK)";
    }
    cout << '\n';
}

void IMS::editShopkeeper() {
    listShopkeepers();
    if (shopkeepers.empty()) {
        return;
    }

    string id = promptLine("Enter shopkeeper ID to edit: ");
    for (auto& s : shopkeepers) {
        if (s.getId() == id) {
            string first = promptLine("New First Name: ");
            string last = promptLine("New Last Name: ");
            string addr = promptLine("New Address: ");
            string contact = promptLine("New Contact No: ");
            string user = promptLine("New Username: ");
            string mail = promptLine("New Email: ");
            string pass = promptLine("New Password: ");
            s.setBasicEditableFields(first, last, addr, contact, user, mail, pass);
            saveShopkeepers();
            cout << "Shopkeeper updated.\n";
            return;
        }
    }
    cout << "Shopkeeper not found.\n";
}

void IMS::editCustomer() {
    listCustomers();
    if (customers.empty()) {
        return;
    }

    string id = promptLine("Enter customer ID to edit: ");
    for (auto& c : customers) {
        if (c.getId() == id) {
            string first = promptLine("New First Name: ");
            string last = promptLine("New Last Name: ");
            string addr = promptLine("New Address: ");
            string contact = promptLine("New Contact No: ");
            string user = promptLine("New Username: ");
            string mail = promptLine("New Email: ");
            string pass = promptLine("New Password: ");
            c.setBasicEditableFields(first, last, addr, contact, user, mail, pass);
            saveCustomers();
            cout << "Customer updated.\n";
            return;
        }
    }
    cout << "Customer not found.\n";
}

void IMS::editProduct() {
    listProducts();
    if (products.empty()) {
        return;
    }

    string id = promptLine("Enter product ID to edit: ");
    int idx = findProductById(id);
    if (idx == -1) {
        cout << "Product not found.\n";
        return;
    }

    products[idx].productName = promptLine("New Product Name: ");
    products[idx].description = promptLine("New Description: ");
    products[idx].quantity = promptInt("New Quantity: ", 0, 1000000);
    string s = promptLine("New Size (type NA if not applicable): ");
    string c = promptLine("New Color (type NA if not applicable): ");
    products[idx].size = (s == "NA" || s == "na") ? "" : s;
    products[idx].color = (c == "NA" || c == "na") ? "" : c;
    products[idx].price = promptDouble("New Price: ", 0.0);

    saveProducts();
    cout << "Product updated.\n";
}

void IMS::deleteShopkeeper() {
    listShopkeepers();
    if (shopkeepers.empty()) {
        return;
    }

    string id = promptLine("Enter shopkeeper ID to delete: ");
    auto before = shopkeepers.size();
    shopkeepers.erase(remove_if(shopkeepers.begin(), shopkeepers.end(), [&](const Shopkeeper& s) {
        return s.getId() == id;
    }), shopkeepers.end());

    if (shopkeepers.size() == before) {
        cout << "Shopkeeper not found.\n";
        return;
    }

    products.erase(remove_if(products.begin(), products.end(), [&](const Product& p) {
        return p.ownerShopkeeperId == id;
    }), products.end());

    saveShopkeepers();
    saveProducts();
    cout << "Shopkeeper and related products deleted.\n";
}

void IMS::deleteCustomer() {
    listCustomers();
    if (customers.empty()) {
        return;
    }

    string id = promptLine("Enter customer ID to delete: ");
    auto before = customers.size();
    customers.erase(remove_if(customers.begin(), customers.end(), [&](const Customer& c) {
        return c.getId() == id;
    }), customers.end());

    if (customers.size() == before) {
        cout << "Customer not found.\n";
        return;
    }

    cartItems.erase(remove_if(cartItems.begin(), cartItems.end(), [&](const CartItem& i) {
        return i.customerId == id;
    }), cartItems.end());

    saveCustomers();
    saveCart();
    cout << "Customer and cart deleted.\n";
}

void IMS::deleteProduct() {
    listProducts();
    if (products.empty()) {
        return;
    }

    string id = promptLine("Enter product ID to delete: ");
    auto before = products.size();
    products.erase(remove_if(products.begin(), products.end(), [&](const Product& p) {
        return p.productId == id;
    }), products.end());

    if (products.size() == before) {
        cout << "Product not found.\n";
        return;
    }

    cartItems.erase(remove_if(cartItems.begin(), cartItems.end(), [&](const CartItem& i) {
        return i.productId == id;
    }), cartItems.end());

    saveProducts();
    saveCart();
    cout << "Product deleted.\n";
}

void IMS::adminMenu() {
    cout << "\n--- Admin Sign In ---\n";
    string u = promptLine("Username: ");
    string p = promptLine("Password: ");

    if (!(u == "admin" && p == "admin")) {
        cout << "Invalid admin credentials.\n";
        return;
    }

    while (true) {
        cout << "\n=== ADMIN MENU ===\n";
        cout << "1. Add Shopkeeper\n";
        cout << "2. Add Customer\n";
        cout << "3. Add Product\n";
        cout << "4. Edit Shopkeeper\n";
        cout << "5. Edit Customer\n";
        cout << "6. Edit Product\n";
        cout << "7. Delete Shopkeeper\n";
        cout << "8. Delete Customer\n";
        cout << "9. Delete Product\n";
        cout << "10. View Shopkeepers\n";
        cout << "11. View Customers\n";
        cout << "12. View Products\n";
        cout << "0. Logout\n";

        int choice = promptInt("Select option: ", 0, 12);
        if (choice == 0) {
            return;
        }

        switch (choice) {
        case 1: addShopkeeper(); break;
        case 2: addCustomer(); break;
        case 3: addProductByAdmin(); break;
        case 4: editShopkeeper(); break;
        case 5: editCustomer(); break;
        case 6: editProduct(); break;
        case 7: deleteShopkeeper(); break;
        case 8: deleteCustomer(); break;
        case 9: deleteProduct(); break;
        case 10: listShopkeepers(); break;
        case 11: listCustomers(); break;
        case 12: listProducts(); break;
        default: break;
        }
    }
}

void IMS::shopkeeperRegister() {
    Shopkeeper s = Shopkeeper::createFromInput();
    if (findShopkeeperByUsername(s.getUsername()) != -1 || findCustomerByUsername(s.getUsername()) != -1) {
        cout << "Username already exists. Registration failed.\n";
        return;
    }
    shopkeepers.push_back(s);
    saveShopkeepers();
    cout << "Shopkeeper registered with ID: " << s.getId() << '\n';
}

void IMS::shopkeeperSignInMenu() {
    cout << "\n--- Shopkeeper Sign In ---\n";
    string user = promptLine("Username: ");
    string pass = promptLine("Password: ");

    int idx = findShopkeeperByUsername(user);
    if (idx == -1 || shopkeepers[idx].getPassword() != pass) {
        cout << "Invalid credentials.\n";
        return;
    }

    shopkeeperSession(shopkeepers[idx]);
}

void IMS::shopkeeperSession(const Shopkeeper& sk) {
    while (true) {
        cout << "\n=== SHOPKEEPER MENU (" << sk.getUsername() << ") ===\n";
        cout << "1. Add New Product\n";
        cout << "2. Edit Product\n";
        cout << "3. View My Products\n";
        cout << "0. Logout\n";

        int choice = promptInt("Select option: ", 0, 3);
        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            int code = selectCategoryCode();
            Product p;
            p.ownerShopkeeperId = sk.getId();
            p.categoryName = categoryByCode(code);
            p.productName = promptLine("Product Name: ");
            p.description = promptLine("Description: ");
            p.productId = Product::generateProductId(code);
            p.size = promptLine("Size (optional, type NA if not applicable): ");
            if (p.size == "NA" || p.size == "na") {
                p.size = "";
            }
            p.color = promptLine("Color (optional, type NA if not applicable): ");
            if (p.color == "NA" || p.color == "na") {
                p.color = "";
            }
            p.quantity = promptInt("Quantity: ", 0, 1000000);
            p.price = promptDouble("Price: ", 0.0);

            products.push_back(p);
            saveProducts();

            cout << "Product added with ID: " << p.productId << '\n';
        } else if (choice == 2) {
            listProductsByOwner(sk.getId());
            string pid = promptLine("Enter Product ID to edit: ");
            int idx = findProductById(pid);
            if (idx == -1 || products[idx].ownerShopkeeperId != sk.getId()) {
                cout << "Product not found under this shopkeeper.\n";
                continue;
            }
            products[idx].productName = promptLine("New Product Name: ");
            products[idx].description = promptLine("New Description: ");
            products[idx].quantity = promptInt("New Quantity: ", 0, 1000000);
            string s = promptLine("New Size (type NA if not applicable): ");
            string c = promptLine("New Color (type NA if not applicable): ");
            products[idx].size = (s == "NA" || s == "na") ? "" : s;
            products[idx].color = (c == "NA" || c == "na") ? "" : c;
            products[idx].price = promptDouble("New Price: ", 0.0);
            saveProducts();
            cout << "Product updated.\n";
        } else if (choice == 3) {
            listProductsByOwner(sk.getId());
        }
    }
}

void IMS::customerRegister() {
    Customer c = Customer::createFromInput();
    if (findCustomerByUsername(c.getUsername()) != -1 || findShopkeeperByUsername(c.getUsername()) != -1) {
        cout << "Username already exists. Registration failed.\n";
        return;
    }
    customers.push_back(c);
    saveCustomers();
    cout << "Customer registered with ID: " << c.getId() << '\n';
}

void IMS::customerSignInMenu() {
    cout << "\n--- Customer Sign In ---\n";
    string user = promptLine("Username: ");
    string pass = promptLine("Password: ");

    int idx = findCustomerByUsername(user);
    if (idx == -1 || customers[idx].getPassword() != pass) {
        cout << "Invalid credentials.\n";
        return;
    }

    double wallet = promptDouble("Enter available money: ", 0.0);
    customerSession(customers[idx], wallet);
}

vector<CartItem> IMS::getCartByCustomer(const string& customerId) const {
    vector<CartItem> customerCart;
    for (const auto& c : cartItems) {
        if (c.customerId == customerId) {
            customerCart.push_back(c);
        }
    }
    return customerCart;
}

void IMS::updateCustomerCart(const string& customerId, const vector<CartItem>& updated) {
    cartItems.erase(remove_if(cartItems.begin(), cartItems.end(), [&](const CartItem& c) {
        return c.customerId == customerId;
    }), cartItems.end());

    for (const auto& u : updated) {
        if (u.quantity > 0) {
            cartItems.push_back(u);
        }
    }
    saveCart();
}

void IMS::printCartWithBill(const string& customerId, double walletAmount) const {
    auto customerCart = getCartByCustomer(customerId);
    if (customerCart.empty()) {
        cout << "Cart is empty.\n";
        return;
    }

    double subtotal = 0.0;
    cout << "\n--- CART ---\n";
    for (const auto& ci : customerCart) {
        int pIdx = findProductById(ci.productId);
        if (pIdx == -1) {
            continue;
        }
        double itemCost = products[pIdx].price * ci.quantity;
        subtotal += itemCost;
        cout << "Product: " << products[pIdx].productName
             << " | ID: " << ci.productId
             << " | Qty: " << ci.quantity
             << " | Unit: " << fixed << setprecision(2) << products[pIdx].price
             << " | Cost: " << itemCost << '\n';
    }

    double tax = subtotal * 0.02;
    double delivery = 200.0;
    double total = subtotal + tax + delivery;

    cout << "Subtotal: " << fixed << setprecision(2) << subtotal << '\n';
    cout << "Sales Tax (2%): " << tax << '\n';
    cout << "Delivery: " << delivery << '\n';
    cout << "Grand Total: " << total << '\n';
    cout << "Available Cash: " << walletAmount << '\n';
}

void IMS::customerSession(const Customer& customer, double walletAmount) {
    while (true) {
        cout << "\n=== CUSTOMER MENU (" << customer.getUsername() << ") ===\n";
        cout << "1. View Products\n";
        cout << "2. Purchase Product / Add to Cart\n";
        cout << "3. View Cart\n";
        cout << "4. Update Cart Quantity\n";
        cout << "5. Delete from Cart\n";
        cout << "6. Checkout\n";
        cout << "0. Logout\n";

        int choice = promptInt("Select option: ", 0, 6);
        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            listProducts(true);
        } else if (choice == 2) {
            listProducts(true);
            string pid = promptLine("Enter Product ID: ");
            int pIdx = findProductById(pid);
            if (pIdx == -1) {
                cout << "Product not found.\n";
                continue;
            }
            if (products[pIdx].quantity <= 0) {
                cout << "Out of stock.\n";
                continue;
            }

            cout << "Available Quantity: " << products[pIdx].quantity << '\n';
            int qty = promptInt("Select quantity: ", 1, products[pIdx].quantity);
            if (!products[pIdx].size.empty()) {
                cout << "Available size: " << products[pIdx].size << '\n';
            }
            if (!products[pIdx].color.empty()) {
                cout << "Available color: " << products[pIdx].color << '\n';
            }

            auto customerCart = getCartByCustomer(customer.getId());
            bool merged = false;
            for (auto& item : customerCart) {
                if (item.productId == pid) {
                    item.quantity += qty;
                    merged = true;
                    break;
                }
            }
            if (!merged) {
                customerCart.emplace_back(customer.getId(), pid, qty);
            }
            updateCustomerCart(customer.getId(), customerCart);
            cout << "Added to cart.\n";
        } else if (choice == 3) {
            printCartWithBill(customer.getId(), walletAmount);
        } else if (choice == 4) {
            auto customerCart = getCartByCustomer(customer.getId());
            if (customerCart.empty()) {
                cout << "Cart is empty.\n";
                continue;
            }
            printCartWithBill(customer.getId(), walletAmount);
            string pid = promptLine("Enter Product ID to update: ");
            int pIdx = findProductById(pid);
            if (pIdx == -1) {
                cout << "Product not found.\n";
                continue;
            }
            int qty = promptInt("Enter new quantity: ", 1, products[pIdx].quantity);
            bool found = false;
            for (auto& item : customerCart) {
                if (item.productId == pid) {
                    item.quantity = qty;
                    found = true;
                }
            }
            if (!found) {
                cout << "Item not in cart.\n";
                continue;
            }
            updateCustomerCart(customer.getId(), customerCart);
            cout << "Cart updated.\n";
        } else if (choice == 5) {
            auto customerCart = getCartByCustomer(customer.getId());
            if (customerCart.empty()) {
                cout << "Cart is empty.\n";
                continue;
            }
            printCartWithBill(customer.getId(), walletAmount);
            string pid = promptLine("Enter Product ID to delete from cart: ");
            auto before = customerCart.size();
            customerCart.erase(remove_if(customerCart.begin(), customerCart.end(), [&](const CartItem& c) {
                return c.productId == pid;
            }), customerCart.end());

            if (before == customerCart.size()) {
                cout << "Item not in cart.\n";
                continue;
            }

            updateCustomerCart(customer.getId(), customerCart);
            cout << "Item removed from cart.\n";
        } else if (choice == 6) {
            auto customerCart = getCartByCustomer(customer.getId());
            if (customerCart.empty()) {
                cout << "Cart is empty.\n";
                continue;
            }

            double subtotal = 0.0;
            bool validStock = true;
            for (const auto& item : customerCart) {
                int pIdx = findProductById(item.productId);
                if (pIdx == -1 || products[pIdx].quantity < item.quantity) {
                    validStock = false;
                    break;
                }
                subtotal += products[pIdx].price * item.quantity;
            }
            if (!validStock) {
                cout << "One or more products are unavailable in requested quantity.\n";
                continue;
            }

            double tax = subtotal * 0.02;
            double delivery = 200.0;
            double total = subtotal + tax + delivery;

            if (total > walletAmount) {
                cout << "Not enough Cash\n";
                continue;
            }

            string shipmentAddress = promptLine("Shipment Address: ");
            (void)shipmentAddress;

            for (const auto& item : customerCart) {
                int pIdx = findProductById(item.productId);
                products[pIdx] += -item.quantity;
            }

            updateCustomerCart(customer.getId(), {});
            saveProducts();

            walletAmount -= total;
            cout << "Thank you for shopping\n";
            cout << "Remaining Wallet: " << fixed << setprecision(2) << walletAmount << '\n';
            listProducts(true);
        }
    }
}
