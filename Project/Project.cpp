#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool validateEmail(string email) {
    bool hasAt = false;
    bool hasDot = false;

    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            hasAt = true; 
        }
        if (email[i] == '.') {
            hasDot = true; 
        }
        if (hasAt && hasDot) {
            return true;
        }
    }
    return false;
}

bool validatePassword(string password) {
    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;
    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') {
            hasUpper = true; // Uppercase letter found
        }
        if (password[i] >= 'a' && password[i] <= 'z') {
            hasLower = true; // Lowercase letter found
        }
        if (password[i] >= '0' && password[i] <= '9') {
            hasDigit = true; // Digit found
        }
        if (password[i] == '!' || password[i] == '@' || password[i] == '#' || 
            password[i] == '$' || password[i] == '%' || password[i] == '^' || 
            password[i] == '&' || password[i] == '*') {
            hasSpecial = true; // Special character found
        }

        if (hasUpper && hasLower && hasDigit && hasSpecial) {
            break;
        }
    }

    // Password must be at least 8 characters long
    if (hasUpper && hasLower && hasDigit && hasSpecial && password[8] != '\0') {
        return true;
    }
    return false;
}

// Node for Linked List to represent Rooms
struct Room {
    int id;
    string type; // Single, Double, Suite
    string status; // Occupied, Booked, Ready, Unavailable
    int availability[30]; // Next 30 days availability
    Room* next;

    Room(int roomId, string roomType, string roomStatus){
        id = roomId; 
        type = roomType; 
        status = roomStatus; 
        next = NULL;
        for (int i = 0; i < 30; i++) {
            availability[i] = 1; // All days initially available
        }
    }
};

// Linked List for Floor Management
class Floor {
private:
    Room* head;

public:
    Floor() {
        head = NULL;
    }

    void addRoom(int id, string type, string status) {
        Room* newRoom = new Room(id, type, status);
        if (!head) {
            head = newRoom;
        } else {
            Room* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newRoom;
        }
    }

    void displayRooms() {
        Room* temp = head;
        while (temp) {
            cout << "Room ID: " << temp->id << ", Type: " << temp->type << ", Status: " << temp->status << endl;
            temp = temp->next;
        }
    }

    Room* findAvailableRoom(const string& type, int nights) {
        Room* temp = head;
        while (temp) {
            if (temp->type == type && temp->status == "Ready") {
                bool available = true;
                for (int i = 0; i < nights && i < 30; i++) {
                    if (temp->availability[i] == 0) {
                        available = false;
                        break;
                    }
                }
                if (available) {
                    return temp;
                }
            }
            temp = temp->next;
        }
        return NULL;
    }

    void updateRoomAvailability(int roomId, int nights) {
        Room* temp = head;
        while (temp) {
            if (temp->id == roomId) {
                for (int i = 0; i < nights && i < 30; i++) {
                    temp->availability[i] = 0; // Mark days as occupied
                }
                return;
            }
            temp = temp->next;
        }
    }

    void resetRoomAvailability(int roomId, int nights) {
        Room* temp = head;
        while (temp) {
            if (temp->id == roomId) {
                for (int i = 0; i < nights && i < 30; i++) {
                    temp->availability[i] = 1; // Reset days to available
                }
                return;
            }
            temp = temp->next;
        }
    }
};

// Queue Node for Booking Requests
struct BookingRequest {
    string customerName;
    string roomType;
    int nights;
    bool highPriority; // Flag for high-priority requests
    BookingRequest* next;

    BookingRequest(string name, string type, int stay, bool priority){
        customerName = name;
        roomType = type;
        nights = stay;
        highPriority = priority;
        next = NULL;
    }
};

// Queue for Booking Management
class BookingQueue {
private:
    BookingRequest* front;
    BookingRequest* rear;
    int count;

public:
    BookingQueue(){
        front = NULL;
        rear = NULL;
        count = 0;
    }

    void enqueue(string customerName, string roomType, int nights, bool highPriority) {
        BookingRequest* newRequest = new BookingRequest(customerName, roomType, nights, highPriority);
        if (!rear || highPriority) {
            newRequest->next = front;
            front = newRequest;
            if (!rear) rear = newRequest;
        } else {
            rear->next = newRequest;
            rear = newRequest;
        }
        count++;
    }

    BookingRequest* dequeue() {
        if (!front){
            return NULL;
        }

        BookingRequest* temp = front;
        front = front->next;
        if (!front)
        {
            rear = NULL;
        }
        count--;
        return temp;
    }

    bool isEmpty() {
        return front == NULL;
    }

    bool isReadyToProcess() {
        return count >= 10;
    }

    void displayQueue() {
        BookingRequest* temp = front;
        while (temp) {
            cout << "Customer: " << temp->customerName << ", Room Type: " << temp->roomType
                 << ", Nights: " << temp->nights
                 << (temp->highPriority ? " (High Priority)" : "") << endl;
            temp = temp->next;
        }
    }
};

// Stack Node for Booking History
struct BookingHistory {
    string customerName;
    int roomId;
    int nights;
    BookingHistory* next;

    BookingHistory(string name, int id, int stay){
        customerName = name;
        roomId = id;
        nights = stay;
        next = NULL;
    }
};

