/** Matthew Makila
 *  Project 3: Social Media Posts
 *  mytest.cpp
 *  4/5/22
 *  This includes the entirety of the testing suite for the project's unit and integration
 *  tests
 */
// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"
#include <random>
#include <ctime>

// the followings are sample priority functions to be used by SQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a post.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Post &post);// works with a MAXHEAP
int priorityFn2(const Post &post);// works with a MINHEAP

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
    // constructor
    bool test_emptyConstructor();
    bool test_copyCons();
    bool test_copyConsEdge(); // empty copy
    bool constructorOrderCheck(Node* p1, Node* p2);
    // assignment op
    bool test_assignOpNorm();
    bool test_assignOpEdge(); // empty obj
    // merge
    bool test_errorMerge();
    bool test_edgeMerge();
    bool test_edgeMerge2(); // merge populated into empty
    bool test_mergeNorm(); // two populated queues
    // insert
    bool test_insertNorm();
    bool checkHeapPropMin(Node* p1, int val);
    bool checkHeapPropMax(Node* p1, int val);
    // remove
    bool test_removeNorm(); // remove 300 elements in RIGHT order
    bool test_removeError(); // removing from empty queue should throw out of range
    // switch priorities
    bool test_prioritySwitchToMax();
    bool test_prioritySwitchtoMin();
    // amortized timing log n
    bool test_insertTime();
    bool test_removeTime();

};

