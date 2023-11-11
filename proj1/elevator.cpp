// UMBC - CMSC 341 - Spring 2022 - Proj1
/** Matthew Makila
 *  Project 1: Elevator class
 *  elevator.cpp
 *  2/28/22
 *  This file includes all the implementation for the elevator class. There are all elevator methods
 */
#include "elevator.h"

// Name: Elevator
// Desc: Constructs empty elevator object
// Preconditions: None
// Postconditions: Creates empty elevator object
Elevator::Elevator(){
    m_top = nullptr;
    m_ground = nullptr;
}

// Name: ~Elevator
// Desc: Deallocates all memory and destroys elevator object
// Preconditions: Elevator object exists
// Postconditions: destroys elevator object
Elevator::~Elevator(){
    clear();
}

// Name: clear
// Desc: Deallocates all memory and destroys elevator object
// Preconditions: Elevator object exists
// Postconditions: destroys elevator object
void Elevator::clear(){
    Floor* curr = m_top;
    while(curr != nullptr){ // remove each floor
        m_top = curr;
        curr = curr ->m_previous;
        delete m_top;
    }
    // Reset all pointers
    m_top = nullptr;
    m_ground = nullptr;
}

// Name: insertAtHead
// Desc: Inserts a new floor into elevator at head of LL
// Preconditions: Elevator object exists
// Postconditions: New floor inserted at beginning of LL or false
bool Elevator::insertAtHead(int floor, string passenger){
    /**REMEMBER M_GROUND IS HEAD OF LIST!**/
    bool isValid = false;
    if (m_ground == nullptr) { // if we don't have any floors
        Floor *newFloor = new Floor(floor, passenger);
        newFloor->m_next = m_top;
        m_ground = newFloor;
        m_top = newFloor;
        isValid = true;
    } else {
        if (floor < m_ground->m_floorNum) { // add floor at head if less than current ground floor
            Floor *newFloor = new Floor(floor, passenger);
            newFloor->m_next = m_ground;
            m_ground->m_previous = newFloor;
            m_ground = newFloor;
            isValid = true;
        }
    }
    return isValid;
}

// Name: insertAtTail
// Desc: Inserts a new floor into elevator at end of LL
// Preconditions: Elevator object exists
// Postconditions: New floor inserted at end of LL or false
bool Elevator::insertAtTail(int floor, string passenger){
    bool isValid = false;
    if (m_top == nullptr) { // if we have no floors
        Floor *newFloor = new Floor(floor, passenger);
        newFloor->m_next = nullptr;
        newFloor->m_previous = nullptr;
        m_ground = newFloor;
        m_top = newFloor;
        isValid = true;
    } else {
        if (floor > m_top->m_floorNum) { // we only want to keep numerically increasing by floor num!
            Floor *newFloor = new Floor(floor, passenger);
            newFloor->m_previous = m_top;
            m_top->m_next = newFloor;
            m_top = newFloor;
            newFloor->m_next = nullptr;
            // cout << "CHECKER" << m_ground->m_previous << endl;
            // newFloor->m_previous = m_ground;
            isValid = true;
        }
    }
    return isValid;
}

// Name: removeFloor
// Desc: Removes a specific floor of elevator
// Preconditions: Elevator object exists with at least 1 floor
// Postconditions: floor removed or false returned
bool Elevator::removeFloor(int floor){
    bool removed = false;
    if (m_ground == nullptr){ // our elevator has no floors: throw exception?
        return removed;
    }
    // make sure floor valid & in the elevator
    if (floor < m_ground->m_floorNum or floor > m_top->m_floorNum){
        throw out_of_range("Floor does not exist to remove!");
    }
    Floor* temp = m_top;
    bool inElevator = false;
    while (temp != nullptr) { // loop and check if desired floor even exists ...
        if (temp->m_floorNum == floor) {
            inElevator = true;
        }
        temp = temp->m_previous;
    }
    if (!inElevator) {
        return removed;
    }
    // check which side to iterate through
    if (abs(floor - m_top->m_floorNum) < abs(floor - m_ground->m_floorNum)) {
        // closer to top, so we iterate from top
        Floor *curr = m_top;
        if (curr->m_floorNum == floor) { // floor to delete is top floor
            Floor* old_top = m_top;
            m_top = m_top->m_previous;
            m_top->m_next = nullptr;
            delete old_top;
            removed = true;
        } else {
            // make sure curr is not nullptr and count is less than floor
            bool found = false;
            while (curr && !found) {
                if (curr->m_previous->m_floorNum == floor) {
                    found = true;
                } else {
                    curr = curr->m_previous;
                }
            }
            // now that we're here, remove curr's previous floor (floor we want gone)
            Floor* oldPrev = curr->m_previous;
            curr->m_previous = curr->m_previous->m_previous;
            curr->m_previous->m_next = curr;
            delete oldPrev;
            removed = true;
        }
    } else {
        // iterate from ground
        Floor *curr = m_ground;
        // int count = m_ground->m_floorNum;
        if (curr->m_next == nullptr) { // only one floor, make obj empty
            clear();
            removed = true;
        } else {
            if (m_ground->m_floorNum == floor) { // floor to delete is ground floor
                Floor* old_ground = m_ground;
                m_ground = m_ground->m_next;
                m_ground->m_previous = nullptr;
                delete old_ground;
                removed = true;
            } else {
                // make sure curr is not nullptr and count is less than floor
                bool found = false;
                while (curr && !found) {
                    if (curr->m_next->m_floorNum == floor) {
                        found = true;
                    } else {
                        curr = curr->m_next;
                    }
                }
                // now that we're here, remove curr's next floor
                Floor* oldNext = curr->m_next;
                curr->m_next = curr->m_next->m_next;
                curr->m_next->m_previous = curr;
                delete oldNext;
                removed = true;
            }
        }
    }
    return removed;
}

