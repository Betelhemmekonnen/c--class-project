// AASTU SWEG 2ND Year Sec G Group 4 Project
// Cinema Management System

// This program provides both the customer and admin interface of a cinema management system

/*
Group Members:
Yohannes Assefa
Yohannes Fantahun
Tsiyon Abebe
Yohannes Abebe
Betelhem Mekonnen
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <istream>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <limits>
#include <time.h>

using namespace std;

//Username and Password set as default for Admin Login
//You can use one of the declared usernames and password to login as customer

string adminUname = "admin", adminPass = "admin";

//
//Declaring Structures and Pointers
//

struct Movie
{
    string name, genre, director;
    float rating, price;
    string time_cinema[1];
    int seats;
};

//Initializing Movie Array

int movieCount;
Movie *movies;
Movie *n_movies;

struct Customer
{
    string name;
    int age;
    string phone, address, username, password;
};

//Initializing customer Array

int customerCount;
Customer *customers;
Customer *n_customers;

struct Ticket
{
    string cust_name, mov_name, timeandcinema;
    int seat_no[10];
    int age, ticketCount;
    float totalCost;
};

//Initializing Booked Tickets log Array

int bookedTickets;
Ticket *tickets;
Ticket *n_tickets;

//keeps track of current logged in user
string currentSession = "Guest";
int loginIndex, ticketIndex, movDelIndex = -1;

int cinemaSize = 0;
int *cinema = new int[cinemaSize];

int delAmt = 0;
int *clearIndex = new int[delAmt];

struct Cinema
{
    string movie, timing;
    int seats[100];
};

Cinema cinemas[6];

//String input for most input fields to handle errors
string choice;
string *choicePtr = &choice;

ifstream ifile;
ofstream ofile;

//Time Pointer for Display on Ticket
// time_t t = time(NULL);
// tm *timePtr = localtime(&t);

//
//prototyping Functions
//
float strtofloat(string);
bool floatCheck(string);

//Delete
void nullifyMovieList();
void nullifyCustomerList();
void deleteCustomer();
void deleteMovInput();
void deleteMovie(int);
void deleteTicket(int);

//File system
void makeCinema();
void inputFile();
void inputCount();
void updateMovie();
void updateTicket();
void updateCustomer();
void updateCount();
void updateCinema();

//Add
void addMovie();
void addCustomer();
void bookMovie();
void inputCustomer(Customer, int);
void inputMovie(Movie, int);
void inputTicket(Ticket, int);

//Menu
void modMovies();
void mainMenu();
void adminMenu();
void adminLogin();
void loginPage();
void customerMenu();
void modCustomers();
void sort();
void editMovie(int);
void editCustomer(int);
void customerLogin();

void deleteMovChoice();
void deleteCustChoice();
void customerLoginChoice();

//Display
void displayCustomers();
void displayTicketList();
void displaySingleCustomer(int);
void displaySingleMovie(int);
void displayMovies();
void displayTicket();
void cinemaStatus();
void searchMovie(string);

int main()
{
    inputFile();
    displayMovies();
    system("pause");
    displayTicketList();
    system("pause");
    loginPage();
    return 0;
}

float strtofloat(string temp)
{
    float num;
    stringstream ss;
    ss << temp;
    ss >> num;
    return num;
}

//Read from file
void inputCount()
{
    ifile.open("count.txt");
    ifile >> movieCount;
    ifile >> customerCount;
    ifile >> bookedTickets;
    ifile.close();

    // cout << movieCount << endl
    //      << customerCount << endl
    //      << bookedTickets << endl;
    // system("pause");

    movies = new Movie[movieCount];
    n_movies = new Movie[movieCount];
    customers = new Customer[customerCount];
    n_customers = new Customer[customerCount];
    tickets = new Ticket[bookedTickets];
    n_tickets = new Ticket[bookedTickets];
}
void inputFile()
{
    inputCount();
    makeCinema();
    ifile.open("movies.txt");

    string temp;

    for (int i = 0; i < movieCount; i++)
    {
        getline(ifile, movies[i].name);
        getline(ifile, movies[i].genre);
        getline(ifile, movies[i].director);
        getline(ifile, temp);
        movies[i].rating = strtofloat(temp);
        getline(ifile, temp);
        movies[i].price = strtofloat(temp);
        getline(ifile, movies[i].time_cinema[0]);
        getline(ifile, temp);
        movies[i].seats = strtofloat(temp);
    }
    ifile.close();

    ifile.open("customers.txt");

    for (int i = 0; i < customerCount; i++)
    {
        getline(ifile, customers[i].name);
        getline(ifile, temp);
        customers[i].age = strtofloat(temp);
        getline(ifile, customers[i].phone);
        getline(ifile, customers[i].address);
        getline(ifile, customers[i].username);
        getline(ifile, customers[i].password);
    }
    ifile.close();

    ifile.open("tickets.txt");

    for (int i = 0; i < bookedTickets; i++)
    {
        getline(ifile, tickets[i].cust_name);
        getline(ifile, tickets[i].mov_name);
        getline(ifile, tickets[i].timeandcinema);
        getline(ifile, temp);
        tickets[i].ticketCount = strtofloat(temp);
        for (int j = 0; j < tickets[i].ticketCount; j++)
        {
            getline(ifile, temp);
            tickets[i].seat_no[j] = strtofloat(temp);
        }
        getline(ifile, temp);
        tickets[i].age = strtofloat(temp);
        getline(ifile, temp);
        tickets[i].totalCost = strtofloat(temp);
    }
    ifile.close();
    //     struct Ticket
    // {
    //     string cust_name, mov_name, timeandcinema;
    //     int ticketCount, seat_no[10];
    //     int age, ;
    //     float totalCost;
    // };
}
void makeCinema()
{
    string temp;
    ifile.open("cinemas.txt");

    for (int i = 0; i < 6; i++)
    {
        getline(ifile, cinemas[i].movie);
        getline(ifile, cinemas[i].timing);
        for (int j = 0; j < 100; j++)
        {
            getline(ifile, temp);
            cinemas[i].seats[j] = strtofloat(temp);
        }
    }
    ifile.close();
}

//Writes to file
void updateCount()
{
    ofile.open("count.txt");
    ofile << movieCount << endl;
    ofile << customerCount << endl;
    ofile << bookedTickets << endl;
    ofile.close();
}
void updateMovie()
{
    updateCount();
    ofile.open("temp.txt");
    for (int i = 0; i < movieCount; i++)
    {
        ofile << movies[i].name << endl;
        ofile << movies[i].genre << endl;
        ofile << movies[i].director << endl;
        ofile << movies[i].rating << endl;
        ofile << movies[i].price << endl;
        ofile << movies[i].time_cinema[0] << endl;
        ofile << movies[i].seats << endl;
    }
    ofile.close();

    remove("movies.txt");
    rename("temp.txt", "movies.txt");
}
void updateTicket()
{
    updateCount();
    ofile.open("temp.txt");

    for (int i = 0; i < bookedTickets; i++)
    {
        ofile << tickets[i].cust_name << endl;
        ofile << tickets[i].mov_name << endl;
        ofile << tickets[i].timeandcinema << endl;
        ofile << tickets[i].ticketCount << endl;
        for (int j = 0; j < tickets[i].ticketCount; j++)
        {
            ofile << tickets[i].seat_no[j] << endl;
        }
        ofile << tickets[i].age << endl;
        ofile << tickets[i].totalCost << endl;
    }
    ofile.close();
    remove("tickets.txt");
    rename("temp.txt", "tickets.txt");
}
void updateCustomer()
{
    updateCount();
    ofile.open("temp.txt");

    for (int i = 0; i < customerCount; i++)
    {
        ofile << customers[i].name << endl;
        ofile << customers[i].age << endl;
        ofile << customers[i].phone << endl;
        ofile << customers[i].address << endl;
        ofile << customers[i].username << endl;
        ofile << customers[i].password << endl;
    }
    ofile.close();
    remove("customers.txt");
    rename("temp.txt", "customers.txt");
}
void updateCinema()
{
    ofile.open("cinematest.txt");

    for (int i = 0; i < 6; i++)
    {
        ofile << cinemas[i].movie << endl;
        ofile << cinemas[i].timing << endl;
        for (int j = 0; j < 100; j++)
        {
            ofile << cinemas[i].seats[j] << ' ';
        }
        ofile << endl;
    }
    ofile.close();
}

//main page of the program from where all navigation occurs
//

void mainMenu()
{
    system("cls");

    // cout << timePtr->tm_mday << "/" << timePtr->tm_mon + 1
    //      << "/" << timePtr->tm_year + 1900 << "  " << timePtr->tm_hour << ":" << timePtr->tm_min;
    cout << "\t\t\t\t"
         << "User: " << currentSession << endl;
    cout << endl;

    cout << "Welcome to Our Cinema" << endl
         << "1. Book a Movie" << endl
         << "2. Show Movies List" << endl
         << "3. Sort Movies by Category" << endl
         << "4. Login" << endl
         << "5. Back" << endl
         << "6. Exit" << endl;

    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                bookMovie();
                system("pause");
                mainMenu();
                break;
            }
            else if (choice == "2")
            {
                displayMovies();
                system("pause");
                mainMenu();
                break;
            }
            else if (choice == "3")
            {
                sort();
                system("pause");
                mainMenu();
                break;
            }
            else if (choice == "4")
            {
                customerLoginChoice();
                mainMenu();
                break;
            }
            else if (choice == "5")
            {
                loginPage();
                break;
            }
            else if (choice == "6")
            {
                exit(0);
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//page to select from customer or admin login
//
void loginPage()
{
    system("cls");
    cout << "Welcome to this Cinema" << endl
         << "1.Customer" << endl
         << "2.Admin" << endl
         << "3.Exit" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                mainMenu();
                break;
            }
            else if (choice == "2")
            {
                adminLogin();
                break;
            }
            else if (choice == "3")
            {
                exit(0);
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//login page for the admin
//
void adminLogin()
{
    string uName, pass;
    while (true)
    {
        system("cls");
        cout << "Enter Credentials" << endl;
        cout << "0. Back" << endl
             << endl;
        cout << "Enter your Username: ";
        cin >> uName;
        if (uName == "0")
        {
            loginPage();
        }
        cout << "Enter your Password: ";
        cin >> pass;
        if (pass == "0")
        {
            loginPage();
        }

        if (uName == adminUname && pass == adminPass)
        {
            adminMenu();
            break;
        }
        else
        {
            cout << "Invalid Credentials, Try Again" << endl;
            Sleep(1000);
            continue;
        }
    }
}

//main menu of the admin side
//
void adminMenu()
{
    system("cls");
    cout << "Welcome to the Admin Page" << endl
         << "1. Customer List" << endl
         << "2. Movie List" << endl
         << "3. Ticket List" << endl
         << "4. Cinema Status" << endl
         << "5. Logout" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                modCustomers();
                break;
            }
            else if (choice == "2")
            {
                modMovies();
                break;
            }
            else if (choice == "3")
            {
                displayTicketList();
                system("pause");
                adminMenu();
                break;
            }
            else if (choice == "4")
            {
                cinemaStatus();
            }
            else if (choice == "5")
            {
                currentSession = "Guest";
                loginPage();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

// display cinemas
void cinemaStatus()
{
    for (int i = 0; i < 6; i++)
    {
        cout << cinemas[i].movie << endl
             << cinemas[i].timing << endl
             << endl;
        for (int n = 1; n <= 100; n++)
        {
            cout << setw(3) << cinemas[i].seats[n - 1];
            if (n % 10 == 0)
            {
                cout << endl;
            }
        }
        cout << endl;
    }
    system("pause");
    adminMenu();
}

//choice between new and existing customer on login
//
void customerLoginChoice()
{
    system("cls");
    cout
        << "1. Registered Customer" << endl
        << "2. New Customer" << endl
        << "3. Back" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                customerLogin();
                break;
            }
            else if (choice == "2")
            {
                addCustomer();
                cout << "Customer Successfully Registered, Please Login";
                Sleep(2000);
                system("cls");
                customerLogin();
                break;
            }
            else if (choice == "3")
            {
                mainMenu();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//login page for the customer
//
void customerLogin()
{
    string uName, pass;

    while (true)
    {
        system("cls");
        cout << "Enter Credentials" << endl;
        cout << "0. Back" << endl
             << endl;
        cout << "Enter your Username: ";
        cin >> uName;
        if (uName == "0")
        {
            customerLoginChoice();
        }

        for (int i = 0; i < customerCount; i++)
        {
            if (uName == customers[i].username)
            {
                loginIndex = i;
                break;
            }
        }

        cout << "Enter your Password: ";
        cin >> pass;
        if (pass == "0")
        {
            customerLoginChoice();
        }
        if (pass == customers[loginIndex].password)
        {
            currentSession = uName;
            system("cls");
            cout << "Login Successful, Redirecting" << endl;
            cout << "| # - - - - - - - - |";
            Sleep(500);
            system("cls");
            cout << "Login Successful, Redirecting" << endl;
            cout << "| # # # # - - - - - |";
            Sleep(300);
            system("cls");
            cout << "Login Successful, Redirecting" << endl;
            cout << "| # # # # # # # # # |";
            Sleep(250);
            customerMenu();
        }
        else
        {
            cout << "Invalid Credentials, Try Again" << endl;
            Sleep(1000);
            continue;
        }

        system("pause");
        customerLogin();
    }
}

//main menu for registered customers
//
void customerMenu()
{
    system("cls");

    cout << "\t\t\t\t\t\t\t"
         << "User: " << currentSession << endl;

    cout << "Welcome to the Customer Page" << endl
         << "1. Book a Movie" << endl
         << "2. Show Movies List" << endl
         << "3. Sort Movies by Category" << endl
         << "4. Show My Status" << endl
         << "5. Logout" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                bookMovie();
                system("pause");
                customerMenu();
                break;
            }
            else if (choice == "2")
            {
                displayMovies();
                system("pause");
                customerMenu();
                break;
            }
            else if (choice == "3")
            {
                sort();
                system("pause");
                customerMenu();
                break;
            }
            else if (choice == "4")
            {
                displayTicket();
                system("pause");
                customerMenu();
                break;
            }
            else if (choice == "5")
            {
                currentSession = "Guest";
                loginPage();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

// Displays all Movies in the Cinema
//
void displayMovies()
{
    for (int i = 0; i <= 135; i++)
    {
        cout << "-";
    }
    cout << endl
         << "|" << left << setw(3) << "No"
         << setw(25) << "Name"
         << setw(11) << "|Genre"
         << setw(21) << "|Director"
         << setw(8) << "|Rating"
         << setw(8) << "|Price"
         << setw(16) << "|Time and Cinema"
         << setw(2) << " " << setw(12) << "|Available Seats    |" << endl;
    for (int i = 0; i < movieCount; i++)
    {
        cout << "|" << setw(3) << left << i + 1
             << setw(25) << movies[i].name << "|"
             << setw(10) << movies[i].genre << "|"
             << setw(20) << movies[i].director << "|"
             << setw(7) << movies[i].rating << "|"
             << setw(7) << movies[i].price << "|"
             << setw(16) << movies[i].time_cinema[0] << " | "
             << setw(2) << " " << setw(15) << movies[i].seats << " | " << endl;
    }
    for (int i = 0; i <= 135; i++)
    {
        cout << "-";
    }
    cout << endl;
}

//Function that receives input on new movie bookings
//
void inputTicket(Ticket n_tickets[], int q)
{
    int mov, time, tcount;
    cout << "pass";
    cout << endl;

    if (currentSession == "Guest")
    {
        cout << "You are not a Regstered customer" << endl;
        cout << "Enter your name(string): ";
        cin.ignore();
        getline(cin, n_tickets[q].cust_name);
        cout << "Enter your age(int): ";
        cin >> n_tickets[q].age;
    }
    else
    {
        n_tickets[q].cust_name = customers[loginIndex].name;
        n_tickets[q].age = customers[loginIndex].age;
    }

    cout << endl
         << "Select a Movie(int): ";
    cin >> mov;
    mov--;

    n_tickets[q].mov_name = movies[mov].name;
    time = 0;
    n_tickets[q].timeandcinema = movies[mov].time_cinema[time];
    while (true)
    {
        cout << "How many Tickets would you like to purchase(int): ";
        cin >> tcount;

        if (tcount > movies[mov].seats)
        {
            cout << "There arent enough tickets, Please select a lesser amount!";
            continue;
        }
        else if (tcount > 10)
        {
            cout << "One Customer cannot book more than 10 Seats";
            continue;
        }
        else
            break;
    }
    int seatID[tcount];

    n_tickets[q].ticketCount = tcount;
    n_tickets[q].totalCost = movies[mov].price * tcount;
    system("cls");

    for (int l = 0; l < tcount; l++)
    {
        for (int m = 0; m < 6; m++)
        {
            if (cinemas[m].timing == n_tickets[q].timeandcinema)
            {
                system("cls");
                for (int n = 1; n <= 100; n++)
                {
                    cout << setw(3) << cinemas[m].seats[n - 1];
                    if (n % 5 == 0)
                    {
                        cout << " ";
                    }
                    if (n % 10 == 0)
                    {
                        cout << endl;
                    }
                }
            }
        }

        cout << endl;
        cout
            << "Choose a seat from 1 to 100: ";
        cin >>
            seatID[l];
        for (int x = 0; x < 6; x++)
        {
            if (movies[mov].time_cinema[time] == cinemas[x].timing)
            {
                for (int i = 0; i < 100; i++)
                {
                    if (cinemas[x].seats[i] == seatID[l])
                    {
                        cinemas[x].seats[i] = 0;
                    }
                }
            }
        }
        movies[mov].seats--;

        for (int k = 0; (k < n_tickets[q].ticketCount); k++)
        {
            n_tickets[q].seat_no[k] = seatID[k];
        }
    }
}
//performs dynamic array resizing to prepare input of new ticket
//
void bookMovie()
{
    system("cls");
    displayMovies();

    int y = 1;

    bookedTickets++;
    delete[] n_tickets;
    n_tickets = new Ticket[bookedTickets];
    for (int i = 0; (i < bookedTickets - y); i++)
    {
        n_tickets[i].cust_name = tickets[i].cust_name;
        n_tickets[i].mov_name = tickets[i].mov_name;
        n_tickets[i].timeandcinema = tickets[i].timeandcinema;
        n_tickets[i].ticketCount = tickets[i].ticketCount;
        n_tickets[i].age = tickets[i].age;
        n_tickets[i].totalCost = tickets[i].totalCost;
        for (int k = 0; k < tickets[i].ticketCount; k++)
        {
            n_tickets[i].seat_no[k] = tickets[i].seat_no[k];
        }
    }

    for (int i = 0; i < y; i++)
    {
        inputTicket(n_tickets, bookedTickets - y + i);
    }

    delete[] tickets;
    tickets = new Ticket[bookedTickets];

    for (int i = 0; i < bookedTickets; i++)
    {
        tickets[i].cust_name = n_tickets[i].cust_name;
        tickets[i].mov_name = n_tickets[i].mov_name;
        tickets[i].timeandcinema = n_tickets[i].timeandcinema;
        tickets[i].ticketCount = n_tickets[i].ticketCount;
        tickets[i].age = n_tickets[i].age;
        tickets[i].totalCost = n_tickets[i].totalCost;
        for (int k = 0; (k < n_tickets[i].ticketCount); k++)
        {
            tickets[i].seat_no[k] = n_tickets[i].seat_no[k];
        }
    }
    cout << "Successfully Added Ticket" << endl;

    cout << "Your Ticket Is: " << endl;
    cout << setw(50) << " ";
    for (int i = 0; i <= 49; i++)
    {
        cout << "*";
    }
    cout << endl
         << setw(50) << " "
         << "|" << setw(27) << "       Cinema name"
         << " \t\t\t   |" << endl;
    cout << setw(50) << " "
         << "|";
    for (int i = 0; i <= 47; i++)
    {
        cout << "=";
    }
    cout << "|" << endl;
    // cout << setw(50) << " "
    //      << "|"
    //      << "Date : " << timePtr->tm_mday << "-" << timePtr->tm_mon + 1
    //      << "-" << timePtr->tm_year + 1900 << "\tTime : " << timePtr->tm_hour << ":" << timePtr->tm_min << setw(16) << right << "|" << endl;

    cout << setw(50) << " "
         << "|"
         << "Name:  " << setw(21) << left << tickets[bookedTickets - 1].cust_name
         << "   "
         << "Age  :" << setw(11) << left << tickets[bookedTickets - 1].age << "|" << endl;
    cout << setw(50) << " "
         << "|"
         << "Movie:  " << setw(40) << left << tickets[bookedTickets - 1].mov_name << "|" << endl;
    cout << setw(50) << " "
         << "|"
         << "Time&cinema: " << setw(35) << left << tickets[bookedTickets - 1].timeandcinema << "|" << endl;
    cout << setw(50) << " "
         << "|"
         << "Ticket purchased:  " << setw(29) << left << tickets[bookedTickets - 1].ticketCount << "|" << endl;
    cout << setw(50) << " "
         << "|"
         << "Seats booked:  ";
    for (int k = 0; k < tickets[bookedTickets - 1].ticketCount; k++)
    {

        cout << tickets[bookedTickets - 1].seat_no[k] << ",";
    }
    cout << endl;
    cout << setw(50) << " "
         << "|" << setw(40) << right << "Total: " << setw(3) << tickets[bookedTickets - 1].totalCost << "birr "
         << "|" << endl;

    cout << setw(50) << " ";
    for (int i = 0; i <= 49; i++)
    {
        cout << "*";
    }
    cout << endl;
    updateTicket();
    updateCinema();
    system("pause");
    if (currentSession == "Guest")
    {
        mainMenu();
    }
    else
    {
        customerMenu();
    }
}

//displays ticket of currently logged in customer
//
void displayTicket()
{
    bool found = false;
    for (int i = 0; i < bookedTickets; i++)
    {
        if (customers[loginIndex].name == tickets[i].cust_name)
        {
            found = true;
            cout << "Your Ticket Is: " << endl;
            cout << setw(50) << " ";
            for (int i = 0; i <= 49; i++)
            {
                cout << "*";
            }
            cout << endl
                 << setw(50) << " "
                 << "|" << setw(27) << "       Cinema name"
                 << " \t\t\t   |" << endl;
            cout << setw(50) << " "
                 << "|";
            for (int i = 0; i <= 47; i++)
            {
                cout << "=";
            }
            cout << "|" << endl;
            // cout << setw(50) << " "
            //      << "|"
            //      << "Date : " << timePtr->tm_mday << "-" << timePtr->tm_mon + 1
            //      << "-" << timePtr->tm_year + 1900 << "\tTime : " << timePtr->tm_hour << ":" << timePtr->tm_min << setw(16) << right << "|" << endl;
            cout << setw(50) << " "
                 << "|"
                 << "Name:  " << setw(21) << left << tickets[i].cust_name
                 << "   "
                 << "Age  :" << setw(11) << left << tickets[i].age << "|" << endl;
            cout << setw(50) << " "
                 << "|"
                 << "Movie:  " << setw(40) << left << tickets[i].mov_name << "|" << endl;
            cout << setw(50) << " "
                 << "|"
                 << "Time&cinema: " << setw(35) << left << tickets[i].timeandcinema << "|" << endl;
            cout << setw(50) << " "
                 << "|"
                 << "Ticket purchased:  " << setw(29) << left << tickets[i].ticketCount << "|" << endl;
            cout << setw(50) << " "
                 << "|"
                 << "Seats booked:  ";
            for (int k = 0; k < tickets[i].ticketCount; k++)
            {

                cout << tickets[i].seat_no[k] << ",";
            }
            cout << endl;
            cout << setw(50) << " "
                 << "|" << setw(40) << right << "Total: " << setw(3) << tickets[i].totalCost << "birr "
                 << "|" << endl;

            cout << setw(50) << " ";
            for (int i = 0; i <= 49; i++)
            {
                cout << "*";
            }
            cout << endl;
        }
        else
        {
            continue;
        }
    }
    if (!found)
    {
        cout << "You have not booked any tickets yet" << endl;
    }
    cout << "1.Back" << endl
         << "2. Clear Reservations?" << endl;
    cin >> choice;
    if (choice == "1")
    {
        customerMenu();
    }
    else if (choice == "2")
    {
        for (int k = 0; k < bookedTickets; k++)
        {
            if (customers[loginIndex].name == tickets[k].cust_name)
            {
                deleteTicket(k);
            }
        }
    }
}

// Functon to show all the booked tickets to an Admin
//
void displayTicketList()
{
    for (int i = 0; i <= 147; i++)
    {
        cout << "-";
    }
    cout << endl
         << "|" << left << setw(3) << "No"
         << "|"
         << setw(25) << "Customer Name"
         << "|"
         << setw(5) << "Age"
         << "|"
         << setw(25) << "Movie Name"
         << "|"
         << setw(16) << "Time and Cinema"
         << "|"
         << setw(6) << "Amount"
         << "|"
         << setw(20) << " "
         << "Seat No" << setw(20) << " " << setw(2) << " " << setw(10) << "|Total Cost" << setw(1) << "|" << endl;
    for (int i = 0; i <= 147; i++)
    {
        cout << "-";
    }
    cout << endl;
    for (int i = 0; i < bookedTickets; i++)
    {
        cout << "|" << setw(3) << left << i + 1;
        cout << "|" << setw(25) << tickets[i].cust_name;
        cout << "|" << setw(5) << tickets[i].age;
        cout << "|" << setw(25) << tickets[i].mov_name;
        cout << "|" << setw(16) << tickets[i].timeandcinema;
        cout << "|" << setw(6) << tickets[i].ticketCount;
        for (int k = 0; k < tickets[i].ticketCount; k++)
        {

            cout << "|" << setw(4) << tickets[i].seat_no[k];
        }
        for (int j = 0; j < (10 - tickets[i].ticketCount); j++)
        {
            cout << "|" << setw(4) << " ";
        }

        cout << "|" << setw(10) << tickets[i].totalCost;
        cout << "|";
        cout << endl;
    }
    for (int i = 0; i <= 147; i++)
    {
        cout << "-";
    }
    cout << endl;
}

//Admin side menu for the Movies for general management
//
void modMovies()
{
    float revenue = 0;

    for (int i = 0; i < movieCount; i++)
    {
        revenue += (movies[i].price * movies[i].seats);
    }
    system("cls");
    displayMovies();

    cout << "Total Revenue from current movies is: " << revenue << endl
         << "Select an action" << endl
         << "1. Add movie" << endl
         << "2. Delete movie" << endl
         << "3. Edit Movies" << endl
         << "4. Back" << endl
         << "5. Exit" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                addMovie();
                modMovies();
                break;
            }
            else if (choice == "2")
            {
                system("cls");
                deleteMovChoice();
                break;
            }
            else if (choice == "3")
            {
                int i;
                cout << "Choose a Movie: ";
                cin >> i;
                i--;
                editMovie(i);
                modMovies();
                break;
            }
            else if (choice == "4")
            {
                adminMenu();
                break;
            }
            else if (choice == "5")
            {
                exit(0);
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//Displays a single movie entry from a passed index
//
void displaySingleMovie(int i)
{
    for (int i = 0; i <= 135; i++)
    {
        cout << "-";
    }
    cout << endl
         << "|" << left << setw(3) << "No"
         << setw(25) << "Name"
         << setw(10) << "Genre"
         << setw(20) << "Director"
         << setw(10) << "Rating"
         << setw(10) << "Price"
         << setw(16) << "Time and Cinema"
         << setw(2) << " " << setw(10) << "Available Seats"
         << "   |" << endl;

    cout << "|" << setw(3) << left << i + 1
         << setw(25) << movies[i].name
         << setw(10) << movies[i].genre
         << setw(20) << movies[i].director
         << setw(10) << movies[i].rating
         << setw(10) << movies[i].price
         << setw(16) << movies[i].time_cinema[0] << "| "
         << setw(4) << " " << setw(10) << movies[i].seats << "        |" << endl;
    for (int i = 0; i <= 135; i++)
    {
        cout << "-";
    }
    cout << endl;
}

//receives input for new movie entries
//
void inputMovie(Movie movies[], int i = 0)
{
    int choice;
    cout << "Input Movie Info" << endl;
    cout << "------------------------" << endl;
    cout << "Enter Movie Name: ";
    getline(cin >> ws, movies[i].name);

    cout << "Enter Movie Genre: ";
    getline(cin >> ws, movies[i].genre);

    cout << "Enter Director: ";
    getline(cin >> ws, movies[i].director);

    cout << "Enter Movie Rating: ";
    cin >> movies[i].rating;

    cout << "Enter Price: ";
    cin >> movies[i].price;

    movies[i].seats = 100;

    for (int i = 0; i < 6; i++)
    {
        cout << i + 1 << ". " << cinemas[i].timing << endl;
    }
    while (true)
    {
        int menuChoice;
        cout << "Select Time and Cinema: ";
        cin >> choice;
        choice--;
        if (cinemas[choice].movie != "null")
        {
            cout << "This Cinema is already showing " << cinemas[choice].movie << endl;
            cout << "1. Remove Movie" << endl
                 << "2. Choose another Cinema" << endl;
            cin >> menuChoice;
            if (menuChoice == 1)
            {
                for (int m = 0; m < movieCount; m++)
                {
                    if (cinemas[choice].movie == movies[m].name)
                    {
                        movDelIndex = m;
                        cout << "Pass 1" << endl;
                    }
                }
                // for (int z = 0; z < bookedTickets; z++)
                // {
                //     if (cinemas[choice].movie == tickets[z].mov_name)
                //     {
                //         deleteTicket(z);
                //         cout << "Pass 2" << endl;
                //     }
                // }
                for (int j = 0; j < 100; j++)
                {
                    cinemas[choice].seats[j] = j + 1;
                }
                cout << "Pass 3" << endl;
                break;
            }
            else if (menuChoice == 2)
            {
                continue;
            }
        }
        else
        {
            break;
        }
    }
    cout << "reached";
    movies[i].time_cinema[0] = cinemas[choice].timing;
    cout << "reached 2";
    cinemas[choice].movie = movies[i].name;
    cout << "reached 3";
}

//performs dynamic array resizing to prepare for new entry
//
void addMovie()
{
    cout << "How many inputs: ";
    int j;
    cin >> j;

    movieCount += j;
    delete[] n_movies;
    cout << "step 1" << endl;
    n_movies = new Movie[movieCount];
    for (int i = 0; i < (movieCount - j); i++)
    {
        n_movies[i].name = movies[i].name;
        n_movies[i].genre = movies[i].genre;
        n_movies[i].director = movies[i].director;
        n_movies[i].rating = movies[i].rating;
        n_movies[i].price = movies[i].price;
        n_movies[i].seats = movies[i].seats;
        n_movies[i].time_cinema[0] = movies[i].time_cinema[0];
    }
    cout << "step 2" << endl;
    for (int x = 0; x < j; x++)
    {
        inputMovie(n_movies, movieCount - j + x);
    }
    cout << "reached 4";
    delete[] movies;
    movies = new Movie[movieCount];

    for (int i = 0; i < movieCount; i++)
    {
        movies[i].name = n_movies[i].name;
        movies[i].genre = n_movies[i].genre;
        movies[i].director = n_movies[i].director;
        movies[i].rating = n_movies[i].rating;
        movies[i].price = n_movies[i].price;
        movies[i].seats = n_movies[i].seats;
        for (int k = 0; k < 1; k++)
        {
            movies[i].time_cinema[k] = n_movies[i].time_cinema[k];
        }
    }
    cout << "reached 5";
    if (movDelIndex != -1)
    {
        deleteMovie(movDelIndex);
    }
    updateMovie();
    updateCinema();
    modMovies();
}

//performs deletion of a single entry on movies
//
void deleteMovInput()
{
    displayMovies();
    int m;
    cout << endl
         << "Which Movie index would you like to delete: ";
    cin >> m;
    m--;
    deleteMovie(m);
}

void deleteMovie(int m)
{
    for (int i = 0; i < 6; i++)
    {
        if (movies[m].name == cinemas[i].movie)
        {
            cinemas[i].movie = "null";
            for (int j = 0; j < 100; j++)
            {
                cinemas[i].seats[j] = j + 1;
            }
        }
    }

    int temp, n = 0;
    temp = movieCount;
    movieCount--;
    delete[] n_movies;
    n_movies = new Movie[movieCount];

    for (int z = 0; z < temp; z++)
    {
        if (z != m)
        {
            n_movies[n].name = movies[z].name;
            n_movies[n].genre = movies[z].genre;
            n_movies[n].director = movies[z].director;
            n_movies[n].rating = movies[z].rating;
            n_movies[n].price = movies[z].price;
            n_movies[n].seats = movies[z].seats;
            for (int l = 0; l < 1; l++)
            {
                n_movies[n].time_cinema[l] = movies[z].time_cinema[l];
            }
            n++;
        }
    }

    delete[] movies;
    movies = new Movie[movieCount];
    for (int p = 0; p < movieCount; p++)
    {
        movies[p].name = n_movies[p].name;
        movies[p].genre = n_movies[p].genre;
        movies[p].director = n_movies[p].director;
        movies[p].rating = n_movies[p].rating;
        movies[p].price = n_movies[p].price;
        movies[p].seats = n_movies[p].seats;
        for (int q = 0; q < 1; q++)
        {
            movies[p].time_cinema[q] = n_movies[p].time_cinema[q];
        }
    }
    updateMovie();
    updateCinema();
    cout << "Movie Successfully Deleted, Going Back to Menu" << endl;
    Sleep(2000);
}

//Delete Ticket
//

void ticketDelInput()
{
    displayTicketList();
    int m;
    cout << endl
         << "Which ticket index would you like to delete: ";
    cin >> m;
    m--;
    deleteTicket(m);
}

void deleteTicket(int m)
{
    delAmt = tickets[m].ticketCount;
    delete[] clearIndex;
    clearIndex = new int[delAmt];
    string m_name = tickets[m].mov_name;
    for (int x = 0; x < delAmt; x++)
    {
        clearIndex[x] = tickets[m].seat_no[x];
    }

    int temp, n = 0;
    temp = bookedTickets;
    bookedTickets--;
    delete[] n_tickets;
    n_tickets = new Ticket[bookedTickets];

    for (int z = 0; z < temp; z++)
    {
        if (z != m)
        {
            n_tickets[n].cust_name = tickets[z].cust_name;
            n_tickets[n].mov_name = tickets[z].mov_name;
            n_tickets[n].timeandcinema = tickets[z].timeandcinema;
            n_tickets[n].age = tickets[z].age;
            n_tickets[n].ticketCount = tickets[z].ticketCount;
            n_tickets[n].totalCost = tickets[z].totalCost;
            for (int l = 0; l < 10; l++)
            {
                n_tickets[n].seat_no[l] = tickets[z].seat_no[l];
            }
            n++;
        }
    }

    delete[] tickets;
    tickets = new Ticket[bookedTickets];
    for (int p = 0; p < bookedTickets; p++)
    {
        tickets[p].cust_name = n_tickets[p].cust_name;
        tickets[p].mov_name = n_tickets[p].mov_name;
        tickets[p].timeandcinema = n_tickets[p].timeandcinema;
        tickets[p].age = n_tickets[p].age;
        tickets[p].ticketCount = n_tickets[p].ticketCount;
        tickets[p].totalCost = n_tickets[p].totalCost;
        for (int q = 0; q < 10; q++)
        {
            tickets[p].seat_no[q] = n_tickets[p].seat_no[q];
        }
    }
    updateTicket();

    for (int a = 0; a < 6; a++)
    {
        if (m_name == cinemas[a].movie)
        {
            for (int b = 0; b < delAmt; b++)
            {
                for (int c = 0; c < 100; c++)
                {
                    if (clearIndex[b] == c + 1)
                    {
                        cinemas[a].seats[c] = clearIndex[b];
                    }
                }
            }
        }
    }
    for (int d = 0; d < movieCount; d++)
    {
        if (m_name == movies[d].name)
        {
            movies[d].seats += delAmt;
        }
    }

    updateCinema();
    updateMovie();
    cout << "Ticket Successfully Deleted, Going Back to Menu" << endl;
    Sleep(2000);
}

//Menu to choose attributes for editing a movie entry
//
void editMovie(int i)
{
    system("cls");
    displaySingleMovie(i);
    cout << "Which attribute would you like to modify: " << endl
         << "1. Name" << endl
         << "2. Genre" << endl
         << "3. Director" << endl
         << "4. Price" << endl
         << "5. Rating" << endl
         << "6. Time and Cinema" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                string temp;
                temp = movies[i].name;
                cout << "Enter the new value: ";
                getline(cin >> ws, movies[i].name);
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateMovie();
                for (int j = 0; j < 6; j++)
                {
                    if (temp == cinemas[j].movie)
                    {
                        cinemas[j].movie = movies[i].name;
                    }
                }
                updateCinema();
                modMovies();
            }
            else if (choice == "2")
            {
                cout << "Enter the new value: ";
                getline(cin >> ws, movies[i].genre);
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateMovie();
                modMovies();
            }
            else if (choice == "3")
            {
                cout << "Enter the new value: ";
                getline(cin >> ws, movies[i].director);
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateMovie();
                modMovies();
            }
            else if (choice == "4")
            {
                cout << "Enter the new value: ";
                cin >> movies[i].price;
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateMovie();
                modMovies();
            }
            else if (choice == "5")
            {
                cout << "Enter the new value: ";
                cin >> movies[i].rating;
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateMovie();
                modMovies();
            }
            else if (choice == "6")
            {
                cout << "Enter the new values: ";
                for (int j = 0; j < 1; j++)
                {
                    getline(cin >> ws, movies[i].time_cinema[j]);
                    cout << endl;
                }
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateMovie();
                modMovies();
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
    }
}

//Admin side menu for customers general management
//
void modCustomers()
{
    system("cls");
    displayCustomers();
    cout << "Select an Action" << endl
         << "1. Add customer" << endl
         << "2. Delete customer" << endl
         << "3. Edit customers" << endl
         << "4. Back" << endl
         << "5. Exit" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                addCustomer();
                system("cls");
                modCustomers();
                break;
            }
            else if (choice == "2")
            {
                system("cls");
                deleteCustChoice();
                break;
            }
            else if (choice == "3")
            {
                int i;
                cout << "Choose a Customer: ";
                cin >> i;
                i--;
                editCustomer(i);
                modCustomers();
                break;
            }
            else if (choice == "4")
            {
                adminMenu();
                break;
            }
            else if (choice == "5")
            {
                exit(0);
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//Admin side function to display all registered customers in the cinema
//
void displayCustomers()
{
    for (int i = 0; i <= 120; i++)
    {
        cout << "-";
    }

    cout << endl
         << "|" << left << setw(3) << "No"
         << setw(30) << "Name"
         << "|"
         << setw(15) << "Age"
         << "|"
         << setw(30) << "Phone"
         << "|"
         << setw(38) << "Address"
         << "|"
         << endl;
    for (int i = 0; i < customerCount; i++)
    {
        cout << "|" << setw(3) << left << i + 1
             << setw(30) << customers[i].name << "|"
             << setw(15) << customers[i].age << "|"
             << setw(30) << customers[i].phone << "|"
             << setw(38) << customers[i].address << "|"
             << endl;
    }

    for (int i = 0; i <= 120; i++)
    {
        cout << "-";
    }
    cout << endl;
}

//displays a single customer entry based on passed index
//
void displaySingleCustomer(int i)
{
    for (int i = 0; i <= 135; i++)
    {
        cout << "-";
    }
    cout << endl
         << "|" << left << setw(3) << "No"
         << setw(30) << "Name"
         << setw(15) << "Age"
         << setw(30) << "Phone"
         << setw(10) << "Address" << setw(47) << "                                              |"
         << endl;
    cout << "|" << setw(3) << left << i + 1
         << setw(30) << customers[i].name
         << setw(15) << customers[i].age
         << setw(30) << customers[i].phone
         << setw(10) << customers[i].address << setw(47) << "                                              |"
         << endl;
    for (int i = 0; i <= 135; i++)
    {
        cout << "-";
    }
}

//receives input for new customer entry
//
void inputCustomer(Customer customers[], int i = 0)
{
    cout << "Input Customer Info" << endl;
    cout << "------------------------" << endl;
    cout << "Enter name: ";
    getline(cin >> ws, customers[i].name);

    cout << "Enter Age: ";
    cin >> customers[i].age;

    cout << "Enter Phone: ";
    cin >> customers[i].phone;

    cout << "Enter Address: ";
    getline(cin >> ws, customers[i].address);

    cout << "Enter Username: ";
    getline(cin >> ws, customers[i].username);

    cout << "Enter Password: ";
    getline(cin >> ws, customers[i].password);
}

//performs dynamic array resizing to prepare for new entry
//
void addCustomer()
{
    int j = 1;

    customerCount++;
    delete[] n_customers;
    n_customers = new Customer[customerCount];
    for (int i = 0; i < (customerCount - j); i++)
    {
        n_customers[i].name = customers[i].name;
        n_customers[i].age = customers[i].age;
        n_customers[i].phone = customers[i].phone;
        n_customers[i].address = customers[i].address;
        n_customers[i].username = customers[i].username;
        n_customers[i].password = customers[i].password;
    }

    for (int i = 0; i < j; i++)
    {
        inputCustomer(n_customers, customerCount - j);
    }
    delete[] customers;
    customers = new Customer[customerCount];

    for (int i = 0; i < customerCount; i++)
    {
        customers[i].name = n_customers[i].name;
        customers[i].age = n_customers[i].age;
        customers[i].phone = n_customers[i].phone;
        customers[i].address = n_customers[i].address;
        customers[i].username = n_customers[i].username;
        customers[i].password = n_customers[i].password;
    }
    updateCustomer();
}

//removes a single entry from the customers list
//
void deleteCustomer()
{
    system("cls");
    displayCustomers();
    int m, temp, n = 0;
    cout << endl
         << "Which Customer would you like to remove: ";
    cin >> m;
    m--;
    temp = customerCount;
    customerCount--;
    delete[] n_customers;
    n_customers = new Customer[customerCount];

    for (int z = 0; z < temp; z++)
    {
        if (z != m)
        {
            n_customers[n].name = customers[z].name;
            n_customers[n].age = customers[z].age;
            n_customers[n].phone = customers[z].phone;
            n_customers[n].address = customers[z].address;
            n_customers[n].username = customers[z].username;
            n_customers[n].password = customers[z].password;
            n++;
        }
    }

    delete[] customers;
    customers = new Customer[customerCount];
    for (int p = 0; p < customerCount; p++)
    {
        customers[p].name = n_customers[p].name;
        customers[p].age = n_customers[p].age;
        customers[p].phone = n_customers[p].phone;
        customers[p].address = n_customers[p].address;
        customers[p].username = n_customers[p].username;
        customers[p].password = n_customers[p].password;
    }

    updateCustomer();
    cout << "Customer Successfully Removed, Going Back to Menu" << endl;
    Sleep(2000);
}

//performs a delete on the customers array to remove all customer entries at once
//
void nullifyCustomerlist()
{
    delete[] customers;
    customerCount = 0;
    customers = new Customer[customerCount];

    cout << "Successfully Deleted all Entries, Going back to menu" << endl;
    Sleep(2000);
    updateCustomer();
    modCustomers();
}

//performs a delete on the movies array to remove all movie entries at once
//
void nullifyMovieList()
{
    delete[] movies;
    movieCount = 0;
    movies = new Movie[movieCount];

    cout << "Successfully Deleted all Entries, Going back to menu" << endl;
    Sleep(2000);
    updateMovie();
    for (int i = 0; i < 6; i++)
    {
        cinemas[i].movie = "null";
        for (int j = 0; j < 100; j++)
        {
            cinemas[i].seats[j] = j + 1;
        }
    }
    updateCinema();
    modMovies();
}

//confirmation page for deleting all customer entries
//
void deleteAllCustomer()
{
    cout
        << "Are you sure you want to remove all entries?" << endl
        << "1. Yes" << endl
        << "2. No" << endl;
    while (true)
    {
        cout << "Enter your Choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                nullifyCustomerlist();
                break;
            }
            else if (choice == "2")
            {
                deleteCustChoice();
                system("pause");
                modCustomers();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//confirmation page for deleting all movie entries
//
void deleteAllMovie()
{
    cout
        << "Are you sure you want to remove all entries?" << endl
        << "1. Yes" << endl
        << "2. No" << endl;
    while (true)
    {
        cout << "Enter your Choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                nullifyMovieList();
                break;
            }
            else if (choice == "2")
            {
                deleteMovChoice();
                system("pause");
                modMovies();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//choice to delete one or all customers
//
void deleteCustChoice()
{
    system("cls");
    cout << "Select a Delete Option" << endl;
    cout << "1. Single Entry" << endl
         << "2. All Entries" << endl
         << "3. Back" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                deleteCustomer();
                modCustomers();
                break;
            }
            else if (choice == "2")
            {
                deleteAllCustomer();
                cout << "All Entries Successfully deleted";
                Sleep(2000);
                modCustomers();
                break;
            }
            else if (choice == "3")
            {
                modCustomers();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//choice to delete one or all movies
//
void deleteMovChoice()
{
    system("cls");
    cout << "Select a Delete Option" << endl;
    cout << "1. Single Entry" << endl
         << "2. All Entries" << endl
         << "3. Back" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                deleteMovInput();
                modMovies();
                break;
            }
            else if (choice == "2")
            {
                deleteAllMovie();
                cout << "All Entries Successfully deleted";
                Sleep(2000);
                modMovies();
                break;
            }
            else if (choice == "3")
            {
                modMovies();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//Menu to choose attributes for editing a customer entry
//
void editCustomer(int i)
{
    system("cls");
    displaySingleCustomer(i);
    cout << "Which attribute would you like to modify: " << endl
         << "1. Name" << endl
         << "2. Age" << endl
         << "3. Address" << endl
         << "4. Phone Number" << endl
         << "5. Username" << endl
         << "6. Password" << endl;
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                cout << "Enter the new value: ";
                getline(cin >> ws, customers[i].name);
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateCustomer();
                modCustomers();
            }
            else if (choice == "2")
            {
                cout << "Enter the new value: ";
                cin >> customers[i].age;
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateCustomer();
                modCustomers();
            }
            else if (choice == "3")
            {
                cout << "Enter the new value: ";
                getline(cin >> ws, customers[i].address);
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateCustomer();
                modCustomers();
            }
            else if (choice == "4")
            {
                cout << "Enter the new value: ";
                cin >> customers[i].phone;
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateCustomer();
                modCustomers();
            }
            else if (choice == "5")
            {
                cout << "Enter the new value: ";
                getline(cin >> ws, customers[i].username);
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateCustomer();
                modCustomers();
            }
            else if (choice == "6")
            {
                cout << "Enter the new value: ";
                getline(cin >> ws, customers[i].password);
                cout << "Value Successfully Changed, Going back to menu";
                Sleep(1000);
                updateCustomer();
                modCustomers();
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
    }
}

//searches movies based on genre provided
//
void searchMovie(string s)
{
    cout << left << setw(3) << "No"
         << setw(25) << "Name"
         << setw(10) << "Genre"
         << setw(20) << "Director"
         << setw(7) << "Rating"
         << setw(7) << "Price"
         << setw(16) << "Time and Cinema"
         << setw(2) << " " << setw(10) << "Available Seats" << endl;
    for (int i = 0; i < movieCount; i++)
    {
        if (movies[i].genre == s)
        {
            cout << setw(3) << left << i + 1
                 << setw(25) << movies[i].name
                 << setw(10) << movies[i].genre
                 << setw(20) << movies[i].director
                 << setw(7) << movies[i].rating
                 << setw(7) << movies[i].price
                 << setw(16) << movies[i].time_cinema[0] << " | "
                 << setw(2) << " " << setw(10) << movies[i].seats << endl;
        }
    }
}

//menu for choosing sort method
//
void sort()
{
    system("cls");
    cout << "Choose a Category" << endl
         << "1. Action" << endl
         << "2. Adventure" << endl
         << "3. Horror" << endl
         << "4. Animation" << endl
         << "5. Back" << endl
         << endl;

    while (true)
    {
        cout << "Enter your choice: ";
        cin >> *choicePtr;
        if (floatCheck(choice))
        {
            if (choice == "1")
            {
                string s = "Action";
                searchMovie(s);
                system("pause");
                customerMenu();
                break;
            }

            else if (choice == "2")
            {
                string s = "Adventure";
                searchMovie(s);
                system("pause");
                customerMenu();
                break;
            }
            else if (choice == "3")
            {
                string s = "Horror";
                searchMovie(s);
                system("pause");
                customerMenu();
                break;
            }
            else if (choice == "4")
            {
                string s = "Animation";
                searchMovie(s);
                system("pause");
                customerMenu();
                break;
            }
            else if (choice == "5")
            {
                customerMenu();
                break;
            }
            else
                cout << "Please Enter a Valid Number" << endl;
            continue;
        }
        else
        {
            cout << "Please Enter a Number" << endl;
            continue;
        }
    }
}

//function to check if string inputs are integers for error handling
//
bool floatCheck(string a)
{ // Checks whether a given string contains a float or not

    int len = 0;

    for (int j = 0; a[j]; j++)
    {
        len++;
    }

    for (int i = 0; a[i]; i++)
    {

        if ((int(a[i]) > 58 || int(a[i]) < 48) && int(a[i]) != 46)
        {
            return false;
        }
        if (i == len - 1)
        {
            if ((int(a[i]) < 58 && int(a[i]) >= 48))
            {
                return true;
            }
        }
    }
    return true;
}