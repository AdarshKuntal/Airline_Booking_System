// // #include <iostream>
// // #include <string>
// // #include <sstream>
// // #include <iomanip> // For formatting output
// // #include <limits>  // For input validation
// // #include <mysql.h>
// // #include <windows.h> // For Sleep() and system("cls")

// // using namespace std;

// // // --- Database Connection Configuration ---
// // const char* HOST = "localhost";
// // const char* USER = "root";
// // const char* PW = "your password"; // <--- !!! CHANGE THIS !!!
// // const char* DB = "mydb";


// // // --- Helper Functions for Robust Input ---

// // // Robustly gets an integer choice from the user
// // int getMenuChoice() {
// //     int choice;
// //     while (true) {
// //         cout << "> Your choice: ";
// //         if (cin >> choice) {
// //             cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //             return choice;
// //         } else {
// //             cout << "Invalid input. Please enter a number." << endl;
// //             cin.clear();
// //             cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //         }
// //     }
// // }

// // // Robustly gets a non-empty string from the user
// // string getStringInput(string prompt) {
// //     string input;
// //     while (true) {
// //         cout << prompt;
// //         getline(cin, input); // get full line
// //         if (!input.empty()) {
// //             return input;
// //         } else {
// //             cout << "Input cannot be empty. Please try again." << endl;
// //         }
// //     }
// // }

// // void pressEnterToContinue() { //pausing the cli
// //     cout << "\nPress Enter to continue..." << endl;
// //     cin.get();
// // }

// // // --- Data Model Classes  ---

// // /**
// //  * @class Flight
// //  * @brief Models a single flight.
// //  */
// // class Flight {
// // public:
// //     int flight_id;
// //     string flight_number;
// //     string departure;
// //     string destination;
// //     int available_seats;
// // };

// // /**
// //  * @class Passenger
// //  * @brief Models a single passenger.
// //  */
// // class Passenger {
// // public:
// //     int passenger_id;
// //     string first_name;
// //     string last_name;
// //     string passport_number;
// // };

// // /**
// //  * @class Reservation
// //  * @brief Models a single reservation.
// //  */
// // class Reservation {
// // public:
// //     int reservation_id;
// //     int passenger_id;
// //     int flight_id;
// // };


// // /**
// //  * @class Database
// //  * @brief Encapsulates all database logic. This is the main "engine".
// //  */
// // class Database {
// // private:
// //     MYSQL* conn; // The database connection object

// //     // Helper to run simple, non-parameterized queries
// //     bool simpleQuery(string query) {
// //         if (mysql_query(conn, query.c_str())) {
// //             cout << "Error: " << mysql_error(conn) << endl;
// //             return false;
// //         }
// //         return true;
// //     }

// // public:
// //     // Constructor
// //     Database() {
// //         conn = mysql_init(NULL);
// //     }

// //     // Destructor (cleans up the connection)
// //     ~Database() {
// //         if (conn) {
// //             mysql_close(conn);
// //         }
// //     }

// //     // Connects to the database. Returns true on success.
// //     bool connect() {
// //         if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
// //             cout << "Error connecting to database: " << mysql_error(conn) << endl;
// //             return false;
// //         } else {
// //             cout << "Logged In To Database!" << endl;
// //             return true;
// //         }
// //     }

// //     // Sets up the 3-table schema (as per resume)
// //     void setupDatabase() {
// //         cout << "Setting up database..." << endl;
// //         simpleQuery("DROP TABLE IF EXISTS Reservations");
// //         simpleQuery("DROP TABLE IF EXISTS Passengers");
// //         simpleQuery("DROP TABLE IF EXISTS Flights");

// //         string createFlights = R"(
// //             CREATE TABLE Flights (
// //                 flight_id INT AUTO_INCREMENT PRIMARY KEY,
// //                 flight_number VARCHAR(10) NOT NULL UNIQUE,
// //                 departure VARCHAR(50) NOT NULL,
// //                 destination VARCHAR(50) NOT NULL,
// //                 available_seats INT NOT NULL
// //             )
// //         )";
        
// //         string createPassengers = R"(
// //             CREATE TABLE Passengers (
// //                 passenger_id INT AUTO_INCREMENT PRIMARY KEY,
// //                 first_name VARCHAR(50) NOT NULL,
// //                 last_name VARCHAR(50) NOT NULL,
// //                 passport_number VARCHAR(20) NOT NULL UNIQUE
// //             )
// //         )";

// //         string createReservations = R"(
// //             CREATE TABLE Reservations (
// //                 reservation_id INT AUTO_INCREMENT PRIMARY KEY,
// //                 passenger_id INT NOT NULL,
// //                 flight_id INT NOT NULL,
// //                 FOREIGN KEY (passenger_id) REFERENCES Passengers(passenger_id),
// //                 FOREIGN KEY (flight_id) REFERENCES Flights(flight_id)
// //             )
// //         )";

// //         if (!simpleQuery(createFlights) || !simpleQuery(createPassengers) || !simpleQuery(createReservations)) {
// //             cout << "Fatal error setting up tables. Exiting." << endl;
// //             exit(1);
// //         }

// //         // Populate with sample data
// //         simpleQuery("INSERT INTO Flights (flight_number, departure, destination, available_seats) VALUES ('F101', 'UAE', 'Canada', 50)");
// //         simpleQuery("INSERT INTO Flights (flight_number, departure, destination, available_seats) VALUES ('F102', 'UAE', 'USA', 40)");
// //         simpleQuery("INSERT INTO Flights (flight_number, departure, destination, available_seats) VALUES ('F103', 'UAE', 'China', 1)"); // For testing
        
// //         simpleQuery("INSERT INTO Passengers (first_name, last_name, passport_number) VALUES ('John', 'Doe', 'A123')");
// //         simpleQuery("INSERT INTO Passengers (first_name, last_name, passport_number) VALUES ('Jane', 'Smith', 'B456')");

// //         cout << "Database setup complete." << endl;
// //         Sleep(2000);
// //     }

// //     // (READ) Displays all flights (concise CLI report)
// //     void displayAllFlights() {
// //         if (mysql_query(conn, "SELECT flight_id, flight_number, departure, destination, available_seats FROM Flights")) {
// //             cout << "Error: " << mysql_error(conn) << endl;
// //             return;
// //         }
        
// //         MYSQL_RES* res = mysql_store_result(conn);
// //         if (res == NULL) return;

// //         MYSQL_ROW row;
// //         cout << "\n--- Available Flights ---" << endl;
// //         cout << left 
// //              << setw(10) << "ID" 
// //              << setw(15) << "Flight No."  /// reserve 15 chars
// //              << setw(15) << "Departure" 
// //              << setw(15) << "Destination" 
// //              << setw(10) << "Available" 
// //              << endl;
// //         cout << string(65, '-') << endl;

// //         while ((row = mysql_fetch_row(res))) {
// //             cout << left 
// //                  << setw(10) << row[0] 
// //                  << setw(15) << row[1] 
// //                  << setw(15) << row[2] 
// //                  << setw(15) << row[3] 
// //                  << setw(10) << row[4] 
// //                  << endl;
// //         }
// //         cout << string(65, '-') << endl;
// //         mysql_free_result(res);
// //     }

// //     // (CREATE) Adds a new passenger (robust CRUD)
// //     void addNewPassenger() {
// //         system("cls");
// //         cout << "--- Add New Passenger ---" << endl;
// //         string fname = getStringInput("Enter first name: ");
// //         string lname = getStringInput("Enter last name: ");
// //         string passport = getStringInput("Enter passport number: ");

// //         // Use Prepared Statements to prevent SQL injection
// //         MYSQL_STMT* stmt = mysql_stmt_init(conn);
// //         string query = "INSERT INTO Passengers (first_name, last_name, passport_number) VALUES (?, ?, ?)";
        
// //         if (mysql_stmt_prepare(stmt, query.c_str(), query.length())) {
// //              cout << "Error preparing statement: " << mysql_stmt_error(stmt) << endl;
// //              mysql_stmt_close(stmt);
// //              return;
// //         }

// //         MYSQL_BIND params[3];
// //         memset(params, 0, sizeof(params));

// //         params[0].buffer_type = MYSQL_TYPE_STRING;
// //         params[0].buffer = (char*)fname.c_str();
// //         params[0].buffer_length = fname.length();

// //         params[1].buffer_type = MYSQL_TYPE_STRING;
// //         params[1].buffer = (char*)lname.c_str();
// //         params[1].buffer_length = lname.length();

// //         params[2].buffer_type = MYSQL_TYPE_STRING;
// //         params[2].buffer = (char*)passport.c_str();
// //         params[2].buffer_length = passport.length();

// //         mysql_stmt_bind_param(stmt, params);
// //         if (mysql_stmt_execute(stmt)) {
// //             cout << "Error adding passenger: " << mysql_stmt_error(stmt) << endl;
// //         } else {
// //             cout << "\nPassenger '" << fname << " " << lname << "' added successfully!" << endl;
// //             cout << "New Passenger ID: " << mysql_insert_id(conn) << endl;
// //         }
// //         mysql_stmt_close(stmt);
// //     }

// //     // (UPDATE/CREATE) Books a seat (TRANSACTION-SAFE)
// //     bool bookReservation(int p_id, int f_id) {
// //         // --- TRANSACTION START ---
// //         if (simpleQuery("START TRANSACTION")) {
// //             // 1. Check for seats AND lock the row
// //             string query_check = "SELECT available_seats FROM Flights WHERE flight_id = ? FOR UPDATE";
// //             int available_seats = 0;
            
