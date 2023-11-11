// CMSC 341 - Spring 2022 - Project 4
/** Matthew Makila
 *  Project 4: DNA Database
 *  dnadb.cpp
 *  4/25/22
 *  This includes the entirety of the implementation for the DnaDb class
 */
#include "dnadb.h"

// Name: DnaDb
// Desc: creates dynamic hash table and initializes values
// Preconditions: None
// Postconditions: Creates new DnaDb with hash table
DnaDb::DnaDb(int size, hash_fn hash){
    m_hash = hash;
    m_currentSize = 0; // num entries starts at 0
    m_currNumDeleted = 0; // num entries deleted is initially 0
    m_oldCap = 0;
    m_oldNumDeleted = 0;
    m_oldSize = 0;
    m_deletionTracker = 0;
    m_oldTable = nullptr;

    // check size
    if (size < MINPRIME) {
        m_currentCap = MINPRIME;
    } else if (size > MAXPRIME) {
        m_currentCap = MAXPRIME;
    } else {
        if (!(isPrime(size))) {
            m_currentCap = findNextPrime(size);
        } else {
            m_currentCap = size;
        }
    }
    m_currentTable = new DNA [m_currentCap]; // allocate memory for table w/ given size
}

// Name: ~DnaDb
// Desc: destroys dynamic hash table and re-initializes values
// Preconditions: DnaDb exists
// Postconditions: Destroys DnaDb with hash table
DnaDb::~DnaDb(){
    // deallocate mem
    delete [] m_currentTable;
    delete [] m_oldTable;
    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_oldCap = 0;
    m_oldNumDeleted = 0;
    m_oldSize = 0;
    m_deletionTracker = 0;

}

// Name: insert
// Desc: inserts DNA object into the hash table with quadratic probing
// Preconditions: hash table exists
// Postconditions: new DNA inserted and collision resolved
bool DnaDb::insert(DNA dna){
    // use hash function here, check lambda value?
    const float threshold = 0.5;

    if (dna.getLocId() < MINLOCID || dna.getLocId() > MAXLOCID) {
        return false; // invalid location ID
    }
    if (dna == getDNA(dna.getSequence(), dna.getLocId())) {
        return false; // DNA strand already exists
    }
    // index = ((Hash(key) % TableSize) + (i x i)) % TableSize
    bool bucketFound = false;
    unsigned int i = 0;
    // search until val found (quad probe after first iteration) or unless every possible i used
    while (!bucketFound && i < m_currentCap) {
        unsigned int myHashIdx = ((m_hash(dna.getSequence()) % m_currentCap) + (i * i)) % m_currentCap;
        if (m_currentTable[myHashIdx] == EMPTY || m_currentTable[myHashIdx] == DELETED) {
            /**do I need to do this?**/
            if (m_currentTable[myHashIdx] == DELETED) {
                m_currNumDeleted--;
            }
            m_currentTable[myHashIdx] = dna;
            m_currentSize++; // increase for new occupied bucket
            bucketFound = true;
        }
        i++;
    }
    if (!bucketFound) {
        return false;
    }
    if (lambda() > threshold || m_oldTable) { // check criteria for rehash
        // rehash: find next prime, rebuild table, transfer buckets, etc
        rehashTable();
    }
    // if all went right, true
    // cout << "Num items inserted: " << m_currentSize << endl;
    // cout << "load factor " << lambda() << endl;

    return true;
}

