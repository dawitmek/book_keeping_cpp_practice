
/*
    Displays the main menu for the program
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>

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

void fileDataToClass(int, std::string);

void findAndChangeMember(std::string, std::string, int);

int choice;
std::string lookup_str;

const std::string inventoryFileName = "Inventory-Database.txt";

std::fstream invFile(inventoryFileName, std::ios::out | std::ios::in);

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
        std::cout << "\tSerendipity Booksellers" << std::endl;
        std::cout << "\t\tMain Menu" << std::endl
                  << std::endl;
        std::cout << "\t1. Cashier Module" << std::endl;
        std::cout << "\t2. Inventory Database Module" << std::endl;
        std::cout << "\t3. Report Module" << std::endl;
        std::cout << "\t4. Exit" << std::endl
                  << std::endl;

        std::cout << "\tEnter Your Choice: ";
        std::cin >> choice;

        /* Loops if number isn't valid */
        while (choice < 1 || choice > 4)
        {
            std::cout << "\n\n\tPlease enter a number in the range of 1 - 4" << std::endl
                      << std::endl
                      << "\tEnter Your Choice: ";
            std::cin >> choice;
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
            std::cout << "\n\tYou selected item " << choice << '.' << std::endl
                      << "\tGoodbye!\n\n";
            break;
        default:
            break;
        }
    } while (choice != 4);
}

class FileOpenException : public std::exception
{
public:
    const char *what()
    {
        return "\tERROR: Opening file isn'.\n\tTerminating program now...\n";
    }
};

InventoryBook allBooks[bookSize];

Menus allMenus;

double SoldBook::total = 0;

int currQty[bookSize] = {};