// //             MYSQL_STMT* stmt_check = mysql_stmt_init(conn);
// //             mysql_stmt_prepare(stmt_check, query_check.c_str(), query_check.length());

// //             MYSQL_BIND param_check[1];
// //             memset(param_check, 0, sizeof(param_check));
// //             param_check[0].buffer_type = MYSQL_TYPE_LONG;
// //             param_check[0].buffer = &f_id;
// //             mysql_stmt_bind_param(stmt_check, param_check);

// //             MYSQL_BIND result_check[1];
// //             memset(result_check, 0, sizeof(result_check));
// //             result_check[0].buffer_type = MYSQL_TYPE_LONG;
// //             result_check[0].buffer = &available_seats;
// //             mysql_stmt_bind_result(stmt_check, result_check);

// //             mysql_stmt_execute(stmt_check);
// //             mysql_stmt_store_result(stmt_check);

// //             if (mysql_stmt_fetch(stmt_check) == 0) { // If fetch is successful
// //                 mysql_stmt_close(stmt_check); // Close check statement

// //                 // 2. Logic: If seats are available, book them
// //                 if (available_seats > 0) {
// //                     // 2a. Update Flights table
// //                     string query_update = "UPDATE Flights SET available_seats = available_seats - 1 WHERE flight_id = ?";
// //                     MYSQL_STMT* stmt_update = mysql_stmt_init(conn);
// //                     mysql_stmt_prepare(stmt_update, query_update.c_str(), query_update.length());
// //                     mysql_stmt_bind_param(stmt_update, param_check); // Re-use param_check (f_id)
// //                     mysql_stmt_execute(stmt_update);
// //                     mysql_stmt_close(stmt_update);

// //                     // 2b. Insert into Reservations table
// //                     string query_insert = "INSERT INTO Reservations (passenger_id, flight_id) VALUES (?, ?)";
// //                     MYSQL_STMT* stmt_insert = mysql_stmt_init(conn);
// //                     mysql_stmt_prepare(stmt_insert, query_insert.c_str(), query_insert.length());

// //                     MYSQL_BIND param_insert[2];
// //                     memset(param_insert, 0, sizeof(param_insert));
// //                     param_insert[0].buffer_type = MYSQL_TYPE_LONG;
// //                     param_insert[0].buffer = &p_id;
// //                     param_insert[1].buffer_type = MYSQL_TYPE_LONG;
// //                     param_insert[1].buffer = &f_id;
// //                     mysql_stmt_bind_param(stmt_insert, param_insert);
// //                     mysql_stmt_execute(stmt_insert);

// //                     cout << "\nBooking successful! Reservation ID: " << mysql_insert_id(conn) << endl;
// //                     mysql_stmt_close(stmt_insert);

// //                     simpleQuery("COMMIT"); // 3. Commit transaction
// //                     return true;
// //                 } else {
// //                     cout << "\nSorry, that flight is full. Rolling back." << endl;
// //                     simpleQuery("ROLLBACK");
// //                 }
// //             } else {
// //                 cout << "\nError: Flight ID not found. Rolling back." << endl;
// //                 simpleQuery("ROLLBACK");
// //             }
// //             if(stmt_check) mysql_stmt_close(stmt_check);
// //         }
// //         return false;
// //     }

// //     // (READ) Shows reservations for a specific passenger
// //     // we check for available p id also
// //     void displayPassengerReservations(int p_id) {
// //         string query = 
// //             "SELECT r.reservation_id, f.flight_number, f.departure, f.destination "
// //             "FROM Reservations r "
// //             "JOIN Flights f ON r.flight_id = f.flight_id "
// //             "WHERE r.passenger_id = ?";

// //         MYSQL_STMT* stmt = mysql_stmt_init(conn);
// //         mysql_stmt_prepare(stmt, query.c_str(), query.length());

// //         MYSQL_BIND param[1];
// //         memset(param, 0, sizeof(param));
// //         param[0].buffer_type = MYSQL_TYPE_LONG;
// //         param[0].buffer = &p_id;
// //         mysql_stmt_bind_param(stmt, param);

// //         // Buffers for results
// //         int res_id;
// //         char flight_num[11], departure[51], destination[51];
// //         unsigned long len_flight, len_dep, len_des;

// //         MYSQL_BIND result[4];
// //         memset(result, 0, sizeof(result));
// //         result[0].buffer_type = MYSQL_TYPE_LONG; result[0].buffer = &res_id;
// //         result[1].buffer_type = MYSQL_TYPE_STRING; result[1].buffer = flight_num; result[1].buffer_length = 11; result[1].length = &len_flight;
// //         result[2].buffer_type = MYSQL_TYPE_STRING; result[2].buffer = departure; result[2].buffer_length = 51; result[2].length = &len_dep;
// //         result[3].buffer_type = MYSQL_TYPE_STRING; result[3].buffer = destination; result[3].buffer_length = 51; result[3].length = &len_des;
        
// //         mysql_stmt_bind_result(stmt, result);
// //         mysql_stmt_execute(stmt);
// //         mysql_stmt_store_result(stmt);

// //         if (mysql_stmt_num_rows(stmt) == 0) {
// //             cout << "\nNo reservations found for Passenger ID " << p_id << "." << endl;
// //         } else {
// //             cout << "\n--- Reservations for Passenger ID " << p_id << " ---" << endl;
// //             cout << left << setw(10) << "Res. ID" << setw(15) << "Flight No." << setw(15) << "Departure" << setw(15) << "Destination" << endl;
// //             cout << string(55, '-') << endl;
// //             while (mysql_stmt_fetch(stmt) == 0) {
// //                 cout << left << setw(10) << res_id << setw(15) << flight_num << setw(15) << departure << setw(15) << destination << endl;
// //             }
// //             cout << string(55, '-') << endl;
// //         }
// //         mysql_stmt_close(stmt);
// //     }

// //     // (DELETE/UPDATE) Cancels a reservation (TRANSACTION-SAFE)
// //     bool cancelReservation(int res_id) {
// //         int f_id = 0; // To store the flight_id from the reservation

// //         // --- TRANSACTION START ---
// //         if (simpleQuery("START TRANSACTION")) {
// //             // 1. Get flight_id from reservation AND lock the row
// //             string query_check = "SELECT flight_id FROM Reservations WHERE reservation_id = ? FOR UPDATE";
// //             MYSQL_STMT* stmt_check = mysql_stmt_init(conn);
// //             mysql_stmt_prepare(stmt_check, query_check.c_str(), query_check.length());

// //             MYSQL_BIND param_check[1];
// //             memset(param_check, 0, sizeof(param_check));
// //             param_check[0].buffer_type = MYSQL_TYPE_LONG;
// //             param_check[0].buffer = &res_id;
// //             mysql_stmt_bind_param(stmt_check, param_check);
// // //returning associated flight id with res id ->result
// //             MYSQL_BIND result_check[1];
// //             memset(result_check, 0, sizeof(result_check));
// //             result_check[0].buffer_type = MYSQL_TYPE_LONG;
// //             result_check[0].buffer = &f_id;
// //             mysql_stmt_bind_result(stmt_check, result_check);

// //             mysql_stmt_execute(stmt_check);
// //             mysql_stmt_store_result(stmt_check);

// //             if (mysql_stmt_fetch(stmt_check) == 0) { // If fetch is successful
// //                 mysql_stmt_close(stmt_check);

// //                 // 2a. Delete the reservation
// //                 string query_delete = "DELETE FROM Reservations WHERE reservation_id = ?";
// //                 MYSQL_STMT* stmt_delete = mysql_stmt_init(conn);
// //                 mysql_stmt_prepare(stmt_delete, query_delete.c_str(), query_delete.length());
// //                 mysql_stmt_bind_param(stmt_delete, param_check); // Re-use param_check (res_id)
// //                 mysql_stmt_execute(stmt_delete);
// //                 mysql_stmt_close(stmt_delete);

// //                 // 2b. Increment available_seats on the flight
// //                 string query_update = "UPDATE Flights SET available_seats = available_seats + 1 WHERE flight_id = ?";
// //                 MYSQL_STMT* stmt_update = mysql_stmt_init(conn);
// //                 mysql_stmt_prepare(stmt_update, query_update.c_str(), query_update.length());

// //                 MYSQL_BIND param_update[1];
// //                 memset(param_update, 0, sizeof(param_update));
// //                 param_update[0].buffer_type = MYSQL_TYPE_LONG;
// //                 param_update[0].buffer = &f_id;
// //                 mysql_stmt_bind_param(stmt_update, param_update);
// //                 mysql_stmt_execute(stmt_update);
// //                 mysql_stmt_close(stmt_update);

// //                 // 3. Commit
// //                 cout << "\nReservation " << res_id << " successfully cancelled." << endl;
// //                 simpleQuery("COMMIT");
// //                 return true;
// //             } else {
// //                 cout << "\nError: Reservation ID " << res_id << " not found. Rolling back." << endl;
// //                 simpleQuery("ROLLBACK");
// //             }
// //             if(stmt_check) mysql_stmt_close(stmt_check);
// //         }
// //         return false;
// //     }

// // };


// // // --- Main Application Loop ---

