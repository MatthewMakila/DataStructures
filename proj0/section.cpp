// UMBC - CMSC 341 - Spring 2022 - Proj0
/** Matthew Makila
 *  Project 0: Section class
 *  section.cpp
 *  2/9/22
 *  This file includes all the implementation for the section class. There are all section methods
 */
#include "section.h"

// Name: Section (Default constructor)
// Desc: Creates a Section object with 0 values and pointer array
// Preconditions: None
// Postconditions: Creates default section object
Section::Section(){
    m_numGradingItems = 0;
    m_numStudents = 0;
    // allocate memory for the grades array
    m_grades = new double *[m_numStudents];
    for (int i = 0; i < m_numStudents; i++) {
        m_grades[i] = new double[m_numGradingItems];
    }
}

// Name: Section (overloaded constructor)
// Desc: Creates a Section object with (greater than 0) values and pointer array initialized to 0
// Preconditions: None
// Postconditions: Creates section object with given user values
Section::Section(int items, int students){
    // only do assignment if vals are positive
    if (items > 0 && students > 0){
        m_numGradingItems = items;
        m_numStudents = students;
        m_grades = new double *[m_numStudents];

        for (int i = 0; i < m_numStudents; i++){
            m_grades[i] = new double [m_numGradingItems];
        }

        // set all data to 0 to start
        for (int i = 0; i < m_numStudents; i++){
            for (int j = 0; j < m_numGradingItems; j++){
                m_grades[i][j] = 0;
            }
        }
    } else {
        // create empty object
        m_numStudents = 0;
        m_numGradingItems = 0;
        m_grades = new double *[m_numStudents];
        for (int i = 0; i < m_numStudents; i++) {
            m_grades[i] = new double[m_numGradingItems];
        }
    }

}

// Name: ~Section (Destructor)
// Desc: Calls clear to destroy object
// Preconditions: Object exists
// Postconditions: Deallocates memory and destroys object
Section::~Section(){
    clear();
    
}

// Name: clear
// Desc: Deallocates all memory given to the section object
// Preconditions: We assume object already exists to be cleared
// Postconditions: Object has all memory deallocated and cleared out
void Section::clear(){
    // deallocates our memory: we want to clear 2D array step by step, starting w/ arrays pointed to for each student
    for (int i = 0; i < m_numStudents; i++){
        delete [] m_grades[i];
    }
    delete [] m_grades;
    m_grades = nullptr;
    m_numStudents = 0;
    m_numGradingItems = 0;

}

// Name: insert
// Desc: Inserts uniform real distribution of numbers into a section object's pointed-to double array
// Preconditions: Object exists with valid values
// Postconditions: Populates object with uniform real numbers
void Section::insert(){
    // insert all grades for every student
    Random numGenerator(0, 100, UNIFORMREAL);
    for (int i = 0; i < m_numStudents; i++){
        for (int j = 0; j < m_numGradingItems; j++){
            m_grades[i][j] = numGenerator.getRealRandNum();
        }
    }
    // cannot remember how to make object properly
    // Random myNum
    // when making rand object, ask user for min, max, and distrib type
}

// Name: studentAverage
// Desc: Takes specific student and averages all their grades
// Preconditions: Object and student are valid and exist
// Postconditions: Calculates average and returns it
double Section::studentAverage(int student){
    // check all grades for one student (ex: student 1's quiz 1, 2, ... , n)
    // check if student index is valid (invalid: negative or greater than total students - 1
    if (student < 0 or student > (m_numStudents - 1)){
        return 0;
    } else { // loop for avg of this student's grades double array
        double total = 0;
        for (int i = 0; i < m_numGradingItems; i++) {
            total += m_grades[student][i];
        }
        return total/m_numGradingItems;
    }
}

// Name: quizAverage
// Desc: Takes specific quiz and averages all grades for it
// Preconditions: Object, quiz, and student are valid and exist
// Postconditions: Calculates average and returns it
double Section::quizAverage(int quiz){
    // check one specific assignment for all students (ex: everybody's quiz 1)
    double result = 0;
    // check if it's a valid quiz (index at least 0 and not greater than total grading items - 1)
    if (quiz < 0 or quiz > (m_numGradingItems - 1)) {
        result = 0;
    } else { // quiz index is valid, then calc the average for quiz from all students
        double total = 0;
        for (int i = 0; i < m_numGradingItems; i++) {
            total += m_grades[i][quiz];
        }
        result = total/m_numGradingItems;
    }
    return result;
    
}

// Name: classAverage
// Desc: averages all grades
// Preconditions: Object is valid and exists with inserted values
// Postconditions: Calculates average and returns it
double Section::classAverage(){
    // check all grades for all assignments (i.e., check every row and col and average them)
    if (m_numStudents == 0 or m_numGradingItems == 0){ // if we have no students or grades, we return 0
        return 0;
    } else {
        double total = 0;
        // sum every grade in pointed-to double arrays
        for (int i = 0; i < m_numStudents; i++) {
            for (int j = 0; j < m_numGradingItems; j++) {
                total += m_grades[i][j];
            }
        }
        // we want the total of every grade / total # of assignments across all students
        return total/(m_numGradingItems*m_numStudents);
    }
    return 0;
    
}

// Name: Section (copy constructor)
// Desc: Copies all data from the passed object within a new object w/ a new memory address
// Preconditions: Object is created already
// Postconditions: Copies all data
Section::Section(const Section& rhs){
    // copy constructor: make deep copy
    // cout << "     COPY CONSTRUCTOR FIRING!" << endl;
    m_numGradingItems = rhs.m_numGradingItems;
    m_numStudents = rhs.m_numStudents;

    m_grades = new double *[m_numStudents];

    for (int i = 0; i < m_numStudents; i++){
        m_grades[i] = new double [m_numGradingItems];
    }

    // set all data to original array's
    for (int i = 0; i < m_numStudents; i++){
        for (int j = 0; j < m_numGradingItems; j++){
            m_grades[i][j] = rhs.m_grades[i][j];
        }
    }
}

// Name: Section (overloaded assignment operator)
// Desc: Copies all data from the passed object within a new object w/ a new memory address
// Preconditions: Object is created already
// Postconditions: returns new object with all data of passed object
const Section& Section::operator=(const Section& rhs){
    // overloaded assignment operator
    // cout << "     ASSIGN OPERATOR FIRING!" << endl;
    clear(); // first clear current array
    if (this != &rhs){  // protect against self-assign:
        m_numGradingItems = rhs.m_numGradingItems;
        m_numStudents = rhs.m_numStudents;

        m_grades = new double *[m_numStudents];

        for (int i = 0; i < m_numStudents; i++){
            m_grades[i] = new double [m_numGradingItems];
        }

        // set all data to original array's
        for (int i = 0; i < m_numStudents; i++){
            for (int j = 0; j < m_numGradingItems; j++){
                m_grades[i][j] = rhs.m_grades[i][j];
            }
        }
    }
    return *this;
}

// Name: dump
// Desc: displays all data and info for a section
// Preconditions: Object is created already and has nonzero info
// Postconditions: Copies all data
void Section::dump(){
    if (m_numGradingItems > 0 && m_numStudents > 0 && m_grades != nullptr){
        cout << setw(8) << "        ";
        for (int a=1;a<=m_numGradingItems;a++)
            cout << setw(8) << "Quiz" << a;
        cout << endl;
        for (int i=1;i <= m_numStudents;i++){
            cout << setw(8) << "Student" << i << " ";
            for (int j=1;j<=m_numGradingItems;j++){
                cout << setw(8) << m_grades[i-1][j-1] << " ";
            }
            cout << endl;
        }
    }
}