/*
    Name: Dawit Mengistie
    Class: CMSC 226
    Professor: Koorosh Azhandeh
    CRN: 21673
    Due Date: 10/23/2022
 */

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

void fileDataToStruct(int, string);

void findAndChangeMember(string, string, int);
void editFile(string, string, string, int);

void setTitle(char *, int);
void setISBN(char *, int);
void setAuthor(char *, int);
void setPub(char *, int);
void setDateAdded(char *, int);
void setQty(int, int);
void setWholesale(double, int);
void setRetail(double, int);

int isEmpty(int);

int choice;
string lookup_str;

const string inventoryFileName = "Inventory-Database.txt";

fstream invFile(inventoryFileName, ios::out | ios::in);

const int bookSize = 20;

struct BookData
{
    char bookTitle[51] = {},
         isbn[14] = {},
         author[31] = {},
         publisher[31] = {},
         dateAdded[11] = {};

    int qtyOnHand;

    double wholesale,
        retail;
};

BookData allBooks[bookSize];

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
                fileDataToStruct(i, currLine); // writes values to file
            }
        }
    }

    /* Runs the code initially then loops if invalid input */
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
            system("clear");
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
         << "7. Wholesale Cost: " << wholesale << endl
         << "8. Retail Price:  " << retail << endl;

    return 0;
}

// Avoids redudant code for inputing values
void inputValues(int index)
{
    cin.sync();
    cout << endl
         << "\tEnter Title: ";
    cin.get(allBooks[index].bookTitle, 51);
    strUpper(allBooks[index].bookTitle);
    cin.sync();

    cout << "\tEnter ISBN: ";
    cin.get(allBooks[index].isbn, 14);
    strUpper(allBooks[index].isbn);
    cin.sync();

    cout << "\tEnter Author's Name: ";
    cin.get(allBooks[index].author, 31);
    strUpper(allBooks[index].author);
    cin.sync();

    cout << "\tEnter Publisher's Name: ";
    cin.get(allBooks[index].publisher, 31);
    strUpper(allBooks[index].publisher);
    cin.sync();

    cout << "\tEnter Date: ";
    cin.get(allBooks[index].dateAdded, 11);
    cin.sync();

    cout << "\tEnter Quantity of Book: ";
    cin >> allBooks[index].qtyOnHand;
    cin.sync();

    cout << "\tEnter Wholesale Price: ";
    cin >> allBooks[index].wholesale;
    cin.sync();

    cout << "\tEnter Retail Price: ";
    cin >> allBooks[index].retail;
    cin.sync();

    cout << "\tBook Added" << endl
         << endl;
}

// invite menu
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