// // int main() {
// //     Database db; // Create the Database object

// //     if (!db.connect()) {
// //         return 1; // Exit if connection fails
// //     }

// //     db.setupDatabase(); // Set up tables and data
// //     //improve

// //     bool exit = false;
// //     while (!exit) {
// //         system("cls");
// //         cout << "\n--- Welcome To Airlines Reservation System ---" << endl;
// //         cout << "********************************************" << endl;
// //         cout << "1. Display All Flights" << endl;
// //         cout << "2. Book a Seat" << endl;
// //         cout << "3. Cancel a Reservation" << endl;
// //         cout << "4. View My Reservations (by Passenger ID)" << endl; //immprovement req to show full table
// //         cout << "5. Add New Passenger" << endl;
// //         cout << "6. Exit" << endl;
// //         cout << "********************************************" << endl;

// //         int val = getMenuChoice();

// //         switch (val) {
// //             case 1:
// //                 db.displayAllFlights();
// //                 pressEnterToContinue();
// //                 break;
// //             case 2:
// //                 { // Braces create a new scope for variables
// //                     system("cls");
// //                     cout << "--- Book a Reservation ---" << endl;
// //                     db.displayAllFlights();
// //                     cout << "Enter Your Passenger ID: ";
// //                     int p_id = getMenuChoice();
// //                     cout << "Enter Flight ID to book: ";
// //                     int f_id = getMenuChoice();
// //                     db.bookReservation(p_id, f_id);
// //                     pressEnterToContinue();
// //                 }
// //                 break;
// //             case 3:
// //                 {
// //                     system("cls");
// //                     cout << "--- Cancel a Reservation ---" << endl;
// //                     cout << "Enter Reservation ID to cancel: ";
// //                     int r_id = getMenuChoice();
// //                     db.cancelReservation(r_id);
// //                     pressEnterToContinue();
// //                 }
// //                 break;
// //             case 4:
// //                 {
// //                     system("cls");
// //                     cout << "--- View My Reservations ---" << endl;
// //                     cout << "Enter Your Passenger ID: ";
// //                     int p_id = getMenuChoice();
// //                     db.displayPassengerReservations(p_id);
// //                     pressEnterToContinue();
// //                 }
// //                 break;
// //             case 5:
// //                 db.addNewPassenger();
// //                 pressEnterToContinue();
// //                 break;
// //             case 6:
// //                 exit = true;
// //                 cout << "\nGood Luck!" << endl;
// //                 Sleep(2000);
// //                 break;
// //             default:
// //                 cout << "\nInvalid Input. Please choose from 1-6." << endl;
// //                 Sleep(2000);
// //                 break;
// //         }
// //     }

// //     return 0; // Destructor for 'db' will automatically close the connection
// // }

// #include <iostream>
// #include <string>
// #include <vector>
// #include <memory>
// #include <stdexcept>
// #include <initializer_list>
// #include <cstdlib>
// #include <cctype>
// #include <algorithm>
// #include <iomanip>   // For formatting output
// #include <limits>    // For input validation
// #include <thread>    // Portable sleep (replaces Windows Sleep())
// #include <chrono>
// #include <mysql/jdbc.h>   // Connector/C++ classic API (sql::Connection sql::PreparedStatement, ...)

// using namespace std;

// // --- Database Connection Configuration ---
// const char* HOST = "localhost";
// const char* USER = "root";
// const char* DB   = "mydb";
// const int   PORT = 3306;
// // The password is NOT stored in source code.
// // Set the environment variable AIRLINE_DB_PW, or the program will ask for it.

// // MySQL error code for "duplicate entry" (UNIQUE violation)
// const int ERR_DUP_ENTRY = 1062;


// // --- Small portable helpers ---

// void clearScreen() {
// #ifdef _WIN32
//     system("cls");
// #else
//     system("clear");
// #endif
// }

// void sleepMs(int ms) {
//     this_thread::sleep_for(chrono::milliseconds(ms));
// }

// // --- Helper Functions for Robust Input ---

// int getMenuChoice() {
//     int choice;
//     while (true) {
//         cout << "> Your choice: ";
//         if (cin >> choice) {
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             return choice;
//         } else {
//             cout << "Invalid input. Please enter a number." << endl;
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         }
//     }
// }

// // Thrown when the user cancels the current operation. main() catches it and
// // returns to the menu. Cancelling only ever happens while input is being
// // collected - before any database work starts - so nothing is left half-done.
// class CancelledError : public runtime_error {
// public:
//     CancelledError() : runtime_error("cancelled") {}
// };

// // Gets an integer >= minVal. For prompts that require a value >= 1 (IDs, seat
// // counts), typing 0 cancels the current operation.
// int getIntInput(const string& prompt, int minVal = numeric_limits<int>::min()) {
//     int value;
//     while (true) {
//         cout << prompt;
//         if (cin >> value) {
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             if (value == 0 && minVal >= 1) throw CancelledError();
//             if (value >= minVal) return value;
//         } else {
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         }
//         cout << "Invalid input. Please enter a whole number";
//         if (minVal != numeric_limits<int>::min()) cout << " >= " << minVal << " (or 0 to cancel)";
//         cout << "." << endl;
//     }
// }

// // Gets a non-empty trimmed string. Typing "cancel" cancels the current operation
// // (disabled with allowCancel = false, e.g. for the password prompt).
// string getStringInput(const string& prompt, size_t maxLen = 0, bool allowCancel = true) {
//     string input;
//     while (true) {
//         cout << prompt;
//         getline(cin, input);

//         size_t start = input.find_first_not_of(" \t\r\n");
//         size_t end   = input.find_last_not_of(" \t\r\n");
//         input = (start == string::npos) ? "" : input.substr(start, end - start + 1);

//         if (allowCancel) {
//             string lower = input;
//             transform(lower.begin(), lower.end(), lower.begin(),
//                       [](unsigned char c) { return (char)tolower(c); });
//             if (lower == "cancel") throw CancelledError();
//         }

//         if (input.empty()) {
//             cout << "Input cannot be empty. Please try again." << endl;
//         } else if (maxLen > 0 && input.length() > maxLen) {
//             cout << "Input too long (max " << maxLen << " characters). Please try again." << endl;
//         } else {
//             return input;
//         }
//     }
// }

// void pressEnterToContinue() {
//     cout << "\nPress Enter to continue..." << endl;
//     cin.get();
// }

// // --- Data Model Classes ---

// class Flight {
// public:
//     int flight_id;
//     string flight_number;
//     string departure;
//     string destination;
//     int available_seats;
// };

// class Passenger {
// public:
//     int passenger_id;
//     string first_name;
//     string last_name;
//     string passport_number;
// };

// class Reservation {
// public:
//     int reservation_id;
//     int passenger_id;
//     int flight_id;
// };


// // A "business rule" failure (flight full, id not found, ...) - triggers a rollback.
// class AppError : public runtime_error {
// public:
//     explicit AppError(const string& msg) : runtime_error(msg) {}
// };

// /**
//  * RAII transaction guard.
//  *  - Disables autocommit for its lifetime.
//  *  - If commit() was not reached (exception, early return, error), the destructor
//  *    ROLLS BACK automatically. Only THIS session's transaction is affected; the
//  *    program keeps running and other sessions' data is never touched.
//  */
// class Tx {
//     sql::Connection* c;
//     bool finished = false;
// public:
//     explicit Tx(sql::Connection* conn) : c(conn) { c->setAutoCommit(false); }
//     void commit() { c->commit(); finished = true; }
//     ~Tx() {
//         try { if (!finished) c->rollback(); } catch (...) {}
//         try { c->setAutoCommit(true); } catch (...) {}
//     }
//     Tx(const Tx&) = delete;
//     Tx& operator=(const Tx&) = delete;
// };


// /**
//  * @class Database
//  * @brief Encapsulates all database logic. This is the main "engine".
//  */
// class Database {
// private:
//     unique_ptr<sql::Connection> conn;

//     static string errText(const sql::SQLException& e) {
//         return string(e.what()) + " (MySQL error " + to_string(e.getErrorCode()) + ")";
//     }

//     // ---------- low-level helpers ----------

//     void exec(const string& sqlText) {
//         unique_ptr<sql::Statement> st(conn->createStatement());
//         st->execute(sqlText);
//     }

//     long long countRows(const string& table) {
//         unique_ptr<sql::Statement> st(conn->createStatement());
//         unique_ptr<sql::ResultSet> rs(st->executeQuery("SELECT COUNT(*) FROM " + table));
//         return rs->next() ? rs->getInt64(1) : 0;
//     }

//     // SELECT <one int column> ... WHERE <one int param>. Returns true if a row was found.
//     bool queryOneInt(const string& sqlText, int param, int& out) {
//         unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(sqlText));
//         ps->setInt(1, param);
//         unique_ptr<sql::ResultSet> rs(ps->executeQuery());
//         if (!rs->next()) return false;
//         out = rs->getInt(1);
//         return true;
//     }

//     // INSERT/UPDATE/DELETE with only INT parameters. Returns affected rows.
//     int execWithInts(const string& sqlText, initializer_list<int> params) {
//         unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(sqlText));
//         int i = 1;
//         for (int p : params) ps->setInt(i++, p);
//         return ps->executeUpdate();
//     }

