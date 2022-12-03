
/*
    Displays the main menu for the program
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

void repListing();
void repWholesale();
void repRetail();
void repCost();
void repAge();
void repQty();
int cashier();
int reports();

int invMenu();

void lookUpBook();
void addBook();
void editBook();
void deleteBook();

void fileDataToClass(int, string);

void findAndChangeMember(string, string, int);

int choice;
string lookup_str;

const string inventoryFileName = "Inventory-Database.txt";

fstream invFile(inventoryFileName, ios::out | ios::in);

const int bookSize = 20;

class BookData
{
protected:
    char bookTitle[51] = {},
         isbn[14] = {},
         author[31] = {},
         publisher[31] = {},
         dateAdded[11] = {};

public:
    void setTitle(char newTitle[]);
    void setISBN(char newISBN[]);
    void setAuthor(char newAuthor[]);
    void setPub(char newPub[]);

    char *getTitle();
    char *getISBN();
    char *getAuthor();
    char *getPub();
    char *getDateAdded();

    bool bookMatch(char[]);
};

class InventoryBook : public BookData
{

protected:
    int qtyOnHand;
    double wholesale,
        retail;

public:
    void setDateAdded(char newDate[]);
    void setQty(int newQty);
    void setWholesale(double newWhole);
    void setRetail(double newRetail);
    int isEmpty();
    void removeBook();
    void delBook();
    int getQty();
    double getWholesale();
    double getRetail();
};

class SoldBook : public InventoryBook
{
private:
    const double taxRate = 0.06;
    int qtySold;
    double tax = 0, subtotal = 0;
    static double total;

public:
    SoldBook calcTax()
    {
        this->tax = qtySold * retail * taxRate;
        return *this;
    }

    SoldBook calcSubtotal()
    {
        subtotal = (this->retail * qtySold) + tax;
        this->total += subtotal;

        return *this;
    }

    SoldBook calcTotal()
    {
        total += this->subtotal;
        return *this;
    }

    void setQtySold(int qty)
    {
        this->qtySold = qty;
    }

    int getQtySold()
    {
        return qtySold;
    }

    static double getTotal()
    {
        return total;
    }

    SoldBook setBook(InventoryBook book)
    {
        strcpy(this->bookTitle, book.getTitle());
        strcpy(this->isbn, book.getISBN());
        strcpy(this->author, book.getAuthor());
        strcpy(this->publisher, book.getPub());
        strcpy(this->dateAdded, book.getDateAdded());
        this->retail = book.getRetail();

        return *this;
    }
};

/**
 * saleBooks[purchaseBooks].setRetail(allBooks[isbnIndex].getRetail)
 * saleBooks[purchaseBooks].calcTax().calcSubtotal()
 *
 */

class Menus
{
private:
    /* data */
public:
    Menus();
    ~Menus();
    void displayMainMenu();
};

Menus::Menus(/* args */) {}

Menus::~Menus() {}

void Menus::displayMainMenu()
{
    do
    {
        cout << "\tSerendipity Booksellers" << endl;
        cout << "\t\tMain Menu" << endl
             << endl;
        cout << "\t1. Cashier Module" << endl;
        cout << "\t2. Inventory Database Module" << endl;
        cout << "\t3. Report Module" << endl;
        cout << "\t4. Exit" << endl
             << endl;

        cout << "\tEnter Your Choice: ";
        cin >> choice;

        /* Loops if number isn't valid */
        while (choice < 1 || choice > 4)
        {
            cout << "\n\n\tPlease enter a number in the range of 1 - 4" << endl
                 << endl
                 << "\tEnter Your Choice: ";
            cin >> choice;
        }

        switch (choice)
        {
        case 1:
            cashier();
            break;
        case 2:
            invMenu();
            break;
        case 3:
            reports();
            break;
        case 4:
            cout << "\n\tYou selected item " << choice << '.' << endl
                 << "\tGoodbye!\n\n";
            break;
        default:
            break;
        }
    } while (choice != 4);
}

InventoryBook allBooks[bookSize];

Menus allMenus;

double SoldBook::total = 0;

int currQty[bookSize] = {};

