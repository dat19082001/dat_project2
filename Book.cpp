#include "Book.h"
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

Book::Book() : Product(), pageCount(0), format("A4") {}

Book::Book(string id, string title, string author, int year, string publisher, double importPrice, double salePrice, int quantity, int quantitySold, int pageCount, string format)
    : Product(id, title, author, year, publisher, importPrice, salePrice, quantity, quantitySold), pageCount(pageCount), format(format) {}

void Book::display() const {
    cout << "BOOK|" << "ID: " << id << "|Title: " << title << "|Author: " << author << "|Year: " << year << "|Pulisher: " << publisher
    << "|Price: " << salePrice << "|Pages: " << pageCount
    << "|Format: " << format << endl;
}

void Book::save(ofstream &out) const {
    out << "BOOK|" << id << "|" << title << "|" << author << "|" << year << "|" << publisher
        << "|" << importPrice << "|" << salePrice << "|" << quantity << "|" << pageCount
        << "|" << format << endl;
}

void Book::loadFromLine(const string& line) {
    stringstream ss(line);
    string type;
    getline(ss, type, '|');
    getline(ss, id, '|');
    getline(ss, title, '|');
    getline(ss, author, '|');
    ss >> year;
    ss.ignore(1, '|');
    getline(ss, publisher, '|');
    ss >> importPrice;
    ss.ignore(1, '|');
    ss >> salePrice;
    ss.ignore(1, '|');
    ss >> quantity;
    ss.ignore(1, '|');
    ss >> pageCount;
    ss.ignore(1, '|');
    getline(ss, format);
}
