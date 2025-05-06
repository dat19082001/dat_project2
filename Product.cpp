#include "Product.h"
#include <vector>
#include "Book.h"
#include "MusicDisc.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <ctime>
using namespace std;

Product::Product() : id(""), title(""), author(""), year(0), publisher(""), importPrice(0), salePrice(0), quantity(0), quantitySold(0) {}

Product::Product(string id, string title, string author, int year, string publisher, double importPrice, double salePrice, int quantity, int quatitySold)
    : id(id), title(title), author(author), year(year), publisher(publisher), importPrice(importPrice), salePrice(salePrice), quantity(quantity), quantitySold(quantitySold) {}

Product::~Product() {}

string Product::getId() const { return id; }
string Product::getTitle() const { return title; }
string Product::getAuthor() const { return author; }
int Product::getYear() const { return year; }
double Product::getImportPrice() const { return importPrice; }
double Product::getSalePrice() const { return salePrice; }
int Product::getQuantity() const { return quantity; }
int Product::getQuantitySold() const { return quantitySold; }

void Product::updateQuantity(int quantitySold)
{
    quantity -= quantitySold;
    this->quantitySold += quantitySold;
}

/*double Product::getTotalRevenue() const
{
    return salePrice * quantitySold;
}*/

void Product::setTitle(const string &newTitle)
{
    title = newTitle;
}

void Product::setImportPrice(double newImport)
{
    importPrice = newImport;
}

void Product::setSalePrice(double newSale)
{
    salePrice = newSale;
}

void Product::setQuantity(int newQuantity)
{
    quantity = newQuantity;
}

Product *searchProductById(vector<Product *> &products, const string &id)
{
    for (auto &product : products)
    {
        if (product->getId() == id)
        {
            return product;
        }
    }
    return nullptr;
}

vector<Product *> searchProductByAuthor(vector<Product *> &products, const string &author)
{
    vector<Product *> result;
    for (auto *product : products)
    {
        if (product->getAuthor().find(author) != string::npos)
        {
            result.push_back(product);
        }
    }
    return result;
}

vector<Product *> searchProductByTitle(vector<Product *> &products, const string &title)
{
    vector<Product *> result;
    for (auto *product : products)
    {
        if (product->getTitle().find(title) != string::npos)
        {
            result.push_back(product);
        }
    }
    return result;
}

vector<Product *> searchProductByYear(vector<Product *> &products, int year)
{
    vector<Product *> result;
    for (auto *product : products)
    {
        if (product->getYear() == year)
        {
            result.push_back(product);
        }
    }
    return result;
}

void loadProductsFromFile(vector<Product *> &products)
{
    ifstream inFile("product.txt");
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string type;
        getline(ss, type, '|');
        if (type == "BOOK")
        {
            Product *product = new Book();
            product->loadFromLine(line);
            products.push_back(product);
        }
        else if (type == "MUSIC")
        {
            Product *product = new MusicDisc();
            product->loadFromLine(line);
            products.push_back(product);
        }
    }
}

void saveProductsToFile(const vector<Product *> &products)
{
    ofstream outFile("product.txt", ios::app);
    for (const auto &product : products)
    {
        product->save(outFile);
    }
}

bool checkDuplicateId(const vector<Product *> &products, const string &id)
{
    for (const auto &product : products)
    {
        if (product->getId() == id)
        {
            return true;
        }
    }
    return false;
}

bool compareById(Product *product1, Product *product2)
{
    return product1->getId() < product2->getId();
}

bool isValidYear(int year)
{
    // Lấy năm hiện tại
    time_t t = time(0);
    struct tm *now = localtime(&t);
    int currentYear = now->tm_year + 1900; // tm_year tính từ 1900

    return year >= 1000 && year <= currentYear;
}