int main()
{
    int choice;

    int numOfLines = 0;

    string tempLines;

    while (getline(invFile, tempLines))
        ++numOfLines;

    /**
     * * Checks if there are already any lines, if not
     * * create 20 null lines
     * ! else
     * * fill in the struct with values from file
     */

    if (numOfLines != bookSize)
    {
        // cout << numOfLines << endl;
        invFile.close();
        invFile.open(inventoryFileName, ios::trunc | ios::out);
        // invFile.seekg(invFile.beg);
        for (int i = 0; i < bookSize - 1; i++)
        {
            invFile << "\n";
        }
        invFile.close();
        invFile.open(inventoryFileName, ios::out | ios::in);
    }
    else
    {
        string currLine;
        invFile.close();
        invFile.open(inventoryFileName, ios::out | ios::in);
        for (int i = 0; i < size(allBooks) - 1; i++)
        {
            getline(invFile, currLine);
            if (currLine != "\0" || !currLine.empty())
            {
                fileDataToClass(i, currLine); // writes values to file
            }
        }
    }

    /* Runs the code initially then loops if invalid input */
    allMenus.displayMainMenu();

    return 0;
}

string charToString(char *str)
{
    string s = (str);

    return s;
}

void strUpper(char *str)
{
    while (*str != 0)
    {
        *str = toupper(*str);
        str++;
    }
}

// displays book info
int bookInfo(char isbn[14], char title[51], char author[31], char publisher[31],
             char date[11], int qty, double wholesale, double retail)
{
    cout << "\t\tSerendipity Booksellers" << endl
         << "\t\t    Book Information" << endl
         << endl
         << "1. ISBN: " << isbn << endl
         << "2. Title: " << title << endl
         << "3. Author: " << author << endl
         << "4. Publisher: " << publisher << endl
         << "5. Date Added: " << date << endl
         << "6. Quantity-On-Hand: " << qty << endl
         << "7. Wholesale Cost: " << setprecision(2) << fixed << wholesale << endl
         << "8. Retail Price:  " << retail << endl;

    return 0;
}

// Avoids redudant code for inputing values
void inputValues(int index)
{
    char tempBookTitle[51] = {},
         tempIsbn[14] = {},
         tempAuthor[31] = {},
         tempPublisher[31] = {},
         tempDateAdded[11] = {};
    int tempQty = 0;

    double tempWholesale = 0,
           tempRetail = 0;

    cin.sync();
    cout << endl
         << "\tEnter Title: ";
    cin.get(tempBookTitle, 51);
    strUpper(tempBookTitle);
    allBooks[index].setTitle(tempBookTitle);
    cin.sync();

    cout << "\tEnter ISBN: ";
    cin.get(tempIsbn, 14);
    strUpper(tempIsbn);
    allBooks[index].setISBN(tempIsbn);
    cin.sync();

    cout << "\tEnter Author's Name: ";
    cin.get(tempAuthor, 31);
    strUpper(tempAuthor);
    allBooks[index].setAuthor(tempAuthor);
    cin.sync();

    cout << "\tEnter Publisher's Name: ";
    cin.get(tempPublisher, 31);
    strUpper(tempPublisher);
    allBooks[index].setPub(tempPublisher);
    cin.sync();

    cout << "\tEnter Date: ";
    cin.get(tempDateAdded, 11);
    allBooks[index].setDateAdded(tempDateAdded);
    cin.sync();

    cout << "\tEnter Quantity of Book: ";
    cin >> tempQty;
    allBooks[index].setQty(tempQty);
    cin.sync();

    cout << "\tEnter Wholesale Price: ";
    cin >> tempWholesale;
    allBooks[index].setWholesale(tempWholesale);
    cin.sync();

    cout << "\tEnter Retail Price: ";
    cin >> tempRetail;
    allBooks[index].setRetail(tempRetail);
    cin.sync();

    cout << "\tBook Added" << endl
         << endl;
}

int invMenu()
{

    /* Runs the code initially then loops if invalid input */
    do
    {
        cout << endl
             << "\t Serendipity Booksellers " << endl
             << "\t      Inventory Database " << endl
             << endl
             << "\t1. Look Up a Book" << endl
             << "\t2. Add a Book" << endl
             << "\t3. Edit a Book's Record" << endl
             << "\t4. Delete a Book" << endl
             << "\t5. Return to the Main Menu" << endl
             << endl;

        cout << "\tInventory Menu\n\tEnter Your Choice: ";
        cin >> choice;

        /* Loops if number isn't valid */
        while (choice < 1 || choice > 5)
        {
            cout << "\n\n\tPlease enter a number in the range of 1 - 5" << endl
                 << endl
                 << "\tInventory Menu\n\tEnter Your Choice: ";

            cin >> choice;
        }

        switch (choice)
        {
        case 1:
            lookUpBook();
            break;
        case 2:
            addBook();
            break;
        case 3:
            editBook();
            break;
        case 4:
            deleteBook();
            break;
        case 5:
            cout << "\n\tYou selected item " << choice << '.' << endl
                 << "\t\tRedirecting back to main menu....\n\n";
            break;
        default:
            break;
        }
    } while (choice != 5);

    return 0;
}

