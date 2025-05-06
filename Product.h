#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector> 
using namespace std;

class Product {
protected:
    string id;
    string title;
    string author;
    int year;
    string publisher;
    double importPrice;
    double salePrice;
    int quantity;
    int quantitySold;

public:
    Product();
    Product(string id, string title, string author, int year, string publisher, double importPrice, double salePrice, int quantity, int quantitySold);

    virtual ~Product();

    virtual void display() const = 0;
    virtual void save(ofstream &out) const = 0;
    virtual void loadFromLine(const string& line) = 0;

    string getId() const;
    string getTitle() const;
    string getAuthor() const;
    int getYear() const;
    double getImportPrice() const;
    double getSalePrice() const;
    int getQuantity() const;
    int getQuantitySold() const;

    void updateQuantity(int quantitySold);
    //double getTotalRevenue() const;

    void setTitle(const string& newTitle);
    void setImportPrice(double newImport);
    void setSalePrice(double newSale);
    void setQuantity(int newQuantity);


};

Product* searchProductById(vector<Product*>& products, const string& id);
vector<Product*> searchProductByAuthor(vector<Product*>& products, const string& author);
vector<Product*> searchProductByYear(vector<Product*>& products, int year) ;
vector<Product*> searchProductByTitle(vector<Product*>& products, const string& title);
void loadProductsFromFile(vector<Product *> &products);
void saveProductsToFile(const vector<Product *> &products);
void addNewProduct(vector<Product *> &products);
void updateProductFile(const vector<Product *> &products); 
void displayProducts(const vector<Product *> &products);
void editProductById(vector<Product *> &products);
void deleteProductById(vector<Product*> &products, const string &id);








#endif