int main()
{
    int choice;

    int numOfLines = 0;

    std::string tempLines;

    while (std::getline(invFile, tempLines))
        ++numOfLines;

    /**
     * * Checks if there are already any lines, if not
     * * create 20 null lines
     * ! else
     * * fill in the struct with values from file
     */

    if (numOfLines != bookSize)
    {
        // std::cout << numOfLines << std::endl;
        invFile.close();
        try
        {
            invFile.open(inventoryFileName, std::ios::trunc | std::ios::out);
        }
        catch (FileOpenException e)
        {
            std::cerr << e.what() << std::endl;
            exit(0);
        }
        // invFile.seekg(invFile.beg);
        for (int i = 0; i < bookSize - 1; i++)
        {
            invFile << "\n";
        }
        invFile.close();

        try
        {
            invFile.open(inventoryFileName, std::ios::out | std::ios::in);
        }
        catch (FileOpenException e)
        {
            std::cerr << e.what() << std::endl;
            exit(0);
        }
    }
    else
    {
        std::string currLine;
        invFile.close();
        try
        {
            invFile.open(inventoryFileName, std::ios::out | std::ios::in);
        }
        catch (FileOpenException e)
        {
            std::cerr << e.what() << std::endl;
            exit(0);
        }

        for (int i = 0; i < std::size(allBooks) - 1; i++)
        {
            std::getline(invFile, currLine);
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

std::string charToString(char *str)
{
    std::string s = (str);

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
    std::cout << "\t\tSerendipity Booksellers" << std::endl
              << "\t\t    Book Information" << std::endl
              << std::endl
              << "1. ISBN: " << isbn << std::endl
              << "2. Title: " << title << std::endl
              << "3. Author: " << author << std::endl
              << "4. Publisher: " << publisher << std::endl
              << "5. Date Added: " << date << std::endl
              << "6. Quantity-On-Hand: " << qty << std::endl
              << "7. Wholesale Cost: " << std::setprecision(2) << std::fixed << wholesale << std::endl
              << "8. Retail Price:  " << retail << std::endl;

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

    std::cin.sync();
    std::cout << std::endl
              << "\tEnter Title: ";
    std::cin.get(tempBookTitle, 51);
    strUpper(tempBookTitle);
    allBooks[index].setTitle(tempBookTitle);
    std::cin.sync();

    std::cout << "\tEnter ISBN: ";
    std::cin.get(tempIsbn, 14);
    strUpper(tempIsbn);
    allBooks[index].setISBN(tempIsbn);
    std::cin.sync();

    std::cout << "\tEnter Author's Name: ";
    std::cin.get(tempAuthor, 31);
    strUpper(tempAuthor);
    allBooks[index].setAuthor(tempAuthor);
    std::cin.sync();

    std::cout << "\tEnter Publisher's Name: ";
    std::cin.get(tempPublisher, 31);
    strUpper(tempPublisher);
    allBooks[index].setPub(tempPublisher);
    std::cin.sync();

    std::cout << "\tEnter Date: ";
    std::cin.get(tempDateAdded, 11);
    allBooks[index].setDateAdded(tempDateAdded);
    std::cin.sync();

    std::cout << "\tEnter Quantity of Book: ";
    std::cin >> tempQty;
    allBooks[index].setQty(tempQty);
    std::cin.sync();

    std::cout << "\tEnter Wholesale Price: ";
    std::cin >> tempWholesale;
    allBooks[index].setWholesale(tempWholesale);
    std::cin.sync();

    std::cout << "\tEnter Retail Price: ";
    std::cin >> tempRetail;
    allBooks[index].setRetail(tempRetail);
    std::cin.sync();

    std::cout << "\tBook Added" << std::endl
              << std::endl;
}

int invMenu()
{

    /* Runs the code initially then loops if invalid input */
    do
    {
        std::cout << std::endl
                  << "\t Serendipity Booksellers " << std::endl
                  << "\t      Inventory Database " << std::endl
                  << std::endl
                  << "\t1. Look Up a Book" << std::endl
                  << "\t2. Add a Book" << std::endl
                  << "\t3. Edit a Book's Record" << std::endl
                  << "\t4. Delete a Book" << std::endl
                  << "\t5. Return to the Main Menu" << std::endl
                  << std::endl;

        std::cout << "\tInventory Menu\n\tEnter Your Choice: ";
        std::cin >> choice;

        /* Loops if number isn't valid */
        while (choice < 1 || choice > 5)
        {
            std::cout << "\n\n\tPlease enter a number in the range of 1 - 5" << std::endl
                      << std::endl
                      << "\tInventory Menu\n\tEnter Your Choice: ";

            std::cin >> choice;
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
            std::cout << "\n\tYou selected item " << choice << '.' << std::endl
                      << "\t\tRedirecting back to main menu....\n\n";
            break;
        default:
            break;
        }
    } while (choice != 5);

    return 0;
}

void fileDataToClass(int index, std::string str)
{
    const char *tempStr = str.c_str();
    char *newStr = new char[strlen(tempStr) + 1];
    strcpy(newStr, tempStr);

    char *token = strtok(newStr, ";");

    while (token != NULL)
    {
        std::string currToken(token),
            initStr = currToken.substr(0, currToken.find("=")),
            tokenVal = currToken.substr(currToken.find("=") + 1, std::size(currToken));

        findAndChangeMember(initStr, tokenVal, index); // edits the values of book as tokens

        token = strtok(NULL, ";");
    }
}

void findAndChangeMember(std::string str, std::string newVal, int index)
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
        allBooks[index].setQty(std::stoi(newStr));
    }
    else if (str == "wholesale")
    {
        allBooks[index].setWholesale(std::stod(newStr));
    }
    else if (str == "retail")
    {
        allBooks[index].setRetail(std::stoi(newStr));
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
    std::string currLine;

    /**
     * Writing to the STRUCT from FILE
     **/
    invFile.close();
    try
    {
        invFile.open(inventoryFileName, std::ios::out | std::ios::in);
    }
    catch (FileOpenException e)
    {
        std::cerr << e.what() << std::endl;
        exit(0);
    }

    for (int i = 0; i < std::size(allBooks); i++)
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
        for (int i = 0; i < std::size(allBooks); i++)
        {
            invFile << "booktitle=" << allBooks[i].getTitle()
                    << ";isbn=" << allBooks[i].getISBN()
                    << ";date=" << allBooks[i].getDateAdded()
                    << ";author=" << allBooks[i].getAuthor()
                    << ";publisher=" << allBooks[i].getPub()
                    << ";quantity=" << allBooks[i].getQty()
                    << ";wholesale=" << std::setprecision(2) << std::fixed << allBooks[i].getWholesale()
                    << ";retail=" << allBooks[i].getRetail() << ";" << (i < 19 ? "\n" : "");
        }
        invFile.close();
    }
    else
    {
        std::cout << "\tNo Inventory Space!" << std::endl;
        exit(0);
    }
}

void lookUpBook()
{
    char bookName[52] = {},
         answer;
    std::cout << std::endl
              << "\tEnter name of the book name: ";
    std::cin.sync();
    std::cin.get(bookName, 52);
    strUpper(bookName);

    for (InventoryBook books : allBooks)
    {
        if (books.bookMatch(bookName))
        {
            std::cin.sync();
            std::cout << "\n\tIs " << bookName << " the book you're looking for?  (y/n)" << std::endl;
            std::cin.get(answer);
            std::cin.sync();

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
    std::cout << "\tNo Book Found" << std::endl;
    exit(0);
}

void editBook()
{
    int choice, newIntVal;
    double newDoubleVal;

    char bookName[52] = {},
         answer;

    std::cout << "\n\tEnter name of the book name: ";
    std::cin.sync();
    std::cin.get(bookName, 52);
    strUpper(bookName);

    for (InventoryBook books : allBooks)
    {
        if (books.bookMatch(bookName))
        {
            std::cin.sync();
            std::cout << "\n\tIs " << bookName << " the book you're looking for?  (y/n)" << std::endl;
            std::cin.get(answer);
            std::cin.sync();

            if (answer == 'y')
            {
                bookInfo(books.getISBN(), books.getTitle(), books.getAuthor(),
                         books.getPub(), books.getDateAdded(), books.getQty(),
                         books.getWholesale(), books.getRetail());
                std::cout << "\tWhat do you want to change? ";
                std::cin >> choice;

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
                    std::cout << "\tEnter new ISBN: ";
                    std::cin.ignore();
                    std::cin.sync();
                    std::cin.get(tempIsbn, 14);
                    books.setISBN(tempIsbn);

                    break;
                case 2:
                    std::cout << "\tEnter new Title: ";
                    std::cin.ignore();
                    std::cin.sync();
                    std::cin.get(tempBookTitle, 51);
                    books.setTitle(tempBookTitle);
                    break;
                case 3:
                    std::cout << "\tEnter new Author: ";
                    std::cin.ignore();
                    std::cin.sync();
                    std::cin.get(tempAuthor, 31);
                    books.setAuthor(tempAuthor);
                    break;
                case 4:
                    std::cout << "\tEnter new Publisher: ";
                    std::cin.ignore();
                    std::cin.sync();
                    std::cin.get(tempPublisher, 31);
                    books.setPub(tempPublisher);
                    break;
                case 5:
                    std::cout << "\tEnter new Date: ";
                    std::cin.ignore();
                    std::cin.sync();
                    std::cin.get(tempDateAdded, 11);
                    books.setDateAdded(tempDateAdded);
                    break;
                case 6:
                    std::cout << "\tEnter new Quantity: ";
                    std::cin.ignore();
                    std::cin.sync();
                    (std::cin >> newIntVal);
                    books.setQty(newIntVal);
                    break;
                case 7:
                    std::cout << "\tEnter new Wholesale Cost: ";
                    std::cin.ignore();
                    std::cin.sync();
                    (std::cin >> newDoubleVal);
                    books.setWholesale(newDoubleVal);
                    break;
                case 8:
                    std::cout << "\tEnter new Retail Price: ";
                    std::cin.ignore();
                    std::cin.sync();
                    (std::cin >> newDoubleVal);
                    books.setRetail(newDoubleVal);
                    break;

                default:
                    std::cout << "\tInvalid Value" << std::endl;
                    std::cout << "\tGoodbye!" << std::endl;
                    exit(0);
                    break;
                }

                if (invFile.is_open())
                    invFile.close();

                try
                {
                    invFile.open(inventoryFileName, std::ios::out);
                }
                catch (FileOpenException e)
                {
                    std::cerr << e.what() << std::endl;
                    exit(0);
                }

                for (int i = 0; i < bookSize; i++)
                {
                    invFile << "booktitle=" << books.getTitle()
                            << ";isbn=" << books.getISBN()
                            << ";date=" << books.getDateAdded()
                            << ";author=" << books.getAuthor()
                            << ";publisher=" << books.getPub()
                            << ";quantity=" << books.getQty()
                            << ";wholesale=" << std::setprecision(2) << std::fixed << books.getWholesale()
                            << ";retail=" << books.getRetail() << ";" << (i < 19 ? "\n" : "");
                }

                bookInfo(books.getISBN(), books.getTitle(), books.getAuthor(),
                         books.getPub(), books.getDateAdded(), books.getQty(),
                         books.getWholesale(), books.getRetail());
                std::cout << "\tGoodbye!" << std::endl;
                invFile.close();
                exit(0);
            }
            else
            {
                continue;
            }
        }
    }
    std::cout << "\n\tBook Not Found" << std::endl;
}

void deleteBook()
{
    char bookName[52],
        answer;

    std::cout << std::endl
              << "\tEnter name of the book name: ";
    std::cin.sync();
    std::cin.get(bookName, 52);
    strUpper(bookName);

    for (InventoryBook books : allBooks)
    {
        if (books.bookMatch(bookName))
        {
            std::cin.sync();
            std::cout << "\n\tIs " << books.getTitle() << " the book you're looking for?  (y/n)" << std::endl;
            std::cin.get(answer);
            std::cin.sync();

            char deleteArr[] = "";

            if (answer == 'y')
            {
                books.delBook();

                if (invFile.is_open())
                    invFile.close();

                try
                {
                    invFile.open(inventoryFileName, std::ios::out);
                }
                catch (FileOpenException e)
                {
                    std::cerr << e.what() << std::endl;
                    exit(0);
                }

                invFile.seekg(invFile.beg);

                for (int i = 0; i < bookSize; i++)
                {
                    invFile << "booktitle=" << books.getTitle()
                            << ";isbn=" << books.getISBN()
                            << ";date=" << books.getDateAdded()
                            << ";author=" << books.getAuthor()
                            << ";publisher=" << books.getPub()
                            << ";quantity=" << books.getQty()
                            << ";wholesale=" << std::setprecision(2) << std::fixed << books.getWholesale()
                            << ";retail=" << books.getRetail() << ";" << (i < 19 ? "\n" : "");
                }

                std::cout << "\tDeleted" << std::endl;
                invFile.close();
                exit(0);
            }
            else
            {
                continue;
            }
        }
    }

    std::cout << "\tNo book Found." << std::endl
              << "\tGoodbye!" << std::endl;
    exit(0);
}

/* function for displaying items for effeciency */
void displayItem(int qty, std::string isbn, std::string title, double price)
{
    std::cout << qty << std::setw(20) << isbn << std::setw(26) << title << std::setw(12) << '$' << std::setprecision(2) << std::fixed << std::setw(6) << price << std::setw(7) << '$' << std::setprecision(2) << std::fixed << std::setw(6) << (double)qty * price << std::endl;
}

int cashier()
{
    /* Runs the code initially then loops user wants to enter another item */

    char repeat = 'y',
         isbnRepeat;

    int arrIndex = 0,
        isbnIndex = -1;
    std::string currDate;
    char isbnLookup[14] = {};

    long int qtySale,
        booksInInventory,
        booksPurchasing = 0;

    double subtotal = 0;

    do
    {
        // fflush(stdin);
        std::cout << "\n\tHow many titles are you purchasing? ";
        std::cin.ignore();
        std::cin >> (booksPurchasing);
        std::cin.sync();
    } while (booksPurchasing < 0 || booksPurchasing > 20);

    for (int i = 0; i < std::size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    std::cout << "\nSerendipity Booksellers\n  Cashier Module" << std::endl
              << std::endl;

    std::cin.sync();

    std::cout << "Date: ";
    std::getline(std::cin, currDate);
    std::cin.sync();
    SoldBook *saleBooks;
    try
    {
        saleBooks = new SoldBook[booksPurchasing];
    }
    catch (std::bad_alloc &e)
    {
        std::cerr << "\n\tERROR: Memory Allocation Error: " << e.what() << " \n\tTerminating program..." << '\n';
        exit(0);
    }

    do
    {
        double price, tax, total, subtotal = 0;
        bool isbnFound = false;

        char isbnUse;

        std::cin.sync();
        std::cout << "Quantity of Book: ";
        std::cin >> qtySale;
        saleBooks[arrIndex].setQtySold(qtySale);
        std::cin.sync();

        do
        {
            std::cin.sync();
            std::cout << "ISBN: ";
            std::cin.get(isbnLookup, 14);
            std::cin.sync();

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
                std::cin.sync();
                std::cout << "Book found with ISBN titled: " << allBooks[isbnIndex].getTitle()
                          << "\nWould you like to use this book? (y/n) ";

                std::cin.sync();
                std::cin.get(isbnUse);
                std::cin.sync();
                isbnRepeat = 'n';
            }
            else
            {
                std::cout << "ISBN not recognized: "
                          << "Would you like to try again? (y/n) ";
                std::cin.sync();
                std::cin.get(isbnRepeat);
                std::cin.sync();
            }
        } while (tolower(isbnRepeat) == 'y');

        if (isbnIndex != -1 && allBooks[isbnIndex].getQty() < 1)
        {
            std::cout << "\tInsufficient quantity of books" << std::endl;
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
            std::cout << "\tNo Book Found" << std::endl;
            exit(0);
        }

        saleBooks[arrIndex].setBook(allBooks[isbnIndex]);
        saleBooks[arrIndex].calcTax().calcSubtotal();

        std::cout << "\nSerendipity Book Sellers" << std::endl
                  << std::endl
                  << "Date: "
                  << currDate << std::endl
                  << std::endl
                  << "Qty" << std::setw(10) << std::setfill(' ') << "ISBN" << std::setw(20) << std::setfill(' ') << "Title" << std::setw(30) << std::setfill(' ') << "Price" << std::setw(13) << std::setfill(' ') << "Total" << std::endl
                  << std::string(80, '_') << std::endl
                  << std::endl;

        for (int i = 0; i < arrIndex + 1; i++)
        {
            displayItem(saleBooks[i].getQtySold(), charToString(saleBooks[i].getISBN()), charToString(saleBooks[i].getTitle()), saleBooks[i].getRetail());
        }

        arrIndex++;
        std::cout << (booksPurchasing - 1 > 0 ? "\n\n\tPress enter for next book entry..." : "\n\n\tLast book entered\n\n\tPress enter to display total.") << std::endl;
        std::cin.get();
        fflush(stdin);
    } while (--booksPurchasing > 0);

    std::cout << "\nSerendipity Book Sellers" << std::endl
              << std::endl
              << "Date: "
              << currDate << std::endl
              << std::endl
              << "Qty" << std::setw(10) << std::setfill(' ') << "ISBN" << std::setw(20) << std::setfill(' ') << "Title" << std::setw(30) << std::setfill(' ') << "Price" << std::setw(13) << std::setfill(' ') << "Total" << std::endl
              << std::string(80, '_') << std::endl
              << std::endl;

    for (int i = 0; i < arrIndex; i++)
    {
        displayItem(saleBooks[i].getQtySold(), charToString(saleBooks[i].getISBN()), charToString(saleBooks[i].getTitle()), saleBooks[i].getRetail());
        subtotal += saleBooks[i].getRetail();
    }

    std::cout << std::endl
              << std::setw(10) << std::left << "\tSubtotal" << std::setw(55) << std::right << '$' << std::setw(6) << std::right << std::setprecision(2) << std::fixed << subtotal << std::endl
              << std::setw(10) << std::left << "\tTax" << std::setw(55) << std::right << '$' << std::setw(6) << std::right << std::setprecision(2) << std::fixed << (subtotal * 0.06) << std::endl
              << std::setw(10) << std::left << "\tTotal" << std::setw(55) << std::right << '$' << std::setw(6) << std::right << std::setprecision(2) << std::fixed << saleBooks[0].getTotal() << std::endl
              << std::endl;

    invFile.close();

    try
    {
        invFile.open(inventoryFileName, std::ios::out | std::ios::in);
    }
    catch (FileOpenException e)
    {
        std::cerr << e.what() << std::endl;
        exit(0);
    }

    invFile.seekg(invFile.beg);
    for (int i = 0; i < std::size(allBooks); i++)
    {
        invFile << "booktitle=" << allBooks[i].getTitle()
                << ";isbn=" << allBooks[i].getISBN()
                << ";date=" << allBooks[i].getDateAdded()
                << ";author=" << allBooks[i].getAuthor()
                << ";publisher=" << allBooks[i].getPub()
                << ";quantity=" << allBooks[i].getQty()
                << ";wholesale=" << std::setprecision(2) << std::fixed << allBooks[i].getWholesale()
                << ";retail=" << allBooks[i].getRetail() << ";" << (i < 19 ? "\n" : "");
    }

    invFile.close();

    std::cout << "\nThank You for Shopping at Serendipity!\n\n";
    std::cout << "\nRedirecting back to main menu.....\n\n";

    return 0;
}

int reports()
{
    int choice;

    /* Runs the code initially then loops if invalid input */
    do
    {
        std::cout << std::endl
                  << "\t\tSerendipity Booksellers" << std::endl
                  << "\t\t    Reports" << std::endl
                  << std::endl
                  << "\t\t1. Inventory Listing " << std::endl
                  << "\t\t2. Inventory Wholesale Value " << std::endl
                  << "\t\t3. Inventory Retail Value " << std::endl
                  << "\t\t4. Listing by Quantity " << std::endl
                  << "\t\t5. Listing by Cost " << std::endl
                  << "\t\t6. Listing by Age " << std::endl
                  << "\t\t7. Return to Main Menu " << std::endl
                  << std::endl;
        std::cout << "\t\tEnter Your Choice: ";
        std::cin >> choice;

        /* Loops if number isn't valid */
        while (choice < 1 || choice > 7)
        {
            std::cout << "\n\n\t\tPlease enter a number in the range of 1 - 7" << std::endl
                      << std::endl
                      << "\t\tEnter Your Choice: ";
            std::cin >> choice;
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
            std::cout << "\n\t\tYou selected item " << choice << '.' << std::endl
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
    std::string tempVal;
    for (int i = 0; i < std::size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    for (int i = 0; i < booksInInventory; i++)
    {

        std::string tempTitle = title ? "\tTitle: " + charToString((allBooks[i].getTitle())) + "\n" : "",
                    tempIsbn = isbn ? "\tISBN: " + charToString((allBooks[i].getISBN())) + "\n" : "",
                    tempDate = date ? "\tDate: " + charToString(allBooks[i].getDateAdded()) + "\n" : "",
                    tempAuthor = auth ? "\tAuthor: " + charToString((allBooks[i].getAuthor())) + "\n" : "",
                    tempPubl = pub ? "\tPublisher: " + charToString((allBooks[i].getPub())) + "\n" : "",
                    tempQty = qtyBool ? "\tQuantity: " + std::to_string(allBooks[i].getQty()) + "\n" : "",
                    tempWhole = wholesaleBool ? "\tWholesale Price: " + std::to_string(allBooks[i].getWholesale()).substr(0, std::to_string(allBooks[i].getWholesale()).find(".") + 3) + "\n" : "",
                    tempRetail = retailBool ? "\tRetail Price: " + std::to_string(allBooks[i].getRetail()).substr(0, std::to_string(allBooks[i].getRetail()).find(".") + 3) + "\n" : "";

        std::cout << tempTitle
                  << tempIsbn
                  << tempDate
                  << tempAuthor
                  << tempPubl
                  << tempQty
                  << tempWhole
                  << tempRetail << std::endl;
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
    std::cout << "Total wholesale cost: " << std::setprecision(2) << std::fixed << totalWholesale << std::endl;
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
    std::cout << "Total retail cost: " << std::setprecision(2) << std::fixed << totalRetail << std::endl;
    system("pause");
}

void repQty()
{
    int booksInInventory;
    for (int i = 0; i < std::size(allBooks); i++)
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
        std::cout << "Title: " << charToString(allBooks[tempArrIndex[i]].getTitle())
                  << "\nISBN: " << charToString(allBooks[tempArrIndex[i]].getISBN())
                  << "\nQuantity: " << allBooks[tempArrIndex[i]].getQty() << std::endl
                  << std::endl;
    }
    system("pause");
}

void repCost()
{
    int booksInInventory;
    for (int i = 0; i < std::size(allBooks); i++)
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
        std::cout << "Title: " << charToString(allBooks[tempArrIndex[i]].getTitle())
                  << "\nISBN: " << charToString(allBooks[tempArrIndex[i]].getISBN())
                  << "\nWholesale Cost: " << allBooks[tempArrIndex[i]].getWholesale() << std::endl
                  << std::endl;
    }
    std::cout << "Enter any button to continue...";
    std::cin.sync();
    std::cin.get();
    std::cin.sync();
}

void repAge()
{
    int booksInInventory;
    for (int i = 0; i < std::size(allBooks); i++)
    {
        if (charToString(allBooks[i].getTitle()) == "\0" || charToString(allBooks[i].getTitle()) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    std::cin.sync();

    std::string tempArr[bookSize] = {},
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
                std::string tempDateAtJ = charToString(allBooks[j].getDateAdded());
                std::string tempDateAtSmall = charToString(allBooks[j].getDateAdded());
                ;
                if (std::stoi(tempDateAtJ.substr(std::size(tempDateAtJ) - 4, std::size(tempDateAtJ))) < std::stoi(tempDateAtSmall.substr(std::size(tempDateAtJ) - 4, std::size(tempDateAtJ))))
                {
                    small = j;
                }
                else if (std::stoi(tempDateAtJ.substr(0, 2)) < std::stoi(tempDateAtSmall.substr(0, 2)))
                {
                    small = j;
                }
                else if (std::stoi(tempDateAtJ.substr(3, 5)) < std::stoi(tempDateAtSmall.substr(3, 5)))
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
            std::cout << "Title: " << allBooks[tempArrIndex[i]].getTitle()
                      << "\nISBN: " << allBooks[tempArrIndex[i]].getISBN()
                      << "\nQuantity On Hand: " << allBooks[tempArrIndex[i]].getQty()
                      << "\nDate Added: " << allBooks[tempArrIndex[i]].getDateAdded() << std::endl
                      << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        if (strstr(e.what(), "stoi"))
        {
            std::cerr << "\n\tError: Incorrect Date Formats\n"
                      << std::endl;
        }
    }
    std::cout << "Enter any button to continue...";
    std::cin.sync();
    std::cin.get();
    std::cin.sync();
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
    std::string bookTitleString(bookTitle),
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
