#include <iostream>
#include <fstream>
#include <string>
#include "cls.h"
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

class User
{
protected:
    string username;
    string password;

public:
    User() {}
    User(string uname, string pwd)
        : username(uname),
          password(pwd) {}
    virtual bool login(const string &uname, const string &pwd) = 0;
};

class RMS
{
protected:
    map<string, vector<string>> a_trains;
    map<string, vector<string>> t_details;

public:
    bool checkAvailability(string departure, string arrival, string date, int numTickets)
    {
        string route = departure + "-" + arrival;
        if (a_trains.find(route) == a_trains.end())
        {
            cout << "route not available...!" << endl;
            return false;
        }

        if (a_trains[route].size() < numTickets)
        {
            cout << "Insufficient seats available." << endl;
            return false;
        }

        cout << "Tickets are available for " << numTickets << " passengers." << endl;
        return true;
    }

    double calculateprice(string departure, string arrival, int numTickets)
    {
        double baseprice = 100.0;
        double totalprice = baseprice * numTickets;
        return totalprice;
    }

    void bookTickets(string departure, string arrival, string date, int numTickets)
    {
        string route = departure + "-" + arrival;
        if (a_trains.find(route) == a_trains.end())
        {
            cout << "Route not available. Cannot book tickets " << endl;
            return;
        }

        if (a_trains[route].size() < numTickets)
        {
            cout << "Insufficient seat available for booking....." << endl;
            return;
        }

        cout << "Tickets booked successfully!" << endl;
    }

    void displayPassengerDetails()
    {
        cout << "Passenger Details:" << endl;

        for (const auto &ticket : t_details)
        {
            cout << "PNR: " << ticket.first << endl;
            cout << "Passenger Details:" << endl;
            for (const auto &passenger : ticket.second)
            {
                cout << passenger << endl;
            }
        }
    }
};

