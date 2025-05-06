#include "MusicDisc.h"
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

MusicDisc::MusicDisc() : Product(), duration(0), sizeKB(0) {}

MusicDisc::MusicDisc(string id, string title, string author, int year, string publisher, double importPrice, double salePrice, int quantity,int quantitySold, int duration, int sizeKB)
    : Product(id, title, author, year, publisher, importPrice, salePrice, quantity, quantitySold), duration(duration), sizeKB(sizeKB) {}

void MusicDisc::display() const {
    cout << "MUSIC|" << "ID: " << id << "|Title: " << title << "|Author: " << author << "|Year: " << year << "|Pulisher: " << publisher
    << "|Price: " << salePrice << "|Duration: " << duration 
    << "|SizeKB: " << sizeKB << endl;
}

void MusicDisc::save(ofstream &out) const {
    out << "MUSIC|" << id << "|" << title << "|" << author << "|" << year << "|" << publisher
        << "|" << importPrice << "|" << salePrice << "|" << quantity << "|" << duration
        << "|" << sizeKB << endl;
}

void MusicDisc::loadFromLine(const string& line) {
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
    ss >> duration;
    ss.ignore(1, '|');
    ss >> sizeKB;
}