void fileDataToStruct(int index, string str)
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
        setTitle(newStr, index);
    }
    else if (str == "isbn")
    {
        setISBN(newStr, index);
    }
    else if (str == "date")
    {
        setDateAdded(newStr, index);
    }
    else if (str == "author")
    {
        setAuthor(newStr, index);
    }
    else if (str == "publisher")
    {
        setPub(newStr, index);
    }
    else if (str == "wholesale")
    {
        setWholesale(stod(newStr), index);
    }
    else if (str == "retail")
    {
        setRetail(stoi(newStr), index);
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
    // for (int i = 0; i < size(allBooks); i++)
    // {
    //     getline(invFile, currLine);
    //     if (currLine != "\0" || !currLine.empty())
    //     {
    //         fileDataToStruct(i, currLine); // writes values to file
    //     }
    // }

    for (int i = 0; i < size(allBooks); i++)
    {
        if (isEmpty(i))
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
            invFile << "booktitle=" << allBooks[i].bookTitle
                    << ";isbn=" << allBooks[i].isbn
                    << ";date=" << allBooks[i].dateAdded
                    << ";author=" << allBooks[i].author
                    << ";publisher=" << allBooks[i].publisher
                    << ";quantity=" << allBooks[i].qtyOnHand
                    << ";wholesale=" << allBooks[i].wholesale
                    << ";retail=" << allBooks[i].retail << ";" << (i < 19 ? "\n" : "");
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
    char bookName[52],
        answer;
    cout << endl
         << "\tEnter name of the book name: ";
    cin.sync();
    cin.get(bookName, 52);
    strUpper(bookName);

    string currLine;

    char fileBookTitle[52] = {};
    invFile.close();
    invFile.open(inventoryFileName, ios::out | ios::in);
    // invFile.seekg(invFile.beg);
    for (int i = 0; i < size(allBooks); i++)
    {
        getline(invFile, currLine);
        cout << currLine.find(';', 0) << endl;
        string temp(currLine.substr(
            currLine.find('=') + 1,
            (currLine.find(';')) - currLine.find('=') - 1));

        temp.copy(fileBookTitle, 51); //

        if (strstr(fileBookTitle, (bookName)))
        {
            cin.sync();
            cout << "Is " << fileBookTitle << " the book you're looking for?  (y/n)" << endl;
            cin.get(answer);
            cin.sync();

            if (answer == 'y')
            {
                bookInfo(allBooks[i].isbn, allBooks[i].bookTitle, allBooks[i].author, allBooks[i].publisher, allBooks[i].dateAdded, allBooks[i].qtyOnHand, allBooks[i].wholesale, allBooks[i].retail);
                invFile.close();
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

    char bookName[100],
        answer;

    cout << endl
         << "\tEnter name of the book name: ";
    cin.sync();
    cin.get(bookName, 100);
    strUpper(bookName);

    string currLine;

    char fileBookTitle[51];
    invFile.seekg(invFile.beg);

    for (int i = 0; i < size(allBooks); i++)
    {
        getline(invFile, currLine);
        string temp(currLine.substr(currLine.find('=') + 1, (currLine.find(';')) - currLine.find('=') - 1));

        temp.copy(fileBookTitle, 51);

        if (strstr(fileBookTitle, bookName))
        {
            cin.sync();
            cout << "Is " << fileBookTitle << " the book you're looking for?  (y/n)" << endl;
            cin.get(answer);
            cin.sync();

            if (answer == 'y')
            {
                bookInfo(allBooks[i].isbn, allBooks[i].bookTitle, allBooks[i].author, allBooks[i].publisher, allBooks[i].dateAdded, allBooks[i].qtyOnHand, allBooks[i].wholesale, allBooks[i].retail);
                cout << "\tWhat do you want to change? ";
                cin >> choice;

                const char *tempStr = currLine.c_str();
                char *newStr = new char[strlen(tempStr) + 1];
                strcpy(newStr, tempStr);

                char *token = strtok(newStr, ";");

                while (token != NULL)
                {
                    string currToken(token),
                        initStr = currToken.substr(0, currToken.find("=")),
                        tokenVal = currToken.substr(currToken.find("=") + 1, size(currToken));

                    token = strtok(NULL, ";");
                }

                char temp_bookTitle[51] = {},
                     temp_isbn[14] = {},
                     temp_author[31] = {},
                     temp_publisher[31] = {},
                     temp_dateAdded[11] = {};

                switch (choice)
                {
                case 1:
                    cout << "\tEnter new ISBN: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(temp_isbn, 14);
                    editFile(currLine, "isbn", temp_isbn, i);
                    break;
                case 2:
                    cout << "\tEnter new Title: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(temp_bookTitle, 51);
                    editFile(currLine, "booktitle", temp_bookTitle, i);
                    break;
                case 3:
                    cout << "\tEnter new Author: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(temp_author, 31);
                    editFile(currLine, "author", temp_author, i);
                    break;
                case 4:
                    cout << "\tEnter new Publisher: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(temp_publisher, 31);
                    editFile(currLine, "publisher", temp_publisher, i);
                    break;
                case 5:
                    cout << "\tEnter new Date: ";
                    cin.ignore();
                    cin.sync();
                    cin.get(temp_dateAdded, 11);
                    editFile(currLine, "date", temp_dateAdded, i);
                    break;
                case 6:
                    cout << "\tEnter new Quantity: ";
                    cin.ignore();
                    cin.sync();
                    (cin >> newIntVal);
                    editFile(currLine, "quantity", to_string(newIntVal), i);
                    break;
                case 7:
                    cout << "\tEnter new Wholesale Cost: ";
                    cin.ignore();
                    cin.sync();
                    (cin >> newDoubleVal);
                    editFile(currLine, "wholesale", to_string(newDoubleVal), i);
                    break;
                case 8:
                    cout << "\tEnter new Retail Price: ";
                    cin.ignore();
                    cin.sync();
                    (cin >> newDoubleVal);
                    editFile(currLine, "retail", to_string(newDoubleVal), i);
                    break;

                default:
                    cout << "\tInvalid Value" << endl;
                    cout << "\tGoodbye!" << endl;
                    exit(0);
                    break;
                }

                string currLine;
                invFile.close();
                invFile.open(inventoryFileName, ios::out | ios::in);
                invFile.seekg(invFile.beg);
                for (int i = 0; i < size(allBooks); i++)
                {
                    getline(invFile, currLine);
                    if (currLine != "\0" || !currLine.empty())
                    {
                        fileDataToStruct(i, currLine); // writes values to file
                    }
                }
                
                bookInfo(allBooks[i].isbn, allBooks[i].bookTitle, allBooks[i].author,
                         allBooks[i].publisher, allBooks[i].dateAdded, allBooks[i].qtyOnHand,
                         allBooks[i].wholesale, allBooks[i].retail);
                cout << "\tGoodbye!" << endl;
                invFile.close();
                exit(0);
            }
        }
        else
        {
            continue;
        }
    }
    cout << "\tBook Not Found" << endl;
    cout << "\n\tGoodbye!" << endl;
    exit(0);
}

void editFile(string line, string whichBookData, string newVal, int indexOfChange)
{
    const char *tempStr = line.c_str();
    char *newStr = new char[strlen(tempStr) + 1];
    strcpy(newStr, tempStr);

    char *token = strtok(newStr, ";");

    string editedStr("");

    while (token != NULL)
    {
        string currToken(token),
            initStr = currToken.substr(0, currToken.find("=")),
            tokenVal = currToken.substr(currToken.find("=") + 1, size(currToken));

        // // TODO: Change the file member values
        /**
         * 1.
         */

        if (initStr == whichBookData)
        {
            editedStr.append(initStr + "=" + newVal + ";");
        }
        else
        {
            editedStr.append(currToken + ";");
        }

        token = strtok(NULL, ";");
    }

    // string newCurrLine;
    invFile.seekg(invFile.beg);

    for (int i = 0; i < bookSize; i++)
    {
        if (i != indexOfChange)
        {
            invFile << "booktitle=" << allBooks[i].bookTitle;
            invFile << ";isbn=" << allBooks[i].isbn;
            invFile << ";date=" << allBooks[i].dateAdded;
            invFile << ";author=" << allBooks[i].author;
            invFile << ";publisher=" << allBooks[i].publisher;
            invFile << ";quantity=" << allBooks[i].qtyOnHand;
            invFile << ";wholesale=" << allBooks[i].wholesale;
            invFile << ";retail=" << allBooks[i].retail << ";\n";
        }
        else
        {
            invFile << editedStr << endl;
        }
    }
}

void deleteBook()
{

    char bookName[100],
        answer;

    cout << endl
         << "\tEnter name of the book name: ";
    cin.sync();
    cin.get(bookName, 100);
    strUpper(bookName);

    string currLine;
    char fileBookTitle[52] = {};
    invFile.close();
    invFile.open(inventoryFileName, ios::out | ios::in);

    for (int i = 0; i < size(allBooks); i++)
    {

        getline(invFile, currLine);
        string temp(currLine.substr(currLine.find('=') + 1, (currLine.find(';')) - currLine.find('=') - 1));

        temp.copy(fileBookTitle, 51);

        if (strstr(fileBookTitle, bookName))
        {
            cin.sync();
            cout << "Is " << allBooks[i].bookTitle << " the book you're looking for?  (y/n)" << endl;
            cin.get(answer);
            cin.sync();

            if (answer == 'y')
            {
                for (int j = 0; j < sizeof(charToString(allBooks[i].bookTitle)) / sizeof(char); j++)
                {
                    allBooks[i].bookTitle[j] = '\0';
                    allBooks[i].isbn[j] = '\0';
                }
                cout << "\tDeleted" << endl;
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
    double price, subtotal = 0, tax, total;
    char repeat = 'y',
         isbnRepeat;

    int arrIndex = 0,
        isbnIndex = -1;
    string isbnLookup, currDate;

    string bookTitleSale[bookSize] = {},
           isbnSale[bookSize] = {},
           authorSale[bookSize] = {},
           publisherSale[bookSize] = {},
           dateAddedSale[bookSize] = {};

    int currQtySale[bookSize] = {};

    double wholesaleSale[bookSize] = {},
           retailSale[bookSize] = {};

    int booksInInventory;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].bookTitle) == "\0" || charToString(allBooks[i].bookTitle) == "")
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

    do
    {
        bool isbnFound = false;

        char isbnUse;

        cin.sync();
        cout << "Quantity of Book: ";
        cin >> currQtySale[arrIndex];

        do
        {
            string tempVal;
            cin.sync();
            cout << "ISBN: ";
            getline(cin, isbnLookup);

            for (int i = 0; i < booksInInventory; i++)
            {
                tempVal = "";
                if (charToString(allBooks[i].isbn) == isbnLookup)
                {
                    isbnIndex = i;
                    isbnFound = true;
                    break;
                }
            }
            if (isbnFound)
            {
                cin.sync();
                cout << "Book found with ISBN named " << allBooks[isbnIndex].bookTitle
                     << "\nWould you like to use this book? (y/n) ";

                cin.get(isbnUse);
                cin.sync();
                isbnRepeat = 'n';
            }
            else
            {
                cout << "ISBN not recognized: "
                     << "Would you like to try again? (y/n) ";
                cin.get(isbnRepeat);
            }
        } while (tolower(isbnRepeat) == 'y');

        if (isbnIndex != -1 && allBooks[isbnIndex].qtyOnHand < 1)
        {
            cout << "\tInsufficient quantity of books" << endl;
            repeat = 'n';
            break;
        }
        else if (isbnIndex != -1)
        {
            allBooks[isbnIndex].qtyOnHand -= currQtySale[arrIndex];
        }
        else
        {
            cout << "\tNo Book Found" << endl;
            exit(0);
        }

        if (isbnUse)
        {
            for (int j = 0; j < sizeof(allBooks[arrIndex].bookTitle) / sizeof(char); j++)
            {
                allBooks[arrIndex].bookTitle[j] = allBooks[isbnIndex].bookTitle[j];
            }
            allBooks[arrIndex].retail = allBooks[isbnIndex].retail;
        }
        else
        {
            cin.sync();
            cout << "Title: ";
            cin.get(allBooks[arrIndex].bookTitle, 51);
            cin.sync();

            cin.sync();
            cout << "Price: ";
            cin >> allBooks[arrIndex].retail;
            cin.sync();
        }

        for (int i = 0; i < booksInInventory; i++)
        {

            subtotal += (double)allBooks[i].retail * (double)currQtySale[i];
        }

        tax = subtotal * 0.06;
        total = subtotal + tax;

        cout << "\nSerendipity Book Sellers" << endl
             << endl
             << "Date: "
             << currDate << endl
             << endl
             << "Qty" << setw(10) << setfill(' ') << "ISBN" << setw(20) << setfill(' ') << "Title" << setw(30) << setfill(' ') << "Price" << setw(13) << setfill(' ') << "Total" << endl
             << string(80, '_') << endl
             << endl;

        for (int i = 0; i < booksInInventory; i++)
        {
            displayItem(currQtySale[i], charToString(allBooks[i].isbn), charToString(allBooks[i].bookTitle), allBooks[i].retail);
        }

        cout << endl
             << setw(10) << left << "\tSubtotal" << setw(55) << right << '$' << setw(6) << right << setprecision(2) << fixed << subtotal << endl
             << setw(10) << left << "\tTax" << setw(55) << right << '$' << setw(6) << right << setprecision(2) << fixed << tax << endl
             << setw(10) << left << "\tTotal" << setw(55) << right << '$' << setw(6) << right << setprecision(2) << fixed << total << endl
             << endl
             << "Would you like to add another item? (y or n): ";
        cin.sync();
        cin >> repeat;

        arrIndex++;
    } while (repeat != 'n');
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
        if (charToString(allBooks[i].bookTitle) == "\0" || charToString(allBooks[i].bookTitle) == "")
        {
            booksInInventory = i;
            break;
        }
    }

    for (int i = 0; i < booksInInventory; i++)
    {
        // string titleTemp = "";
        // string isbnTemp = "";
        // string dateTemp = "";
        // string authorTemp = "";
        // string publisherTemp = "";

        string tempTitle = title ? "\tTitle: " + charToString((allBooks[i].bookTitle)) + "\n" : "",
               tempIsbn = isbn ? "\tISBN: " + charToString((allBooks[i].isbn)) + "\n" : "",
               tempDate = date ? "\tDate: " + charToString(allBooks[i].dateAdded) + "\n" : "",
               tempAuthor = auth ? "\tAuthor: " + charToString((allBooks[i].author)) + "\n" : "",
               tempPubl = pub ? "\tPublisher: " + charToString((allBooks[i].publisher)) + "\n" : "",
               tempQty = qtyBool ? "\tQuantity: " + to_string(allBooks[i].qtyOnHand) + "\n" : "",
               tempWhole = wholesaleBool ? "\tWholesale Price: " + to_string(allBooks[i].wholesale) + "\n" : "",
               tempRetail = retailBool ? "\tRetail Price: " + to_string(allBooks[i].retail) + "\n" : "";

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
    cout << "Enter any key to continue..." << endl;
    cin.sync();
    cin.get();
}

void repWholesale()
{
    double totalWholesale = 0;
    printListings(true, true, true, true, false, false, false, false);
    for (int i = 0; i < bookSize; i++)
    {
        totalWholesale += allBooks[i].wholesale * (double)allBooks[i].qtyOnHand;
    }
    cout << "Total wholesale cost: " << totalWholesale << endl;
    cout << "Enter any key to continue..." << endl;
    cin.sync();
    cin.get();
}

void repRetail()
{
    double totalRetail = 0;
    printListings(true, true, true, false, true, false, false, false);
    for (int i = 0; i < bookSize; i++)
    {
        totalRetail += allBooks[i].retail * (double)allBooks[i].qtyOnHand;
    }
    cout << "Total retail cost: " << totalRetail << endl;
    cout << "Enter any key to continue..." << endl;
    cin.sync();
    cin.get();
}

void repQty()
{
    int booksInInventory;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].bookTitle) == "\0" || charToString(allBooks[i].bookTitle) == "")
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
        tempArr[i] = allBooks[i].qtyOnHand;
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
        cout << "Title: " << charToString(allBooks[tempArrIndex[i]].bookTitle)
             << "\nISBN: " << charToString(allBooks[tempArrIndex[i]].isbn)
             << "\nQuantity: " << allBooks[tempArrIndex[i]].qtyOnHand << endl
             << endl;
    }
    cout << "Enter any button to continue...";
    cin.get();
    cin.sync();
}