class Admin : public User ,
             public RMS
{
private:
    static const string adminfile;

public:
    Admin(string uname, string pwd) : User(uname, pwd) {}

    bool login(const string &uname, const string &pwd) override
    {
        ifstream file(adminfile);
        if (!file.is_open())
        {
            cerr << "Error: File not found." << endl;
            return false;
        }

        string line;
        while (getline(file, line))
        {
            if (line == username + " " + password)
            {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    void changeTicketAvailability()
    {
        string departure, arrival, date;
        int newAvailability;

        cout << "Enter departure city: ";
        cin >> departure;
        cout << "Enter arrival city: ";
        cin >> arrival;
        cout << "Enter date (DD/MM/YYYY): ";
        cin >> date;
        cout << "Enter new availability: ";
        cin >> newAvailability;

        string route = departure + "-" + arrival;
        if (a_trains.find(route) == a_trains.end())
        {
            cout << "Route not available. Cannot change availability." << endl;
            return;
        }

        // Update ticket availability for the specified route and date
        a_trains[route][date] = to_string(newAvailability);

        cout << "Ticket availability updated successfully!" << endl;
    }
};

const string Admin::adminfile = "admin.txt";

class Passenger : public User, public RMS
{
public:
    static const string passengerfile;

public:
    Passenger(string uname, string pwd)
        : User(uname, pwd)
    {
    }

    bool login(const string &uname, const string &pwd) override
    {
        ifstream file(passengerfile);
        if (!file.is_open())
        {
            cerr << "Error: File not found." << endl;
            return false;
        }

        string line;
        while (getline(file, line))
        {
            if (line == username + " " + password)
            {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    void bookTicket(string departure, string arrival, string date, int numTickets)
    {
        if (!checkAvailability(departure, arrival, date, numTickets))
        {
            cout << "Sorry, the requested tickets are not available." << endl;
            return;
        }

        double fare = calculateprice(departure, arrival, numTickets);

        // get passenger details
        vector<string> passengerDetails;
        for (int i = 0; i < numTickets; ++i)
        {
            string name, age, gender;
            cout << "Passenger " << i + 1 << " Name: ";
            cin >> name;
            cout << "Age: ";
            cin >> age;
            cout << "Gender: ";
            cin >> gender;
            passengerDetails.push_back(name + " " + age + " " + gender);
        }

        // generate PNR
        string pnr = "PNR" + to_string(rand() % 10000);

        // update ticket details
        t_details[pnr] = passengerDetails;

        // display booking details
        cout << "\nBooking Details:\n";
        cout << "PNR: " << pnr << endl;
        cout << "From: " << departure << endl;
        cout << "To: " << arrival << endl;
        cout << "Date: " << date << endl;
        cout << "Number of Tickets: " << numTickets << endl;
        cout << "Fare per Ticket: Rs" << fare << endl;
        cout << "Passenger Details:\n";
        for (const auto &passenger : passengerDetails)
        {
            cout << passenger << endl;
        }
        cout << "Total Fare: Rs" << fare * numTickets << endl;
    }
};
const string Passenger::passengerfile = "passenger.txt";

void loginpage()
{
    while (true)
    {
        clearScreen();
        cout << "=== Login Menu ===" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Passenger Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string adminUsername, adminPassword;
            cout << "Enter admin username: ";
            cin >> adminUsername;
            cout << "Enter admin password: ";
            cin >> adminPassword;

            Admin admin(adminUsername, adminPassword);

            if (admin.login(adminUsername, adminPassword))
            {
                cout << "Admin logged in successfully!" << endl;

                while (true)
                {
                    clearScreen();
                    cout << "=== Admin Menu ===" << endl;
                    cout << "1. Change Ticket Availability" << endl;
                    cout << "2. Logout" << endl;
                    cout << "Enter your choice: ";

                    int adminChoice;
                    cin >> adminChoice;

                    switch (adminChoice)
                    {
                    case 1:
                        admin.changeTicketAvailability();
                        break;
                    case 2:
                        return;
                    default:
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }

                    cout << "Press Enter to continue...";
                    cin.ignore();
                    cin.get();
                }
            }
            else
            {
                cout << "Invalid admin credentials." << endl;
            }
            break;
        }

        case 2:
        {
            string passengerUsername, passengerPassword;
            cout << "Enter passenger username: ";
            cin >> passengerUsername;
            cout << "Enter passenger password: ";
            cin >> passengerPassword;

            Passenger passenger(passengerUsername, passengerPassword);
            if (passenger.login(passengerUsername, passengerPassword))
            {
                clearScreen();
                cout << "Passenger logged in successfully!" << endl;

                while (true)
                {
                    clearScreen();
                    cout << "=== Passenger Menu ===" << endl;
                    cout << "1. Book Tickets" << endl;
                    cout << "2. Display Passenger Details" << endl;
                    cout << "3. Logout" << endl;
                    cout << "Enter your choice: ";

                    int passengerChoice;
                    cin >> passengerChoice;

                    switch (passengerChoice)
                    {
                    case 1:
                    {
                        string departure, arrival, date;
                        int numTickets;

                        cout << "Enter departure city: ";
                        cin >> departure;
                        cout << "Enter arrival city: ";
                        cin >> arrival;
                        cout << "Enter date (DD/MM/YYYY): ";
                        cin >> date;
                        cout << "Enter number of tickets: ";
                        cin >> numTickets;

                        passenger.bookTicket(departure, arrival, date, numTickets);
                        break;
                    }
                    case 2:
                        passenger.displayPassengerDetails();
                        break;
                    case 3:
                        return;
                    default:
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }

                    cout << "Press Enter to continue...";
                    cin.ignore();
                    cin.get();
                }
            }
            else
            {
                cout << "Invalid passenger credentials." << endl;
            }
            break;
        }
        case 3:
            return;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

int main()
{
    clearScreen();
    loginpage();
    return 0;
}