void fileDataToClass(int index, string str)
{
    const char *tempStr = str.c_str();
    char *newStr = new char[strlen(tempStr) + 1];
    strcpy(newStr, tempStr);

    char *token = strtok(newStr, ";");

    while (token != NULL)
    {
        string currToken(token),
            initStr = currToken.substr(0, currToken.find("=")),
            tokenVal = currToken.substr(currToken.find("=") + 1, size(currToken));

        findAndChangeMember(initStr, tokenVal, index); // edits the values of book as tokens

        token = strtok(NULL, ";");
    }
}

void findAndChangeMember(string str, string newVal, int index)
{
    const char *tempStr = newVal.c_str();
    char *newStr = new char[strlen(tempStr) + 1];
    strcpy(newStr, tempStr);

    if (str == "booktitle")
    {
        allBooks[index].setTitle(newStr);
    }
    else if (str == "isbn")
    {
        allBooks[index].setISBN(newStr);
    }
    else if (str == "date")
    {
        allBooks[index].setDateAdded(newStr);
    }
    else if (str == "author")
    {
        allBooks[index].setAuthor(newStr);
    }
    else if (str == "publisher")
    {
        allBooks[index].setPub(newStr);
    }
    else if (str == "quantity")
    {
        allBooks[index].setQty(stoi(newStr));
    }
    else if (str == "wholesale")
    {
        allBooks[index].setWholesale(stod(newStr));
    }
    else if (str == "retail")
    {
        allBooks[index].setRetail(stoi(newStr));
    }
}

void addBook()
{
    /*
        1.  Read through file ✓
        2.  Write existing records to struct ✓
        3.  When found empty spot in struct, ask for user input (Book Data)
            and write it in struct member ✓
        4.  Write the new data over old file data ✓
     */
    int arrIndex = -1;
    string currLine;

    /**
     * Writing to the STRUCT from FILE
     **/
    invFile.close();
    invFile.open(inventoryFileName, ios::out | ios::in);

    for (int i = 0; i < size(allBooks); i++)
    {
        if (allBooks[i].isEmpty())
        {
            arrIndex = i;
            break;
        }
    }

    if (arrIndex >= 0)
    {
        inputValues(arrIndex);
        /*
            Loop through struct, write values in file in format
         */
        invFile.seekg(invFile.beg);
        for (int i = 0; i < size(allBooks); i++)
        {
            invFile << "booktitle=" << allBooks[i].getTitle()
                    << ";isbn=" << allBooks[i].getISBN()
                    << ";date=" << allBooks[i].getDateAdded()
                    << ";author=" << allBooks[i].getAuthor()
                    << ";publisher=" << allBooks[i].getPub()
                    << ";quantity=" << allBooks[i].getQty()
                    << ";wholesale=" << setprecision(2) << fixed << allBooks[i].getWholesale()
                    << ";retail=" << allBooks[i].getRetail() << ";" << (i < 19 ? "\n" : "");
        }
        invFile.close();
    }
    else
    {
        cout << "\tNo Inventory Space!" << endl;
        exit(0);
    }
}

void lookUpBook()
{
    char bookName[52] = {},
         answer;
    cout << endl
         << "\tEnter name of the book name: ";
    cin.sync();
    cin.get(bookName, 52);
    strUpper(bookName);

    for (InventoryBook books : allBooks)
    {
        if (books.bookMatch(bookName))
        {
            cin.sync();
            cout << "\n\tIs " << bookName << " the book you're looking for?  (y/n)" << endl;
            cin.get(answer);
            cin.sync();

            if (answer == 'y')
            {
                bookInfo(books.getISBN(), books.getTitle(), books.getAuthor(),
                         books.getPub(), books.getDateAdded(), books.getQty(),
                         books.getWholesale(), books.getRetail());
                exit(0);
            }
            else
            {
                continue;
            }
        }
    }

    invFile.close();
    cout << "\tNo Book Found" << endl;
    exit(0);
}

