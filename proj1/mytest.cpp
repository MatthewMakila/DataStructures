// UMBC - CMSC 341 - Spring 2022 - Proj1
/** Matthew Makila
*  Project 1: Elevator class
*  mytest.cpp
*  2/28/22
*  This testing file includes all the tests for the elevator. There are all testing methods
*  defined here
*/
#include "elevator.h"

const char* FAIL_STATEMENT = "*****TEST FAILED: ";
const char* PASS_STATEMENT = "     TEST PASSED: ";

class Tester{
public:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
    // Constructor test
    bool test_constructor();
    // Insert at head tests
    bool test_insertAtHead();
    bool test_insertAtHeadError();
    // Insert at tail tests
    bool test_insertAtTail();
    bool test_insertAtTailError();
    // Remove tests
    bool test_remove();
    bool test_removeError();
    bool test_removeError2();
    // insert tests
    bool test_insert();
    bool test_insertError(); // BREAK INTO 2?
    bool test_insertError2();
    // checkFloor tests
    bool test_checkFloor();
    bool test_checkFloorError(); // BREAK INTO 2?
    bool test_checkFloorError2();
    // enter and exit tests
    bool test_enter();
    bool test_enterError(); // BREAK INTO 2?
    bool test_enterError2();
    bool test_exit();
    bool test_exitError(); // BREAK INTO 2?
    bool test_exitError2();
    // move tests
    // Copy constructor tests
    bool test_copyConstructor(); // test should loop through and compare each floor's address & stored val
    bool test_copyConstructorEdge();
    bool test_copyConstructorEdge2();
    bool test_copyConstructorEmpty();
    // Assignment Op tests
    bool test_assignOp();
    bool test_assignOpEdge();
    bool test_assignOpEdge2();
    bool test_assignOpEmpty();
    // time tests
    bool test_insertionTimes();
};