//     unsigned long long lastInsertId() {
//         unique_ptr<sql::Statement> st(conn->createStatement());
//         unique_ptr<sql::ResultSet> rs(st->executeQuery("SELECT LAST_INSERT_ID()"));
//         return rs->next() ? rs->getUInt64(1) : 0;
//     }

//     // ---------- schema helpers ----------

//     // CREATE TABLE IF NOT EXISTS - never touches existing data
//     void createTables() {
//         exec(R"(
//             CREATE TABLE IF NOT EXISTS Flights (
//                 flight_id INT AUTO_INCREMENT PRIMARY KEY,
//                 flight_number VARCHAR(10) NOT NULL UNIQUE,
//                 departure VARCHAR(50) NOT NULL,
//                 destination VARCHAR(50) NOT NULL,
//                 available_seats INT NOT NULL,
//                 CHECK (available_seats >= 0)
//             )
//         )");

//         exec(R"(
//             CREATE TABLE IF NOT EXISTS Passengers (
//                 passenger_id INT AUTO_INCREMENT PRIMARY KEY,
//                 first_name VARCHAR(50) NOT NULL,
//                 last_name VARCHAR(50) NOT NULL,
//                 passport_number VARCHAR(20) NOT NULL UNIQUE
//             )
//         )");

//         exec(R"(
//             CREATE TABLE IF NOT EXISTS Reservations (
//                 reservation_id INT AUTO_INCREMENT PRIMARY KEY,
//                 passenger_id INT NOT NULL,
//                 flight_id INT NOT NULL,
//                 UNIQUE KEY uq_passenger_flight (passenger_id, flight_id),
//                 FOREIGN KEY (passenger_id) REFERENCES Passengers(passenger_id),
//                 FOREIGN KEY (flight_id) REFERENCES Flights(flight_id)
//             )
//         )");
//     }

//     void seedSampleData() {
//         exec("INSERT INTO Flights (flight_number, departure, destination, available_seats) VALUES ('F101', 'UAE', 'Canada', 50)");
//         exec("INSERT INTO Flights (flight_number, departure, destination, available_seats) VALUES ('F102', 'UAE', 'USA', 40)");
//         exec("INSERT INTO Flights (flight_number, departure, destination, available_seats) VALUES ('F103', 'UAE', 'China', 1)"); // For testing
//         exec("INSERT INTO Passengers (first_name, last_name, passport_number) VALUES ('John', 'Doe', 'A123')");
//         exec("INSERT INTO Passengers (first_name, last_name, passport_number) VALUES ('Jane', 'Smith', 'B456')");
//     }

//     // Generic "SELECT and print as a table" helper
//     void printTable(const string& title, const string& sqlText,
//                     const vector<string>& headers, const vector<int>& widths,
//                     const string& emptyMsg) {
//         try {
//             unique_ptr<sql::Statement> st(conn->createStatement());
//             unique_ptr<sql::ResultSet> rs(st->executeQuery(sqlText));

//             vector<vector<string>> rows;
//             while (rs->next()) {
//                 vector<string> r;
//                 for (size_t i = 0; i < headers.size(); i++) {
//                     unsigned int col = (unsigned int)(i + 1); // columns are 1-based
//                     r.push_back(rs->isNull(col) ? "NULL" : rs->getString(col).asStdString());
//                 }
//                 rows.push_back(r);
//             }

//             cout << "\n--- " << title << " ---" << endl;
//             if (rows.empty()) {
//                 cout << emptyMsg << endl;
//                 return;
//             }

//             int total = 0;
//             for (int w : widths) total += w;

//             cout << left;
//             for (size_t i = 0; i < headers.size(); i++) cout << setw(widths[i]) << headers[i];
//             cout << endl << string(total, '-') << endl;
//             for (const auto& r : rows) {
//                 for (size_t i = 0; i < r.size(); i++) cout << setw(widths[i]) << r[i];
//                 cout << endl;
//             }
//             cout << string(total, '-') << endl;
//             cout << rows.size() << " row(s)" << endl;
//         } catch (const sql::SQLException& e) {
//             cout << "Error: " << errText(e) << endl;
//         }
//     }

// public:
//     // Connects to the database. Returns true on success.
//     bool connect(const string& password) {
//         try {
//             sql::Driver* driver = sql::mysql::get_driver_instance(); // singleton, do not delete

//             sql::ConnectOptionsMap opts;
//             opts["hostName"] = sql::SQLString(HOST);
//             opts["userName"] = sql::SQLString(USER);
//             opts["password"] = sql::SQLString(password);
//             opts["schema"]   = sql::SQLString(DB);
//             opts["port"]     = PORT;
//             opts["OPT_CHARSET_NAME"] = sql::SQLString("utf8mb4");
//             // TLS: the connector's default is to use TLS when the server offers it,
//             // which is what caching_sha2_password logins (MySQL 8.4 / 9.x) need.

//             conn.reset(driver->connect(opts));
//             cout << "Logged In To Database!" << endl;
//             return true;
//         } catch (const sql::SQLException& e) {
//             cout << "Error connecting to database: " << errText(e) << endl;
//             return false;
//         }
//     }

//     // Creates the tables ONLY if they do not exist and seeds sample data ONLY on an
//     // empty database. Existing data (and other open sessions) are never affected.
//     bool setupDatabase() {
//         cout << "Checking database schema..." << endl;
//         try {
//             createTables();
//             if (countRows("Flights") == 0 && countRows("Passengers") == 0) {
//                 cout << "Empty database detected - loading sample data." << endl;
//                 seedSampleData();
//             }
//         } catch (const sql::SQLException& e) {
//             cout << "Fatal error setting up tables: " << errText(e) << endl;
//             return false;
//         }
//         cout << "Database ready." << endl;
//         sleepMs(1500);
//         return true;
//     }

//     // Explicit, confirmed full reset (the OLD behaviour of setupDatabase()).
//     void resetDatabase() {
//         clearScreen();
//         cout << "--- Reset Database ---" << endl;
//         cout << "WARNING: this DROPS and recreates all tables. ALL flights, passengers and\n"
//                 "reservations will be deleted for EVERY open session." << endl;
//         string confirm = getStringInput("Type RESET to confirm (anything else cancels): ");
//         if (confirm != "RESET") {
//             cout << "Cancelled. Nothing was changed." << endl;
//             return;
//         }
//         try {
//             exec("SET FOREIGN_KEY_CHECKS = 0");
//             exec("DROP TABLE IF EXISTS Reservations");
//             exec("DROP TABLE IF EXISTS Passengers");
//             exec("DROP TABLE IF EXISTS Flights");
//             exec("SET FOREIGN_KEY_CHECKS = 1");
//             createTables();
//             seedSampleData();
//             cout << "Database has been reset to the sample data." << endl;
//         } catch (const sql::SQLException& e) {
//             try { exec("SET FOREIGN_KEY_CHECKS = 1"); } catch (...) {}
//             cout << "Error during reset: " << errText(e) << endl;
//         }
//     }

//     // (READ) Displays all flights
//     void displayAllFlights() {
//         printTable("Available Flights",
//                    "SELECT flight_id, flight_number, departure, destination, available_seats "
//                    "FROM Flights ORDER BY flight_id",
//                    {"ID", "Flight No.", "Departure", "Destination", "Available"},
//                    {10, 15, 15, 15, 10},
//                    "No flights found.");
//     }

//     // (READ) Displays every passenger together with their ID
//     void displayAllPassengers() {
//         printTable("All Passengers",
//                    "SELECT passenger_id, first_name, last_name, passport_number "
//                    "FROM Passengers ORDER BY passenger_id",
//                    {"Passenger ID", "First Name", "Last Name", "Passport No."},
//                    {15, 18, 18, 18},
//                    "No passengers registered yet.");
//     }

//     // (READ) Look up a passenger ID from a passport number
//     void findPassengerId() {
//         string passport = getStringInput("Enter passport number: ", 20);
//         try {
//             unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
//                 "SELECT passenger_id, first_name, last_name FROM Passengers WHERE passport_number = ?"));
//             ps->setString(1, passport);
//             unique_ptr<sql::ResultSet> rs(ps->executeQuery());
//             if (rs->next()) {
//                 cout << "\nPassenger found: " << rs->getString("first_name") << " "
//                      << rs->getString("last_name")
//                      << "  ->  Passenger ID: " << rs->getInt("passenger_id") << endl;
//             } else {
//                 cout << "\nNo passenger with passport number '" << passport << "'." << endl;
//             }
//         } catch (const sql::SQLException& e) {
//             cout << "Error: " << errText(e) << endl;
//         }
//     }

//     // (CREATE) Adds a new passenger
//     void addNewPassenger() {
//         clearScreen();
//         cout << "--- Add New Passenger ---" << endl;
//         cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
//         string fname    = getStringInput("Enter first name: ", 50);
//         string lname    = getStringInput("Enter last name: ", 50);
//         string passport = getStringInput("Enter passport number: ", 20);

//         try {
//             unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
//                 "INSERT INTO Passengers (first_name, last_name, passport_number) VALUES (?, ?, ?)"));
//             ps->setString(1, fname);
//             ps->setString(2, lname);
//             ps->setString(3, passport);
//             ps->executeUpdate();

//             cout << "\nPassenger '" << fname << " " << lname << "' added successfully!" << endl;
//             cout << "New Passenger ID: " << lastInsertId() << endl;
//         } catch (const sql::SQLException& e) {
//             if (e.getErrorCode() == ERR_DUP_ENTRY)
//                 cout << "Error: a passenger with passport number '" << passport << "' already exists." << endl;
//             else
//                 cout << "Error adding passenger: " << errText(e) << endl;
//         }
//     }