void editBook()
{
    int choice, newIntVal;
    double newDoubleVal;

    char bookName[52] = {},
         answer;

    cout << "\n\tEnter name of the book name: ";
    cin.sync();
    cin.get(bookName, 52);
    strUpper(bookName);

    for (InventoryBook books : allBooks)
    {
        if (books.bookMatch(bookName))
        {
            cin.sync();
            cout << "\n\tIs " << bookName << " the book you're looking for?  (y/n)" << endl;
            cin.get(answer);
            cin.sync();

            if (answer == 'y')
            {
                bookInfo(books.getISBN(), books.getTitle(), books.getAuthor(),
                         books.getPub(), books.getDateAdded(), books.getQty(),
                         books.getWholesale(), books.getRetail());
                cout << "\tWhat do you want to change? ";
                cin >> choice;

                char tempBookTitle[51] = {},
                     tempIsbn[14] = {},
                     tempAuthor[31] = {},
                     tempPublisher[31] = {},
                     tempDateAdded[11] = {};
                int tempQty = 0;

                double tempWholesale = 0,
                       tempRetail = 0;

                switch (choice)
                {
                case 1:
                    cout << "\tEnter new ISBN: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(tempIsbn, 14);
                    books.setISBN(tempIsbn);

                    break;
                case 2:
                    cout << "\tEnter new Title: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(tempBookTitle, 51);
                    books.setTitle(tempBookTitle);
                    break;
                case 3:
                    cout << "\tEnter new Author: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(tempAuthor, 31);
                    books.setAuthor(tempAuthor);
                    break;
                case 4:
                    cout << "\tEnter new Publisher: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(tempPublisher, 31);
                    books.setPub(tempPublisher);
                    break;
                case 5:
                    cout << "\tEnter new Date: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(tempDateAdded, 11);
                    books.setDateAdded(tempDateAdded);
                    break;
                case 6:
                    cout << "\tEnter new Quantity: ";
                    cin.ignore();
                    cin.sync();
                    (cin >> newIntVal);
                    books.setQty(newIntVal);
                    break;
                case 7:
                    cout << "\tEnter new Wholesale Cost: ";
                    cin.ignore();
                    cin.sync();
                    (cin >> newDoubleVal);
                    books.setWholesale(newDoubleVal);
                    break;
                case 8:
                    cout << "\tEnter new Retail Price: ";
                    cin.ignore();
                    cin.sync();
                    (cin >> newDoubleVal);
                    books.setRetail(newDoubleVal);
                    break;

                default:
                    cout << "\tInvalid Value" << endl;
                    cout << "\tGoodbye!" << endl;
                    exit(0);
                    break;
                }

                if (invFile.is_open())
                    invFile.close();

                invFile.open(inventoryFileName, ios::out);

                for (int i = 0; i < bookSize; i++)
                {
                    invFile << "booktitle=" << books.getTitle()
                            << ";isbn=" << books.getISBN()
                            << ";date=" << books.getDateAdded()
                            << ";author=" << books.getAuthor()
                            << ";publisher=" << books.getPub()
                            << ";quantity=" << books.getQty()
                            << ";wholesale=" << setprecision(2) << fixed << books.getWholesale()
                            << ";retail=" << books.getRetail() << ";" << (i < 19 ? "\n" : "");
                }

                bookInfo(books.getISBN(), books.getTitle(), books.getAuthor(),
                         books.getPub(), books.getDateAdded(), books.getQty(),
                         books.getWholesale(), books.getRetail());
                cout << "\tGoodbye!" << endl;
                invFile.close();
                exit(0);
            }
            else
            {
                continue;
            }
        }
    }
    cout << "\n\tBook Not Found" << endl;
}

void deleteBook()
{
    char bookName[52],
        answer;

    cout << endl
         << "\tEnter name of the book name: ";
    cin.sync();
    cin.get(bookName, 52);
    strUpper(bookName);

    for (InventoryBook books : allBooks)
    {
        if (books.bookMatch(bookName))
        {
            cin.sync();
            cout << "\n\tIs " << books.getTitle() << " the book you're looking for?  (y/n)" << endl;
            cin.get(answer);
            cin.sync();

            char deleteArr[] = "";

            if (answer == 'y')
            {
                books.delBook();

                if (invFile.is_open())
                    invFile.close();

                invFile.open(inventoryFileName, ios::out);
                invFile.seekg(invFile.beg);

                for (int i = 0; i < bookSize; i++)
                {
                    invFile << "booktitle=" << books.getTitle()
                            << ";isbn=" << books.getISBN()
                            << ";date=" << books.getDateAdded()
                            << ";author=" << books.getAuthor()
                            << ";publisher=" << books.getPub()
                            << ";quantity=" << books.getQty()
                            << ";wholesale=" << setprecision(2) << fixed << books.getWholesale()
                            << ";retail=" << books.getRetail() << ";" << (i < 19 ? "\n" : "");
                }

                cout << "\tDeleted" << endl;
                invFile.close();
                exit(0);
            }
            else
            {
                continue;
            }
        }
    }

    cout << "\tNo book Found." << endl
         << "\tGoodbye!" << endl;
    exit(0);
}