// Name: insertFloor
// Desc: Inserts a new floor into elevator at proper location
// Preconditions: Elevator object exists
// Postconditions: New floor inserted or false (throw ex if out of range)
bool Elevator::insertFloor(int floor){
    bool inserted = false;
    /**SHOULD THIS RETURN FALSE IF EMPTY?**/
    if (m_ground == nullptr){ // our elevator has no floors, just insert one with existing method ...
        insertAtHead(floor);
        return true;
    }
    // make sure floor valid & in the elevator
    if (floor < m_ground->m_floorNum or floor > m_top->m_floorNum){
        throw out_of_range("Cannot insert floor!");
    }
    // check if floor exists
    Floor* temp = m_top;
    bool alreadyThere = false;
    while (temp != nullptr) {
        if (temp->m_floorNum == floor) {
            alreadyThere = true;
        }
        temp = temp->m_previous;
    }
    if (alreadyThere) { // we return false if the floor is already there
        return inserted;
    }

    // check which side to iterate through
    if (abs(floor - m_top->m_floorNum) < abs(floor - m_ground->m_floorNum)) {
        // closer to top, so we iterate from top
        Floor *curr = m_top;
        bool found = false;
        while (curr && !found) {
            if (curr->m_floorNum < floor)
                found = true;
            else {
                curr = curr->m_previous;
            }
        }
        // now that we're here, remove curr's previous floor (floor we want gone)
        Floor* newFloor = new Floor(floor);
        Floor* ante = curr->m_next;
        newFloor->m_previous = curr;
        newFloor->m_next = curr->m_next;
        curr->m_next = newFloor;
        ante->m_previous = newFloor;
        inserted = true;
    } else {
        // iterate from ground
        Floor *curr = m_ground;
        bool found = false;
        while (curr && !found) {
            if (curr->m_floorNum > floor)
                found = true;
            else {
                curr = curr->m_next;
            }
        }
        // now that we're here, remove curr's previous floor (floor we want gone)
        Floor* newFloor = new Floor(floor);
        Floor* ante = curr->m_previous;
        newFloor->m_next = curr;
        newFloor->m_previous = curr->m_previous;
        curr->m_previous = newFloor;
        ante->m_next = newFloor;
        inserted = true;
    }
    return inserted;
}

// Name: checkFloor
// Desc: Checks if floor exists in elevator
// Preconditions: Elevator object exists with at least one floor
// Postconditions: floor found or false (throw ex if out of range)
bool Elevator::checkFloor(int floor){
    bool valid = false;
    if (m_ground == nullptr){ // if elevator has no floors, floor isn't there...
        return valid;
    }
    // make could even be in the elevator
    if (floor < m_ground->m_floorNum or floor > m_top->m_floorNum){
        throw out_of_range("Floor out of range!");
    }
    // check if floor exists inside of elevator
    Floor* temp = m_top;
    while (temp != nullptr) {
        if (temp->m_floorNum == floor) {
            valid = true;
            return valid;
        }
        temp = temp->m_previous;
    }
    return valid;
}

