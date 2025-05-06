#include "Invoice.h"
#include "Book.h"
#include "MusicDisc.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <iomanip> //setfill setw
#include "Product.h"

Invoice::Invoice(string customerName, string customerPhone, vector<pair<Product *, int>> soldProductsList)
{
    this->customerName = customerName;
    this->customerPhone = customerPhone;
    this->soldProducts = soldProductsList;
    // Sử dụng time_t để lấy thời gian hệ thống
    time_t now = time(0);
    tm *ltm = localtime(&now); // Chuyển thời gian hệ thống sang thời gian địa phương

    // Định dạng thời gian thành chuỗi
    stringstream dateStream;
    dateStream << setfill('0') << setw(4) << (1900 + ltm->tm_year) << "-"
           << setw(2) << (1 + ltm->tm_mon) << "-"
           << setw(2) << ltm->tm_mday << " "
           << setw(2) << ltm->tm_hour << ":"
           << setw(2) << ltm->tm_min << ":"
           << setw(2) << ltm->tm_sec;

    dateTime = dateStream.str();
}

string Invoice::getDateTime() const
{
    return dateTime;
}

void Invoice::generateInvoice()
{
    cout << "Invoice generated for " << customerName << " (" << customerPhone << ") on " << dateTime << endl;
    for (const auto &item : soldProducts)
    {
        Product *product = item.first;
        int quantitySold = item.second;
        product->display();
        cout << "Quantity Sold: " << quantitySold << endl;
    }
}

void Invoice::saveInvoice(ofstream &out)
{
    out << "INVOICE|" << customerName << "|" << customerPhone << "|" << dateTime << endl;

    double totalAmount = 0;

    for (const auto &item : soldProducts)
    {
        Product *product = item.first;
        int quantitySold = item.second;

        if (dynamic_cast<Book *>(product)) // kiểm tra xem product có phải Book hay không
        {
            out << "BOOK|";
        }
        else if (dynamic_cast<MusicDisc *>(product))
        {
            out << "MUSIC|";
        }

        out << product->getId() << "|" << product->getTitle() << "|"
            << product->getSalePrice() << "|" << quantitySold << endl;
        totalAmount += product->getSalePrice() * quantitySold;
    }
    out << "TOTAL|" << totalAmount << endl;
}

void Invoice::display() const
{
    cout << "Invoice: " << customerName << ", " << customerPhone << ", Date: " << dateTime << endl;
    for (const auto &item : soldProducts)
    {
        Product *product = item.first;
        int quantitySold = item.second;
        product->display();
        cout << "Quantity Sold: " << quantitySold << endl;
    }
}

double Invoice::getTotalAmount()
{
    totalAmount = 0;
    for (auto &item : soldProducts)
    {
        totalAmount += item.first->getSalePrice() * item.second;
    }
    return totalAmount;
}

void processSale(vector<Product *> &products)
{
    string customerName, customerPhone, productId;
    int quantitySold;
    vector<pair<Product *, int>> soldProducts;

    cout << "Enter customer name: ";
    cin.ignore();
    getline(cin, customerName);
    cout << "Enter customer phone: ";
    getline(cin, customerPhone);

    char moreProducts;
    do
    {
        cout << "Enter product ID to sell: ";
        cin >> productId;
        Product *product = searchProductById(products, productId);
        if (product)
        {
            cout << "Enter quantity to sell: ";
            cin >> quantitySold;
            if (quantitySold <= product->getQuantity())
            {
                product->updateQuantity(quantitySold); // Update the quantity of product
                soldProducts.push_back(make_pair(product, quantitySold));
                cout << "Product sold.\n";
            }
            else
            {
                cout << "Insufficient stock.\n";
            }
        }
        else
        {
            cout << "Product not found.\n";
        }

        cout << "Do you want to sell another product (y/n)? ";
        cin >> moreProducts;
    } while (moreProducts == 'y' || moreProducts == 'Y');
    if (!soldProducts.empty())
    {
        Invoice invoice(customerName, customerPhone, soldProducts);
        invoice.generateInvoice();

        cout << "Total amount: " << invoice.getTotalAmount() << endl;

        // Save invoice to file
        ofstream invoiceFile("invoice.txt", ios::app);
        if (!invoiceFile.is_open())
        {
            cout << "Failed to open invoice file.\n";
            return;
        }

        invoiceFile << endl;

        invoice.saveInvoice(invoiceFile);
        cout << "Invoice saved.\n";
    }
    else
    {
        cout << "No products sold. No invoice created.\n";
    }

    // Immediately update the product file after a sale
    updateProductFile(products);

    cout << "Invoice saved and product file updated.\n";
}