/* function for displaying items for effeciency */
void displayItem(int qty, string isbn, string title, double price)
{
    cout << qty << setw(20) << isbn << setw(26) << title << setw(12) << '$' << setprecision(2) << fixed << setw(6) << price << setw(7) << '$' << setprecision(2) << fixed << setw(6) << (double)qty * price << endl;
}

int cashier()
{
    /* Runs the code initially then loops user wants to enter another item */

    char repeat = 'y',
         isbnRepeat;

    int arrIndex = 0,
        isbnIndex = -1;
    string currDate;
    char isbnLookup[14] = {};

    int qtySale,
        booksInInventory,
        booksPurchasing;

    double subtotal = 0;

    do
    {
        cin.sync();
        cout << "\n\tHow many titles are you purchasing? ";
        cin >> (booksPurchasing);
        cin.sync();

    } while (booksPurchasing < 0 || booksPurchasing > 20);

    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    cout << "\nSerendipity Booksellers\n  Cashier Module" << endl
         << endl;

    cin.sync();

    cout << "Date: ";
    getline(cin, currDate);
    cin.sync();

    SoldBook *saleBooks = new SoldBook[booksPurchasing];

    do
    {
        double price, tax, total, subtotal = 0;
        bool isbnFound = false;

        char isbnUse;

        cin.sync();
        cout << "Quantity of Book: ";
        cin >> qtySale;
        saleBooks[arrIndex].setQtySold(qtySale);
        cin.sync();

        do
        {
            cin.sync();
            cout << "ISBN: ";
            cin.get(isbnLookup, 14);
            cin.sync();

            strUpper(isbnLookup);

            for (int i = 0; i < booksInInventory; i++)
            {
                if (strstr(allBooks[i].getISBN(), isbnLookup))
                {
                    isbnIndex = i;
                    isbnFound = true;
                    break;
                }
            }
            if (isbnFound)
            {
                cin.sync();
                cout << "Book found with ISBN titled: " << allBooks[isbnIndex].getTitle()
                     << "\nWould you like to use this book? (y/n) ";

                cin.sync();
                cin.get(isbnUse);
                cin.sync();
                isbnRepeat = 'n';
            }
            else
            {
                cout << "ISBN not recognized: "
                     << "Would you like to try again? (y/n) ";
                cin.sync();
                cin.get(isbnRepeat);
                cin.sync();
            }
        } while (tolower(isbnRepeat) == 'y');

        if (isbnIndex != -1 && allBooks[isbnIndex].getQty() < 1)
        {
            cout << "\tInsufficient quantity of books" << endl;
            repeat = 'n';
            break;
        }
        else if (isbnIndex != -1)
        {
            /**
             * TODO: Move to the end of the function
             */

            allBooks[isbnIndex].setQty(allBooks[isbnIndex].getQty() - qtySale);
        }
        else
        {
            cout << "\tNo Book Found" << endl;
            exit(0);
        }

        saleBooks[arrIndex].setBook(allBooks[isbnIndex]);
        saleBooks[arrIndex].calcTax().calcSubtotal();

        cout << "\nSerendipity Book Sellers" << endl
             << endl
             << "Date: "
             << currDate << endl
             << endl
             << "Qty" << setw(10) << setfill(' ') << "ISBN" << setw(20) << setfill(' ') << "Title" << setw(30) << setfill(' ') << "Price" << setw(13) << setfill(' ') << "Total" << endl
             << string(80, '_') << endl
             << endl;

        for (int i = 0; i < arrIndex + 1; i++)
        {
            displayItem(saleBooks[i].getQtySold(), charToString(saleBooks[i].getISBN()), charToString(saleBooks[i].getTitle()), saleBooks[i].getRetail());
        }

        arrIndex++;
        cout << (booksPurchasing - 1 > 0 ? "\n\n\tPress enter for next book entry..." : "\n\n\tLast book entered\n\n\tPress enter to display total.") << endl;
        cin.get();
        fflush(stdin);
    } while (--booksPurchasing > 0);

    cout << "\nSerendipity Book Sellers" << endl
         << endl
         << "Date: "
         << currDate << endl
         << endl
         << "Qty" << setw(10) << setfill(' ') << "ISBN" << setw(20) << setfill(' ') << "Title" << setw(30) << setfill(' ') << "Price" << setw(13) << setfill(' ') << "Total" << endl
         << string(80, '_') << endl
         << endl;

    for (int i = 0; i < arrIndex; i++)
    {
        displayItem(saleBooks[i].getQtySold(), charToString(saleBooks[i].getISBN()), charToString(saleBooks[i].getTitle()), saleBooks[i].getRetail());
        subtotal += saleBooks[i].getRetail();
    }

    cout << endl
         << setw(10) << left << "\tSubtotal" << setw(55) << right << '$' << setw(6) << right << setprecision(2) << fixed << subtotal << endl
         << setw(10) << left << "\tTax" << setw(55) << right << '$' << setw(6) << right << setprecision(2) << fixed << (subtotal * 0.06) << endl
         << setw(10) << left << "\tTotal" << setw(55) << right << '$' << setw(6) << right << setprecision(2) << fixed << saleBooks[0].getTotal() << endl
         << endl;

    invFile.close();

    invFile.open(inventoryFileName, ios::out | ios::in);

    invFile.seekg(invFile.beg);
    for (int i = 0; i < size(allBooks); i++)
    {
        invFile << "booktitle=" << allBooks[i].getTitle()
                << ";isbn=" << allBooks[i].getISBN()
                << ";date=" << allBooks[i].getDateAdded()
                << ";author=" << allBooks[i].getAuthor()
                << ";publisher=" << allBooks[i].getPub()
                << ";quantity=" << allBooks[i].getQty()
                << ";wholesale=" << setprecision(2) << fixed << allBooks[i].getWholesale()
                << ";retail=" << allBooks[i].getRetail() << ";" << (i < 19 ? "\n" : "");
    }

    invFile.close();

    cout << "\nThank You for Shopping at Serendipity!\n\n";
    cout << "\nRedirecting back to main menu.....\n\n";

    return 0;
}