void addNewProduct(vector<Product *> &products)
{
    int type;
    do
    {
        cout << "Enter product type (1 for Book, 2 for Music Disc): ";
        cin >> type;

        if (type != 1 && type != 2)
        {
            cout << "Invalid product type. Please enter 1 for Book or 2 for Music Disc.\n";
        }
    } while (type != 1 && type != 2);

    string id, title, author, publisher, format;
    int year, quantity, pageCount, duration, sizeKB;
    double importPrice, salePrice;

    regex idPattern("^\\d{3}$");
    do
    {
        cout << "Enter product ID (3 digits, Ex: 001): ";
        cin >> id;
        if (!regex_match(id, idPattern))
        {
            cout << " Invalid format. ID must be exactly 3 digits.\n";
            continue;
        }

        if (checkDuplicateId(products, id))
        {
            cout << "Duplicate ID. Please enter a unique ID.\n";
        }
    } while (!regex_match(id, idPattern) || checkDuplicateId(products, id));

    regex flexiblePattern("^([A-Z][^ ]*)( [A-Z][^ ]*)*$");
    cin.ignore();
    do
    {
        cout << "Enter product title: ";
        getline(cin, title);
        if (!regex_match(title, flexiblePattern))
        {
            cout << "Invalid format. Each word must start with a capital letter (letters, numbers, and special characters allowed after).\n";
        }
    } while (!regex_match(title, flexiblePattern));

    do
    {
        cout << "Enter author: ";
        getline(cin, author);
        if (!regex_match(author, flexiblePattern))
        {
            cout << "Invalid format. Each word must start with a capital letter (letters, numbers, and special characters allowed after).\n";
        }
    } while (!regex_match(author, flexiblePattern));

    do
    {
        cout << "Enter publisher: ";
        getline(cin, publisher);
        if (!regex_match(publisher, flexiblePattern))
        {
            cout << "Invalid format. Each word must start with a capital letter (letters, numbers, and special characters allowed after).\n";
        }
    } while (!regex_match(publisher, flexiblePattern));

    do
    {
        cout << "Enter year of release (4 digits, not in the future): ";
        cin >> year;
        if (!isValidYear(year))
        {
            cout << "Invalid year. Year must be a 4-digit number and not in the future.\n";
        }
    } while (!isValidYear(year));

    do
    {
        cout << "Enter import price: ";
        if (cin >> importPrice)
        {
            if (importPrice > 0)
                break;
            else
                cout << "Import price must be greater than 0.\n";
        }
        else
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();             // xóa cờ lỗi
            cin.ignore(10000, '\n'); // bỏ qua dòng sai
        }
    } while (true);

    do
    {
        cout << "Enter sale price: ";
        if (cin >> salePrice)
        {
            if (salePrice > 0)
                break;
            else
                cout << "Sale price must be greater than 0.\n";
        }
        else
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();             // xóa cờ lỗi
            cin.ignore(10000, '\n'); // bỏ qua dòng sai
        }
    } while (true);

    do
    {
        cout << "Enter quantity: ";
        if (cin >> quantity)
        {
            if (quantity > 0)
                break;
            else
                cout << "Quantity must be a positive integer.\n";
        }
        else
        {
            cout << "Invalid input. Please enter a positive integer.\n";
            cin.clear();             // xóa trạng thái lỗi
            cin.ignore(10000, '\n'); // bỏ qua phần còn lại của dòng
        }
    } while (true);

    if (type == 1)
    {
        do
        {
            cout << "Enter page count: ";
            if (cin >> pageCount)
            {
                if (pageCount > 0)
                    break;
                else
                    cout << "Page count must be a positive integer.\n";
            }
            else
            {
                cout << "Invalid input. Please enter a positive integer.\n";
                cin.clear();             // xóa trạng thái lỗi
                cin.ignore(10000, '\n'); // bỏ qua phần còn lại của dòng
            }
        } while (true);

        cout << "Enter format: ";
        cin.ignore();
        getline(cin, format);
        Product *product = new Book(id, title, author, year, publisher, importPrice, salePrice, quantity, 0, pageCount, format);
        products.push_back(product);
    }
    else if (type == 2)
    {
        do
        {
            cout << "Enter duration (in seconds): ";
            if (cin >> duration)
            {
                if (duration > 0)
                    break;
                else
                    cout << "Duration (in seconds) must be a positive integer.\n";
            }
            else
            {
                cout << "Invalid input. Please enter a positive integer.\n";
                cin.clear();             // xóa trạng thái lỗi
                cin.ignore(10000, '\n'); // bỏ qua phần còn lại của dòng
            }
        } while (true);

        do
        {
            cout << "Enter size (in KB): ";
            if (cin >> sizeKB)
            {
                if (sizeKB > 0)
                    break;
                else
                    cout << "Size (kB) must be a positive integer.\n";
            }
            else
            {
                cout << "Invalid input. Please enter a positive integer.\n";
                cin.clear();             // xóa trạng thái lỗi
                cin.ignore(10000, '\n'); // bỏ qua phần còn lại của dòng
            }
        } while (true);

        Product *product = new MusicDisc(id, title, author, year, publisher, importPrice, salePrice, quantity, 0, duration, sizeKB);
        products.push_back(product);
    }

    sort(products.begin(), products.end(), compareById);

    cout << "New product added and list sorted by ID.\n";
}