// Stack for Booking History
class BookingStack {
private:
    BookingHistory* top;

public:
    BookingStack(){
        top = NULL;
    }

    void push(string customerName, int roomId, int nights) {
        BookingHistory* newRecord = new BookingHistory(customerName, roomId, nights);
        newRecord->next = top;
        top = newRecord;
    }

    BookingHistory* pop() {
        if (!top){
            return NULL;
        }
        BookingHistory* temp = top;
        top = top->next;
        return temp;
    }

    void displayStack() {
        BookingHistory* temp = top;
        while (temp) {
            cout << "Customer: " << temp->customerName << ", Room ID: " << temp->roomId << ", Nights: " << temp->nights << endl;
            temp = temp->next;
        }
    }

    bool isEmpty() {
        return top == NULL;
    }
};

int main() {
    string email, password;
    cout << "Enter email: ";
    cin >> email;
    while (!validateEmail(email)) {
        cout << "Invalid email. Please enter again: ";
        cin >> email;
    }

    cout << "Enter password: ";
    cin >> password;
    while (!validatePassword(password)) {
        cout << "Password must contain at least 8 digit, one uppercase, one lowercase, one digit, and one special character. Enter again: ";
        cin >> password;
    }
    cout<<"Login Succesful!...\n"<<endl;

    Floor hotelFloors[5];
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j <= 10; j++) {
            string type = (j % 3 == 0) ? "Suite" : (j % 2 == 0) ? "Double" : "Single";
            hotelFloors[i].addRoom(j + i * 10, type, "Ready");
        }
    }

    BookingQueue bookingQueue;
    BookingStack bookingHistory;

    int choice;
    do {
       	cout << "=========================\n";
    	cout << "=-----------------------=\n";
        cout << "=  --- Galaxy Hotel --- =\n";
        cout << "=-----------------------=\n";
        cout << "=========================\n\n";
        cout << "1. Display Rooms\n";
        cout << "2. Add Booking Request\n";
        cout << "3. Process Booking Requests\n";
        cout << "4. Display Booking History\n";
        cout << "5. Cancel Most Recent Booking\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            for (int i = 0; i < 5; i++) {
                cout << "\nFloor " << i + 1 << ":\n";
                hotelFloors[i].displayRooms();
            }
            break;

        case 2: {
            string customerName, roomType;
            int nights;
            char priority;
            cout << "Enter customer name: ";
            cin >> customerName;
            cout << "Enter room type (Single/Double/Suite): ";
            cin >> roomType;
            cout << "Enter number of nights: ";
            cin >> nights;
            if (nights > 30) {
                cout << "Bookings cannot exceed 30 days.\n";
                break;
            }
            cout << "Is this a high-priority request? (y/n): ";
            cin >> priority;
            bookingQueue.enqueue(customerName, roomType, nights, priority == 'y' || priority == 'Y');
            cout << "Booking request added.\n";

            if (bookingQueue.isReadyToProcess()) {
                cout << "Processing batch of 10 requests...\n";
                for (int i = 0; i < 10 && !bookingQueue.isEmpty(); i++) {
                    BookingRequest* request = bookingQueue.dequeue();
                    bool booked = false;
                    for (int i = 0; i < 5; i++) {
                        Room* room = hotelFloors[i].findAvailableRoom(request->roomType, request->nights);
                        if (room) {
                            room->status = "Occupied";
                            hotelFloors[i].updateRoomAvailability(room->id, request->nights);
                            bookingHistory.push(request->customerName, room->id, request->nights);
                            cout << "Booking Successful: " << request->customerName << " in Room " << room->id << endl;
                            booked = true;
                            break;
                        }
                    }
                    if (!booked) {
                        cout << "No available rooms for " << request->roomType << ".\n";
                    }
                }
            }
            break;
        }

        case 3:
            cout << "\nProcessing remaining requests...\n";
            while (!bookingQueue.isEmpty()) {
                BookingRequest* request = bookingQueue.dequeue();
                bool booked = false;
                for (int i = 0; i < 5; i++) {
                    Room* room = hotelFloors[i].findAvailableRoom(request->roomType, request->nights);
                    if (room) {
                        room->status = "Occupied";
                        hotelFloors[i].updateRoomAvailability(room->id, request->nights);
                        bookingHistory.push(request->customerName, room->id, request->nights);
                        cout << "Booking Successful: " << request->customerName << " in Room " << room->id << endl;
                        booked = true;
                        break;
                    }
                }
                if (!booked) {
                    cout << "No available rooms for " << request->roomType << ".\n";
                }
            }
            break;

        case 4:
            cout << "\nBooking History:\n";
            bookingHistory.displayStack();
            break;

        case 5:
            if (!bookingHistory.isEmpty()) {
                BookingHistory* recent = bookingHistory.pop();
                for (int i = 0; i < 5; i++) {
                    hotelFloors[i].resetRoomAvailability(recent->roomId, recent->nights);
                }
                cout << "Most recent booking canceled for Customer: " << recent->customerName << " in Room ID: " << recent->roomId << endl;
                delete recent;
            } else {
                cout << "No bookings to cancel.\n";
            }
            break;

        case 6:
            cout << "Exiting system. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