int reports()
{
    int choice;

    /* Runs the code initially then loops if invalid input */
    do
    {
        cout << endl
             << "\t\tSerendipity Booksellers" << endl
             << "\t\t    Reports" << endl
             << endl
             << "\t\t1. Inventory Listing " << endl
             << "\t\t2. Inventory Wholesale Value " << endl
             << "\t\t3. Inventory Retail Value " << endl
             << "\t\t4. Listing by Quantity " << endl
             << "\t\t5. Listing by Cost " << endl
             << "\t\t6. Listing by Age " << endl
             << "\t\t7. Return to Main Menu " << endl
             << endl;
        cout << "\t\tEnter Your Choice: ";
        cin >> choice;

        /* Loops if number isn't valid */
        while (choice < 1 || choice > 7)
        {
            cout << "\n\n\t\tPlease enter a number in the range of 1 - 7" << endl
                 << endl
                 << "\t\tEnter Your Choice: ";
            cin >> choice;
        }

        switch (choice)
        {
        case 1:
            repListing();
            break;
        case 2:
            repWholesale();
            break;
        case 3:
            repRetail();
            break;
        case 4:
            repQty();
            break;
        case 5:
            repCost();
            break;
        case 6:
            repAge();
            break;
        case 7:
            cout << "\n\t\tYou selected item " << choice << '.' << endl
                 << "\t\tRedirecting back to main menu....\n\n";
            break;
        default:
            break;
        }
    } while (choice != 7);

    return 0;
}

