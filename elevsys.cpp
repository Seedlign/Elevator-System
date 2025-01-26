#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

#define MAX_FLOORS 9
#define MAX_REQUESTS 100

void logActivity(const string& message) {
    ofstream logFile("elevator_log.txt", ios::app);
    if (logFile.is_open()) {
        logFile << message << "\n";
        logFile.close();
    } else {
        cout << "Error: Unable to open log file!\n";
    }
}

void viewLogFile() {
    ifstream logFile("elevator_log.txt");
    if (logFile.is_open()) {
        string line;
        cout << "\n================ ELEVATOR ACTIVITY LOG ================\n";
        while (getline(logFile, line)) {
            cout << line << "\n";
        }
        cout << "=======================================================\n";
        logFile.close();
    } else {
        cout << "Error: Unable to open log file!\n";
    }
}

void clearLogFile() {
    ofstream logFile("elevator_log.txt", ios::trunc); // Open in truncation mode to clear the file
    if (logFile.is_open()) {
        logFile.close();
        cout << "Log file cleared successfully.\n";
        logActivity("Log file cleared.");
    } else {
        cout << "Error: Unable to open log file!\n";
    }
}

class Queue {
    int data[MAX_REQUESTS];
    int front, rear;

public:
    Queue() : front(-1), rear(-1) {}

    bool isEmpty() {
        return front == -1;
    }

    bool isFull() {
        return rear == MAX_REQUESTS - 1;
    }

    void enqueue(int value) {
        if (isFull()) {
            cout << "Queue is full!\n";
            return;
        }
        if (isEmpty()) front = 0;
        data[++rear] = value;
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty!\n";
            return -1;
        }
        int value = data[front];
        if (front == rear) front = rear = -1;
        else front++;
        return value;
    }

    int peek() {
        if (isEmpty()) return -1;
        return data[front];
    }
};

class PrioQueue {
    int data[MAX_REQUESTS];
    int size;
    bool isMaxHeap;

public:
    PrioQueue(bool isMaxHeap = true) : size(0), isMaxHeap(isMaxHeap) {}

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size == MAX_REQUESTS;
    }

    void enqueue(int value) {
        if (isFull()) {
            cout << "Priority Queue is full!\n";
            return;
        }
        data[size] = value;
        size++;
        int i = size - 1;
        if (isMaxHeap) {
            while (i > 0 && data[(i - 1) / 2] < data[i]) {
                swap(data[i], data[(i - 1) / 2]);
                i = (i - 1) / 2;
            }
        } else {
            while (i > 0 && data[(i - 1) / 2] > data[i]) {
                swap(data[i], data[(i - 1) / 2]);
                i = (i - 1) / 2;
            }
        }
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Priority Queue is empty!\n";
            return -1;
        }
        int root = data[0];
        data[0] = data[size - 1];
        size--;
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;
            if (isMaxHeap) {
                if (left < size && data[left] > data[largest]) {
                    largest = left;
                }
                if (right < size && data[right] > data[largest]) {
                    largest = right;
                }
            } else {
                if (left < size && data[left] < data[largest]) {
                    largest = left;
                }
                if (right < size && data[right] < data[largest]) {
                    largest = right;
                }
            }
            if (largest == i) break;
            swap(data[i], data[largest]);
            i = largest;
        }
        return root;
    }

    int peek() {
        if (isEmpty()) return -1;
        return data[0];
    }

    void displayQueue() {
        if (isEmpty()) {
            cout << "No pending requests.\n";
            return;
        }
        cout << "Request Queue: ";
        for (int i = 0; i < size; i++) {
            cout << "[" << data[i] << "]";
            if (i < size - 1) cout << "/";
        }
        cout << "\n";
    }

    int* getData() {
        return data;
    }

    int getSize() {
        return size;
    }
};

class Stack {
    int data[MAX_REQUESTS];
    int top;

public:
    Stack() : top(-1) {}

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX_REQUESTS - 1;
    }

    void push(int value) {
        if (isFull()) {
            cout << "Stack is full!\n";
            return;
        }
        data[++top] = value;
    }

    int pop() {
        if (isEmpty()) {
            cout << "Stack is empty!\n";
            return -1;
        }
        return data[top--];
    }

    int peek() {
        if (isEmpty()) return -1;
        return data[top];
    }
};

