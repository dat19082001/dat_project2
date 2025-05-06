#ifndef MUSICDISC_H
#define MUSICDISC_H

#include "Product.h"

class MusicDisc : public Product {
private:
    int duration; // Thời gian đĩa nhạc (tính bằng giây)
    int sizeKB;   // Kích thước đĩa nhạc (tính bằng KB)

public:
    MusicDisc();
    MusicDisc(string id, string title, string author, int year, string publisher, double importPrice, double salePrice, int quantity, int quantitySold, int duration, int sizeKB);
    void display() const override;
    void save(ofstream &out) const override;
    void loadFromLine(const string& line) override;

    int getDuration() const;
    int getSizeKB() const;
};

#endif