void printListings(bool title, bool isbn, bool qtyBool, bool wholesaleBool, bool retailBool, bool date, bool auth, bool pub)
{
    int booksInInventory;
    string tempVal;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    for (int i = 0; i < booksInInventory; i++)
    {

        string tempTitle = title ? "\tTitle: " + charToString((allBooks[i].getTitle())) + "\n" : "",
               tempIsbn = isbn ? "\tISBN: " + charToString((allBooks[i].getISBN())) + "\n" : "",
               tempDate = date ? "\tDate: " + charToString(allBooks[i].getDateAdded()) + "\n" : "",
               tempAuthor = auth ? "\tAuthor: " + charToString((allBooks[i].getAuthor())) + "\n" : "",
               tempPubl = pub ? "\tPublisher: " + charToString((allBooks[i].getPub())) + "\n" : "",
               tempQty = qtyBool ? "\tQuantity: " + to_string(allBooks[i].getQty()) + "\n" : "",
               tempWhole = wholesaleBool ? "\tWholesale Price: " + to_string(allBooks[i].getWholesale()).substr(0, to_string(allBooks[i].getWholesale()).find(".") + 3) + "\n" : "",
               tempRetail = retailBool ? "\tRetail Price: " + to_string(allBooks[i].getRetail()).substr(0, to_string(allBooks[i].getRetail()).find(".") + 3) + "\n" : "";

        cout << tempTitle
             << tempIsbn
             << tempDate
             << tempAuthor
             << tempPubl
             << tempQty
             << tempWhole
             << tempRetail << endl;
    }
}

void repListing()
{
    printListings(true, true, true, true, true, true, true, true);
    system("pause");
}

void repWholesale()
{
    double totalWholesale = 0;
    printListings(true, true, true, true, false, false, false, false);
    for (int i = 0; i < bookSize; i++)
    {
        totalWholesale += allBooks[i].getWholesale() * (double)allBooks[i].getQty();
    }
    cout << "Total wholesale cost: " << setprecision(2) << fixed << totalWholesale << endl;
    system("pause");

}

void repRetail()
{
    double totalRetail = 0;
    printListings(true, true, true, false, true, false, false, false);
    for (int i = 0; i < bookSize; i++)
    {
        totalRetail += allBooks[i].getRetail() * (double)allBooks[i].getQty();
    }
    cout << "Total retail cost: " << setprecision(2) << fixed << totalRetail << endl;
    system("pause");
}

void repQty()
{
    int booksInInventory;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    int tempArr[bookSize] = {},
        tempArrIndex[bookSize] = {},
        small,
        temp,
        tempIndex,
        counter = 0;

    for (int i = 0; i < booksInInventory; i++)
    {
        tempArr[i] = allBooks[i].getQty();
        tempArrIndex[i] = i;
    }

    for (int i = 0; i < booksInInventory; i++)
    {
        small = i;
        for (int j = i + 1; j < booksInInventory; j++)
        {
            if (tempArr[j] < tempArr[small])
            {
                small = j;
            }
            temp = tempArr[i];
            tempArr[i] = tempArr[small];
            tempArr[small] = temp;

            tempIndex = tempArrIndex[i];
            tempArrIndex[i] = tempArrIndex[small];
            tempArrIndex[small] = tempIndex;
        }
    }

    for (int i = 0; i < booksInInventory; i++)
    {
        cout << "Title: " << charToString(allBooks[tempArrIndex[i]].getTitle())
             << "\nISBN: " << charToString(allBooks[tempArrIndex[i]].getISBN())
             << "\nQuantity: " << allBooks[tempArrIndex[i]].getQty() << endl
             << endl;
    }
    system("pause");
}

void repCost()
{
    int booksInInventory;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    double tempArr[bookSize] = {},
           temp = 0;

    int tempArrIndex[bookSize] = {},
        small,
        tempIndex,
        counter = 0;

    for (int i = 0; i < booksInInventory; i++)
    {
        tempArr[i] = allBooks[i].getWholesale();
        tempArrIndex[i] = i;
    }

    for (int i = 0; i < booksInInventory; i++)
    {
        small = i;
        for (int j = i + 1; j < booksInInventory; j++)
        {
            if (tempArr[j] < tempArr[small])
            {
                small = j;
            }
            temp = tempArr[i];
            tempArr[i] = tempArr[small];
            tempArr[small] = temp;

            tempIndex = tempArrIndex[i];
            tempArrIndex[i] = tempArrIndex[small];
            tempArrIndex[small] = tempIndex;
        }
    }

    for (int i = booksInInventory - 1; i >= 0; i--)
    {
        cout << "Title: " << charToString(allBooks[tempArrIndex[i]].getTitle())
             << "\nISBN: " << charToString(allBooks[tempArrIndex[i]].getISBN())
             << "\nWholesale Cost: " << allBooks[tempArrIndex[i]].getWholesale() << endl
             << endl;
    }
    cout << "Enter any button to continue...";
    cin.sync();
    cin.get();
    cin.sync();
}

