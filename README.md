
---

# **Elevator System**

**Group 1 - ODD: Algorithms 2025**

This project simulates an elevator system with features like floor requests, movement prioritization, and scenario handling (e.g., elevator failure). The system uses **min-heap** and **max-heap** data structures to manage pickup and dropoff requests efficiently. It also effectively uses **queue** for storing request, **stack** to keep track of recently visted floors, **file handling** to store the logs
**array** for display of state.

---

## **Pseudocode for the System**

### **Initialization**
1. Set `CURRENT_FLOOR = 1`.
2. Set `DIRECTION = IDLE`.
3. Initialize:
   - **Min-Heap** for dropoff requests.
   - **Max-Heap** for pickup requests.

---

## **Main Menu**
The system provides the following options:

1. **Request**
2. **Move Elevator**
3. **Display Elevator State**
4. **Show Request Queue**
5. **Scenarios** (e.g., Elevator Failure)
6. **Log File**
7. **Exit**

---

### **Details for Each Choice**

#### **1. Request**
- Prompt the user to input a floor number (**1–9**).
  - Re-prompt if the input is non-integer or outside the valid range.
- Ask the user if the request is for **pickup** or **dropoff**.
  - Use **Max-Heap** for pickup requests.
  - Use **Min-Heap** for dropoff requests.
- Add the request to the appropriate priority queue.

#### **2. Move Elevator**
- Move the elevator to the highest or lowest floor based on the current scenario.
- Prioritize **dropoff requests** over **pickup requests**.
  - For example, if there is a pickup request on the 4th floor and a dropoff request on the 8th floor, the elevator will prioritize the dropoff first.

#### **3. Display Elevator State**
- Display the following information using an array of strings:
  - **Current Floor**
  - **Direction** (`IDLE`, `UP`, or `DOWN`)
  - **Next Stop**

#### **4. Show Request Queue**
- Display all requests in the queue, sorted from largest to smallest.
- Use **Min-Heap** and **Max-Heap** to sort and prioritize floors.

#### **5. Scenarios Menu**
- **Elevator Failure**:
  - Trigger a boolean flag (`SYSTEM_FAILED = TRUE`).
  - If the system is failed:
    - Users cannot request the elevator.
    - The system can be restarted by resetting the boolean flag (`SYSTEM_FAILED = FALSE`).
- Exit the scenarios menu to return to the main menu.

#### **6. Log File**
- Log all inputs and outputs to a file named `elevator.txt`.
- To reset the log, delete `elevator.txt`.

#### **7. Exit**
- Exit the program.

---

## **Input Validation**
- Handle non-integer inputs by prompting the user to enter a valid number (**1–7**).
- Ensure inputs do not exceed the valid range (**1–7**).

---

## **Data Structures**
- **Min-Heap**: Used to prioritize dropoff requests (lowest floor first).
- **Max-Heap**: Used to prioritize pickup requests (highest floor first).

---

## **Example Workflow**
1. A user requests a pickup on the 5th floor.
2. Another user requests a dropoff on the 8th floor.
3. The elevator prioritizes the dropoff request and moves to the 8th floor.
4. After completing the dropoff, the elevator moves to the 5th floor for the pickup.

---

## **Log File**
- All user inputs and system outputs are logged in `elevator.txt`.
- To reset the log, delete the file.

---

## **Scenarios**
- **Elevator Failure**:
  - When triggered, the system stops accepting requests.
  - The system can be reset to resume normal operation.

---

## **Exit**
- Exit the program.

---

## **Transparency**
- There is minimal usage of AI for practicing purposes. Understand that we are not perfect and we still need aid. Thank you.