bool Tester::test_insertAtHead() {
    Elevator myE;
    bool testFlag = true;
    for (int i = 300; i > 0; i--) {
        if (!myE.insertAtHead(i)) {
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_insertAtHeadError() {
    Elevator myE;
    bool testFlag = false;
    myE.insertAtHead(1);
    if (!myE.insertAtHead(2)){
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_insertAtTail() {
    Elevator myE;
    bool testFlag = true;
    for (int i = 1; i <= 300; i++) {
        if (!myE.insertAtTail(i)) {
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_insertAtTailError() {
    Elevator myE;
    bool testFlag = false;
    myE.insertAtTail(1);
    if (!myE.insertAtTail(0)){
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_constructor() {
    Elevator myE;
    bool testFlag = false;
    if (myE.m_ground == nullptr && myE.m_top == nullptr) { // we just want to make sure object is empty and exists
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_remove() {
    // test emptying elevators forward and backwards
    bool testFlag = true;
    Elevator myE;
    for (int i = 1; i <= 10; i++) {
        myE.insertAtTail(i);
    }
    for (int i = 1; i <= 10; i++) {
        if (!myE.removeFloor(i)) {
            testFlag = false;
        }
    }

    for (int i = 1; i <= 10; i++) {
        myE.insertAtTail(i);
    }
    for (int i = 10; i > 0; i--) {
        if (!myE.removeFloor(i)) {
            testFlag = false;
        }
    }

    return testFlag;
}

bool Tester::test_removeError() {
    bool testFlag = false;
    Elevator myE;
    for (int i = 1; i <= 10; i++) {
        myE.insertAtTail(i);
    }
    try {
        myE.removeFloor(11);  // should throw exception
    } catch (...) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_removeError2() {
    bool testFlag = false;
    Elevator myE;
    for (int i = 1; i <= 10; i++) {
        myE.insertAtTail(i);
    }
    myE.removeFloor(5);
    if (!myE.removeFloor(5)) { // should not allow
        testFlag = true;
    }
    return testFlag;;
}

bool Tester::test_insert() {
    bool testFlag = true;
    Elevator myE;
    myE.insertAtHead(1);
    myE.insertAtTail(100);
    for (int i = 2; i <= 99; i++) {
        if (!myE.insertFloor(i)) { // should add 2 - 99
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_insertError() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    // test 1: inserting a floor out of range
    try {
        myE.insertFloor(2);  // should throw exception
    } catch (...) {
        testFlag = true;
    }
    return testFlag; // should return true if BOTH tests worked
}

bool Tester::test_insertError2() {
    bool testFlag = false;
    Elevator myE;
    for (int i = 1; i <= 5; i++) {
        myE.insertAtTail(i);
    }
    // test 2: inserting a floor that already exists
    if (!myE.insertFloor(3)){
        testFlag = true;
    }
    return testFlag; // should return true if BOTH tests worked
}

bool Tester::test_checkFloor() {
    bool testFlag = true;
    Elevator myE;
    for (int i = 1; i <= 10; i++) {
        myE.insertAtTail(i);
    }
    for (int i = 1; i <= 10; i++) {
        if (!myE.checkFloor(i)) { // make sure every floor we inserted is there
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_checkFloorError() {
    bool testFlag = false;
    Elevator myE;
    for (int i = 1; i <= 5; i++) {
        myE.insertAtTail(i);
    }
    // test 1: checking a floor out of range
    try {
        myE.checkFloor(11);  // should throw exception
    } catch (...) {
        testFlag = true;
    }
    return testFlag; // should return true if BOTH tests worked
}

bool Tester::test_checkFloorError2() {
    bool testFlag = false;
    Elevator myE;
    for (int i = 1; i <= 5; i++) {
        myE.insertAtTail(i);
    }
    myE.removeFloor(2);
    // test 2: checking a floor that DNE
    if (!myE.checkFloor(2)){
        testFlag = true;
    }
    return testFlag; // should return true if BOTH tests worked
}

bool Tester::test_enter() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    if (myE.enter(1,"Greg")) { // simply put person inside and check if valid
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_enterError() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    if (!myE.enter(3, "Jake")) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_enterError2() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    myE.enter(1, "Greg");
    if (!myE.enter(2,"Jimmy")) { // should fail since person already in elevator
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_exit() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    myE.enter(1, "Greg");
    if (myE.exit(1, "Greg") == "Greg") { // simply take person out and check if valid
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_exitError() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    if (myE.exit(3,"Jimmy") == "") { // should fail since out of range
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_exitError2() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    myE.enter(1, "Greg");
    if (myE.exit(2,"Jimmy") == "") { // should fail since person already in elevator
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_copyConstructor() {
    bool testFlag = true;
    Elevator myE;
    for (int i = 1; i <= 30; i++) {
        myE.insertAtTail(i);
    }
    myE.enter(20, "Matt");
    Elevator myE2 = Elevator(myE);

    Floor* curr = myE.m_ground;
    Floor* curr2 = myE2.m_ground;
    while (curr && curr2) {
        // make sure mem addresses diff but data the same
        if ((curr == curr2) or (curr->m_floorNum != curr2->m_floorNum) or (curr->m_passenger != curr2->m_passenger)) {
            testFlag = false;
        }
        curr = curr->m_next;
        curr2 = curr2->m_next;
    }
    return testFlag;
}

bool Tester::test_copyConstructorEdge() {
    bool testFlag = false;
    Elevator myE;
    myE.insertAtTail(1);
    Elevator myE2 = Elevator(myE);
    // confirm same data but diff addresses
    if ((myE.m_ground != myE2.m_ground) && (myE.m_ground->m_floorNum == myE2.m_top->m_floorNum)) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_copyConstructorEdge2() {
    bool testFlag = true;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    Elevator myE2 = Elevator(myE);

    Floor* curr = myE.m_ground;
    Floor* curr2 = myE2.m_ground;
    while (curr && curr2) {
        // make sure mem addresses diff but data the same
        if ((curr == curr2) or (curr->m_floorNum != curr2->m_floorNum) or (curr->m_passenger != curr2->m_passenger)) {
            testFlag = false;
        }
        curr = curr->m_next;
        curr2 = curr2->m_next;
    }
    return testFlag;
}

bool Tester::test_copyConstructorEmpty() {
    bool testFlag = false;
    Elevator myE;
    Elevator myE2 = Elevator(myE);
    // confirm same data but diff addresses
    if (myE.m_ground == nullptr && myE2.m_ground == nullptr) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_assignOp() {
    bool testFlag = true;
    Elevator myE;
    for (int i = 1; i <= 30; i++) {
        myE.insertAtTail(i);
    }
    myE.enter(20, "Matt");
    Elevator myE2;
    myE2 = myE; // do assignment

    Floor* curr = myE.m_ground;
    Floor* curr2 = myE2.m_ground;
    while (curr && curr2) {
        // make sure mem addresses diff but data the same
        if ((curr == curr2) or (curr->m_floorNum != curr2->m_floorNum) or (curr->m_passenger != curr2->m_passenger)) {
            testFlag = false;
        }
        curr = curr->m_next;
        curr2 = curr2->m_next;
    }
    return testFlag;
}

bool Tester::test_assignOpEdge() {
    /**DO EDGE CASES WITH 1 AND 2 FLOORS**/
    bool testFlag = true;
    Elevator myE;
    myE.insertAtTail(1);
    Elevator myE2;
    myE2 = myE;
    // confirm same data but diff addresses
    Floor* curr = myE.m_ground;
    Floor* curr2 = myE2.m_ground;
    while (curr && curr2) {
        // make sure mem addresses diff but data the same
        if ((curr == curr2) or (curr->m_floorNum != curr2->m_floorNum) or (curr->m_passenger != curr2->m_passenger)) {
            testFlag = false;
        }
        curr = curr->m_next;
        curr2 = curr2->m_next;
    }
    return testFlag;
}

bool Tester::test_assignOpEdge2() {
    bool testFlag = true;
    Elevator myE;
    myE.insertAtTail(1);
    myE.insertAtTail(2);
    Elevator myE2;
    myE2 = myE;

    Floor* curr = myE.m_ground;
    Floor* curr2 = myE2.m_ground;
    while (curr && curr2) {
        // make sure mem addresses diff but data the same
        if ((curr == curr2) or (curr->m_floorNum != curr2->m_floorNum) or (curr->m_passenger != curr2->m_passenger)) {
            testFlag = false;
        }
        curr = curr->m_next;
        curr2 = curr2->m_next;
    }
    return testFlag;
}

bool Tester::test_assignOpEmpty() {
    bool testFlag = false;
    Elevator myE;
    Elevator myE2;
    myE2 = myE;
    // confirm same data but diff addresses
    if (myE.m_ground == nullptr && myE2.m_ground == nullptr) {
        testFlag = true;
    }
    return testFlag;
}

int main() {
    Tester myTest;
    cout << "/** TEST CONSTRUCTOR **/" << endl;
    // test constructor works
    {
        if (myTest.test_constructor()) {
            cout << PASS_STATEMENT << "[default constructor creates empty obj.]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[default constructor creates empty obj.]" << endl;
        }
    }

    cout << "\n/** TEST INSERT HEAD **/" << endl;
    // test norm case insert at head
    {
        if (myTest.test_insertAtHead()) {
            cout << PASS_STATEMENT << "[norm. case for insertAtHead 300 floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm. case for insertAtHead 300 floors]" << endl;
        }
    }
    // test error case insert at head
    {
        if (myTest.test_insertAtHeadError()) {
            cout << PASS_STATEMENT << "[error case for insertAtHead wrong order floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for insertAtHead wrong order floors]" << endl;
        }
    }

    cout << "\n/** TEST INSERT TAIL **/" << endl;
    // test norm case insert at tail
    {
        if (myTest.test_insertAtTail()) {
            cout << PASS_STATEMENT << "[norm. case for insertAtTail 300 floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm. case for insertAtTail 300 floors]" << endl;
        }
    }
    // test error case insert tail
    {
        if (myTest.test_insertAtTailError()) {
            cout << PASS_STATEMENT << "[error case for insertAtTail wrong order floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for insertAtTail wrong order floors]" << endl;
        }
    }

    cout << "\n/** TEST REMOVE **/" << endl;
    // test remove norm. case
    {
        if (myTest.test_remove()) {
            cout << PASS_STATEMENT << "[norm. case for removing all existing floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm. case for removing all existing floors]" << endl;
        }
    }
    // test remove error case 1 (exception)
    {
        if (myTest.test_removeError()) {
            cout << PASS_STATEMENT << "[error case for removing floor out of bounds (exception thrown)!]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for removing floor out of bounds]" << endl;
        }
    }
    // test remove error case 2
    {
        if (myTest.test_removeError2()) {
            cout << PASS_STATEMENT << "[error case for removing non-existent floor]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for removing non-existent floor]" << endl;
        }
    }

    cout << "\n/** TEST INSERT **/" << endl;
    // test insert on norm case
    {
        if (myTest.test_insert()) {
            cout << PASS_STATEMENT << "[norm. case for inserting between floors in elevator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm. case for inserting between floors in elevator]" << endl;
        }
    }
    // test insert on error exception
    {
        if (myTest.test_insertError()) {
            cout << PASS_STATEMENT << "[error case for inserting floors out of bounds]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for inserting floors out of bounds]" << endl;
        }
    }
    // test insert on error 2
    {
        if (myTest.test_insertError2()) {
            cout << PASS_STATEMENT << "[error case for inserting floors that already exist]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for inserting floors that already exist]" << endl;
        }
    }
    cout << "\n/** TEST CHECKFLOOR **/" << endl;
    // test checkFloor on norm case
    {
        if (myTest.test_checkFloor()) {
            cout << PASS_STATEMENT << "[norm. case for checkFloor on all floors in elevator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm. case for checkFloor on all floors in elevator]" << endl;
        }
    }
    // test checkFloor on error case: exception
    {
        if (myTest.test_checkFloorError()) {
            cout << PASS_STATEMENT << "[error cases for checking out of bounds]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error cases for checking out of bounds]" << endl;
        }
    }
    // test checkFloor on error case 2
    {
        if (myTest.test_checkFloorError2()) {
            cout << PASS_STATEMENT << "[error cases for checking non-existent floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error cases for checking non-existent floors]" << endl;
        }
    }

    cout << "\n/** TEST ENTER **/" << endl;
    // test enter on norm case
    {
        if (myTest.test_enter()) {
            cout << PASS_STATEMENT << "[norm case for entering elevator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm case for entering elevator]" << endl;
        }
    }
    // test enter on errors case exception
    {
        if (myTest.test_enterError()) {
            cout << PASS_STATEMENT << "[error cases for entering floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error cases for entering bad floors]" << endl;
        }
    }
    // test enter on errors case
    {
        if (myTest.test_enterError2()) {
            cout << PASS_STATEMENT << "[error cases for entering occupied elevator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error cases for entering occupied elevator]" << endl;
        }
    }

    cout << "\n/** TEST EXIT **/" << endl;
    // test enter on norm case
    {
        if (myTest.test_exit()) {
            cout << PASS_STATEMENT << "[norm case for exiting elevator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm case for exiting elevator]" << endl;
        }
    }
    // test enter on errors case exception
    {
        if (myTest.test_exitError()) {
            cout << PASS_STATEMENT << "[error case for exiting bad floors]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for exiting bad floors]" << endl;
        }
    }
    // test enter on errors case
    {
        if (myTest.test_exitError2()) {
            cout << PASS_STATEMENT << "[error case for exiting valid floor w/ no passenger]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[error case for exiting valid floor w/ no passenger]" << endl;
        }
    }

    cout << "\n/** TEST COPY CONSTRUCTOR **/" << endl;
    // test copy cons
    {
        if (myTest.test_copyConstructor()) {
            cout << PASS_STATEMENT << "[norm case for deep copy with copy constructor]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm case for deep copy with copy constructor]" << endl;
        }
    }
    // test copy cons edge 1 floor
    {
        if (myTest.test_copyConstructorEdge()) {
            cout << PASS_STATEMENT << "[edge case for 1 floor deep copy with copy constructor]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[edge case for 1 floor deep copy with copy constructor]" << endl;
        }
    }
    // test copy cons edge 2 floor
    {
        if (myTest.test_copyConstructorEdge2()) {
            cout << PASS_STATEMENT << "[edge case for 2 floor deep copy with copy constructor]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[edge case for 2 floor deep copy with copy constructor]" << endl;
        }
    }
    // test copy cons empty obj
    {
        if (myTest.test_copyConstructorEmpty()) {
            cout << PASS_STATEMENT << "[edge case for empty deep copy with copy constructor]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[edge case for empty deep copy with copy constructor]" << endl;
        }
    }

    cout << "\n/** TEST ASSIGNMENT OPERATOR **/" << endl;
    // test assign op
    {
        if (myTest.test_assignOp()) {
            cout << PASS_STATEMENT << "[norm case for deep copy with assignment operator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[norm case for deep copy with assignment operator]" << endl;
        }
    }
    // test assign op edge 1 floor
    {
        if (myTest.test_assignOpEdge()) {
            cout << PASS_STATEMENT << "[edge case for 1 floor deep copy with assign operator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[edge case for 1 floor deep copy with assign operator]" << endl;
        }
    }
    // test assign op edge 2 floor
    {
        if (myTest.test_assignOpEdge2()) {
            cout << PASS_STATEMENT << "[edge case for 2 floor deep copy with assign operator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[edge case for 2 floor deep copy with assign operator]" << endl;
        }
    }
    // test assign op empty obj
    {
        if (myTest.test_assignOpEmpty()) {
            cout << PASS_STATEMENT << "[edge case for empty deep copy with assign operator]" << endl;
        } else {
            cout << FAIL_STATEMENT << "[edge case for empty deep copy with assign operator]" << endl;
        }
    }
    return 0;
}