void repCost()
{
    int booksInInventory;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].bookTitle) == "\0" || charToString(allBooks[i].bookTitle) == "")
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
        tempArr[i] = allBooks[i].wholesale;
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
        cout << "Title: " << charToString(allBooks[tempArrIndex[i]].bookTitle)
             << "\nISBN: " << charToString(allBooks[tempArrIndex[i]].isbn)
             << "\nWholesale Cost: " << allBooks[tempArrIndex[i]].wholesale << endl
             << endl;
    }
    cout << "Enter any button to continue...";
    cin.get();
    cin.sync();
}

void repAge()
{
    int booksInInventory;
    for (int i = 0; i < size(allBooks); i++)
    {
        if (charToString(allBooks[i].bookTitle) == "\0" || charToString(allBooks[i].bookTitle) == "")
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
        tempArr[i] = allBooks[i].dateAdded;
        tempArrIndex[i] = i;
    }
    string tempVal1 = "";
    string tempVal2 = "";

    for (int i = 0; i < booksInInventory; i++)
    {
        small = i;
        for (int j = i + 1; j < booksInInventory; j++)
        {
            tempVal1 = "";
            tempVal2 = "";

            string tempDateAtJ = charToString(allBooks[j].dateAdded);
            string tempDateAtSmall = charToString(allBooks[j].dateAdded);
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
        cout << "Title: " << allBooks[tempArrIndex[i]].bookTitle
             << "\nISBN: " << allBooks[tempArrIndex[i]].isbn
             << "\nQuantity On Hand: " << allBooks[tempArrIndex[i]].qtyOnHand
             << "\nDate Added: " << allBooks[tempArrIndex[i]].dateAdded << endl
             << endl;
    }
    cout << "Enter any button to continue...";
    cin.get();
    cin.sync();
}

void setTitle(char newTitle[], int subscript)
{
    strUpper(newTitle);
    strcpy(allBooks[subscript].bookTitle, newTitle);
}

void setISBN(char newISBN[], int subscript)
{
    strUpper(newISBN);
    strcpy(allBooks[subscript].isbn, newISBN);
}

void setAuthor(char newAuthor[], int subscript)
{
    strUpper(newAuthor);
    strcpy(allBooks[subscript].author, newAuthor);
}

void setPub(char newPub[], int subscript)
{
    strUpper(newPub);
    strcpy(allBooks[subscript].publisher, newPub);
}

void setDateAdded(char newDate[], int subscript)
{
    strUpper(newDate);
    strcpy(allBooks[subscript].dateAdded, newDate);
}

void setQty(int newQty, int subscript)
{
    allBooks[subscript].qtyOnHand = newQty;
}

void setWholesale(double newWhole, int subscript)
{
    allBooks[subscript].wholesale = newWhole;
}

void setRetail(double newRetail, int subscript)
{
    (allBooks[subscript].retail = newRetail);
}

int isEmpty(int subscript)
{
    if (allBooks[subscript].bookTitle[0] == '\0')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void removeBook(int subscript)
{
    allBooks[subscript].bookTitle[0] = '\0';
}