bool Tester::test_emptyConstructor() {
    bool testFlag = false;
    SQueue myQ(priorityFn2, MINHEAP);
    // make sure initialized vals & funcs right
    if (myQ.m_size == 0 && myQ.m_heapType == MINHEAP && !myQ.m_heap && myQ.m_priorFunc == priorityFn2) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_copyCons() {
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    SQueue myQ(priorityFn2, MINHEAP);

    for (int i = 0; i < 300; i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        myQ.insertPost(aPost);
    }
    SQueue myQ1(myQ);
    if (constructorOrderCheck(myQ.m_heap, myQ1.m_heap) && (myQ.m_size == myQ1.m_size)) {
        testFlag = true;
    }

    return testFlag;
}

bool Tester::test_copyConsEdge() {
    // copy an empty object
    bool testFlag = false;
    SQueue myQ(priorityFn2, MINHEAP);
    SQueue myQ1(myQ);
    if ((!myQ.m_heap && !myQ1.m_heap) && (myQ.m_size == 0 && myQ1.m_size == 0)) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_assignOpNorm() {
    // assign norm objects
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    SQueue myQ(priorityFn2, MINHEAP);
    SQueue myQ1(priorityFn2, MINHEAP);

    for (int i = 0; i < 300; i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        myQ.insertPost(aPost);
    }
    myQ1 = myQ;
    if (constructorOrderCheck(myQ.m_heap, myQ1.m_heap) && (myQ.m_size == myQ1.m_size)) {
        testFlag = true;
    }

    return testFlag;
}

bool Tester::test_assignOpEdge() {
    // assign an empty object
    bool testFlag = false;
    SQueue myQ(priorityFn2, MINHEAP);
    SQueue myQ1(priorityFn2, MINHEAP);

    myQ1 = myQ;
    if ((!myQ.m_heap && !myQ1.m_heap) && (myQ.m_size == 0 && myQ1.m_size == 0)) {
        testFlag = true;
    }
    return testFlag;}

bool Tester::constructorOrderCheck(Node* p1, Node* p2) {
    // check copied objects have correct data and different mem addresses
    if ((p1 && p2) && (p1 != p2) && (priorityFn2(p1->getPost()) == priorityFn2(p2->getPost()))) {
        constructorOrderCheck(p1->m_left, p2->m_left);
        constructorOrderCheck(p1->m_right, p2->m_right);
        return true;
    }
    return false;
}

bool Tester::test_errorMerge() {
    bool testFlag = false;
    SQueue myQ(priorityFn1, MAXHEAP);
    SQueue myQ2(priorityFn2, MINHEAP);
    try {
        myQ.mergeWithQueue(myQ2);
    } catch (...) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_edgeMerge() {
    // test two empty queues return empty queue
    bool testFlag = false;
    SQueue myQ(priorityFn2, MINHEAP);
    SQueue myQ2(priorityFn2, MINHEAP);
    myQ.mergeWithQueue(myQ2);
    if (myQ.m_heap == nullptr) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_edgeMerge2() {
    // test if empty and normal queue merge right
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    SQueue myQ(priorityFn2, MINHEAP);

    for (int i = 0; i < 10; i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        myQ.insertPost(aPost);
    }
    SQueue myQ2(priorityFn2, MINHEAP); // create empty queue
    myQ2.mergeWithQueue(myQ); // should give myQ2 myQ's contents and empty myQ
    if (myQ2.m_heap && (myQ2.m_size == 10 && myQ.m_size == 0)) { // their sizes should have switched
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_mergeNorm() {
    // test if two populated queues merge right
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    SQueue myQ(priorityFn2, MINHEAP);
    SQueue myQ2(priorityFn2, MINHEAP);

    for (int i = 0; i < 10; i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        myQ.insertPost(aPost);
    }
    for (int i = 0; i < 15; i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        myQ2.insertPost(aPost);
    }
    // perform merge into myQ
    myQ.mergeWithQueue(myQ2);
    if (myQ.m_size == 25 && myQ2.m_size == 0) { // ensure the queues have correct sizes
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_insertNorm() {
    // insert 300 things and make sure heap prop is preserved (each node less than its parent for MINHEAP)
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    SQueue myQ(priorityFn2, MINHEAP);
    SQueue myQ2(priorityFn1, MAXHEAP);


    for (int i = 0; i < 300; i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        myQ.insertPost(aPost);
        myQ2.insertPost(aPost);
    }
    // test heap properties!!!
    if (checkHeapPropMin(myQ.m_heap, priorityFn2(myQ.m_heap->getPost())) &&
        checkHeapPropMax(myQ2.m_heap, priorityFn1(myQ2.m_heap->getPost()))) {
        testFlag = true;
    }

    return testFlag;
}

bool Tester::checkHeapPropMin(Node *p1, int val) {
    // FOR A MINHEAP!
    if (p1 == nullptr){
        return true;
    }
    if (priorityFn2(p1->getPost()) < val) { // make sure priority val < than the previous node's priority
        return false;
    }
    return checkHeapPropMin(p1->m_left, priorityFn2(p1->getPost())) && checkHeapPropMin(p1->m_right, priorityFn2(p1->getPost()));
}

bool Tester::checkHeapPropMax(Node *p1, int val) {
    // FOR A MINHEAP!
    if (p1 == nullptr){
        return true;
    }
    if (priorityFn1(p1->getPost()) > val) { // make sure priority val > than the previous node's priority
        return false;
    }
    return checkHeapPropMax(p1->m_left, priorityFn1(p1->getPost())) && checkHeapPropMax(p1->m_right, priorityFn1(p1->getPost()));
}

bool Tester::test_removeNorm() {
    // insert 300 things and make sure heap prop is preserved (each node less than its parent for MINHEAP)
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    SQueue myQ(priorityFn2, MINHEAP);
    SQueue myQ2(priorityFn1, MAXHEAP);

    for (int i = 0; i < 300; i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        myQ.insertPost(aPost);
        myQ2.insertPost(aPost);
    }
    // create arrays to store posts we pop to check order later
    int size1 = myQ.m_size;
    int size2 = myQ2.m_size;
    Post postArrayQ[size1];
    Post postArrayQ2[size2];

    for (int i = 0; i < 300; i++) { // do the removals and store into checker arrs
        postArrayQ[i] = myQ.getNextPost();
        postArrayQ2[i] = myQ2.getNextPost();
    }
    // check that posts were removed in order of priority
    if (myQ.m_size == 0 && myQ2.m_size == 0) {
        testFlag = true;
        for (int i = 0; i < 299; i++) {
            if (priorityFn2(postArrayQ[i]) > priorityFn2(postArrayQ[i + 1]) ||
                priorityFn1(postArrayQ2[i]) < priorityFn1(postArrayQ2[i + 1])) {
                testFlag = false;
            }
        }
    }

    return testFlag;
}

bool Tester::test_removeError() {
    // remove from empty test
    bool testFlag = false;
    SQueue myQ(priorityFn2, MINHEAP);
    try {
        myQ.getNextPost();
    } catch (...) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_prioritySwitchToMax() {
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);

    for (int i=0;i<10;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    // switch queue to MAXHEAP
    queue1.setPriorityFn(priorityFn1, MAXHEAP);
    if (checkHeapPropMax(queue1.m_heap, priorityFn1(queue1.m_heap->getPost()))) {
        testFlag = true;
        queue1.setPriorityFn(priorityFn2, MINHEAP); // switch heap back and make sure data works
        if (!checkHeapPropMin(queue1.m_heap, priorityFn2(queue1.m_heap->getPost()))) {
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_prioritySwitchtoMin() {
    bool testFlag = false;
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);

    for (int i=0;i<10;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    // switch queue to MINHEAP
    queue1.setPriorityFn(priorityFn2, MINHEAP);
    if (checkHeapPropMin(queue1.m_heap, priorityFn2(queue1.m_heap->getPost()))) {
        testFlag = true;
        queue1.setPriorityFn(priorityFn1, MAXHEAP); // switch heap back and make sure data works
        if (!checkHeapPropMax(queue1.m_heap, priorityFn1(queue1.m_heap->getPost()))) {
            testFlag = false;
        }
    }
    return testFlag;
}

bool Tester::test_insertTime() {
    // set up test to prove O log(n) timing for insertion with clock
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    bool testFlag = false;

    SQueue queue1(priorityFn1, MAXHEAP);

    double T = 0;
    double T2 = 0;
    double timeRatio = 0;
    clock_t start, stop;
    start = clock();
    for (int i=0;i<10000;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    stop = clock();
    T = stop - start;
    cout << "     Inserting 1000 Posts took " << T << " clock ticks or (" << T / CLOCKS_PER_SEC << ") sec." << endl;

    // clock_t start, stop;
    SQueue queue2(priorityFn1, MAXHEAP);
    start = clock();
    for (int i=0;i<20000;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue2.insertPost(aPost);
    }
    stop = clock();
    T2 = stop - start;
    cout << "     Inserting 2000 Posts took " << T2 << " clock ticks or (" << T2 / CLOCKS_PER_SEC << ") sec." << endl;
    timeRatio = T2 / (2 * T);
    cout << "     Time ratio: " << timeRatio << endl;
    if (timeRatio > 0.7 && timeRatio < 1.5) {
        testFlag = true;
    }
    return testFlag;
}

bool Tester::test_removeTime() {
    // set up test to prove O log(n) timing for insertion with clock
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    bool testFlag = false;

    SQueue queue1(priorityFn1, MAXHEAP);
    SQueue queue2(priorityFn1, MAXHEAP);
    for (int i=0;i<10000;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    for (int i=0;i<20000;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue2.insertPost(aPost);
    }

    double T = 0;
    double T2 = 0;
    double timeRatio = 0;
    clock_t start, stop;
    start = clock();
    for (int i=0;i<10000;i++){
        queue1.getNextPost();
    }
    stop = clock();
    T = stop - start;
    cout << "     Removing 1000 Posts took " << T << " clock ticks or (" << T / CLOCKS_PER_SEC << ") sec." << endl;

    // clock_t start, stop;
    start = clock();
    for (int i=0;i<20000;i++){
        queue2.getNextPost();
    }
    stop = clock();
    T2 = stop - start;
    cout << "     Removing 2000 Posts took " << T2 << " clock ticks or (" << T2 / CLOCKS_PER_SEC << ") sec." << endl;
    timeRatio = T2 / (2 * T);
    cout << "     Time ratio: " << timeRatio << endl;
    if (timeRatio > 0.7 && timeRatio < 1.5) {
        testFlag = true;
    }
    return testFlag;
}

int main() {
    cout << "\nConstructor Tests\n" << endl;
    Tester myTester;
    // test empty constructor
    {
        if (myTester.test_emptyConstructor()) {
            cout << PASS_STATEMENT << "empty constructor" << endl;
        } else {
            cout << FAIL_STATEMENT << "empty constructor" << endl;
        }
    }
    // test copy constructor norm
    {
        if (myTester.test_copyCons()) {
            cout << PASS_STATEMENT << "copy constructor 300 elements" << endl;
        } else {
            cout << FAIL_STATEMENT << "copy constructor 300 elements" << endl;
        }
    }
    // test copy constructor edge
    {
        if (myTester.test_copyConsEdge()) {
            cout << PASS_STATEMENT << "copy constructor empty object" << endl;
        } else {
            cout << FAIL_STATEMENT << "copy constructor empty object" << endl;
        }
    }
    cout << "\nAssignment Operator Tests\n" << endl;
    // test empty constructor
    {
        if (myTester.test_assignOpNorm()) {
            cout << PASS_STATEMENT << "assignment of 300-element queue to another" << endl;
        } else {
            cout << FAIL_STATEMENT << "assignment of 300-element queue to another" << endl;
        }
    }
    // test copy constructor norm
    {
        if (myTester.test_assignOpEdge()) {
            cout << PASS_STATEMENT << "assignment of empty object" << endl;
        } else {
            cout << FAIL_STATEMENT << "assignment of empty object" << endl;
        }
    }
    cout << "\nMerge Tests\n" << endl;
    // test merge of 2 populated queues
    {
        if (myTester.test_mergeNorm()) {
            cout << PASS_STATEMENT << "Merging of 2 populated queues" << endl;
        } else {
            cout << FAIL_STATEMENT << "Merging of 2 populated queues" << endl;
        }
    }
    // test error merge of diff prior funcs
    {
        if (myTester.test_errorMerge()) {
            cout << PASS_STATEMENT << "Error merging different priority functions" << endl;
        } else {
            cout << FAIL_STATEMENT << "Error merging different priority functions" << endl;
        }
    }
    // test edge merge of 2 empty queues
    {
        if (myTester.test_edgeMerge()) {
            cout << PASS_STATEMENT << "Merging 2 empty queues" << endl;
        } else {
            cout << FAIL_STATEMENT << "Merging 2 empty queues" << endl;
        }
    }
    // test edge merge of empty and 1 normal queue
    {
        if (myTester.test_edgeMerge2()) {
            cout << PASS_STATEMENT << "Merging of empty and 1 normal queue" << endl;
        } else {
            cout << FAIL_STATEMENT << "Merging of empty and 1 normal queue" << endl;
        }
    }
    cout << "\nInsertion Tests\n" << endl;
    // test insertion norm
    {
        if (myTester.test_insertNorm()) {
            cout << PASS_STATEMENT << "inserting 300 elements (min/max queue)" << endl;
        } else {
            cout << FAIL_STATEMENT << "inserting 300 elements (min/max queue)" << endl;
        }
    }
    cout << "\nRemoval Tests\n" << endl;
    // test removal norm
    {
        if (myTester.test_removeNorm()) {
            cout << PASS_STATEMENT << "removing 300 elements (min/max queue)" << endl;
        } else {
            cout << FAIL_STATEMENT << "removing 300 elements (min/max queue)" << endl;
        }
    }
    // test removal edge
    {
        if (myTester.test_removeError()) {
            cout << PASS_STATEMENT << "removing from empty queue" << endl;
        } else {
            cout << FAIL_STATEMENT << "removing from empty queue" << endl;
        }
    }
    cout << "\nSet Priority & Queue Rebuild Tests\n" << endl;
    // test set priority (min to max)
    {
        if (myTester.test_prioritySwitchToMax()) {
            cout << PASS_STATEMENT << "rebuilding queue (min -> max queue)" << endl;
        } else {
            cout << FAIL_STATEMENT << "rebuilding queue (min -> max queue)" << endl;
        }
    }
    // test set priority (max to min)
    {
        if (myTester.test_prioritySwitchtoMin()) {
            cout << PASS_STATEMENT << "rebuilding queue (max -> min queue)" << endl;
        } else {
            cout << FAIL_STATEMENT << "rebuilding queue (max -> min queue)" << endl;
        }
    }
    cout << "\nTiming Tests\n" << endl;
    // test insert time ratio
    {
        if (myTester.test_insertTime()) {
            cout << PASS_STATEMENT << "Inserting in amortized log(n) time" << endl;
        } else {
            cout << FAIL_STATEMENT << "Inserting in amortized log(n) time" << endl;
        }
    }
    // test remove time ratio
    {
        if (myTester.test_removeTime()) {
            cout << PASS_STATEMENT << "Removing in amortized log(n) time" << endl;
        } else {
            cout << FAIL_STATEMENT << "Removing in amortized log(n) time" << endl;
        }
    }
    return 0;
}

int priorityFn1(const Post &post) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-510]
    //the highest priority would be 500+10 = 510
    //the lowest priority would be 0+1 = 1
    //the larger value means the higher priority
    int priority = post.getNumLikes() + post.getInterestLevel();
    return priority;
}

int priorityFn2(const Post &post) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [2-55]
    //the highest priority would be 1+1 = 2
    //the lowest priority would be 5+50 = 55
    //the smaller value means the higher priority
    int priority = post.getPostTime() + post.getConnectLevel();
    return priority;
}