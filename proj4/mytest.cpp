/** Matthew Makila
 *  Project 4: DNA Database
 *  mytest.cpp
 *  4/25/22
 *  This includes the entirety of the testing suite for the project's unit and integration
 *  tests
 */

// CMSC 341 - Spring 2022 - Project 4
#include "dnadb.h"
#include <random>
#include <vector>

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
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
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

unsigned int hashCode(const string str);
string sequencer(int size, int seedNum);

class Tester{
public:

    void myAssert(bool result, const string& message);
    // constructor tests
    /**Do i need these?**/
    bool test_constructorEdge(); // empty obj
    bool test_constructorEdge2(); // create obj w/ bad info
    bool test_constructorNorm(); // make reg dnadb
    // find tests
    bool test_findError(); // key doesn't exist
    bool test_findNorm(); // w/ non-colliding keys
    bool test_findNorm1(); // w/ colliding keys
    // insert tests
    bool test_insertError(); // duplicate objects
    bool test_insertNorm(); // collisions
    bool test_insertNorm2(); // no collisions
    // remove tests
    bool test_removeNorm(); // w/ collisions
    bool test_removeError(); // key not there
    bool test_removeNorm2(); // w/o collisions
    // rehash tests ...
    bool test_rehashInsert();
    bool test_rehashInsertComplete();
    bool test_rehashDelete();
    bool test_rehashDeleteComplete();
};

void Tester::myAssert(bool result, const string& message) {
    if (result) {
        cout << PASS_STATEMENT << message << endl;
    } else {
        cout << FAIL_STATEMENT << message << endl;
    }
}

bool Tester::test_constructorEdge() {
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    if (dnadb.m_currNumDeleted == 0 && dnadb.m_currentSize == 0 && dnadb.m_currentCap == 101 &&
    dnadb.m_oldNumDeleted == 0 && dnadb.m_oldTable == nullptr && dnadb.m_oldCap == 0 && dnadb.m_oldSize == 0) {
        return true;
    }
    return false;
}

bool Tester::test_findError() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i=0;i<10;i++){
        // generating random data
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        if (i != 5) { // exclude 5th DNA to check
            dnadb.insert(dataObj);
        }
    }
    if (dataList[5] == dnadb.getDNA(dataList[5].getSequence(), dataList[5].getLocId())) {
        return false;
    }
    return result;
}

bool Tester::test_findNorm() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    // a few non-colliding keys
    DNA data1 = DNA("CAATG", 1500);
    DNA data2 = DNA("GAGTC", 1501);
    DNA data3 = DNA("CTGTA", 1502);
    DNA data4 = DNA("ACGCC", 1503);
    DNA data5 = DNA("GGGTG", 1504);

    dataList.push_back(data1);
    dataList.push_back(data2);
    dataList.push_back(data3);
    dataList.push_back(data4);
    dataList.push_back(data5);

    for (int i = 0; i < 5; i++) {
        dnadb.insert(dataList[i]);
    }
    for (int i = 0; i < 5; i++) {
        if (!(dataList[i] == dnadb.getDNA(dataList[i].getSequence(), dataList[i].getLocId()))) {
            return false;
        }
    }
    return result;
}

bool Tester::test_findNorm1() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    // a few colliding keys
    DNA data1 = DNA("CAATG", 1500);
    DNA data2 = DNA("CAATG", 1501);
    DNA data3 = DNA("CAATG", 1502);
    DNA data4 = DNA("CAATG", 1503);
    DNA data5 = DNA("CAATG", 1504);
    dataList.push_back(data1);
    dataList.push_back(data2);
    dataList.push_back(data3);
    dataList.push_back(data4);
    dataList.push_back(data5);

    for (int i = 0; i < 5; i++) {
        dnadb.insert(dataList[i]);
    }

    if (dnadb.m_currentSize != 5) {
        return false;
    }
    // checking whether all data are inserted
    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    return result;
}

bool Tester::test_insertNorm() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i=0;i<50;i++){
        // generating random data
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the DnaDb object
        if (!dnadb.insert(dataObj)) { // check if insert is good
            return false;
        }
    }
    if (dnadb.m_currentSize != 50) {
        return false;
    }
    // checking whether all data are inserted
    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    return result;
}

bool Tester::test_insertNorm2() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    // non-colliding keys
    DNA data1 = DNA("CAATG", 1500);
    DNA data2 = DNA("GAGTC", 1501);
    DNA data3 = DNA("CTGTA", 1502);
    DNA data4 = DNA("ACGCC", 1503);
    DNA data5 = DNA("GGGTG", 1504);

    dataList.push_back(data1);
    dataList.push_back(data2);
    dataList.push_back(data3);
    dataList.push_back(data4);
    dataList.push_back(data5);

    for (int i = 0; i < 5; i++) {
        dnadb.insert(dataList[i]);
    }

    if (dnadb.m_currentSize != 5) {
        return false;
    }
    if (!(dnadb.m_currentTable[84] == data1 && dnadb.m_currentTable[66] == data2 && dnadb.m_currentTable[90] == data3
    && dnadb.m_currentTable[98] == data4 && dnadb.m_currentTable[57] == data5)) {
        return false;
    }
    // checking whether all data are inserted
    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    return result;
}

bool Tester::test_insertError() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    DNA data1 = DNA("CAATG", 1500); // create key to use as duplicate
    dnadb.insert(data1);

    if (dnadb.insert(data1)) { // no duplicate data!
        return false;
    }
    return result;
}

