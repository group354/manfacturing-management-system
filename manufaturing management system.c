#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <limits>
using namespace std;

class Employee {
protected:
    string name;
    string role;
    double salary;

public:
    Employee(string name, string role, double salary) : name(name), role(role), salary(salary) {
        if (name.empty() || role.empty()) {
            throw invalid_argument("Name and role cannot be empty");
        }
    }

    virtual ~Employee() {}
    virtual void display() const = 0;
    virtual void saveToFile(ofstream& outFile) const = 0;

    static void readFromFile(ifstream& inFile) {
        string name, role;
        double salary;

        while (inFile >> name >> role >> salary) {
            cout << "Employee Name: " << name << ", Role: " << role << ", Salary: " << salary << endl;
        }
    }
};

class Manager : public Employee {
public:
    Manager(string name, double salary) : Employee(name, "Manager", salary) {}

    void display() const override {
        cout << "Manager: " << name << ", Role: " << role << ", Salary: " << salary << endl;
    }

    void saveToFile(ofstream& outFile) const override {
        outFile << name << " " << role << " " << salary << endl;
    }
};

class Engineer : public Employee {
public:
    Engineer(string name, double salary) : Employee(name, "Engineer", salary) {}

    void display() const override {
        cout << "Engineer: " << name << ", Role: " << role << ", Salary: " << salary << endl;
    }

    void saveToFile(ofstream& outFile) const override {
        outFile << name << " " << role << " " << salary << endl;
    }
};

class Product {
protected:
    string name;
    int quantity;
    double price;

public:
    Product(string name, int quantity, double price) : name(name), quantity(quantity), price(price) {
        if (name.empty()) {
            throw invalid_argument("Name cannot be empty");
        }
        if (quantity < 0) {
            throw invalid_argument("Quantity cannot be negative");
        }
    }

    virtual ~Product() {}
    virtual void display() const {
        cout << "Product: " << name << ", Quantity: " << quantity << ", Price: " << price << endl;
    }

    virtual void saveToFile(ofstream& outFile) const {
        outFile << name << " " << quantity << " " << price << endl;
    }

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void updateQuantity(int soldQuantity) {
        if (soldQuantity > quantity) {
            throw invalid_argument("Insufficient stock for the sale");
        }
        quantity -= soldQuantity;
    }

    static void readFromFile(ifstream& inFile) {
        string name;
        int quantity;
        double price;

        while (inFile >> name >> quantity >> price) {
            cout << "Product: " << name << ", Quantity: " << quantity << ", Price: " << price << endl;
        }
    }
};

class ElectronicProduct : public Product {
public:
    ElectronicProduct(string name, int quantity, double price) : Product(name, quantity, price) {}

    void display() const override {
        cout << "Electronic Product: " << name << ", Quantity: " << quantity << ", Price: " << price << endl;
    }
};

class FurnitureProduct : public Product {
public:
    FurnitureProduct(string name, int quantity, double price) : Product(name, quantity, price) {}

    void display() const override {
        cout << "Furniture Product: " << name << ", Quantity: " << quantity << ", Price: " << price << endl;
    }
};

class Sale {
private:
    string productName;
    int quantitySold;
    double totalPrice;

public:
    Sale(string productName, int quantitySold, double totalPrice)
        : productName(productName), quantitySold(quantitySold), totalPrice(totalPrice) {}

    void display() const {
        cout << "Sale - Product: " << productName << ", Quantity Sold: " << quantitySold << ", Total Price: " << totalPrice << endl;
    }

    double getTotalPrice() const { return totalPrice; }

    void saveToFile(ofstream& outFile) const {
        outFile << productName << " " << quantitySold << " " << totalPrice << endl;
    }

    static void readFromFile(ifstream& inFile) {
        string productName;
        int quantitySold;
        double totalPrice;

        while (inFile >> productName >> quantitySold >> totalPrice) {
            cout << "Sale - Product: " << productName << ", Quantity Sold: " << quantitySold << ", Total Price: " << totalPrice << endl;
        }
    }
};

int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) break;
        cout << "Invalid input, please enter an integer!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) break;
        cout << "Invalid input, please enter a number!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

unique_ptr<Employee> createEmployee() {
    string name;
    cout << "Enter employee name: ";
    cin >> name;
    double salary = getDoubleInput("Enter salary: ");
    string role;
    cout << "Enter role (Manager/Engineer): ";
    cin >> role;

    if (role == "Manager") {
        return make_unique<Manager>(name, salary);
    } else if (role == "Engineer") {
        return make_unique<Engineer>(name, salary);
    } else {
        throw invalid_argument("Invalid role entered!");
    }
}

