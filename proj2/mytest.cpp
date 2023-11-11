// UMBC - CMSC 341 - Spring 2022 - Proj2
/** Matthew Makila
 *  Project 2: Fleet of Spaceships
 *  mytest.cpp
 *  3/27/22
 *  This includes the entirety of the testing suite for the project's unit and integration
 *  tests
 */
#include "fleet.h"
#include <random>
#include <cstdlib>
#include <ctime>

const char* FAIL_STATEMENT = "*****TEST FAILED: ";
const char* PASS_STATEMENT = "     TEST PASSED: ";

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{
public:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
    // test insert
    bool test_insertionNorm(); // make perfect tree?
    bool test_insertionEdge(); // inserting when tree empty, inserting sum smaller/bigger than rest
    bool test_insertionError(); // MAKE ANOTHER ONE FOR DUPLICATES
    // test balance
    bool test_balanced();
    bool test_balancedRemove();
    // test remove
    bool test_removeNorm();
    bool test_removeEdge(); // removing last node, removing leaf vs internal node
    bool test_removeError(); // removing something that doesn't exist
    bool test_removeLost();
    // test BST property (left of a node is lesser val, right of a node is greater val)
    bool test_InsertBSTProperty();
    bool test_RemoveBSTProperty();
    bool checkBST(Ship *aShip, int min, int max);
    // test find
    bool test_findShip();
    bool test_findShipError();
    // test log(n) timing
    bool test_insertionTime();
    bool test_removalTime();
};

