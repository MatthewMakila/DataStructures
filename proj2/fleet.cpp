// UMBC - CMSC 341 - Spring 2022 - Proj2
/** Matthew Makila
 *  Project 2: Fleet of Spaceships
 *  fleet.cpp
 *  3/27/22
 *  This class file contains the implementation of every method in the fleet class
 */
#include "fleet.h"

// Name: Fleet
// Desc: Constructs empty Fleet object
// Preconditions: None
// Postconditions: Creates empty Fleet object
Fleet::Fleet(){
    m_root = nullptr;
}

// Name: ~Fleet
// Desc: Deallocates Fleet object with clear()
// Preconditions: Object exists
// Postconditions: Empties Fleet object
Fleet::~Fleet(){
    clear();
}

// Name: insert
// Desc: inserts a const ship object into the fleet
// Preconditions: object exists
// Postconditions: inserts (if valid) a new ship into fleet in proper order
void Fleet::insert(const Ship& ship){
    if (ship.getID() >= MINID && ship.getID() <= MAXID && !findShip(ship.getID())) { // only insert valid IDs
        if (m_root == nullptr) { // first ship in fleet
            Ship* newShip = new Ship(ship.getID(), ship.getType(), ship.getState());
            m_root = newShip;
        } else {
            Ship* temp = insertHelper(m_root, ship);
            m_root = temp;
        }
    }
}

// Name: insertHelper
// Desc: Recusrive help for insert
// Preconditions: insert is called upon
// Postconditions: inserts (if valid) a new ship into fleet in proper order
Ship *Fleet::insertHelper(Ship* aShip, const Ship& newShip) {
    if (aShip == nullptr) { // return the current node
        Ship* myShip = new Ship(newShip.getID(), newShip.getType(), newShip.getState());
        return myShip;
    }
    if (newShip.getID() < aShip->getID()) { // if key is less than current node's key, go to its left
        aShip->m_left = insertHelper(aShip->m_left, newShip);
        // updateHeight(aShip->m_left);
        // aShip->m_left = rebalance(aShip->m_left);
    } if (newShip.getID() > aShip->getID()){ // if key > current node's key, go to its right node
        aShip->m_right = insertHelper(aShip->m_right, newShip);
        // updateHeight(aShip->m_right);
        // aShip->m_right = rebalance(aShip->m_right);
    }
    updateHeight(aShip); // call update height
    if (abs(checkImbalance(aShip)) > 1) {  // if true, we need rebalance
        return rebalance(aShip);
    }
    return aShip;
}

// Name: clear
// Desc: deallocates memory of fleet
// Preconditions: object exists
// Postconditions: memory is freed
void Fleet::clear(){
    clearHelp(m_root);
}

// Name: clearHelp
// Desc: recursive helper that deallocates memory of fleet
// Preconditions: object exists
// Postconditions: memory is freed
void Fleet::clearHelp(Ship *pShip) {
    if (pShip) { // if not null
        clearHelp(pShip->m_left); // deallocate all mem from left and right nodes of root
        clearHelp(pShip->m_right);
        delete pShip;
        pShip = nullptr;
    }
}

// Name: remove
// Desc: removes a ship from fleet given its ID
// Preconditions: object exists and is in fleet
// Postconditions: if valid ID, the ship is removed and order is restored to BST
void Fleet::remove(int id){
    if (findShip(id)) { // make sure we only remove if node exists
        Ship* temp = removeHelper(m_root, id);
        m_root = temp;
        // what do we do to remove? delete ???
    }
}

// Name: removeHelper
// Desc: removes a ship from fleet given its ID
// Preconditions: object exists and is in fleet
// Postconditions: if valid ID, the ship is removed and order is restored to BST
Ship* Fleet::removeHelper(Ship* aShip, int shipID) {
    if (aShip == nullptr) {
        return aShip;
    }
    if (shipID < aShip->getID()) {
        aShip->m_left = removeHelper(aShip->m_left, shipID);
    } else if (shipID > aShip->getID()) {
        aShip->m_right = removeHelper(aShip->m_right, shipID);
    } else { // found correct node
        if (!aShip->m_left || !aShip->m_right) { // check if only 1 or 0 child
            Ship* temp = nullptr;
            if (aShip->m_right) {
                temp = aShip->m_right;
            } else if (aShip->m_left) {
                temp = aShip->m_left;
            }

            if (temp == nullptr) { // 0 child
                temp = aShip;
                aShip = nullptr;
            } else  // just 1 child
            *aShip = *temp;
            delete temp;

        } else { // we have 2 child, must remove more delicately
            // find next child in order (hop right and go as far left as you can)
            Ship* temp = aShip->m_right;
            while (temp->m_left) {
                temp = temp->m_left;
            }
            // copy child data into curr and delete child
            aShip->setID(temp->getID());
            aShip->m_right = removeHelper(aShip->m_right, temp->getID());
        }
    }
    if (!aShip) { // null ship
        return aShip;
    }
    // CHECK BAL etc
    updateHeight(aShip);
    if (abs(checkImbalance(aShip)) > 1) {  // if true, we need to rebalance
        return rebalance(aShip);
    }
    return aShip;
}

