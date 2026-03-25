#pragma once

#include <string>
#include <vector>
#include <ostream>

class Person {
protected:
    std::string firstName;
    std::string lastName;
    std::string id;
    std::string username;
    std::string email;
    std::string password;
    std::string registrationDate;
    std::string gender;
    std::string bloodGroup;
    std::string contactNo;
    std::string address;

public:
    Person();
    virtual ~Person() = default;

    const std::string& getId() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;

    void setBasicEditableFields(
        const std::string& first,
        const std::string& last,
        const std::string& addr,
        const std::string& contact,
        const std::string& user,
        const std::string& mail,
        const std::string& pass
    );

    virtual std::string toStorageRow() const = 0;
    virtual std::string roleName() const = 0;
};

class Shopkeeper : public Person {
private:
    static int nextSerial;

public:
    static void seedCounterFromExisting(int maxSerial);
    static std::string generateId();

    Shopkeeper();

    std::string toStorageRow() const override;
    std::string roleName() const override;

    static Shopkeeper fromStorageRow(const std::string& line);
    static Shopkeeper createFromInput();
};

class Customer : public Person {
private:
    static int nextSerial;

public:
    static void seedCounterFromExisting(int maxSerial);
    static std::string generateId();

    Customer();

    std::string toStorageRow() const override;
    std::string roleName() const override;

    static Customer fromStorageRow(const std::string& line);
    static Customer createFromInput();
};

class Product {
private:
    static int globalSerial;

public:
    std::string productId;
    std::string ownerShopkeeperId;
    std::string categoryName;
    std::string productName;
    std::string description;
    std::string size;
    std::string color;
    int quantity;
    double price;

    Product();

    static void seedCounterFromExisting(int maxSerial);
    static std::string generateProductId(int categoryCode);

    bool isOutOfStock() const;
    std::string toStorageRow() const;
    static Product fromStorageRow(const std::string& line);

    Product& operator+=(int deltaQuantity);

    friend std::ostream& operator<<(std::ostream& os, const Product& p);
};

class CartItem {
public:
    std::string customerId;
    std::string productId;
    int quantity;

    CartItem();
    CartItem(const std::string& cId, const std::string& pId, int qty);

    std::string toStorageRow() const;
    static CartItem fromStorageRow(const std::string& line);
};

class IMS {
private:
    std::vector<Shopkeeper> shopkeepers;
    std::vector<Customer> customers;
    std::vector<Product> products;
    std::vector<CartItem> cartItems;

    static const std::string shopkeeperFile;
    static const std::string customerFile;
    static const std::string productFile;
    static const std::string cartFile;

    void loadAll();
    void saveShopkeepers() const;
    void saveCustomers() const;
    void saveProducts() const;
    void saveCart() const;

    int findShopkeeperByUsername(const std::string& user) const;
    int findCustomerByUsername(const std::string& user) const;
    int findProductById(const std::string& productId) const;

public:
    static std::string trim(const std::string& s);
    static std::vector<std::string> split(const std::string& s, char delim);

    static std::string promptLine(const std::string& label);
    static int promptInt(const std::string& label, int minValue, int maxValue);
    static double promptDouble(const std::string& label, double minValue);
    static bool promptBool(const std::string& label);

    static std::string todayDate();
    static int parseNumericSuffix(const std::string& idStr);

private:

    int selectCategoryCode() const;
    std::string categoryByCode(int code) const;

    void listShopkeepers() const;
    void listCustomers() const;
    void listProducts(bool onlyInStock = false) const;
    void listProductsByOwner(const std::string& ownerId) const;

    void addShopkeeper();
    void addCustomer();
    void addProductByAdmin();
    void editShopkeeper();
    void editCustomer();
    void editProduct();
    void deleteShopkeeper();
    void deleteCustomer();
    void deleteProduct();

    void adminMenu();

    void shopkeeperRegister();
    void shopkeeperSignInMenu();
    void shopkeeperSession(const Shopkeeper& sk);

    void customerRegister();
    void customerSignInMenu();
    void customerSession(const Customer& customer, double walletAmount);

    std::vector<CartItem> getCartByCustomer(const std::string& customerId) const;
    void updateCustomerCart(const std::string& customerId, const std::vector<CartItem>& updated);
    void printCartWithBill(const std::string& customerId, double walletAmount) const;

public:
    IMS();
    void run();
};