//     // (CREATE) Adds a new flight
//     void addNewFlight() {
//         clearScreen();
//         cout << "--- Add New Flight ---" << endl;
//         cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
//         string number = getStringInput("Enter flight number (e.g. F104): ", 10);
//         transform(number.begin(), number.end(), number.begin(),
//                   [](unsigned char c) { return (char)toupper(c); });
//         string from = getStringInput("Enter departure location: ", 50);
//         string to   = getStringInput("Enter destination: ", 50);
//         int seats   = getIntInput("Enter number of seats: ", 1);

//         if (from == to) {
//             cout << "Error: departure and destination cannot be the same." << endl;
//             return;
//         }

//         try {
//             unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
//                 "INSERT INTO Flights (flight_number, departure, destination, available_seats) VALUES (?, ?, ?, ?)"));
//             ps->setString(1, number);
//             ps->setString(2, from);
//             ps->setString(3, to);
//             ps->setInt(4, seats);
//             ps->executeUpdate();

//             cout << "\nFlight " << number << " (" << from << " -> " << to << ", " << seats
//                  << " seats) added successfully!" << endl;
//             cout << "New Flight ID: " << lastInsertId() << endl;
//         } catch (const sql::SQLException& e) {
//             if (e.getErrorCode() == ERR_DUP_ENTRY)
//                 cout << "Error: flight number '" << number << "' already exists." << endl;
//             else
//                 cout << "Error adding flight: " << errText(e) << endl;
//         }
//     }

//     // (UPDATE/CREATE) Books a seat (TRANSACTION-SAFE)
//     // Any failing step throws; the Tx guard rolls back ONLY this transaction and the
//     // program carries on.
//     bool bookReservation(const string& passport, int f_id) {
//         try {
//             Tx tx(conn.get());

//             // 1. Find the passenger ID from the passport number (passenger must exist)
//             int p_id = 0;
//             {
//                 unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
//                     "SELECT passenger_id FROM Passengers WHERE passport_number = ?"));
//                 ps->setString(1, passport);
//                 unique_ptr<sql::ResultSet> rs(ps->executeQuery());
//                 if (!rs->next())
//                     throw AppError("Error: no passenger with passport number '" + passport + "'.");
//                 p_id = rs->getInt(1);
//             }

//             // 2. Check seats AND lock the flight row
//             int available_seats = 0;
//             if (!queryOneInt("SELECT available_seats FROM Flights WHERE flight_id = ? FOR UPDATE", f_id, available_seats))
//                 throw AppError("Error: Flight ID not found.");
//             if (available_seats <= 0)
//                 throw AppError("Sorry, that flight is full.");

//             // 3a. Take the seat
//             if (execWithInts("UPDATE Flights SET available_seats = available_seats - 1 WHERE flight_id = ?", {f_id}) != 1)
//                 throw AppError("Failed to update the seat count.");

//             // 3b. Create the reservation (fails cleanly on duplicate booking / bad FK)
//             if (execWithInts("INSERT INTO Reservations (passenger_id, flight_id) VALUES (?, ?)", {p_id, f_id}) != 1)
//                 throw AppError("Failed to create the reservation.");
//             unsigned long long newId = lastInsertId();

//             // 4. Commit
//             tx.commit();
//             cout << "\nBooking successful! Reservation ID: " << newId
//                  << " (Passenger ID: " << p_id << ")" << endl;
//             return true;
//         } catch (const AppError& e) {
//             cout << "\n" << e.what() << " Rolling back - no changes were saved." << endl;
//         } catch (const sql::SQLException& e) {
//             if (e.getErrorCode() == ERR_DUP_ENTRY)
//                 cout << "\nThis passenger already has a reservation on that flight.";
//             else
//                 cout << "\nDatabase error: " << errText(e) << ".";
//             cout << " Rolling back - no changes were saved." << endl;
//         }
//         return false;
//     }

//     // (READ) Shows reservations for a specific passenger
//     void displayPassengerReservations(int p_id) {
//         try {
//             unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
//                 "SELECT r.reservation_id, f.flight_number, f.departure, f.destination "
//                 "FROM Reservations r "
//                 "JOIN Flights f ON r.flight_id = f.flight_id "
//                 "WHERE r.passenger_id = ? ORDER BY r.reservation_id"));
//             ps->setInt(1, p_id);
//             unique_ptr<sql::ResultSet> rs(ps->executeQuery());

//             if (!rs->next()) {
//                 cout << "\nNo reservations found for Passenger ID " << p_id << "." << endl;
//                 return;
//             }

//             cout << "\n--- Reservations for Passenger ID " << p_id << " ---" << endl;
//             cout << left << setw(10) << "Res. ID" << setw(15) << "Flight No."
//                  << setw(15) << "Departure" << setw(15) << "Destination" << endl;
//             cout << string(55, '-') << endl;
//             do {
//                 cout << left << setw(10) << rs->getInt(1)
//                      << setw(15) << rs->getString(2).asStdString()
//                      << setw(15) << rs->getString(3).asStdString()
//                      << setw(15) << rs->getString(4).asStdString() << endl;
//             } while (rs->next());
//             cout << string(55, '-') << endl;
//         } catch (const sql::SQLException& e) {
//             cout << "Error: " << errText(e) << endl;
//         }
//     }

//     // (DELETE/UPDATE) Cancels a reservation (TRANSACTION-SAFE)
//     bool cancelReservation(int res_id) {
//         try {
//             Tx tx(conn.get());

//             // 1. Find the flight AND lock the reservation row
//             int f_id = 0;
//             if (!queryOneInt("SELECT flight_id FROM Reservations WHERE reservation_id = ? FOR UPDATE", res_id, f_id))
//                 throw AppError("Error: Reservation ID " + to_string(res_id) + " not found.");

//             // 2a. Delete the reservation
//             if (execWithInts("DELETE FROM Reservations WHERE reservation_id = ?", {res_id}) != 1)
//                 throw AppError("Failed to delete the reservation.");

//             // 2b. Give the seat back
//             if (execWithInts("UPDATE Flights SET available_seats = available_seats + 1 WHERE flight_id = ?", {f_id}) != 1)
//                 throw AppError("Failed to update the seat count.");

//             // 3. Commit
//             tx.commit();
//             cout << "\nReservation " << res_id << " successfully cancelled." << endl;
//             return true;
//         } catch (const AppError& e) {
//             cout << "\n" << e.what() << " Rolling back - no changes were saved." << endl;
//         } catch (const sql::SQLException& e) {
//             cout << "\nDatabase error: " << errText(e) << ". Rolling back - no changes were saved." << endl;
//         }
//         return false;
//     }
// };


// // --- Main Application Loop ---

// string getDbPassword() {
//     const char* env = getenv("AIRLINE_DB_PW");
//     if (env && *env) return string(env);
//     return getStringInput("Enter MySQL password for user '" + string(USER) + "': ", 0, false);
// }

// int main() {
//     cout << "PROGRAM STARTED" << endl;
//     Database db;

//     if (!db.connect(getDbPassword())) {
//         return 1; // Exit if connection fails
//     }

//     if (!db.setupDatabase()) { // Creates tables only if missing - never wipes data
//         return 1;
//     }

//     bool running = true;
//     while (running) {
//         clearScreen();
//         cout << "\n--- Welcome To Airlines Reservation System ---" << endl;
//         cout << "********************************************" << endl;
//         cout << " 1. Display All Flights" << endl;
//         cout << " 2. Book a Seat" << endl;
//         cout << " 3. Cancel a Reservation" << endl;
//         cout << " 4. View My Reservations (by Passenger ID)" << endl;
//         cout << " 5. Add New Passenger" << endl;
//         cout << " 6. Add New Flight" << endl;
//         cout << " 7. List All Passengers (with IDs)" << endl;
//         cout << " 8. Find Passenger ID (by Passport No.)" << endl;
//         cout << " 9. Reset Database (deletes ALL data)" << endl;
//         cout << "10. Exit" << endl;
//         cout << "********************************************" << endl;

//         int val = getMenuChoice();

//         try {
//         switch (val) {
//             case 1:
//                 db.displayAllFlights();
//                 pressEnterToContinue();
//                 break;
//             case 2: {
//                 clearScreen();
//                 cout << "--- Book a Reservation ---" << endl;
//                 cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
//                 db.displayAllFlights();
//                 string passport = getStringInput("Enter your passport number: ", 20);
//                 int f_id = getIntInput("Enter Flight ID to book: ", 1);
//                 db.bookReservation(passport, f_id);
//                 pressEnterToContinue();
//                 break;
//             }
//             case 3: {
//                 clearScreen();
//                 cout << "--- Cancel a Reservation ---" << endl;
//                 cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
//                 int r_id = getIntInput("Enter Reservation ID to cancel: ", 1);
//                 db.cancelReservation(r_id);
//                 pressEnterToContinue();
//                 break;
//             }
//             case 4: {
//                 clearScreen();
//                 cout << "--- View My Reservations ---" << endl;
//                 cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
//                 int p_id = getIntInput("Enter Your Passenger ID: ", 1);
//                 db.displayPassengerReservations(p_id);
//                 pressEnterToContinue();
//                 break;
//             }
//             case 5:
//                 db.addNewPassenger();
//                 pressEnterToContinue();
//                 break;
//             case 6:
//                 db.addNewFlight();
//                 pressEnterToContinue();
//                 break;
//             case 7:
//                 clearScreen();
//                 db.displayAllPassengers();
//                 pressEnterToContinue();
//                 break;
//             case 8:
//                 clearScreen();
//                 cout << "--- Find Passenger ID ---" << endl;
//                 cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
//                 db.findPassengerId();
//                 pressEnterToContinue();
//                 break;
//             case 9:
//                 db.resetDatabase();
//                 pressEnterToContinue();
//                 break;
//             case 10:
//                 running = false;
//                 cout << "\nGood Luck!" << endl;
//                 sleepMs(1500);
//                 break;
//             default:
//                 cout << "\nInvalid Input. Please choose from 1-10." << endl;
//                 sleepMs(1500);
//                 break;
//         }
//         } catch (const CancelledError&) {
//             // Raised while collecting input, before any database work started.
//             cout << "\nCancelled. Nothing was changed." << endl;
//             sleepMs(1200);
//         }
//     }