// Name: move
// Desc: Checks if elevator can move between an origin and destination floor. If so, move passenger
// Preconditions: Elevator object exists with at least two floors
// Postconditions: move possible found or false (throw ex if out of range)
bool Elevator::move(int origin, int destination){
    // check if origin and destination floors are valid or out of bounds
    string myPassenger = "";
    bool validMove = false;
    if (!(checkFloor(origin) && checkFloor(destination))) {
        cout << "bad boy!" << endl;
        return validMove;
    }
    if (destination == origin) { // moving to same floor? return false
        cout << "nice try" << endl;
        return validMove;
    }
    bool aPassenger = false; // check if elevator has passenger at origin
    Floor* curr = m_ground;
    while (curr) {
        if (curr->m_floorNum == origin && curr->m_passenger != "") {
            aPassenger = true;
            myPassenger = exit(origin, curr->m_passenger); // take them out for prep to put them at new floor
        }
        curr = curr->m_next;
    }
    if (!aPassenger) {
        cout << "nobody's home" << endl;
        return validMove;
    }

    if ((destination - origin) > 0) { // we're moving up so start at ground
        Floor* curr = m_ground;
        while (curr) {
            if (curr->m_floorNum == destination) {
                enter(curr->m_floorNum, myPassenger);
                validMove = true;
                return validMove;
            }
            curr = curr->m_next;
        }
    } else { // moving down so start at top
        Floor* curr = m_top;
        while (curr) {
            if (curr->m_floorNum == destination) {
                enter(curr->m_floorNum, myPassenger);
                validMove = true;
                return validMove;
            }
            curr = curr->m_previous;
        }
    }
    return validMove;
}

// Name: exit
// Desc: Takes passenger out of elevator
// Preconditions: Floor exists and passenger inside
// Postconditions: passenger removed (back to "") or return ""
string Elevator::exit(int floor, string passenger){
    /**TRY AND CATCH EXCEPTION WITH CHECKFLOOR**/
    try {
        checkFloor(floor);
    } catch (...) {
        return "";
    }
    // traverse till we find passenger
    Floor* curr = m_ground;
    while (curr) {
        if (curr->m_passenger == passenger) { // if found, take them out and return name
            curr->m_passenger = "";
            return passenger;
        }
        curr = curr->m_next;
    }
    return "";
}

// Name: enter
// Desc: Places passenger into elevator at a floor
// Preconditions: Floor exists and no passenger already inside elevator
// Postconditions: updates floor's passenger or false
bool Elevator::enter(int floor, string passenger){
    bool entered = false;
    /**TRY AND CATCH EXCEPTION WITH CHECKFLOOR**/
    try {
        checkFloor(floor);
    } catch (...) {
        return entered;
    }
    Floor* curr = m_ground;
    while (curr) {
        if (curr->m_passenger != "") { // if we find a passenger, false
            return entered;
        }
        curr = curr->m_next;
    }
    curr = m_ground;
    while (curr) { // traverse to correct floor and insert passenger
        if (curr->m_floorNum == floor) {
            curr->m_passenger = passenger;
            entered = true;
            return entered;
        }
        curr = curr->m_next;
    }

    return entered;
}

// Name: Elevator
// Desc: Constructs deep copy of elevator object
// Preconditions: Elevator object exists to copy
// Postconditions: Deep copy created
Elevator::Elevator(const Elevator & rhs){
    m_ground = nullptr;
    m_top = nullptr;
    Floor* curr = rhs.m_ground;
    while (curr) {
        insertAtTail(curr->m_floorNum, curr->m_passenger);
        curr = curr->m_next;
    }
}

// Name: Elevator Assignment Operator
// Desc: Creates deep copy of the RHS
// Preconditions: Elevator object exists
// Postconditions: deep copy created
const Elevator & Elevator::operator=(const Elevator & rhs){
    if (&rhs != this) { // prevent self-assign
        clear(); // clear out old data
        Floor* curr = rhs.m_ground;
        while (curr) {
            insertAtTail(curr->m_floorNum, curr->m_passenger);
            curr = curr->m_next;
        }
    }
    return *this;
}

// Name: dump
// Desc: displays all data and info for a section
// Preconditions: Object is created already and has nonzero info
// Postconditions: Displays all data in elevator
void Elevator::dump(){
    if (m_top != nullptr){
        Floor *temp = m_top;
        cout << "Top Floor " ;
        while(temp->m_previous != nullptr){
            cout << temp->m_floorNum << " (" << temp->m_passenger << ")" << endl;
            temp = temp->m_previous;
        }
        cout << "Ground Floor " << temp->m_floorNum << " (" << temp->m_passenger << ")" << endl;
    }
}