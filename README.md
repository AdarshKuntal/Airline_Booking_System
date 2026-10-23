✈️ Airline Reservation System

A console-based Airline Reservation System built in C++ with MySQL integration. The project demonstrates practical use of Object-Oriented Programming, relational databases, CRUD operations, prepared statements, and transaction-safe booking workflows.

🚀 Features
Flight Management — View available flights with flight number, route, and seat availability.
Passenger Management — Add passengers with validated personal and passport information.
Seat Booking — Book seats with real-time availability checks.
Reservation Management — View reservations for a passenger and track flight details.
Cancellation — Cancel reservations and automatically restore the available seat count.
Transaction Safety — Booking and cancellation use SQL transactions and row-level locking to prevent inconsistent seat counts.
Input Validation — Handles invalid numeric input and prevents empty passenger information.
Formatted CLI Reports — Displays flights and reservations in structured, readable tables.
🏗️ System Design

The system follows an object-oriented design with three core entities:

Flight
 ├── flight_id
 ├── flight_number
 ├── departure
 ├── destination
 └── available_seats

Passenger
 ├── passenger_id
 ├── first_name
 ├── last_name
 └── passport_number

Reservation
 ├── reservation_id
 ├── passenger_id
 └── flight_id

The MySQL database uses a relational structure:

Passengers ───────┐
                  ├── Reservations ──── Flights
                  │
          Foreign Keys
🔄 Booking Workflow
User selects flight
        ↓
Check flight availability
        ↓
Lock flight row
        ↓
Check available seats
        ↓
Decrease seat count
        ↓
Create reservation
        ↓
COMMIT transaction

If any step fails, the transaction is rolled back, keeping the database consistent.

🛠️ Tech Stack
Technology	Usage
C++17	Core application and OOP
MySQL	Relational database
MySQL C API	Database connectivity
SQL	CRUD operations and transactions
Windows API	Console interface utilities
📂 Project Structure
Airline-Reservation-System/
│
├── main.cpp
└── README.md

The entire application is implemented in main.cpp, including the data models, database layer, reservation logic, and CLI.

⚙️ Setup
1. Install MySQL

Create a MySQL database named:

CREATE DATABASE mydb;
2. Configure Database Credentials

Update the credentials in main.cpp:

const char* HOST = "localhost";
const char* USER = "root";
const char* PW   = "your_password";
const char* DB   = "mydb";
3. Compile

Make sure the MySQL development libraries are available, then compile with a C++17-compatible compiler.

g++ main.cpp -std=c++17 -o AirlineReservation
4. Run
./AirlineReservation

On the first run, the application automatically creates the required tables and inserts sample flight/passenger data.

🎯 Key Technical Highlights
Prepared Statements

Passenger creation and reservation operations use prepared SQL statements, providing safer parameter handling compared with constructing SQL queries directly.

Transaction-Safe Reservations

Booking uses:

START TRANSACTION;
SELECT ... FOR UPDATE;
UPDATE Flights SET available_seats = available_seats - 1;
INSERT INTO Reservations (...);
COMMIT;

Cancellation similarly deletes the reservation and restores the seat count within a transaction.

Database Integrity

The schema uses:

Primary keys
Unique passport numbers
Foreign keys
Transaction rollback
Row-level locking

to maintain consistency between flights, passengers, and reservations.

📋 Available Operations
1. Display All Flights
2. Book a Seat
3. Cancel a Reservation
4. View My Reservations
5. Add New Passenger
6. Exit
💡 What This Project Demonstrates

This project brings together C++ OOP + SQL + database design + transaction management in a practical application, with particular emphasis on maintaining consistent reservation and seat-availability data.