void readAndDisplayAllInvoices(const string &fileName)
{
    ifstream invoiceFile(fileName);
    if (!invoiceFile.is_open())
    {
        cout << "Failed to open invoice file.\n";
        return;
    }

    string line;
    while (getline(invoiceFile, line))
    {
        if (line.rfind("INVOICE|", 0) == 0) // Kiểm tra dòng bắt đầu với "INVOICE"
        {
            stringstream ss(line);
            string token, customerName, customerPhone, dateTimeStr;

            // Đọc thông tin khách hàng và thời gian
            getline(ss, token, '|'); // INVOICE
            getline(ss, customerName, '|');
            getline(ss, customerPhone, '|');
            getline(ss, dateTimeStr, '|');

            // In ra thông tin hóa đơn
            cout << "INVOICE\n";
            cout << "Customer: " << customerName << " | Phone: " << customerPhone << " | Date: " << dateTimeStr << endl;
            cout << "----------------------------------\n";

            // Đọc các sản phẩm và in ra chi tiết
            while (getline(invoiceFile, line))
            {
                if (line.rfind("TOTAL", 0) == 0) // Kiểm tra dòng bắt đầu với "TOTAL"
                {
                    string newLine;
                    size_t pos = line.find('|');
                    if (pos != string::npos)
                    {
                        newLine = line.substr(pos + 1);
                    }

                    float totalAmount = stof(newLine);

                    cout << "TOTAL: " << totalAmount << "\n";

                    break; // Dừng khi gặp dòng "TOTAL"
                }

                stringstream itemStream(line);
                string type, id, name;
                float price;
                int quantity;

                // Đọc thông tin sản phẩm
                getline(itemStream, type, '|');
                getline(itemStream, id, '|');
                getline(itemStream, name, '|');
                itemStream >> price;
                itemStream.ignore(); // Bỏ qua '|'
                itemStream >> quantity;

                // In ra thông tin sản phẩm
                cout << type << " | ID: " << id << " | "
                     << "Title: " << name
                     << " | Price: " << price
                     << " | Quantity: " << quantity
                     << " | Subtotal: " << price * quantity << endl;
            }
            cout << "==================================\n\n";
        }
    }

    invoiceFile.close();
}



// Hàm chuyển đổi chuỗi thời gian thành tm
tm stringToTime(const string &timeStr)
{
    tm t = {};
    stringstream ss(timeStr);
    ss >> get_time(&t, "%Y-%m-%d %H:%M:%S"); // Chuyển chuỗi thành tm
    return t;
}

// Hàm so sánh hai mốc thời gian
bool isInTimeRange(const tm &startTime, const tm &endTime, const tm &invoiceTime)
{
    // So sánh mốc thời gian
    time_t start = mktime(const_cast<tm *>(&startTime)); // Chuyển từ tm sang time_t
    time_t end = mktime(const_cast<tm *>(&endTime));
    time_t invoice = mktime(const_cast<tm *>(&invoiceTime));

    return (difftime(invoice, start) >= 0) && (difftime(end, invoice) >= 0); // So sánh mốc thời gian
}

