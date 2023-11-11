// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"

// Name: SQueue
// Desc: Constructs empty SQueue object
// Preconditions: None
// Postconditions: Creates empty SQueue object
SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType) {
    // all initialized properly?
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_heap = nullptr;
    m_size = 0;
}

// Name: ~SQueue
// Desc: Destroys SQueue object
// Preconditions: SQueue object instance exists
// Postconditions: Destroys SQueue object
SQueue::~SQueue() {
    clear();
    m_heap = nullptr;
    m_size = 0;

}

// Name: clear
// Desc: deallocates SQueue object memory
// Preconditions: SQueue object instance exists
// Postconditions: Destroys SQueue object
void SQueue::clear() {
    clearHelp(m_heap);
}

// Name: clearHelp
// Desc: Helper to deallocate SQueue object memory
// Preconditions: SQueue object instance exists
// Postconditions: Destroys SQueue object
void SQueue::clearHelp(Node *aNode) const {
    // post order traverse for delete
    if (aNode) {
        clearHelp(aNode->m_left);
        clearHelp(aNode->m_right);
        delete aNode;
    }
}


// Name: SQueue
// Desc: Constructs a deep copy of an SQueue object
// Preconditions: SQueue object instance exists to copy
// Postconditions: Creates deep copy SQueue object
SQueue::SQueue(const SQueue& rhs) {
    m_priorFunc = rhs.m_priorFunc;
    m_size = rhs.m_size;
    m_heapType = rhs.m_heapType;
    if (!rhs.m_heap) {
        m_heap = nullptr;
    } else {
        m_heap = copyHelper(rhs.m_heap);
    }
}

// Name: copyHelper
// Desc: Helper to do preorder traversal for tree copy
// Preconditions: SQueue object instance exists to copy
// Postconditions: Creates deep copy SQueue object
Node *SQueue::copyHelper(Node *pNode) {
    if (pNode) {
        Node* copyNode = new Node(pNode->getPost());
        copyNode->m_left = copyHelper(pNode->m_left);
        copyNode->m_right = copyHelper(pNode->m_right);
        return copyNode;
    }
    return pNode;
}


// Name: SQueue
// Desc: Assigns a deep copy of an SQueue object
// Preconditions: SQueue object instance exists to copy
// Postconditions: Creates deep copy SQueue object from rhs assignment
SQueue& SQueue::operator=(const SQueue& rhs) {
    if (this != &rhs) { // protect against self-merge!
        clear(); // clear current obj
        // set all vars
        m_priorFunc = rhs.m_priorFunc;
        m_size = rhs.m_size;
        m_heapType = rhs.m_heapType;
        if (!rhs.m_heap) {
            m_heap = nullptr;
        } else {
            m_heap = copyHelper(rhs.m_heap);
        }
    }
    return *this;
}

// Name: insertPost
// Desc: Adds a new post to the queue via merge operation
// Preconditions: SQueue object exists
// Postconditions: new post added to queue
void SQueue::insertPost(const Post& post) {
    // LOGARITHMIC TIME since merging is logarithmic
    if (!m_heap) {
        m_heap = new Node(post);
    } else {
        Node* newPost = new Node(post);
        m_heap = mergeHelper(m_heap, newPost);
    }
    m_size += 1; // don't need whole numPost method since we can easily do it in const. time here
}

// Name: getNextPost
// Desc: Returns the root post node
// Preconditions: SQueue object instance exists
// Postconditions: the root post (with highest priority) returned
Post SQueue::getNextPost() {
    // LOGARITHMIC TIME since merge is logarithmic
    // define our old root, grab its post, and break its children into new queues to merge
    if (m_size == 0) { // make sure we can actually perform a removal
        throw out_of_range("Cannot remove from an empty queue!");
    }
    Node* oldHeap = m_heap;
    Post myPost = m_heap->getPost();
    m_heap = mergeHelper(m_heap->m_left, m_heap->m_right);
    // once children merged, we can effectively remove old root and decrement size
    delete oldHeap;
    m_size -= 1;
    return myPost;
}

// Name: mergeWithQueue
// Desc: Merges two queue objects
// Preconditions: SQueue objects both exist
// Postconditions: Empties rhs queue to merge into a new queue
void SQueue::mergeWithQueue(SQueue& rhs) {
    if (this != &rhs) { // protect against self-merge!
        if (m_priorFunc == rhs.m_priorFunc) { // only perform merge with same priorFuncs

            while (rhs.m_heap != nullptr) { // since getNext deletes root, this effectively also destroys rhs
                insertPost(rhs.getNextPost());
            }
        } else {
            throw domain_error("Cannot merge queues of different priority functions!");
        }
    }
}

