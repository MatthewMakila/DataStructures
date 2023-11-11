// UMBC - CMSC 341 - Spring 2022 - Proj0
/** Matthew Makila
 *  Project 0: Section class
 *  mytest.cpp
 *  2/9/22
 *  This file includes all testing material for the section class. There are tests for all section methods
 */
#include "section.h"

const char* FAIL_STATEMENT = "*****TEST FAILED: ";
const char* PASS_STATEMENT = "     TEST PASSED: ";

class Tester {
public:
    // the following is implemented as an example of measurement
    void measureInsertionTime(int numTrials, int inputSize);
    // the following is implemented as a test case example
    bool testStudentAveErrorCase();

    /******************************************
    * Test function declarations go here! *
    ******************************************/
    // tests for constructor
    bool testConstructorErrorCase();
    bool testConstructorNormCase();
    bool testConstructorEdge();
    // tests for copy constructor
    bool testCopyConstructor();
    bool testCopyConstructorEdge();
    void testCopyConInsertTime(int numTrials, int inputSize);
    // tests for assignment op
    bool testAssignOp();
    bool testAssignOpEdge();
    // tests for student avg
    bool testStudentAveNormCase();
    // tests for student quiz avg
    bool testStudentQuizAveErrorCase();
    bool testStudentQuizAveNormCase();
    // tests for class avg
    bool testClassAvgEdge();
    bool testClassAvg();
    // tests for destructor and clear method
    bool testDestructor();
};
int main(){
    Tester tester; // test object

    cout << "---CONSTRUCTOR TESTS---" << endl;
    // Test the constructor error
    {
        if (tester.testConstructorErrorCase()){
            cout << PASS_STATEMENT << "negative inputs for overloaded constructor" << endl;
        } else {
            cout << FAIL_STATEMENT << "negative inputs for overloaded constructor" << endl;
        }
    }
    // Test the constructor normal
    {
        if (tester.testConstructorNormCase()){
            cout << PASS_STATEMENT << "normal inputs for overloaded constructor" << endl;
        } else {
            cout << FAIL_STATEMENT << "normal inputs for overloaded constructor" << endl;
        }
    }
    // Test the constructor edge case
    {
        if (tester.testConstructorEdge()){
            cout << PASS_STATEMENT << "edge case (1 student, 1 quiz) input for overloaded constructor" << endl;
        } else {
            cout << FAIL_STATEMENT << "edge case (1 student, 1 quiz) input for overloaded constructor" << endl;
        }
    }
    // Test Copy Constructor
    {
        if (tester.testCopyConstructor()){
            cout << PASS_STATEMENT << "copy constructor for different mem. addresses but same data" << endl;
        } else {
            cout << FAIL_STATEMENT << "copy constructor for different mem. addresses but same data" << endl;
        }
    }
    // Test Copy Constructor Edge
    {
        if (tester.testCopyConstructorEdge()) {
            cout << PASS_STATEMENT << "copy constructor for edge case (empty)" << endl;
        } else {
            cout << FAIL_STATEMENT << "copy constructor for edge case (empty)" << endl;
        }
    }
    // Test Copy Constructor Insertion Time
    {
        // measuring the running time of insertion
        cout << "\n     Measuring the performance of insertion function on Copy Constructor:\n\n";
        // measuring the insertion of 100x100 grades, and 200x200 grades
        // the 100x100 grades is determined by 100 items times 100 students
        tester.testCopyConInsertTime(4,100);
    }
    /****************
     ** TEST BREAK **
     ****************/
    cout << "\n---ASSIGNMENT OPERATOR TESTS---" << endl;
    // test Assignment Op norm
    {
        if (tester.testAssignOp()){
            cout << PASS_STATEMENT << "normal inputs for assign operator" << endl;
        } else {
            cout << FAIL_STATEMENT << "normal inputs for assign operator" << endl;
        }
    }
    // test assignment op edge case
    {
        if (tester.testAssignOpEdge()) {
            cout << PASS_STATEMENT << "edge case (assign empty object) for assign operator" << endl;
        } else {
            cout << FAIL_STATEMENT << "edge case (assign empty object) for assign operator" << endl;
        }
    }

    /****************
     ** TEST BREAK **
     ****************/
    cout << "\n---STUDENT AVE TESTS---" << endl;
    {
        // testing norm case for calculation of student average
        if (tester.testStudentAveNormCase())
            cout << PASS_STATEMENT << "normal input for studentAverage()" << endl;
        else
            cout << FAIL_STATEMENT << "normal input for studentAverage()" << endl;
    }

    {
        // testing error case for calculation of student average
        if (tester.testStudentAveErrorCase())
            cout << PASS_STATEMENT << "Error case (student index does not exist) of studentAverage()" << endl;
        else
            cout << FAIL_STATEMENT << "Error case (student index does not exist) of studentAverage()" << endl;
    }
    /****************
     ** TEST BREAK **
     ****************/
    cout << "\n---STUDENT QUIZ AVE TESTS---" << endl;
    {
        // testing error case for calculation of student quiz average
        if (tester.testStudentQuizAveErrorCase())
            cout << PASS_STATEMENT << "Error case (non-existent quiz index) for quizAverage()" << endl;
        else
            cout << FAIL_STATEMENT << "Error case (non-existent quiz index) for quizAverage()" << endl;
    }

    {
        // testing normal case for calculation of student quiz average
        if (tester.testStudentQuizAveNormCase())
            cout << PASS_STATEMENT << "normal input for quizAverage()" << endl;
        else
            cout << FAIL_STATEMENT << "normal input for quizAverage()" << endl;
    }
    /****************
     ** TEST BREAK **
     ****************/
    cout << "\n---CLASS AVE TESTS---" << endl;
    // Class Avg Edge Case
    {
        if (tester.testClassAvgEdge())
            cout << PASS_STATEMENT << "Edge case (no students in class) for classAverage()" << endl;
        else
            cout << FAIL_STATEMENT << "Edge case (no students in class) for classAverage()" << endl;
    }
    // Class Avg Norm Case
    {
        if (tester.testClassAvg())
            cout << PASS_STATEMENT << "Normal case for classAverage()" << endl;
        else
            cout << FAIL_STATEMENT << "Normal case for classAverage()" << endl;
    }
    /****************
     ** TEST BREAK **
     ****************/
    // Test Destructor
    cout << "\n---DESTRUCTOR TEST---" << endl;
    {
        if (tester.testDestructor()){
            cout << PASS_STATEMENT << "Destructor deallocating memory and destroying object" << endl;
        } else {
            cout << FAIL_STATEMENT << "Destructor deallocating memory and destroying object" << endl;
        }
    }
    {
        // the following is a sample usage of the Section class
        // create an object with 5 grading items and 8 students
        cout << "\nSample usage of the Section class:\n\n";
        Section aSection(5,8);
        aSection.insert();
        aSection.dump();
    }
    {
        // measuring the running time of insertion
        cout << "\nMeasuring the performance of insertion function:\n\n";
        // measuring the insertion of 100x100 grades, and 200x200 grades
        // the 100x100 grades is determined by 100 items times 100 students
        tester.measureInsertionTime(2,100);
    }

    cout << endl;
    return 0;
}

