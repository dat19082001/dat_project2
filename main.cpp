#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Product.h"
#include "Book.h"
#include "MusicDisc.h"
#include "Invoice.h"
#include <ctime>
#include <iomanip>

using namespace std;

int main()
{
    vector<Product *> products;
    loadProductsFromFile(products);

    // Menu
    int choice;
    do
    {
        cout << "\nMenu:\n";
        cout << "1. Add new product\n";
        cout << "2. Display all products\n";
        cout << "3. Search product\n";
        cout << "4. Edit product\n";
        cout << "5. Delete product\n";
        cout << "6. Process sale\n";
        cout << "7. ShowAllInvoice\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            addNewProduct(products);
            updateProductFile(products); // Update immediately after adding a product
        }
        else if (choice == 2)
        {
            displayProducts(products);
        }
        else if (choice == 3) // SearchProduct
        {
            int searchChoice;
            cout << "Choose search criteria:\n";
            cout << "1. Search by Product ID\n";
            cout << "2. Search by Author\n";
            cout << "3. Search by Title\n";
            cout << "4. Search by Year\n";
            cout << "Enter your choice: ";
            cin >> searchChoice;

            if (searchChoice == 1)
            {
                string id;
                cout << "Enter product ID to search: ";
                cin >> id;
                Product *product = searchProductById(products, id);
                if (product)
                {
                    product->display();
                }
                else
                {
                    cout << "Product not found.\n";
                }
            }
            else if (searchChoice == 2) 
            {
                string author;
                cout << "Enter author name to search: ";
                cin.ignore();
                getline(cin, author);
                vector<Product *> result = searchProductByAuthor(products, author);
                if (!result.empty())
                {
                    cout << "Products by " << author << ":\n";
                    for (auto *product : result)
                    {
                        product->display();
                    }
                }
                else
                {
                    cout << "No products found by author " << author << ".\n";
                }
            }
            else if (searchChoice == 3) // Tìm kiếm theo tựa đề
            {
                string title;
                cout << "Enter title to search: ";
                cin.ignore(); // Đảm bảo không có dấu hiệu còn lại trong buffer
                getline(cin, title);
                vector<Product *> result = searchProductByTitle(products, title);
                if (!result.empty())
                {
                    cout << "Products with title " << title << ":\n";
                    for (auto *product : result)
                    {
                        product->display();
                    }
                }
                else
                {
                    cout << "No products found with title " << title << ".\n";
                }
            }
            else if (searchChoice == 4) // Tìm kiếm theo năm sản xuất
            {
                int year;
                cout << "Enter production year to search: ";
                cin >> year;
                vector<Product *> result = searchProductByYear(products, year);
                if (!result.empty())
                {
                    cout << "Products from year " << year << ":\n";
                    for (auto *product : result)
                    {
                        product->display();
                    }
                }
                else
                {
                    cout << "No products found from year " << year << ".\n";
                }
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
        else if (choice==4)
        {
            editProductById(products);
        }
        else if (choice == 5)
        {
            string productId;
            cout << "Enter product ID to delete: ";
            cin >> productId;
            deleteProductById(products, productId);
        }
        else if (choice == 6)
        {
            processSale(products);
        }
        else if (choice == 7)
        {
            readInvoices("invoice.txt", products);
        }

    } while (choice != 8);

    return 0;
}