void readInvoices(const string &fileName, const vector<Product *> &products)
{
    ifstream invoiceFile(fileName);
    if (!invoiceFile.is_open())
    {
        cout << "Failed to open invoice file.\n";
        return;
    }

    // Nhập thời gian bắt đầu và kết thúc
    string startTimeStr, endTimeStr;
    regex timeRegex(R"(^(?:\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]) (?:[01]\d|2[0-3]):[0-5]\d:[0-5]\d$)");

    cin.ignore();
    do
    {
        cout << "Enter start time (YYYY-MM-DD HH:MM:SS): ";
        getline(cin, startTimeStr);
        if (!regex_match(startTimeStr, timeRegex))
            cout << "Invalid time format!\n";
    } while (!regex_match(startTimeStr, timeRegex));
    
    do
    {
        cout << "Enter end time (YYYY-MM-DD HH:MM:SS): ";
        getline(cin, endTimeStr);
        if (!regex_match(endTimeStr, timeRegex))
            cout << "Invalid time format!\n";
    } while (!regex_match(endTimeStr, timeRegex));

    tm startTime = stringToTime(startTimeStr);
    tm endTime = stringToTime(endTimeStr);

    string line;
    float totalProfit = 0.0f;       // Lợi nhuận tổng
    float totalRevenue = 0.0f; // Doanh thu tổng

    while (getline(invoiceFile, line))
    {
        if (line.rfind("INVOICE|", 0) == 0)
        { // Kiểm tra dòng bắt đầu với "INVOICE"
            stringstream ss(line);
            string token, customerName, customerPhone, dateTimeStr;

            // Đọc thông tin khách hàng và thời gian
            getline(ss, token, '|'); // INVOICE
            getline(ss, customerName, '|');
            getline(ss, customerPhone, '|');
            getline(ss, dateTimeStr, '|');

            // Chuyển thời gian từ chuỗi sang dạng tm để so sánh
            tm invoiceTime = stringToTime(dateTimeStr);

            // Kiểm tra xem thời gian hóa đơn có nằm trong khoảng đã nhập không
            if (isInTimeRange(startTime, endTime, invoiceTime))
            {

                // In ra thông tin hóa đơn
                cout << "INVOICE\n";
                cout << "Customer: " << customerName << " | Phone: " << customerPhone << " | Date: " << dateTimeStr << endl;
                cout << "----------------------------------\n";

                float subProfit=0.0f;
                // Đọc các sản phẩm và in ra chi tiết
                while (getline(invoiceFile, line))
                {
                    stringstream itemStream(line);
                    string type, id, name;
                    float salePrice;
                    int quantity;
                    float importPrice;

                    if (line.rfind("TOTAL", 0) == 0)
                    { // Kiểm tra dòng bắt đầu với "TOTAL"
                        string newLine;
                        size_t pos = line.find('|');
                        if (pos != string::npos)
                        {
                            newLine = line.substr(pos + 1);
                        }

                        float totalAmount = stof(newLine);
                        cout << "TOTAL: " << totalAmount << "\n";
                        cout << "SUBPROFIT: " << subProfit << "\n";
                        totalRevenue += totalAmount; // Cộng tổng doanh thu

                        break; // Dừng khi gặp dòng "TOTAL"
                    }

                    // Đọc thông tin sản phẩm
                    getline(itemStream, type, '|');
                    getline(itemStream, id, '|');
                    getline(itemStream, name, '|');
                    itemStream >> salePrice;
                    itemStream.ignore(); // Bỏ qua '|'
                    itemStream >> quantity;
                    for (auto *p : products)
                    {
                        if (p->getId() == id)
                        {
                            importPrice = p->getImportPrice();
                            break;
                        }
                    }
                    subProfit += (salePrice - importPrice) * quantity;
                    totalProfit += (salePrice - importPrice) * quantity;

                    // In ra thông tin sản phẩm
                    cout << type << " | ID: " << id << " | "
                         << "Title: " << name
                         << " | Price: " << salePrice
                         << " | Quantity: " << quantity
                         << " | Subtotal: " << salePrice * quantity
                         << " | Profit: " << (salePrice - importPrice) * quantity << endl; //
                }
                cout << "==================================\n\n";
            }
            else
            {
                // Bỏ qua hóa đơn không nằm trong khoảng thời gian
                while (getline(invoiceFile, line))
                {
                    if (line.rfind("TOTAL", 0) == 0)
                        break;
                }
            }
        }
    }

    cout << "Total REVENUE in the selected time range: " << totalRevenue << "\n";
    cout << "Total PROFIT in the selected time range: " << totalProfit << "\n";

    invoiceFile.close();
}