bool Tester::test_removeNorm() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    // non-colliding keys
    DNA data1 = DNA("CAATG", 1500);
    DNA data2 = DNA("GAGTC", 1501);
    DNA data3 = DNA("CTGTA", 1502);
    DNA data4 = DNA("ACGCC", 1503);
    DNA data5 = DNA("GGGTG", 1504);
    dataList.push_back(data1);
    dataList.push_back(data2);
    dataList.push_back(data3);
    dataList.push_back(data4);
    dataList.push_back(data5);

    for (int i = 0; i < 5; i++) {
        dnadb.insert(dataList[i]);
    }
    for (int i = 0; i < 2; i++) {
        if (!(dnadb.remove(dataList[i]))) {
            return false; // false if bad removal
        }
    }
    if (dnadb.m_currentSize != 3) {
        return false;
    }
    return result;
}

bool Tester::test_removeNorm2() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    // a few colliding keys
    DNA data1 = DNA("CAATG", 1500);
    DNA data2 = DNA("CAATG", 1501);
    DNA data3 = DNA("CAATG", 1502);
    DNA data4 = DNA("CAATG", 1503);
    DNA data5 = DNA("CAATG", 1504);
    dataList.push_back(data1);
    dataList.push_back(data2);
    dataList.push_back(data3);
    dataList.push_back(data4);
    dataList.push_back(data5);

    for (int i = 0; i < 5; i++) {
        dnadb.insert(dataList[i]);
    }
    for (int i = 0; i < 2; i++) {
        if (!(dnadb.remove(dataList[i]))) {
            return false; // false if bad removal
        }
    }
    if (dnadb.m_currentSize != 3) {
        return false;
    }
    return result;
}

bool Tester::test_removeError() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    DNA data1 = DNA("CAATG", 1500);
    if (dnadb.remove(data1)) {
        return false;
    }
    return result;
}

bool Tester::test_rehashInsert() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i=0;i<51;i++){ // should be in middle of rehash
        // generating random data
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the DnaDb object
        dnadb.insert(dataObj);
    }
    // checking whether all data found
    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    if (!(dnadb.m_oldTable && dnadb.m_currentTable)) {
        return false; // make sure both tables exist since rehash triggered
    }
    return result;
}

bool Tester::test_rehashInsertComplete() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i=0;i<54;i++){ // should be finished rehash
        // generating random data
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the DnaDb object
        dnadb.insert(dataObj);
    }
    // checking whether all data found
    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    if (dnadb.m_oldTable || dnadb.m_currentSize != 54 || dnadb.m_currentCap != 211) {
        return false; // old table shouldn't exist since rehash complete, should be 54 in currTable of cap 211
    }
    return result;
}

bool Tester::test_rehashDelete() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i=0;i<50;i++){
        // generating random data
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the DnaDb object
        dnadb.insert(dataObj);
    }
    // dumping the data points to the standard output
    for (int i=0;i<41;i++){
        if (!(dnadb.remove(dataList[i]))) {
            return false;
        }
    }
    // make sure both tables exist since rehash triggered
    if (!(dnadb.m_oldTable && dnadb.m_currentTable)) {
        return false;
    }
    return result;
}

bool Tester::test_rehashDeleteComplete() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i=0;i<50;i++){
        // generating random data
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the DnaDb object
        dnadb.insert(dataObj);
    }
    // dumping the data points to the standard output
    for (int i=0;i<43;i++){
        if (!(dnadb.remove(dataList[i]))) {
            return false;
        }
    }
    // make sure both tables exist since rehash triggered
    if (dnadb.m_oldTable || dnadb.m_currentSize != 7) {
        return false; // old table shouldn't exist since rehash complete, should be 7 in currTable of cap 101
    }
    return result;
}

int main(){
    Tester myTest;
    vector<DNA> dataList;
    Random RndLocation(MINLOCID,MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);

    cout << "\nConstructor Tests\n" << endl;
    myTest.myAssert(myTest.test_constructorEdge(), "constructing default object");

    cout << "\nFind Tests\n" << endl;
    myTest.myAssert(myTest.test_findError(), "Finding invalid missing key");
    myTest.myAssert(myTest.test_findNorm(), "Finding valid non-colliding keys");
    myTest.myAssert(myTest.test_findNorm1(), "Finding valid colliding keys");


    cout << "\nInsertion Tests\n" << endl;
    myTest.myAssert(myTest.test_insertNorm(), "inserting 50 elements with collisions");
    myTest.myAssert(myTest.test_insertNorm2(), "inserting elements with no collisions");
    myTest.myAssert(myTest.test_insertError(), "inserting duplicate DNA");


    cout << "\nRemoval Tests\n" << endl;
    myTest.myAssert(myTest.test_removeNorm(), "removing non-colliding elements");
    myTest.myAssert(myTest.test_removeNorm2(), "removing colliding elements");
    myTest.myAssert(myTest.test_removeError(), "removing non-existent element");


    cout << "\nRehash Tests\n" << endl;
    myTest.myAssert(myTest.test_rehashInsert(), "trigger rehash inserting elements");
    myTest.myAssert(myTest.test_rehashInsertComplete(), "complete rehash inserting elements");
    myTest.myAssert(myTest.test_rehashDelete(), "trigger rehash deleting elements");
    myTest.myAssert(myTest.test_rehashDeleteComplete(), "complete rehash inserting elements");
    return 0;
}

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}
string sequencer(int size, int seedNum){
    //this function returns a random DNA sequence
    string sequence = "";
    Random rndObject(0,3);
    rndObject.setSeed(seedNum);
    for (int i=0;i<size;i++){
        sequence = sequence + ALPHA[rndObject.getRandNum()];
    }
    return sequence;
}