bool Tester::test_insertionNorm() {
    Random typeGen(0, 4);
    bool testFlag = true;
    Fleet fleet;
    for (int i = 10000; i <= 10300; i++) {
        // check if insert successfully operates
        Ship ship(i,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(ship);
    }
    for (int i = 10000; i <= 10300; i++) { // make sure ship exists in fleet
        if (!fleet.findShip(i)) {
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_insertionEdge() {
    bool testFlag = false;
    Fleet fleet;
    Ship ship(50000,static_cast<SHIPTYPE>(0));
    fleet.insert(ship);
    // our root should be just the ship we entered with no leaves
    if (fleet.m_root->getID() == 50000 && !fleet.m_root->m_left && !fleet.m_root->m_right) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_insertionError() {
    bool testFlag = false;
    Fleet fleet;
    Ship ship(10001,static_cast<SHIPTYPE>(0));
    Ship ship2(10001,static_cast<SHIPTYPE>(0));
    fleet.insert(ship);
    fleet.insert(ship2); // should not work
    // fleet's root should be our first ship and have no children
    if (fleet.m_root->getID() == 10001 && !(fleet.m_root->m_left) && !(fleet.m_root->m_right)) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_removeNorm() {
    bool testFlag = true;
    Fleet fleet;
    for (int i = 10000; i <= 10300; i++) {
        // check if insert successfully operates
        Ship ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship);
    }
    for (int i = 10000; i <= 10300; i++) {
        // check if insert successfully operates
        fleet.remove(i);
        if (fleet.findShip(i)) { // we just removed it, should NOT exist
            testFlag = false;
        }
    }
    if (fleet.m_root) {
        testFlag = false;
    }
    return testFlag;
}

bool Tester::test_removeEdge() {
    // remove only existing ship
    bool testFlag = false;
    Fleet fleet;
    Ship ship(50000,static_cast<SHIPTYPE>(0));
    fleet.insert(ship);
    fleet.remove(50000);
    if (fleet.m_root == nullptr) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_removeError() {
    // remove ship that isn't there
    bool testFlag = false;
    Fleet fleet;
    Ship ship(50000,static_cast<SHIPTYPE>(0));
    fleet.insert(ship);
    fleet.remove(10500);
    if (fleet.m_root->getID() == 50000 && !(fleet.m_root->m_left) && !(fleet.m_root->m_right)) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_removeLost() {
    bool testFlag = true;
    Fleet fleet;
    for (int i = 10000; i <= 10100; i++) {
        Ship ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship);
        if (i % 2 == 0) { // make all odd lost
            fleet.setState(i, LOST);
        }
    }
    fleet.removeLost();
    cout << endl;
    for (int i = 10000; i <= 10100; i++) {
        // all even ships SHOULD be gone, check for sure ...
        if (i % 2 == 0) {
            if (fleet.findShip(i)) {
                testFlag = false;
            }
        }
    }
    return testFlag;
}

bool Tester::test_balanced() {
    bool testFlag = true;
    Fleet fleet;
    for (int i = 10000; i <= 10300; i++) {
        // check if insert successfully operates
        Ship ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship); // should balance here
    }
    if (abs(fleet.checkImbalance(fleet.m_root)) > 1) { // check if any given node is not balanced after insertion
        testFlag = false;
    }
    return testFlag;
}

bool Tester::test_balancedRemove() {
    bool testFlag = true;
    Fleet fleet;
    for (int i = 10000; i <= 10300; i++) {
        // check if insert successfully operates
        Ship ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship); // should balance here
    }

    for (int i = 10000; i <= 10300; i++) {
        if (i % 2 == 0) { // remove even ID ships
            fleet.remove(i); // should balance
        }
    }
    if (abs(fleet.checkImbalance(fleet.m_root)) > 1) {
        testFlag = false;
    }
    return testFlag;
}

bool Tester::test_InsertBSTProperty() {
    // check if BST is in order
    bool testFlag = false;
    Fleet fleet;
    for (int i = 10000; i <= 10100; i++) {
        // check if insert successfully operates
        Ship ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship); // should balance here
    }
    if (checkBST(fleet.m_root, MINID, MAXID)) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_RemoveBSTProperty() {
    // check if BST is in order
    bool testFlag = false;
    Fleet fleet;
    for (int i = 10000; i <= 10100; i++) {
        // check if insert successfully operates
        Ship ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship); // should balance here
    }
    for (int i = 10000; i <= 10100; i++) {
        if (i % 2 == 0) { // remove even ID ships
            fleet.remove(i); // should balance
        }
    }
    if (checkBST(fleet.m_root, MINID, MAXID)) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::checkBST(Ship *aShip, int minVal, int maxVal) {
    if (aShip == nullptr){
        return true;
    }
    if (aShip->m_id < minVal) { // make sure val < than the pass in key for left of nodes & in range
        return false;
    }
    if (aShip->m_id > maxVal) { // make sure val > than passed in key for right of nodes & in range
        return false;
    }
    return checkBST(aShip->m_left, minVal, aShip->m_id) && checkBST(aShip->m_right, aShip->m_id, maxVal);
}

bool Tester::test_findShip() {
    // check if a valid inserted ships exist
    bool testFlag = true;
    Fleet fleet;
    for (int i = 10000; i <= 10005; i++) {
        Ship ship = Ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship);
        if (!fleet.findShip(i)) {
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_findShipError() {
    // check if a valid inserted ships exist
    bool testFlag = false;
    Fleet fleet;
    for (int i = 10000; i <= 10005; i++) {
        Ship ship = Ship(i,static_cast<SHIPTYPE>(0));
        fleet.insert(ship);
    }
    if (!fleet.findShip(10006)) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_insertionTime() {
    // set up test to prove O log(n) timing for insertion with clock
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    int ID = 0;
    bool testFlag = false;
    double T = 0;
    double T2 = 0;
    double timeRatio = 0;
    int fleetSize1 = 10000;
    int fleetSize2 = 20000;
    Fleet fleet;
    // const int a = 2;
    clock_t start, stop;
    start = clock();
    for(int i = 0; i < fleetSize1; i++){
        ID = idGen.getRandNum();
        Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(ship);
    }
    stop = clock();
    T = stop - start;
    cout << "     Inserting 1000 ships took " << T << " clock ticks or (" << T / CLOCKS_PER_SEC << ") sec." << endl;

    Fleet fleet2;
    // clock_t start, stop;
    start = clock();
    for(int i = 0; i < fleetSize2; i++){
        ID = idGen.getRandNum();
        Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet2.insert(ship);
    }
    stop = clock();
    T2 = stop - start;
    cout << "     Inserting 2000 ships took " << T2 << " clock ticks or (" << T2 / CLOCKS_PER_SEC << ") sec." << endl;
    timeRatio = T2 / (2 * T);
    cout << "     Time ratio: " << timeRatio << endl;
    if (timeRatio > 0.7 && timeRatio < 1.5) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_removalTime() {
    // set up test to prove O log(n) timing for removal with clock
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    bool testFlag = false;
    double T = 0;
    double T2 = 0;
    int fleetStart = 10000;
    int fleet1End = 20000;
    int fleet2End = 30000;
    double timeRatio = 0;
    Fleet fleet;
    for(int i = fleetStart; i < fleet1End; i++){
        Ship ship(i,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(ship);
    }
    Fleet fleet2;
    for(int i = fleetStart; i < fleet2End; i++){
        Ship ship(i,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet2.insert(ship);
    }
    clock_t start, stop;
    start = clock();
    for(int i = fleetStart; i < fleet1End; i++){
        fleet.remove(i);
    }
    stop = clock();
    T = stop - start;
    cout << "     Removing 10000 ships took " << T << " clock ticks or (" << T / CLOCKS_PER_SEC << ") sec." << endl;
    start = clock();
    for(int i = fleetStart; i < fleet2End; i++){
        fleet2.remove(i);
    }
    stop = clock();
    T2 = stop - start;
    cout << "     Removing 20000 ships took " << T2 << " clock ticks or (" << T2 / CLOCKS_PER_SEC << ") sec." << endl;
    timeRatio = T2 / (2 * T);
    cout << "     Time ratio: " << timeRatio << endl;
    if (timeRatio > 0.7 && timeRatio < 1.5) {
        testFlag = true;
    }
    return testFlag;
}

int main() {
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    Tester myTest;
    cout << "\nTesting find methods\n" << endl;
    // test find normal case
    {
        if (myTest.test_findShip()) {
            cout << PASS_STATEMENT << "norm. case with find" << endl;
        } else {
            cout << FAIL_STATEMENT << "norm. case with find" << endl;
        }
    }
    // test find error case
    {
        if (myTest.test_findShipError()) {
            cout << PASS_STATEMENT << "error case with find" << endl;
        } else {
            cout << FAIL_STATEMENT << "error case with find" << endl;
        }
    }

    cout << "\nTesting insertion methods\n" << endl;
    // test insert normal case
    {

        if (myTest.test_insertionNorm()) {
            cout << PASS_STATEMENT << "norm. case with insertion of 300 elements" << endl;
        } else {
            cout << FAIL_STATEMENT << "norm. case with insertion of 300 elements" << endl;
        }
    }
    // test insert edge case
    {
        if (myTest.test_insertionEdge()) {
            cout << PASS_STATEMENT << "edge case with insertion of just one ship" << endl;
        } else {
            cout << FAIL_STATEMENT << "edge case with insertion of just one ship" << endl;
        }
    }
    // test insert error case
    {
        if (myTest.test_insertionError()) {
            cout << PASS_STATEMENT << "error case with insertion of invalid IDs" << endl;
        } else {
            cout << FAIL_STATEMENT << "error case with insertion of invalid IDs" << endl;
        }
    }

    cout << "\nTesting removal methods\n" << endl;
    // test remove normal case
    {
        if (myTest.test_removeNorm()) {
            cout << PASS_STATEMENT << "norm. case with removal of 300 elements" << endl;
        } else {
            cout << FAIL_STATEMENT << "norm. case with removal of 300 elements" << endl;
        }
    }
    // test remove edge case
    {
        if (myTest.test_removeEdge()) {
            cout << PASS_STATEMENT << "edge case with removal of only ship" << endl;
        } else {
            cout << FAIL_STATEMENT << "edge case with removal of only ship" << endl;
        }
    }
    // test remove error case
    {
        if (myTest.test_removeError()) {
            cout << PASS_STATEMENT << "error case with removal of invalid IDs" << endl;
        } else {
            cout << FAIL_STATEMENT << "error case with removal of invalid IDs" << endl;
        }
    }
    // test remove lost case
    {
        if (myTest.test_removeLost()) {
            cout << PASS_STATEMENT << "removing BST with 'lost' nodes" << endl;
        } else {
            cout << FAIL_STATEMENT << "removing BST with 'lost' nodes" << endl;
        }
    }

    cout << "\nTesting insertion/removal times\n" << endl;
    // test insertion time
    {
        if (myTest.test_insertionTime()) {
            cout << PASS_STATEMENT << "Getting log(n) time ratio with 1000 and 2000 nodes" << endl;
        } else {
            cout << FAIL_STATEMENT << "Getting log(n) time ratio with 1000 and 2000 nodes" << endl;
        }
    }
    // test removal time
    {
        if (myTest.test_removalTime()) {
            cout << PASS_STATEMENT << "Getting log(n) time ratio with 1000 and 2000 nodes" << endl;
        } else {
            cout << FAIL_STATEMENT << "Getting log(n) time ratio with 1000 and 2000 nodes" << endl;
        }
    }

    cout << "\nTesting balance methods\n" << endl;
    // test remove error case
    {
        if (myTest.test_balanced()) {
            cout << PASS_STATEMENT << "norm case with balance test after insertion of 300 elements" << endl;
        } else {
            cout << FAIL_STATEMENT << "norm case with balance test after insertion of 300 elements" << endl;
        }
    }
    // test remove lost case
    {
        if (myTest.test_balancedRemove()) {
            cout << PASS_STATEMENT << "Checking balance after insertion of 300 elements and removal of half" << endl;
        } else {
            cout << FAIL_STATEMENT << "Checking balance after insertion of 300 elements and removal of half" << endl;
        }
    }

    cout << "\nTesting BST property\n" << endl;
    // test remove error case
    {
        if (myTest.test_InsertBSTProperty()) {
            cout << PASS_STATEMENT << "Checking BST property test after insertion of 300 elements" << endl;
        } else {
            cout << FAIL_STATEMENT << "Checking BST property test after insertion of 300 elements" << endl;
        }
    }
    // test remove lost case
    {
        if (myTest.test_RemoveBSTProperty()) {
            cout << PASS_STATEMENT << "Checking BST property test after insertion of 300 elements & removals" << endl;
        } else {
            cout << FAIL_STATEMENT << "Checking BST property test after insertion of 300 elements & removals" << endl;
        }
    }
    return 0;
}