// Name: measureInsertionTime
// Desc: Creates test object to clock how long the insertion method takes to populate it
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
void Tester::measureInsertionTime(int numTrials, int inputSize) {
    //Measuring the efficiency of insertion algorithm with system clock ticks
    //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
    //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k=0;k<numTrials;k++) {
        Section testObject(inputSize,inputSize);
        start = clock();
        testObject.insert();//the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "\tInserting " << inputSize * inputSize << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        inputSize = inputSize * a;//increase the input size by the scaling factor
    }
}

// Name: testStudentAveErrorCase
// Desc: Creates test object to test error case for the student average
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testStudentAveErrorCase() {
    bool result = true;
    // create an object with 1 student and 10 grading items
    Section testObject(10,1);
    testObject.insert();
    double average = testObject.studentAverage(1);// out of range index
    if (average == 0.0)
        result = true;
    else
        result = false;
    return result;
}

// Name: testStudentAveNormCase
// Desc: Creates test object to test if valid inputs work for calculating the student average
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testStudentAveNormCase() {
    bool result = true;
    // create an object with 2 student and 4 grading items
    Section testObject(4,2);
    testObject.insert();
    double average = testObject.studentAverage(1);// the second student
    double expectedAverage = (58.33+2.51+70.92+26.55)/4;
    if (average == expectedAverage)
        result = true;
    else
        result = false;
    return result;
}

// Name: testStudentQuizAveErrorCase
// Desc: Creates test object to test error case for the quiz average
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testStudentQuizAveErrorCase() {
    bool result = true;
    // create an object with 1 student and 10 grading items
    Section testObject(5,3);
    testObject.insert();
    double average = testObject.quizAverage(-1);// out of range index
    if (average == 0.0)
        result = true;
    else
        result = false;
    return result;
}

// Name: testStudentQuizAveNormCase
// Desc: Creates test object to test norm inputs case for the quiz average
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testStudentQuizAveNormCase() {
    bool result = true;
    // create an object with 1 student and 10 grading items
    Section testObject(3,3);
    testObject.insert();
    double average = testObject.quizAverage(0);// norm (first quiz)
    double expectedAvg = (29.87+83.19+70.92)/3;
    if (average == expectedAvg)
        result = true;
    else
        result = false;
    return result;
}

// Name: testConstructorErrorCase
// Desc: Creates test object to test error case for the constructor
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testConstructorErrorCase() {
    bool result = false;
    // create obj with -1 students and -1 items
    Section testObject(-1, -1);
    // catch bad inputs
    if (testObject.m_numGradingItems == 0 && testObject.m_numStudents == 0){
        result = true;
    }
    return result;
}

// Name: testConstructorNormCase
// Desc: Creates test object to test normal inputs for building an object
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testConstructorNormCase() {
    // should make obj with 10 items and students
    bool result = false;
    Section testObject(10, 10);
    if (testObject.m_numGradingItems == 10 && testObject.m_numStudents == 10){
        result = true;
    }
    return result;
}