// Name: updateHeight
// Desc: Updates the height of a given node based on its children
// Preconditions: object exists and is passed to this function
// Postconditions: the ship height is reevaluated
void Fleet::updateHeight(Ship* aShip){
    if (!aShip->m_right && !aShip->m_left) { // no children
        aShip->m_height = 0;
    }
    else if (aShip->m_right && aShip->m_left) { // if tree has both children, make its height tallest child + 1
        if (aShip->m_right->m_height > aShip->m_left->m_height) {
            aShip->m_height = aShip->m_right->m_height + 1;
        } else {
            aShip->m_height = aShip->m_left->m_height + 1;
        }
    }
    else if (aShip->m_right) { // NOT a leaf so update the height...
        aShip->m_height = aShip->m_right->m_height + 1;
    } else if (aShip->m_left) {
        aShip->m_height = aShip->m_left->m_height + 1;
    }
}

// Name: checkImbalance
// Desc: Checks if there is an imbalance at a given node (if height of its children differ by more than 1)
// Preconditions: object exists and is in fleet
// Postconditions: integer height difference between a node's children is returned
int Fleet::checkImbalance(Ship* aShip){
    // check if height of aShip differs from its left and right nodes' heights
    // by > 1.
    int leftChildHeight = -1;
    int rightChildHeight = -1;
    if (aShip->m_left != nullptr) {
        leftChildHeight = aShip->m_left->m_height;
    }
    if (aShip->m_right != nullptr) {
        rightChildHeight = aShip->m_right->m_height;
    }
    // if diff negative, right is taller than left child
    // if (abs val) this is > 1, there is imbalance
    return leftChildHeight - rightChildHeight;
}

// Name: rebalance
// Desc: given a ship, it will assess an imbalance and perform a shift to keep the tree efficient for traversal
// Preconditions: object exists and is passed to this function per an imbalance
// Postconditions: balance is restored to BST node
Ship* Fleet::rebalance(Ship* aShip){
    int balanceFactor = checkImbalance(aShip);
    // break into 4 rotation cases:
    if (balanceFactor > 1 && (checkImbalance(aShip->m_left) >= 0)) { // rotate single right
        return RightRotateHelp(aShip); // return new rotated root ptr
    }
    else if (balanceFactor > 1 && (checkImbalance(aShip->m_left) < 0)) {  // rotate single left
        aShip->m_left = LeftRotateHelp(aShip->m_left);
        return RightRotateHelp(aShip);
    }
    else if (balanceFactor < -1 && (checkImbalance(aShip->m_right) <= 0)) {
        return LeftRotateHelp(aShip);
    }
    else if (balanceFactor < -1 && (checkImbalance(aShip->m_right) > 0)) {  // rotate single left
        aShip->m_right = RightRotateHelp(aShip->m_right);
        return LeftRotateHelp(aShip);
    }
    return aShip;
}

// Name: RightRotateHelp
// Desc: given a ship, it will perform a shift to keep the tree efficient for traversal
// Preconditions: object exists and is passed to this function per an imbalance
// Postconditions: balance is restored to BST node
Ship *Fleet::RightRotateHelp(Ship *aShip) {
    // make x, y, z ptrs and perform shift on them & return new root
    Ship* x = aShip->m_left;
    Ship* z = x->m_right;

    x->m_right = aShip;
    aShip->m_left = z;

    updateHeight(aShip);
    updateHeight(x);
    return x;
}

// Name: LeftRotateHelp
// Desc: given a ship, it will perform a shift to keep the tree efficient for traversal
// Preconditions: object exists and is passed to this function per an imbalance
// Postconditions: balance is restored to BST node
Ship *Fleet::LeftRotateHelp(Ship *aShip) {
    // make y & z ptrs, perform shift on them & return new root
    // literally opposite of right rotate ...
    Ship* y = aShip->m_right;
    Ship* z = y->m_left;

    y->m_left = aShip;
    aShip->m_right = z;

    updateHeight(aShip);
    updateHeight(y);
    return y;
}