class Elevator {
    int currentFloor;
    string direction;
    PrioQueue pickQueue;
    PrioQueue dropQueue;
    Stack history;
    bool isFailed;

public:
    Elevator() : currentFloor(1), direction("idle"), pickQueue(true), dropQueue(false), isFailed(false) {
        logActivity("Elevator initialized at floor 1.");
    }

    int getCurrentFloor() {
        return currentFloor;
    }

    string getDirection() {
        return direction;
    }

    int getNextStop() {
        if (!dropQueue.isEmpty()) {
            return dropQueue.peek();
        }
        if (!pickQueue.isEmpty()) {
            return pickQueue.peek();
        }
        return -1;
    }

    void requestFloor(int floor, bool isPickRequest) {
        if (isFailed) {
            cout << "Elevator failure. Cannot accept new requests right now. Please wait patiently.\n";
            return;
        }
        if (floor < 1 || floor > MAX_FLOORS) {
            cout << "Invalid floor request! Please choose a floor between 1 and " << MAX_FLOORS << ".\n";
            return;
        }

        if (currentFloor == floor) {
            if (isPickRequest) {
                cout << "Elevator is already at floor " << floor << ". User has been picked up.\n";
                logActivity("User picked up at floor " + to_string(floor));
            } else {
                cout << "Elevator is already at floor " << floor << ". User has been dropped off.\n";
                logActivity("User dropped off at floor " + to_string(floor));
            }
            return;
        }

        bool requestExists = false;
        if (isPickRequest) {
            for (int i = 0; i < pickQueue.getSize(); i++) {
                if (pickQueue.getData()[i] == floor) {
                    requestExists = true;
                    break;
                }
            }
        } else {
            for (int i = 0; i < dropQueue.getSize(); i++) {
                if (dropQueue.getData()[i] == floor) {
                    requestExists = true;
                    break;
                }
            }
        }

        if (!requestExists) {
            if (isPickRequest) {
                pickQueue.enqueue(floor);
                cout << "Pick-up request added: " << floor << "\n";
                logActivity("Pick-up request added: " + to_string(floor));
            } else {
                dropQueue.enqueue(floor);
                cout << "Drop-off request added: " << floor << "\n";
                logActivity("Drop-off request added: " + to_string(floor));
            }
        } else {
            cout << "Request for floor " << floor << " already exists in the queue.\n";
        }
    }

    void moveElevator() {
        if (isFailed) {
            cout << "Elevator is failed. Cannot move until reset.\n";
            return;
        }
        if (pickQueue.isEmpty() && dropQueue.isEmpty()) {
            cout << "No pending requests. Elevator is idle.\n";
            return;
        }

        int nextFloor = -1;

        if (!dropQueue.isEmpty()) {
            nextFloor = dropQueue.peek();
        } else if (!pickQueue.isEmpty()) {
            nextFloor = pickQueue.peek();
        }

        if (nextFloor > currentFloor) {
            direction = "up";
            while (currentFloor < nextFloor) {
                currentFloor++;
                cout << "Moved to floor: " << currentFloor << " (Direction: " << direction << ")\n";
                logActivity("Moved to floor: " + to_string(currentFloor) + " (Direction: " + direction + ")");
            }
        } else if (nextFloor < currentFloor) {
            direction = "down";
            while (currentFloor > nextFloor) {
                currentFloor--;
                cout << "Moved to floor: " << currentFloor << " (Direction: " << direction << ")\n";
                logActivity("Moved to floor: " + to_string(currentFloor) + " (Direction: " + direction + ")");
            }
        }

        if (!dropQueue.isEmpty() && dropQueue.peek() == currentFloor) {
            dropQueue.dequeue();
        } else if (!pickQueue.isEmpty() && pickQueue.peek() == currentFloor) {
            pickQueue.dequeue();
        }

        if (pickQueue.isEmpty() && dropQueue.isEmpty()) {
            direction = "idle";
            cout << "Elevator is now idle.\n";
            logActivity("Elevator is now idle.");
        }
    }