void updateProductFile(const vector<Product *> &products)
{
    ofstream outFile("product.txt"); // Open in overwrite mode to rewrite entire content
    for (const auto &product : products)
    {
        product->save(outFile); // Save each product after any change
    }
}

void displayProducts(const vector<Product *> &products)
{
    for (const auto &product : products)
    {
        product->display();
    }
}

void editProductById(vector<Product *> &products)
{
    string productId;
    cout << "Enter product ID to edit (3 digits, Ex: 001): ";
    cin >> productId;

    Product *product = searchProductById(products, productId);
    if (product == nullptr)
    {
        cout << "Product not found.\n";
        return;
    }

    int choice;
    do
    {
        cout << "\n--- Edit Menu ---\n";
        cout << "1. Edit product name\n";
        cout << "2. Edit import price\n";
        cout << "3. Edit sale price\n";
        cout << "4. Edit quantity\n";
        cout << "5. Exit editing\n";
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(); // Clear newline from buffer

        switch (choice)
        {
        case 1:
        {
            string newTitle;
            regex flexiblePattern("^([A-Z][^ ]*)( [A-Z][^ ]*)*$");
            do
            {
                cout << "Enter product new title: ";
                getline(cin, newTitle);
                if (!regex_match(newTitle, flexiblePattern))
                {
                    cout << "Invalid format. Each word must start with a capital letter (letters, numbers, and special characters allowed after).\n";
                }
            } while (!regex_match(newTitle, flexiblePattern));
            product->setTitle(newTitle);
            updateProductFile(products);
            cout << "Title updated.\n";
            break;
        }
        case 2:
        {
            double newImportPrice;
            do
            {
                cout << "Enter new import price: ";
                if (cin >> newImportPrice)
                {
                    if (newImportPrice > 0)
                        break;
                    else
                        cout << "Import price must be greater than 0.\n";
                }
                else
                {
                    cout << "Invalid input. Please enter a number.\n";
                    cin.clear();             // xóa cờ lỗi
                    cin.ignore(10000, '\n'); // bỏ qua dòng sai
                }
            } while (true);

            product->setImportPrice(newImportPrice);
            updateProductFile(products);

            cout << "Import price updated.\n";
            break;
        }
        case 3:
        {
            double newSalePrice;
            do
            {
                cout << "Enter new sale price: ";
                if (cin >> newSalePrice)
                {
                    if (newSalePrice > 0)
                        break;
                    else
                        cout << "Sale price must be greater than 0.\n";
                }
                else
                {
                    cout << "Invalid input. Please enter a number.\n";
                    cin.clear();             // xóa cờ lỗi
                    cin.ignore(10000, '\n'); // bỏ qua dòng sai
                }
            } while (true);

            product->setSalePrice(newSalePrice);
            updateProductFile(products);

            cout << "Sale price updated.\n";
            break;
        }
        case 4:
        {
            int newQuantity;
            cout << "Enter new quantity: ";
            cin >> newQuantity;
            product->setQuantity(newQuantity);
            updateProductFile(products);

            cout << "Quantity updated.\n";
            break;
        }
        case 5:
            cout << "Exiting edit menu.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    updateProductFile(products);
    cout << "Product information updated and saved.\n";
}

void deleteProductById(vector<Product *> &products, const string &id)
{

    auto it = find_if(products.begin(), products.end(),
                      [&id](Product *product)
                      {
                          return product->getId() == id;
                      });

    if (it != products.end())
    {

        delete *it;
        products.erase(it);
        cout << "Product with ID \"" << id << "\" deleted successfully.\n";
    }
    else
    {
        cout << "Product with ID \"" << id << "\" not found.\n";
    }
    updateProductFile(products);
}