// Name: dumpTree
// Desc: given the root, it will output the fleet's tree hierarchy
// Preconditions: root exists and is passed
// Postconditions: tree structure is output
void Fleet::dumpTree() const {
    dump(m_root);
}

// Name: dump
// Desc: helper that will output the fleet's tree hierarchy
// Preconditions: root exists and is passed
// Postconditions: tree structure is output
void Fleet::dump(Ship* aShip) const{
    if (aShip != nullptr){
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}

// Name: listShips
// Desc: given the root, it will output the ships in order with details
// Preconditions: root exists
// Postconditions: tree details output in order
void Fleet::listShips() const {
    inOrder(m_root);
}

// Name: inOrder
// Desc: helper that will output the ships in order with details
// Preconditions: root exists
// Postconditions: tree details output in order
void Fleet::inOrder(Ship *pShip) const {
    if (pShip) {
        inOrder(pShip->m_left);
        cout << pShip->m_id << ":" << pShip->getStateStr() << ":" << pShip->getTypeStr() << endl;
        inOrder(pShip->m_right);
    }
}

// Name: setState
// Desc: given a ship ID and a STATE, it will update the ship's state to STATE
// Preconditions: ship exists
// Postconditions: the ship's STATE var is updated to the given STATE
bool Fleet::setState(int id, STATE state){
    // return true if set works out
    return setStateHelp(id, state, m_root);
}

// Name: setStateHelp
// Desc: Helper that will use root to update the ship's state to STATE
// Preconditions: ship exists
// Postconditions: the ship's STATE var is updated to the given STATE
bool Fleet::setStateHelp(int shipID, STATE state, Ship* aShip) {
    if (aShip == nullptr) {
        return false; // could not find ID
    }
    if (aShip->m_id == shipID) { // found ID
        aShip->setState(state);
        return true;
    }
    if (shipID < aShip->m_id) {
        return setStateHelp(shipID, state, aShip->m_left);
    }
    if (shipID > aShip->m_id){
        return setStateHelp(shipID, state, aShip->m_right);
    }
    return false;
}

// Name: removeLost
// Desc: given the root, it will search for ships with LOST state and remove them
// Preconditions: fleet exists
// Postconditions: the fleet has its lost ships removed
void Fleet::removeLost(){
    while (isLost(m_root)) {
        removeLostHelp(m_root);
    }
}

// Name: removeLostHelp
// Desc: Helper that will search for ships with LOST state and remove them
// Preconditions: fleet exists
// Postconditions: the fleet has its lost ships removed
void Fleet::removeLostHelp(Ship *aShip) {
    if (aShip) {
        removeLostHelp(aShip->m_left);
        removeLostHelp(aShip->m_right);
        if (aShip->getState() == LOST) {
            remove(aShip->m_id);
        }
    }
}

// Name: isLost
// Desc: looks for a ship with state LOST
// Preconditions: fleet exists
// Postconditions: t/f based on whether ship is found with lost state
bool Fleet::isLost(Ship* aShip){
    if (!aShip) {
        return false;
    }
    if (aShip->m_state == LOST) { // if curr is lost, return true
        return true;
    }
    if (isLost(aShip->m_left)) { // search to left for lost
        return true;
    }
    if (isLost(aShip->m_right)) { // search to right for lost
        return true;
    }
    return false;
}

// Name: findShip
// Desc: looks for a ship with given ID
// Preconditions: fleet exists
// Postconditions: t/f based on whether ship exists
bool Fleet::findShip(int id) const {
    // if we find it, return true; if ship doesn't exist, false
    return findHelper(m_root, id);
}

// Name: findHelper
// Desc: looks for a ship with given ID and the root
// Preconditions: fleet exists
// Postconditions: t/f based on whether ship exists
bool Fleet::findHelper(Ship* aShip, int shipID) const {
    if (aShip == nullptr) {
        return false; // could not find ID
    }
    if (aShip->m_id == shipID) { // found ID
        return true;
    }
    if (shipID < aShip->m_id) {
        return findHelper(aShip->m_left, shipID);
    }
    if (shipID > aShip->m_id){
        return findHelper(aShip->m_right, shipID);
    }
    return false;
}