// Name: mergeHelper
// Desc: The whole merge operation to construct queues properly
// Preconditions: SQueue object instance exists and insert or remove called
// Postconditions: Shifts the nodes of the tree in accordance with heap property
Node* SQueue::mergeHelper(Node *p1, Node *p2) {
    if (!p1) { // if one of the queues doesn't exist, return the existing one
        return p2;
    } else if (!p2) {
        return p1;
    }
    if (m_heapType == MINHEAP) {
        // MIN HEAP: make sure p1 has higher priority (smaller val)
        if (m_priorFunc(p1->getPost()) > m_priorFunc(p2->getPost())) {
            // perform swap of p1 with p2 if p1 does have lower priority
            swap(p1, p2);
        }
        // swap p1's left and right subtrees
        swap(p1->m_left, p1->m_right);

        // now recursively merge p2 w/ left of p1 to be new left of p1
        p1->m_left = mergeHelper(p2, p1->m_left);

    } else if (m_heapType == MAXHEAP) {
        // MAX HEAP: make sure p1 has higher priority (larger val)
        if (m_priorFunc(p1->getPost()) < m_priorFunc(p2->getPost())) {
            // perform swap of p1 with p2 if p1 does have lower priority
            swap(p1, p2);
        }
        // swap p1's left and right subtrees
        swap(p1->m_left, p1->m_right);

        // now recursively merge p2 w/ left of p1 to be new left of p1
        p1->m_left = mergeHelper(p2, p1->m_left);
    }
    // return newly merged heap
    return p1;
}

// Name: numPosts
// Desc: returns number of posts/nodes in tree
// Preconditions: SQueue object exists
// Postconditions: returns num posts
int SQueue::numPosts() const {
    // I think we should traverse entire tree and update size for every node ...
    // int sizeCheck = 0;
    // sizeCheck = numPostHelp(m_heap);
    return m_size;
}

// Name: printPostsQueue
// Desc: uses preorder traversal to print node details
// Preconditions: SQueue object instance exists
// Postconditions: prints node details
void SQueue::printPostsQueue() const {
    printPostsQueueHelp(m_heap);
}

// Name: printPostsQueue
// Desc: helper uses preorder traversal to print node details
// Preconditions: SQueue object instance exists
// Postconditions: prints node details
void SQueue::printPostsQueueHelp(Node *pNode) const {
    if (pNode) {
        cout << "[" << m_priorFunc(pNode->getPost()) << "]" << pNode->getPost() << endl;
        printPostsQueueHelp(pNode->m_left);
        printPostsQueueHelp(pNode->m_right);
    }
}

// Name: getPriorityFn
// Desc: returns prior. func.
// Preconditions: SQueue object instance exists
// Postconditions: returns the priority function
prifn_t SQueue::getPriorityFn() const {
    return m_priorFunc;
}

// Name: setPriorityFn
// Desc: Switches the queue functionality to that specified and rebuilds queue
// Preconditions: SQueue object instance exists
// Postconditions: switches priority function and rebuilds heap accordingly
void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    // rebuild heap...?
    // could we be clever enough to create a new queue (empty) and merge with old one, but now with diff. priorities?
    // reset the priority and heap type (Min to Max or Max to Min)
    m_priorFunc = priFn;
    m_heapType = heapType;
    int size = m_size; // save original size for destroying and rebuilding heap
    Post postArr[size]; // hold all elements to use for rebuild
    // let's take every post off of our current heap
    for (int i = 0; i < size; i++) {
        postArr[i] = getNextPost();
    }
    for (int i = 0; i < size; i++) {
        insertPost(postArr[i]); // should set up new heap but with new priority
    }
}

// Name: dump()
// Desc: uses inorder traversal to print node details
// Preconditions: SQueue object instance exists
// Postconditions: prints node details
void SQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }
}

// Name: dump
// Desc: helper uses preorder traversal to print node details
// Preconditions: SQueue object instance exists
// Postconditions: prints node details
void SQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_post) << ":" << pos->m_post.getPostID();
    dump(pos->m_right);
    cout << ")";
  }
}

// Name: overloaded <<
// Desc: allows for cout of post details
// Preconditions: post object instance exists
// Postconditions: prints post details
ostream& operator<<(ostream& sout, const Post& post) {
  sout << "Post ID: " << post.getPostID() 
        << ", number of likes: " << post.getNumLikes()
        << ", interest level: " << post.getInterestLevel() 
        << ", connection level: " << post.getConnectLevel()
        << ", post time: " << post.getPostTime();
  return sout;
}

// Name: overloaded <<
// Desc: allows for cout of Node details
// Preconditions: Node object instance exists
// Postconditions: prints node details
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPost();
  return sout;
}