//     return 0; // Database's unique_ptr closes the connection automatically
// }



#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <initializer_list>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <iomanip>   // For formatting output
#include <limits>    // For input validation
#include <thread>    // Portable sleep (replaces Windows Sleep())
#include <chrono>
#include <mysql/jdbc.h>   // Connector/C++ classic API (sql::Connection sql::PreparedStatement, ...)

using namespace std;

// --- Database Connection Configuration ---
const char* HOST = "localhost";
const char* USER = "root";
const char* DB   = "mydb";
const int   PORT = 3306;
// The password is NOT stored in source code.
// Set the environment variable AIRLINE_DB_PW, or the program will ask for it.

// MySQL error code for "duplicate entry" (UNIQUE violation)
const int ERR_DUP_ENTRY = 1062;


// --- Small portable helpers ---

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// --- Helper Functions for Robust Input ---

int getMenuChoice() {
    int choice;
    while (true) {
        cout << "> Your choice: ";
        if (cin >> choice) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        } else {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Thrown when the user cancels the current operation. main() catches it and
// returns to the menu. Cancelling only ever happens while input is being
// collected - before any database work starts - so nothing is left half-done.
class CancelledError : public runtime_error {
public:
    CancelledError() : runtime_error("cancelled") {}
};

// Gets an integer >= minVal. For prompts that require a value >= 1 (IDs, seat
// counts), typing 0 cancels the current operation.
int getIntInput(const string& prompt, int minVal = numeric_limits<int>::min()) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value == 0 && minVal >= 1) throw CancelledError();
            if (value >= minVal) return value;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Invalid input. Please enter a whole number";
        if (minVal != numeric_limits<int>::min()) cout << " >= " << minVal << " (or 0 to cancel)";
        cout << "." << endl;
    }
}

// Gets a non-empty trimmed string. Typing "cancel" cancels the current operation
// (disabled with allowCancel = false, e.g. for the password prompt).
string getStringInput(const string& prompt, size_t maxLen = 0, bool allowCancel = true) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        size_t start = input.find_first_not_of(" \t\r\n");
        size_t end   = input.find_last_not_of(" \t\r\n");
        input = (start == string::npos) ? "" : input.substr(start, end - start + 1);

        if (allowCancel) {
            string lower = input;
            transform(lower.begin(), lower.end(), lower.begin(),
                      [](unsigned char c) { return (char)tolower(c); });
            if (lower == "cancel") throw CancelledError();
        }

        if (input.empty()) {
            cout << "Input cannot be empty. Please try again." << endl;
        } else if (maxLen > 0 && input.length() > maxLen) {
            cout << "Input too long (max " << maxLen << " characters). Please try again." << endl;
        } else {
            return input;
        }
    }
}

void pressEnterToContinue() {
    cout << "\nPress Enter to continue..." << endl;
    cin.get();
}

// --- Data Model Classes ---

class Flight {
public:
    int flight_id;
    string flight_number;
    string departure;
    string destination;
    int available_seats;
};

class Passenger {
public:
    int passenger_id;
    string first_name;
    string last_name;
    string passport_number;
};

class Reservation {
public:
    int reservation_id;
    int passenger_id;
    int flight_id;
};


// A "business rule" failure (flight full, id not found, ...) - triggers a rollback.
class AppError : public runtime_error {
public:
    explicit AppError(const string& msg) : runtime_error(msg) {}
};

/**
 * RAII transaction guard.
 *  - Disables autocommit for its lifetime.
 *  - If commit() was not reached (exception, early return, error), the destructor
 *    ROLLS BACK automatically. Only THIS session's transaction is affected; the
 *    program keeps running and other sessions' data is never touched.
 */
class Tx {
    sql::Connection* c;
    bool finished = false;
public:
    explicit Tx(sql::Connection* conn) : c(conn) { c->setAutoCommit(false); }
    void commit() { c->commit(); finished = true; }
    ~Tx() {
        try { if (!finished) c->rollback(); } catch (...) {}
        try { c->setAutoCommit(true); } catch (...) {}
    }
    Tx(const Tx&) = delete;
    Tx& operator=(const Tx&) = delete;
};


/**
 * @class Database
 * @brief Encapsulates all database logic. This is the main "engine".
 */
class Database {
private:
    unique_ptr<sql::Connection> conn;

    static string errText(const sql::SQLException& e) {
        return string(e.what()) + " (MySQL error " + to_string(e.getErrorCode()) + ")";
    }

    // ---------- low-level helpers ----------

    void exec(const string& sqlText) {
        unique_ptr<sql::Statement> st(conn->createStatement());
        st->execute(sqlText);
    }

    long long countRows(const string& table) {
        unique_ptr<sql::Statement> st(conn->createStatement());
        unique_ptr<sql::ResultSet> rs(st->executeQuery("SELECT COUNT(*) FROM " + table));
        return rs->next() ? rs->getInt64(1) : 0;
    }

    // SELECT <one int column> ... WHERE <one int param>. Returns true if a row was found.
    bool queryOneInt(const string& sqlText, int param, int& out) {
        unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(sqlText));
        ps->setInt(1, param);
        unique_ptr<sql::ResultSet> rs(ps->executeQuery());
        if (!rs->next()) return false;
        out = rs->getInt(1);
        return true;
    }

    // INSERT/UPDATE/DELETE with only INT parameters. Returns affected rows.
    int execWithInts(const string& sqlText, initializer_list<int> params) {
        unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(sqlText));
        int i = 1;
        for (int p : params) ps->setInt(i++, p);
        return ps->executeUpdate();
    }

    unsigned long long lastInsertId() {
        unique_ptr<sql::Statement> st(conn->createStatement());
        unique_ptr<sql::ResultSet> rs(st->executeQuery("SELECT LAST_INSERT_ID()"));
        return rs->next() ? rs->getUInt64(1) : 0;
    }

    // ---------- schema helpers ----------

    // CREATE TABLE IF NOT EXISTS - never touches existing data
    void createTables() {
        exec(R"(
            CREATE TABLE IF NOT EXISTS Flights (
                flight_id INT AUTO_INCREMENT PRIMARY KEY,
                flight_number VARCHAR(10) NOT NULL UNIQUE,
                departure VARCHAR(50) NOT NULL,
                destination VARCHAR(50) NOT NULL,
                available_seats INT NOT NULL,
                departure_time DATETIME NOT NULL,
                CHECK (available_seats >= 0)
            )
        )");

        exec(R"(
            CREATE TABLE IF NOT EXISTS Passengers (
                passenger_id INT AUTO_INCREMENT PRIMARY KEY,
                first_name VARCHAR(50) NOT NULL,
                last_name VARCHAR(50) NOT NULL,
                passport_number VARCHAR(20) NOT NULL UNIQUE
            )
        )");

        exec(R"(
            CREATE TABLE IF NOT EXISTS Reservations (
                reservation_id INT AUTO_INCREMENT PRIMARY KEY,
                passenger_id INT NOT NULL,
                flight_id INT NOT NULL,
                UNIQUE KEY uq_passenger_flight (passenger_id, flight_id),
                FOREIGN KEY (passenger_id) REFERENCES Passengers(passenger_id),
                FOREIGN KEY (flight_id) REFERENCES Flights(flight_id)
            )
        )");

        // Migration for databases created before departure_time existed
        unique_ptr<sql::Statement> st(conn->createStatement());
        unique_ptr<sql::ResultSet> rs(st->executeQuery(
            "SELECT COUNT(*) FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = DATABASE() "
            "AND TABLE_NAME = 'Flights' AND COLUMN_NAME = 'departure_time'"));
        if (rs->next() && rs->getInt(1) == 0)
            exec("ALTER TABLE Flights ADD COLUMN departure_time DATETIME NOT NULL DEFAULT (NOW() + INTERVAL 1 DAY)");
    }

    void seedSampleData() {
        exec("INSERT INTO Flights (flight_number, departure, destination, available_seats, departure_time) VALUES ('F101', 'UAE', 'Canada', 50, NOW() + INTERVAL 2 DAY)");
        exec("INSERT INTO Flights (flight_number, departure, destination, available_seats, departure_time) VALUES ('F102', 'UAE', 'USA', 40, NOW() + INTERVAL 1 DAY)");
        exec("INSERT INTO Flights (flight_number, departure, destination, available_seats, departure_time) VALUES ('F103', 'UAE', 'China', 1, NOW() + INTERVAL 2 MINUTE)"); // For testing
        exec("INSERT INTO Passengers (first_name, last_name, passport_number) VALUES ('John', 'Doe', 'A123')");
        exec("INSERT INTO Passengers (first_name, last_name, passport_number) VALUES ('Jane', 'Smith', 'B456')");
    }

    // Generic "SELECT and print as a table" helper
    void printTable(const string& title, const string& sqlText,
                    const vector<string>& headers, const vector<int>& widths,
                    const string& emptyMsg) {
        try {
            unique_ptr<sql::Statement> st(conn->createStatement());
            unique_ptr<sql::ResultSet> rs(st->executeQuery(sqlText));

            vector<vector<string>> rows;
            while (rs->next()) {
                vector<string> r;
                for (size_t i = 0; i < headers.size(); i++) {
                    unsigned int col = (unsigned int)(i + 1); // columns are 1-based
                    r.push_back(rs->isNull(col) ? "NULL" : rs->getString(col).asStdString());
                }
                rows.push_back(r);
            }

            cout << "\n--- " << title << " ---" << endl;
            if (rows.empty()) {
                cout << emptyMsg << endl;
                return;
            }

            int total = 0;
            for (int w : widths) total += w;

            cout << left;
            for (size_t i = 0; i < headers.size(); i++) cout << setw(widths[i]) << headers[i];
            cout << endl << string(total, '-') << endl;
            for (const auto& r : rows) {
                for (size_t i = 0; i < r.size(); i++) cout << setw(widths[i]) << r[i];
                cout << endl;
            }
            cout << string(total, '-') << endl;
            cout << rows.size() << " row(s)" << endl;
        } catch (const sql::SQLException& e) {
            cout << "Error: " << errText(e) << endl;
        }
    }

