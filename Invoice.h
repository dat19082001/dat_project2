#ifndef INVOICE_H
#define INVOICE_H

#include "Product.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

class Invoice
{
private:
    string customerName;
    string customerPhone;
    string dateTime;
    vector<pair<Product *, int>> soldProducts;
    double totalAmount;

public:
    Invoice(string customerName, string customerPhone, vector<pair<Product *, int>> soldProductsList);
    void generateInvoice();
    void addSoldProduct(Product *product, int quantitySold)
    {
        soldProducts.push_back(make_pair(product, quantitySold));
    }
    double getTotalAmount();

    void saveInvoice(ofstream &out);
    string getDateTime() const;
    void display() const;    
};
void processSale(vector<Product *> &products);
void readAndDisplayAllInvoices(const string &fileName);
void readInvoices(const string &fileName,const vector<Product *> &products);
#endif