unique_ptr<Product> createProduct() {
    string name;
    cout << "Enter product name: ";
    cin >> name;
    int quantity = getIntInput("Enter quantity: ");
    double price = getDoubleInput("Enter price: ");
    string type;
    cout << "Enter product type (Electronic/Furniture): ";
    cin >> type;

    if (type == "Electronic") {
        return make_unique<ElectronicProduct>(name, quantity, price);
    } else if (type == "Furniture") {
        return make_unique<FurnitureProduct>(name, quantity, price);
    } else {
        throw invalid_argument("Invalid product type entered!");
    }
}

void saveEmployeesToFile(const vector<unique_ptr<Employee>>& employees, const string& fileName) {
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (const auto& employee : employees) {
            employee->saveToFile(outFile);
        }
        cout << "Employee details saved to " << fileName << "!" << endl;
    } else {
        cout << "Unable to open file: " << fileName << "!" << endl;
    }
}

void saveProductsToFile(const vector<unique_ptr<Product>>& products, const string& fileName) {
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (const auto& product : products) {
            product->saveToFile(outFile);
        }
        cout << "Product details saved to " << fileName << "!" << endl;
    } else {
        cout << "Unable to open file: " << fileName << "!" << endl;
    }
}

void saveSalesToFile(const vector<Sale>& sales, const string& fileName) {
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (const auto& sale : sales) {
            sale.saveToFile(outFile);
        }
        cout << "Sales details saved to " << fileName << "!" << endl;
    } else {
        cout << "Unable to open file: " << fileName << "!" << endl;
    }
}

void readEmployeesFromFile(const string& fileName) {
    ifstream inFile(fileName);
    if (inFile.is_open()) {
        cout << "Employee details from file:" << endl;
        Employee::readFromFile(inFile);
    } else {
        cout << "Unable to open file: " << fileName << "!" << endl;
    }
}

void readProductsFromFile(const string& fileName) {
    ifstream inFile(fileName);
    if (inFile.is_open()) {
        cout << "Product details from file:" << endl;
        Product::readFromFile(inFile);
    } else {
        cout << "Unable to open file: " << fileName << "!" << endl;
    }
}

void readSalesFromFile(const string& fileName) {
    ifstream inFile(fileName);
    if (inFile.is_open()) {
        cout << "Sales details from file:" << endl;
        Sale::readFromFile(inFile);
    } else {
        cout << "Unable to open file: " << fileName << "!" << endl;
    }
}

void calculateTotalSales(const vector<unique_ptr<Product>>& products) {
    double totalSales = 0.0;

    for (const auto& product : products) {
        totalSales += product->getQuantity() * product->getPrice();
    }

    cout << "Total Sales (Inventory Value): " << totalSales << endl;
}

int main() {
    vector<unique_ptr<Employee>> employees;
    vector<unique_ptr<Product>> products;
    vector<Sale> sales;

    try {
        int choice;
        do {
            cout << "\n--- System Menu ---\n";
            cout << "1. Add Employee\n";
            cout << "2. Add Product\n";
            cout << "3. Process Sale\n";
            cout << "4. Display Employees\n";
            cout << "5. Display Products\n";
            cout << "6. Display Sales\n";
            cout << "7. Calculate Total Sales\n";
            cout << "8. Exit\n";
            choice = getIntInput("Enter your choice: ");

            switch (choice) {
                case 1:
                    employees.push_back(createEmployee());
                    break;

                case 2:
                    products.push_back(createProduct());
                    break;

                case 3: {
                    string productName;
                    cout << "Enter product name: ";
                    cin >> productName;

                    int quantitySold = getIntInput("Enter quantity sold: ");
                    auto it = find_if(products.begin(), products.end(), [&productName](const auto& product) {
                        return product->getName() == productName;
                    });

                    if (it != products.end()) {
                        (*it)->updateQuantity(quantitySold);
                        double totalPrice = quantitySold * (*it)->getPrice();
                        sales.emplace_back(productName, quantitySold, totalPrice);
                        cout << "Sale recorded successfully.\n";
                    } else {
                        cout << "Product not found.\n";
                    }
                    break;
                }

                case 4:
                    cout << "\n--- Employees ---\n";
                    for (const auto& employee : employees) {
                        employee->display();
                    }
                    break;

                case 5:
                    cout << "\n--- Products ---\n";
                    for (const auto& product : products) {
                        product->display();
                    }
                    break;

                case 6:
                    cout << "\n--- Sales ---\n";
                    for (const auto& sale : sales) {
                        sale.display();
                    }
                    break;

                case 7:
                    calculateTotalSales(products);
                    break;

                case 8:
                    cout << "Exiting system. Goodbye!\n";
                    break;

                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } while (choice != 8);

    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