public:
    // Connects to the database. Returns true on success.
    bool connect(const string& password) {
        try {
            sql::Driver* driver = sql::mysql::get_driver_instance(); // singleton, do not delete

            sql::ConnectOptionsMap opts;
            opts["hostName"] = sql::SQLString(HOST);
            opts["userName"] = sql::SQLString(USER);
            opts["password"] = sql::SQLString(password);
            opts["schema"]   = sql::SQLString(DB);
            opts["port"]     = PORT;
            opts["OPT_CHARSET_NAME"] = sql::SQLString("utf8mb4");
            // TLS: the connector's default is to use TLS when the server offers it,
            // which is what caching_sha2_password logins (MySQL 8.4 / 9.x) need.

            conn.reset(driver->connect(opts));
            cout << "Logged In To Database!" << endl;
            return true;
        } catch (const sql::SQLException& e) {
            cout << "Error connecting to database: " << errText(e) << endl;
            return false;
        }
    }

    // Creates the tables ONLY if they do not exist and seeds sample data ONLY on an
    // empty database. Existing data (and other open sessions) are never affected.
    bool setupDatabase() {
        cout << "Checking database schema..." << endl;
        try {
            createTables();
            if (countRows("Flights") == 0 && countRows("Passengers") == 0) {
                cout << "Empty database detected - loading sample data." << endl;
                seedSampleData();
            }
        } catch (const sql::SQLException& e) {
            cout << "Fatal error setting up tables: " << errText(e) << endl;
            return false;
        }
        cout << "Database ready." << endl;
        sleepMs(1500);
        return true;
    }

    // Explicit, confirmed full reset (the OLD behaviour of setupDatabase()).
    void resetDatabase() {
        clearScreen();
        cout << "--- Reset Database ---" << endl;
        cout << "WARNING: this DROPS and recreates all tables. ALL flights, passengers and\n"
                "reservations will be deleted for EVERY open session." << endl;
        string confirm = getStringInput("Type RESET to confirm (anything else cancels): ");
        if (confirm != "RESET") {
            cout << "Cancelled. Nothing was changed." << endl;
            return;
        }
        try {
            exec("SET FOREIGN_KEY_CHECKS = 0");
            exec("DROP TABLE IF EXISTS Reservations");
            exec("DROP TABLE IF EXISTS Passengers");
            exec("DROP TABLE IF EXISTS Flights");
            exec("SET FOREIGN_KEY_CHECKS = 1");
            createTables();
            seedSampleData();
            cout << "Database has been reset to the sample data." << endl;
        } catch (const sql::SQLException& e) {
            try { exec("SET FOREIGN_KEY_CHECKS = 1"); } catch (...) {}
            cout << "Error during reset: " << errText(e) << endl;
        }
    }

    // Removes flights whose departure time has passed, along with their reservations
    void purgeDeparted() {
        try {
            Tx tx(conn.get());
            exec("SET @cut = NOW()");
            exec("DELETE r FROM Reservations r JOIN Flights f ON r.flight_id = f.flight_id WHERE f.departure_time <= @cut");
            exec("DELETE FROM Flights WHERE departure_time <= @cut");
            tx.commit();
        } catch (const sql::SQLException& e) {
            cout << "Error: " << errText(e) << endl;
        }
    }

    // (READ) Displays all flights
    void displayAllFlights() {
        printTable("Available Flights",
                   "SELECT flight_id, flight_number, departure, destination, available_seats, "
                   "DATE_FORMAT(departure_time, '%Y-%m-%d %H:%i:%s') "
                   "FROM Flights WHERE departure_time > NOW() ORDER BY departure_time",
                   {"ID", "Flight No.", "Departure", "Destination", "Available", "Departs At"},
                   {6, 12, 12, 14, 11, 20},
                   "No flights found.");
    }

    // (READ) Displays every passenger together with their ID
    void displayAllPassengers() {
        printTable("All Passengers",
                   "SELECT passenger_id, first_name, last_name, passport_number "
                   "FROM Passengers ORDER BY passenger_id",
                   {"Passenger ID", "First Name", "Last Name", "Passport No."},
                   {15, 18, 18, 18},
                   "No passengers registered yet.");
    }

    // (READ) Look up a passenger ID from a passport number
    void findPassengerId() {
        string passport = getStringInput("Enter passport number: ", 20);
        try {
            unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
                "SELECT passenger_id, first_name, last_name FROM Passengers WHERE passport_number = ?"));
            ps->setString(1, passport);
            unique_ptr<sql::ResultSet> rs(ps->executeQuery());
            if (rs->next()) {
                cout << "\nPassenger found: " << rs->getString("first_name") << " "
                     << rs->getString("last_name")
                     << "  ->  Passenger ID: " << rs->getInt("passenger_id") << endl;
            } else {
                cout << "\nNo passenger with passport number '" << passport << "'." << endl;
            }
        } catch (const sql::SQLException& e) {
            cout << "Error: " << errText(e) << endl;
        }
    }

    // (CREATE) Adds a new passenger
    void addNewPassenger() {
        clearScreen();
        cout << "--- Add New Passenger ---" << endl;
        cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
        string fname    = getStringInput("Enter first name: ", 50);
        string lname    = getStringInput("Enter last name: ", 50);
        string passport = getStringInput("Enter passport number: ", 20);

        try {
            unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
                "INSERT INTO Passengers (first_name, last_name, passport_number) VALUES (?, ?, ?)"));
            ps->setString(1, fname);
            ps->setString(2, lname);
            ps->setString(3, passport);
            ps->executeUpdate();

            cout << "\nPassenger '" << fname << " " << lname << "' added successfully!" << endl;
            cout << "New Passenger ID: " << lastInsertId() << endl;
        } catch (const sql::SQLException& e) {
            if (e.getErrorCode() == ERR_DUP_ENTRY)
                cout << "Error: a passenger with passport number '" << passport << "' already exists." << endl;
            else
                cout << "Error adding passenger: " << errText(e) << endl;
        }
    }

    // (CREATE) Adds a new flight
    void addNewFlight() {
        clearScreen();
        cout << "--- Add New Flight ---" << endl;
        cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
        string number = getStringInput("Enter flight number (e.g. F104): ", 10);
        transform(number.begin(), number.end(), number.begin(),
                  [](unsigned char c) { return (char)toupper(c); });
        string from = getStringInput("Enter departure location: ", 50);
        string to   = getStringInput("Enter destination: ", 50);
        int seats   = getIntInput("Enter number of seats: ", 1);
        string when = getStringInput("Departure (YYYY-MM-DD HH:MM, or +N = N seconds from now): ", 20);

        if (from == to) {
            cout << "Error: departure and destination cannot be the same." << endl;
            return;
        }

        try {
            bool rel = when[0] == '+';
            int secs = 0;
            if (rel) {
                try { secs = stoi(when.substr(1)); } catch (...) {}
                if (secs <= 0) { cout << "Error: use +N with N > 0." << endl; return; }
            } else {
                unique_ptr<sql::PreparedStatement> chk(conn->prepareStatement(
                    "SELECT STR_TO_DATE(?, '%Y-%m-%d %H:%i') > NOW()"));
                chk->setString(1, when);
                unique_ptr<sql::ResultSet> rs(chk->executeQuery());
                if (!rs->next() || rs->isNull(1) || !rs->getBoolean(1)) {
                    cout << "Error: enter a valid FUTURE time as YYYY-MM-DD HH:MM." << endl;
                    return;
                }
            }

            unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
                string("INSERT INTO Flights (flight_number, departure, destination, available_seats, departure_time) VALUES (?, ?, ?, ?, ")
                + (rel ? "DATE_ADD(NOW(), INTERVAL ? SECOND))" : "STR_TO_DATE(?, '%Y-%m-%d %H:%i'))")));
            ps->setString(1, number);
            ps->setString(2, from);
            ps->setString(3, to);
            ps->setInt(4, seats);
            if (rel) ps->setInt(5, secs); else ps->setString(5, when);
            ps->executeUpdate();

            cout << "\nFlight " << number << " (" << from << " -> " << to << ", " << seats
                 << " seats) added successfully!" << endl;
            cout << "New Flight ID: " << lastInsertId() << endl;
        } catch (const sql::SQLException& e) {
            if (e.getErrorCode() == ERR_DUP_ENTRY)
                cout << "Error: flight number '" << number << "' already exists." << endl;
            else
                cout << "Error adding flight: " << errText(e) << endl;
        }
    }

    // (UPDATE/CREATE) Books a seat (TRANSACTION-SAFE)
    // Any failing step throws; the Tx guard rolls back ONLY this transaction and the
    // program carries on.
    bool bookReservation(const string& passport, int f_id) {
        try {
            Tx tx(conn.get());

            // 1. Find the passenger ID from the passport number (passenger must exist)
            int p_id = 0;
            {
                unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
                    "SELECT passenger_id FROM Passengers WHERE passport_number = ?"));
                ps->setString(1, passport);
                unique_ptr<sql::ResultSet> rs(ps->executeQuery());
                if (!rs->next())
                    throw AppError("Error: no passenger with passport number '" + passport + "'.");
                p_id = rs->getInt(1);
            }

            // 2. Check seats AND lock the flight row (flight must not have departed)
            int available_seats = 0;
            if (!queryOneInt("SELECT available_seats FROM Flights WHERE flight_id = ? AND departure_time > NOW() FOR UPDATE", f_id, available_seats))
                throw AppError("Error: Flight not found or already departed.");
            if (available_seats <= 0)
                throw AppError("Sorry, that flight is full.");

            // 3a. Take the seat
            if (execWithInts("UPDATE Flights SET available_seats = available_seats - 1 WHERE flight_id = ?", {f_id}) != 1)
                throw AppError("Failed to update the seat count.");

            // 3b. Create the reservation (fails cleanly on duplicate booking / bad FK)
            if (execWithInts("INSERT INTO Reservations (passenger_id, flight_id) VALUES (?, ?)", {p_id, f_id}) != 1)
                throw AppError("Failed to create the reservation.");
            unsigned long long newId = lastInsertId();

            // 4. Commit
            tx.commit();
            cout << "\nBooking successful! Reservation ID: " << newId
                 << " (Passenger ID: " << p_id << ")" << endl;
            return true;
        } catch (const AppError& e) {
            cout << "\n" << e.what() << " Rolling back - no changes were saved." << endl;
        } catch (const sql::SQLException& e) {
            if (e.getErrorCode() == ERR_DUP_ENTRY)
                cout << "\nThis passenger already has a reservation on that flight.";
            else
                cout << "\nDatabase error: " << errText(e) << ".";
            cout << " Rolling back - no changes were saved." << endl;
        }
        return false;
    }

    // (READ) Shows reservations for a specific passenger
    void displayPassengerReservations(int p_id) {
        try {
            unique_ptr<sql::PreparedStatement> ps(conn->prepareStatement(
                "SELECT r.reservation_id, f.flight_number, f.departure, f.destination "
                "FROM Reservations r "
                "JOIN Flights f ON r.flight_id = f.flight_id "
                "WHERE r.passenger_id = ? ORDER BY r.reservation_id"));
            ps->setInt(1, p_id);
            unique_ptr<sql::ResultSet> rs(ps->executeQuery());

            if (!rs->next()) {
                cout << "\nNo reservations found for Passenger ID " << p_id << "." << endl;
                return;
            }

            cout << "\n--- Reservations for Passenger ID " << p_id << " ---" << endl;
            cout << left << setw(10) << "Res. ID" << setw(15) << "Flight No."
                 << setw(15) << "Departure" << setw(15) << "Destination" << endl;
            cout << string(55, '-') << endl;
            do {
                cout << left << setw(10) << rs->getInt(1)
                     << setw(15) << rs->getString(2).asStdString()
                     << setw(15) << rs->getString(3).asStdString()
                     << setw(15) << rs->getString(4).asStdString() << endl;
            } while (rs->next());
            cout << string(55, '-') << endl;
        } catch (const sql::SQLException& e) {
            cout << "Error: " << errText(e) << endl;
        }
    }

    // (DELETE/UPDATE) Cancels a reservation (TRANSACTION-SAFE)
    bool cancelReservation(int res_id) {
        try {
            Tx tx(conn.get());

            // 1. Find the flight AND lock the reservation row
            int f_id = 0;
            if (!queryOneInt("SELECT flight_id FROM Reservations WHERE reservation_id = ? FOR UPDATE", res_id, f_id))
                throw AppError("Error: Reservation ID " + to_string(res_id) + " not found.");

            // 2a. Delete the reservation
            if (execWithInts("DELETE FROM Reservations WHERE reservation_id = ?", {res_id}) != 1)
                throw AppError("Failed to delete the reservation.");

            // 2b. Give the seat back
            if (execWithInts("UPDATE Flights SET available_seats = available_seats + 1 WHERE flight_id = ?", {f_id}) != 1)
                throw AppError("Failed to update the seat count.");

            // 3. Commit
            tx.commit();
            cout << "\nReservation " << res_id << " successfully cancelled." << endl;
            return true;
        } catch (const AppError& e) {
            cout << "\n" << e.what() << " Rolling back - no changes were saved." << endl;
        } catch (const sql::SQLException& e) {
            cout << "\nDatabase error: " << errText(e) << ". Rolling back - no changes were saved." << endl;
        }
        return false;
    }
};