    void displayState() {
        string state[3];
        state[0] = "Current Floor: " + to_string(currentFloor);
        state[1] = "Direction: " + direction;
        if (!pickQueue.isEmpty() || !dropQueue.isEmpty()) {
            state[2] = "Next Stop: " + to_string(getNextStop());
        } else {
            state[2] = "Next Stop: None";
        }

        cout << "\n================ ELEVATOR STATE ================\n";
        for (int i = 0; i < 3; i++) {
            cout << state[i] << "\n";
        }
        if (isFailed) {
            cout << "Status: FAILED (Cannot move until reset)\n";
        }
        cout << "================================================\n";
    }

    void displayRequestQueue() {
        cout << "\n================ REQUEST QUEUE ================\n";
        cout << "Pick-up Queue: ";
        pickQueue.displayQueue();
        cout << "Drop-off Queue: ";
        dropQueue.displayQueue();
        cout << "================================================\n";
    }

    void reset() {
        currentFloor = 1;
        direction = "idle";
        while (!pickQueue.isEmpty()) {
            pickQueue.dequeue();
        }
        while (!dropQueue.isEmpty()) {
            dropQueue.dequeue();
        }
        while (!history.isEmpty()) {
            history.pop();
        }
        isFailed = false;
        cout << "Elevator has been reset to floor 1.\n";
        logActivity("Elevator reset to floor 1.");
    }

    void simulateFailure() {
        isFailed = true;
        cout << "Elevator failure simulated! Elevator is stuck on floor " << currentFloor << ".\n";
        logActivity("Elevator failure simulated on floor " + to_string(currentFloor));
    }
};

void scenarioMenu(Elevator& elevator) {
    int scenarioChoice;
    while (true) {
        cout << "\n================ SCENARIO MENU ================\n";
        cout << "1. Simulate Elevator Failure\n";
        cout << "2. Reset Elevator\n";
        cout << "3. Return to Main Menu\n";
        cout << "================================================\n";
        cout << "Enter your choice: ";
        cin >> scenarioChoice;

        if (cin.fail() || scenarioChoice < 1 || scenarioChoice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number between 1 and 3.\n";
            continue;
        }

        switch (scenarioChoice) {
            case 1:
                elevator.simulateFailure();
                break;
            case 2:
                elevator.reset();
                break;
            case 3:
                return;
            default:
                cout << "Invalid choice. Try again!\n";
        }
    }
}

int main() {
    Elevator elevator;
    int choice, floor;
    char requestType;

    while (true) {
        cout << "\n================ Uno Grupo Elevatore ================\n";
        cout << "1. Request Floor\n";
        cout << "2. Move Elevator\n";
        cout << "3. Display Elevator State\n";
        cout << "4. Display Request Queue\n";
        cout << "5. Scenario Menu\n";
        cout << "6. View Activity Log\n";
        cout << "7. Clear Activity Log\n";
        cout << "8. Exit\n";
        cout << "=====================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 8) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number between 1 and 8.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                while (true) {
                    cout << "Enter floor (1-" << MAX_FLOORS << "): ";
                    cin >> floor;
                    if (cin.fail() || floor < 1 || floor > MAX_FLOORS) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number between 1 and " << MAX_FLOORS << ".\n";
                    } else {
                        break;
                    }
                }

                while (true) {
                    cout << "For pick-up? (y/n): ";
                    cin >> requestType;
                    if (cin.fail() || (requestType != 'y' && requestType != 'Y' && requestType != 'n' && requestType != 'N')) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter 'y' for yes or 'n' for no.\n";
                    } else {
                        break;
                    }
                }

                elevator.requestFloor(floor, requestType == 'y' || requestType == 'Y');
                break;
            }
            case 2:
                elevator.moveElevator();
                break;
            case 3:
                elevator.displayState();
                break;
            case 4:
                elevator.displayRequestQueue();
                break;
            case 5:
                scenarioMenu(elevator);
                break;
            case 6:
                viewLogFile();
                break;
            case 7:
                clearLogFile();
                break;
            case 8:
                cout << "Exiting program. Goodbye!\n";
                logActivity("Program exited.");
                return 0;
            default:
                cout << "Invalid choice. Try again!\n";
        }
    }
}