// Name: remove
// Desc: removes a DNA object from hash table with quadratic probing
// Preconditions: DNA object exists in hash table
// Postconditions: removes DNA object if exists in table
bool DnaDb::remove(DNA dna){
    // should remove call getDNA, vice versa, or is that separate?
    const float threshold2 = 0.8;
    bool goodRemove = false;

    if (m_oldTable) { // check if val in old table if still in rehash
        // we probe
        bool bucketFound = false;
        unsigned int i = 0;
        while (!bucketFound && i < m_oldCap) {
            unsigned int idx = ((m_hash(dna.getSequence()) % m_oldCap) + (i * i)) % m_oldCap;
            if (!(m_oldTable[idx] == EMPTY && !(m_oldTable[idx] == DELETED))) {
                if (m_oldTable[idx].getSequence() == dna.getSequence() && m_oldTable[idx].getLocId() == dna.getLocId()) {
                    m_oldTable[idx] = DELETED;
                    m_oldSize--; // increase for new occupied bucket
                    m_oldNumDeleted++;
                    bucketFound = true;
                    goodRemove = true;
                }
            }
            i++;
        }
    }
    // make sure we didn't already find the DNA
    if (!goodRemove) {
        // we probe
        bool bucketFound = false;
        unsigned int i = 0;
        while (!bucketFound && i < m_currentCap) {
            unsigned int idx = ((m_hash(dna.getSequence()) % m_currentCap) + (i * i)) % m_currentCap;
            if (!(m_currentTable[idx] == EMPTY) && !(m_currentTable[idx] == DELETED)) {
                if (m_currentTable[idx].getSequence() == dna.getSequence() && m_currentTable[idx].getLocId() == dna.getLocId()) {
                    m_currentTable[idx] = DELETED;
                    m_currentSize--; // increase for new occupied bucket
                    m_currNumDeleted++;
                    bucketFound = true;
                    goodRemove = true;
                }
            }
            i++;
        }

    }

    if (deletedRatio() > threshold2 || m_oldTable) {
        rehashTable();
    }
    return goodRemove;
}

// Name: rehashTable
// Desc: creates new dynamic hash table and re-initializes values, moves data from old table to new
// Preconditions: a rehash was triggered from removal by either lambda or deletedRatio increase
// Postconditions: Creates new hash table and shifts data 25% at a time from old table
void DnaDb::rehashTable() {
    const float fourth = 0.25;
    if (!(m_oldTable)) { // if first step of rehash!
        // create old table
        m_oldTable = m_currentTable;
        m_oldSize = m_currentSize;
        m_oldCap = m_currentCap;
        m_oldNumDeleted = m_currNumDeleted;
        m_deletionTracker = 0;
        // reset new table
        unsigned int newCap = (m_currentSize - m_currNumDeleted)*4;
        if (newCap < 1) {
            newCap *= -1;
        }
        m_currentCap = findNextPrime(newCap);
        m_currentTable = new DNA [m_currentCap];
        m_currentSize = 0;
        m_currNumDeleted = 0;
    }
    // incrementally transfer 25% of oldSize (next 25% after next operation, etc)
    int count = 0;
    float ratio = float(count) / float(m_oldSize);
    for (unsigned int k = 0; k < m_oldCap; k++){
        if (ratio <= fourth) {
            // check if old table idx has DNA, if so, rehash into new table
            if (!(m_oldTable[k] == EMPTY) && !(m_oldTable[k] == DELETED)) {
                // perform quad probed insertion
                unsigned int i = 0;
                bool bucketFound = false;
                while (!bucketFound && i < m_currentCap) {
                    unsigned int idx = ((m_hash(m_oldTable[k].getSequence()) % m_currentCap) + (i * i)) % m_currentCap;
                    if (m_currentTable[idx] == EMPTY || m_currentTable[idx] == DELETED) {
                        if (m_currentTable[idx] == DELETED) {
                            m_currNumDeleted--;
                        }
                        m_currentTable[idx] = m_oldTable[k]; /**Uses Assign Op**/
                        m_oldTable[k] = DELETED;
                        m_currentSize++; // increase for new occupied bucket
                        m_deletionTracker++;
                        bucketFound = true;
                    }
                    i++;
                }
                count++;
            }
        }
        ratio = float(count) / float(m_oldSize);
        // remove old table after completely copied over (emptied)
        if (m_deletionTracker == m_oldSize) { // we've gone through all entries in old table
            delete [] m_oldTable;
            m_oldTable = nullptr;
            m_oldCap = 0;
            m_oldNumDeleted = 0;
            m_oldSize = 0;
        }
    }
}