// --- Main Application Loop ---

string getDbPassword() {
    const char* env = getenv("AIRLINE_DB_PW");
    if (env && *env) return string(env);
    return getStringInput("Enter MySQL password for user '" + string(USER) + "': ", 0, false);
}

int main() {
    cout << "PROGRAM STARTED" << endl;
    Database db;

    if (!db.connect(getDbPassword())) {
        return 1; // Exit if connection fails
    }

    if (!db.setupDatabase()) { // Creates tables only if missing - never wipes data
        return 1;
    }

    bool running = true;
    while (running) {
        clearScreen();
        cout << "\n--- Welcome To Airlines Reservation System ---" << endl;
        cout << "********************************************" << endl;
        cout << " 1. Display All Flights" << endl;
        cout << " 2. Book a Seat" << endl;
        cout << " 3. Cancel a Reservation" << endl;
        cout << " 4. View My Reservations (by Passenger ID)" << endl;
        cout << " 5. Add New Passenger" << endl;
        cout << " 6. Add New Flight" << endl;
        cout << " 7. List All Passengers (with IDs)" << endl;
        cout << " 8. Find Passenger ID (by Passport No.)" << endl;
        cout << " 9. Reset Database (deletes ALL data)" << endl;
        cout << "10. Exit" << endl;
        cout << "********************************************" << endl;

        int val = getMenuChoice();
        db.purgeDeparted();

        try {
        switch (val) {
            case 1:
                db.displayAllFlights();
                pressEnterToContinue();
                break;
            case 2: {
                clearScreen();
                cout << "--- Book a Reservation ---" << endl;
                cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
                db.displayAllFlights();
                string passport = getStringInput("Enter your passport number: ", 20);
                int f_id = getIntInput("Enter Flight ID to book: ", 1);
                db.bookReservation(passport, f_id);
                pressEnterToContinue();
                break;
            }
            case 3: {
                clearScreen();
                cout << "--- Cancel a Reservation ---" << endl;
                cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
                int r_id = getIntInput("Enter Reservation ID to cancel: ", 1);
                db.cancelReservation(r_id);
                pressEnterToContinue();
                break;
            }
            case 4: {
                clearScreen();
                cout << "--- View My Reservations ---" << endl;
                cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
                int p_id = getIntInput("Enter Your Passenger ID: ", 1);
                db.displayPassengerReservations(p_id);
                pressEnterToContinue();
                break;
            }
            case 5:
                db.addNewPassenger();
                pressEnterToContinue();
                break;
            case 6:
                db.addNewFlight();
                pressEnterToContinue();
                break;
            case 7:
                clearScreen();
                db.displayAllPassengers();
                pressEnterToContinue();
                break;
            case 8:
                clearScreen();
                cout << "--- Find Passenger ID ---" << endl;
                cout << "(type 'cancel' at any prompt - or 0 for an ID - to go back)" << endl;
                db.findPassengerId();
                pressEnterToContinue();
                break;
            case 9:
                db.resetDatabase();
                pressEnterToContinue();
                break;
            case 10:
                running = false;
                cout << "\nGood Luck!" << endl;
                sleepMs(1500);
                break;
            default:
                cout << "\nInvalid Input. Please choose from 1-10." << endl;
                sleepMs(1500);
                break;
        }
        } catch (const CancelledError&) {
            // Raised while collecting input, before any database work started.
            cout << "\nCancelled. Nothing was changed." << endl;
            sleepMs(1200);
        }
    }

    return 0; // Database's unique_ptr closes the connection automatically
}