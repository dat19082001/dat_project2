#ifndef BOOK_H
#define BOOK_H

#include "Product.h"


class Book : public Product {
private:
    int pageCount;
    string format; // Định dạng sách (ví dụ: A4, A5...)

public:
    Book();
    Book(string id, string title, string author, int year, string publisher, double importPrice, double salePrice, int quantity, int quantitySold, int pageCount, string format);
    void display() const override;
    void save(ofstream &out) const override;
    void loadFromLine(const string& line) override;

    int getPageCount() const;
    string getFormat() const;
};

#endif