// Name: getDNA
// Desc: finds a DNA object
// Preconditions: DnaDb exists
// Postconditions: Returns given DNA object, empty if none found
DNA DnaDb::getDNA(string sequence, int location){
    DNA foundDNA = EMPTY;
    if (m_oldTable) {
        // search old table
        // probe i guess
        unsigned int i = 0;
        while (i < m_oldCap) {
            unsigned int idx = ((m_hash(sequence) % m_oldCap) + (i * i)) % m_oldCap;
            if (!(m_oldTable[idx] == EMPTY) && !(m_oldTable[idx] == DELETED)) {
                if (m_oldTable[idx].getSequence() == sequence && m_oldTable[idx].getLocId() == location) {
                    foundDNA = m_oldTable[idx];
                    break;
                }
            }
            i++;
        }
    }
    // iterate over current table
    if (foundDNA == EMPTY) {
        // search old table
        // probe i guess
        unsigned int i = 0;
        while (i < m_currentCap) {
            unsigned int idx = ((m_hash(sequence) % m_currentCap) + (i * i)) % m_currentCap;
            if (!(m_currentTable[idx] == EMPTY) && !(m_currentTable[idx] == DELETED)) {
                if (m_currentTable[idx].getSequence() == sequence && m_currentTable[idx].getLocId() == location) {
                    foundDNA = m_currentTable[idx];
                    break;
                }
            }
            i++;
        }
    }
    return foundDNA;
}

// Name: lambda
// Desc: ratio of occupied buckets to table size
// Preconditions: None
// Postconditions: returns ratio
float DnaDb::lambda() const {
      // returns current float lambda val
      // (total number of buckets with data or that used to have data) / table cap size
      return float(m_currentSize + m_currNumDeleted) / float(m_currentCap);
}

// Name: deletedRatio
// Desc: ratio of deleted buckets to occupied buckets
// Preconditions: None
// Postconditions: returns ratio
float DnaDb::deletedRatio() const {
    return float(m_currNumDeleted) / float(m_currNumDeleted + m_currentSize);
}

// Name: dump
// Desc: dumps tables
// Preconditions: DnaDb exists
// Postconditions: dumps tables if exist
void DnaDb::dump() const {
    cout << "Dump for current table: " << endl;
    if (m_currentTable != nullptr)
        for (unsigned int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for old table: " << endl;
    if (m_oldTable != nullptr)
        for (unsigned int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

// Finds out if a number is prime
bool DnaDb::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

// finds next prime from a given int
int DnaDb::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

// creates DNA object
DNA::DNA(string sequence, int location) {
    if ((location >= MINLOCID && location <= MAXLOCID) ||
        (location == 0 && sequence == "DELETED")){
        // this is a normal or a DELETED object
        m_sequence = sequence;
        m_location = location;
    }
    else{
        // this is the empty object
        m_sequence = "";
        m_location = 0;
    }
}

// gets sequence of DNA object
string DNA::getSequence() const {
    return m_sequence;
}

// gets ID of DNA object
int DNA::getLocId() const {
    return m_location;
}

// Overloaded assignment operator
const DNA& DNA::operator=(const DNA& rhs){
    if (this != &rhs){
        m_sequence = rhs.m_sequence;
        m_location = rhs.m_location;
    }
    return *this;
}

// Overloaded insertion operator.  Prints DNA's sequence (key),
// and the location ID. This is a friend function in DNA class.
ostream& operator<<(ostream& sout, const DNA &dna ) {
    if (!dna.m_sequence.empty())
        sout << dna.m_sequence << " (Location ID " << dna.m_location << ")";
    else
        sout << "";
  return sout;
}

// Overloaded equality operator. This is a friend function in DNA class.
// To test inequality we may negate the results of this operator.
bool operator==(const DNA& lhs, const DNA& rhs){
    return ((lhs.m_sequence == rhs.m_sequence) && (lhs.m_location == rhs.m_location));
}