// Name: testConstructorEdge
// Desc: Creates test object to test edge inputs for building an object
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testConstructorEdge() {
    // constructor should make empty object
    bool result = false;
    Section testObject(1, 1);
    if (testObject.m_numGradingItems == 1 && testObject.m_numStudents == 1){
        result = true;
    }
    return result;
}

// Name: testCopyConstructor
// Desc: Creates test object to test (deep) copying an object
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testCopyConstructor() {
    // check if address of original and deep copy are different (should be!)
    bool result = true;
    Section testObject(3,2);
    testObject.insert();
    Section testObject2(testObject);
    if (testObject.m_grades != testObject2.m_grades){ // check if mem address are diff (deep copy actually worked!)
        for (int i = 0; i < testObject.m_numStudents; i++){ // check if all data is copied the same
            for (int j = 0; j < testObject.m_numGradingItems; j++) {
                if (testObject.m_grades[i][j] != testObject2.m_grades[i][j]) {
                    result = false;
                }
            }
        }
    } else {
        result = false;
    }

    return result;
}

// Name: testCopyConstructorEdge
// Desc: Creates test object to test (deep) copying an empty object
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testCopyConstructorEdge() {
    // check if address of original and deep copy are different (should be!)
    bool result = true;
    Section testObject(0,0);
    testObject.insert();
    Section testObject2(testObject);
    if (testObject.m_grades != testObject2.m_grades){ // check if mem address are diff (deep copy actually worked!)
        for (int i = 0; i < testObject.m_numStudents; i++){ // check if all data is copied the same
            for (int j = 0; j < testObject.m_numGradingItems; j++) {
                if (testObject.m_grades[i][j] != testObject2.m_grades[i][j]) {
                    result = false;
                }
            }
        }
    } else {
        result = false;
    }

    return result;
}

// Name: testCopyConInsertTime
// Desc: Creates test object to test how fast insert works on a (deep) copied object
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
void Tester::testCopyConInsertTime(int numTrials, int inputSize) {
    //Measuring the efficiency of insertion algorithm with system clock ticks
    //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
    //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k=0;k<numTrials;k++) {
        Section testObject(inputSize,inputSize);
        Section copiedObject(testObject);
        start = clock();
        copiedObject.insert();//the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "\tInserting " << inputSize * inputSize << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        inputSize = inputSize * a;//increase the input size by the scaling factor
    }
}

// Name: testAssignOp
// Desc: Creates test object to test assignment of one object to another
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testAssignOp() {
    // test if assign works to clear out old object reassign it
    bool result = true;
    Section testObject(3,2);
    Section testObject2(2, 4);
    testObject.insert();
    testObject2 = testObject;
    if (testObject.m_grades != testObject2.m_grades){ // check if mem address are diff (deep copy actually worked!)
        for (int i = 0; i < testObject.m_numStudents; i++){ // check if all data is copied the same
            for (int j = 0; j < testObject.m_numGradingItems; j++) {
                if (testObject.m_grades[i][j] != testObject2.m_grades[i][j]) {
                    result = false;
                }
            }
        }
    }
    return result;
}

// Name: testAssignOpEdge
// Desc: Creates test object to test assignment of empty object to another
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testAssignOpEdge() {
    // can it copy an empty object
    bool result = true;
    Section testObject(0,0);
    Section testObject2(2, 4);
    testObject.insert();
    testObject2 = testObject;
    if (testObject.m_grades != testObject2.m_grades){ // check if mem address are diff (deep copy actually worked!)
        for (int i = 0; i < testObject.m_numStudents; i++){ // check if all data is copied the same
            for (int j = 0; j < testObject.m_numGradingItems; j++) {
                if (testObject.m_grades[i][j] != testObject2.m_grades[i][j]) {
                    result = false;
                }
            }
        }
    }
    return result;
}

// Name: testClassAvgEdge
// Desc: Creates test object to test class avg with no students
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testClassAvgEdge() {
    // build object with no students
    bool result = false;
    Section testObject(5,0);
    if (testObject.classAverage() == 0){ // should return 0 since no students
        result = true;
    }
    return result;
}

// Name: testClassAvg
// Desc: Creates test object to test class avg with norm input of students and items
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testClassAvg() {
    // build object with just 2 students and 2 items each
    bool result = false;
    Section testObject(2,2);
    testObject.insert();
    if (testObject.classAverage() == (29.87+49.45+44.3+83.19)/4){ // if we get correct average total for this test, it passes
        result = true;
    }
    return result;
}

// Name: testDestructor
// Desc: Creates test object to test that destructor (clear) works
// Preconditions: None
// Postconditions: Returns a bool for whether test passes or not
bool Tester::testDestructor() {
    // build object and destroy it.
    bool result = false;
    Section testObject(2,4);
    testObject.insert();
    testObject.clear(); // this should get rid of all allocated memory, counts as destructor test since it calls this
    // if all data is deallocated, test passes
    if (testObject.m_grades == nullptr && testObject.m_numGradingItems == 0 && testObject.m_numStudents == 0){
        result = true;
    }
    return result;
}