void repAge()
{
    int booksInInventory;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    cin.sync();

    string tempArr[bookSize] = {},
           temp;

    int tempArrIndex[bookSize] = {},
        tempIndex = 0,
        small,
        counter = 0;

    for (int i = 0; i < booksInInventory; i++)
    {
        tempArr[i] = allBooks[i].getDateAdded();
        tempArrIndex[i] = i;
    }

    try
    {

        for (int i = 0; i < booksInInventory; i++)
        {
            small = i;
            for (int j = i + 1; j < booksInInventory; j++)
            {
                string tempDateAtJ = charToString(allBooks[j].getDateAdded());
                string tempDateAtSmall = charToString(allBooks[j].getDateAdded());
                ;
                if (stoi(tempDateAtJ.substr(size(tempDateAtJ) - 4, size(tempDateAtJ))) < stoi(tempDateAtSmall.substr(size(tempDateAtJ) - 4, size(tempDateAtJ))))
                {
                    small = j;
                }
                else if (stoi(tempDateAtJ.substr(0, 2)) < stoi(tempDateAtSmall.substr(0, 2)))
                {
                    small = j;
                }
                else if (stoi(tempDateAtJ.substr(3, 5)) < stoi(tempDateAtSmall.substr(3, 5)))
                {
                    small = j;
                }

                temp = tempArr[i];
                tempArr[i] = tempArr[small];
                tempArr[small] = temp;

                tempIndex = tempArrIndex[i];
                tempArrIndex[i] = tempArrIndex[small];
                tempArrIndex[small] = tempIndex;
            }
        }

        for (int i = 0; i < booksInInventory; i++)
        {
            cout << "Title: " << allBooks[tempArrIndex[i]].getTitle()
                 << "\nISBN: " << allBooks[tempArrIndex[i]].getISBN()
                 << "\nQuantity On Hand: " << allBooks[tempArrIndex[i]].getQty()
                 << "\nDate Added: " << allBooks[tempArrIndex[i]].getDateAdded() << endl
                 << endl;
        }
    }
    catch (const std::exception &e)
    {
        if (strstr(e.what(), "stoi"))
        {
            cerr << "\n\tError: Incorrect Date Formats\n"
                 << endl;
        }
    }
    cout << "Enter any button to continue...";
    cin.sync();
    cin.get();
    cin.sync();
}

void BookData::setTitle(char newTitle[])
{
    strUpper(newTitle);
    strcpy(bookTitle, newTitle);
}

void BookData::setISBN(char newISBN[])
{
    strUpper(newISBN);
    strcpy(isbn, newISBN);
}

void BookData::setAuthor(char newAuthor[])
{
    strUpper(newAuthor);
    strcpy(author, newAuthor);
}

void BookData::setPub(char newPub[])
{
    strUpper(newPub);
    strcpy(publisher, newPub);
}

void InventoryBook::setDateAdded(char newDate[])
{
    strUpper(newDate);
    strcpy(dateAdded, newDate);
}

void InventoryBook::setQty(int newQty)
{
    qtyOnHand = newQty;
}

void InventoryBook::setWholesale(double newWhole)
{
    wholesale = newWhole;
}

void InventoryBook::setRetail(double newRetail)
{
    (retail = newRetail);
}

int InventoryBook::isEmpty()
{
    if (bookTitle[0] == '\0')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void InventoryBook::removeBook()
{
    bookTitle[0] = '\0';
}

char *BookData::getTitle()
{
    return bookTitle;
}
char *BookData::getISBN()
{
    return isbn;
}
char *BookData::getAuthor()
{
    return author;
}
char *BookData::getPub()
{
    return publisher;
}
char *BookData::getDateAdded()
{
    return dateAdded;
}
int InventoryBook::getQty()
{
    return qtyOnHand;
}
double InventoryBook::getWholesale()
{
    return wholesale;
}
double InventoryBook::getRetail()
{
    return retail;
}

bool BookData::bookMatch(char title[])
{
    string bookTitleString(bookTitle),
        titleString(title);
    if (bookTitleString == titleString)
    {
        return true;
    }
    return false;
}

void InventoryBook::delBook()
{
    char deleteArr[] = "";
    strcpy(bookTitle, deleteArr);
    strcpy(isbn, deleteArr);
    strcpy(author, deleteArr);
    strcpy(publisher, deleteArr);
    strcpy(dateAdded, deleteArr);
    qtyOnHand = 0;
    wholesale = 0;
    retail = 0;
}
