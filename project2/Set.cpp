// Set.cpp

#include "Set.h"

/**
 Set constructor
 Sets head to nullptr
 */
Set::Set() {
    head = nullptr;
}
/**
 Set destructor
 Deletes all nodes in Set
 */
Set::~Set() {
    if(!empty()) {
        Node *p = head;
        while(p != nullptr) {
            Node* n = p->next;
            delete p;
            p = n;
        }
    }
    head = nullptr;
}

/**
 Set Copy constructor
 Copies values of Nodes from one head to another during object construction
 */
Set::Set(const Set& copy){
    head = nullptr;
    Node* t = copy.head;
    while(t != nullptr) {
        insert(t->value);
        t = t->next;
    }
}

/**
 Set assignment operator
 Swaps in values from this Set into the other set
 */
Set& Set::operator=(const Set& copy){
    if(this == &copy) return *this;
    Set temp(copy);
    swap(temp);
    return *this;
}

/**
 Checks to see if Set contains an item
 Returns true if it does, false if it does not
 */
bool Set::contains(const ItemType& value) const
{
    Node* t = head;
    while(t != nullptr) {
        if(t->value == value) {
            return true;
        }
        t = t->next;
    }
    return false;
}

/**
 Insert item into set, returns true if the item is inserted,
 false if the item is already inside it
 Inserts in a sorted order
 */
bool Set::insert(const ItemType& value)
{
    if(contains(value)) return false;
    if(head == nullptr) {
        Node* t = new Node;
        t->value = value;
        head = t;
    }
    else if(value <= head->value) {
        Node* t;
        t = new Node;
        t->value = value;
        t->next = head;
        head->prev = t;
        head = t;
    }
    else {
        Node* p = head;
        while(p->next != nullptr && p->next->value < value) {
            p = p->next;
        }
        Node* insertedNode = new Node;
        insertedNode->value = value;
        insertedNode->next = p->next;
        insertedNode->prev = p;
        if(p->next != nullptr)
            p->next->prev = insertedNode;
        p->next = insertedNode;
    }
    return true;
}

/**
 Erase the specified value from the set
 Returns true if the item was succesfully erased,
 false if the item isn't in the set
 */
bool Set::erase(const ItemType& value)
{
    if(head == nullptr) return false;
    if(head->value == value) {
        Node* toDelete = head;
        head = toDelete->next;
        delete toDelete;
        return true;
    }
    Node* curr = head;
    while(curr !=nullptr) {
        if(curr->next != nullptr && curr->next->value == value)
            break;
        curr = curr->next;
    }
    if(curr !=nullptr) {
        Node* toDelete = curr->next;
        curr->next = toDelete->next;
        delete toDelete;
        return true;
    }
    return false;
}

/**
 Get the item at the specified index, since the linkedlist is already sorted, and copies into
 value the accessed value
 Returns false if i is out of range
 */
bool Set::get(int i, ItemType& value) const
{
    if(i<0 || i>=size()) return false;
    Node* curr = head;
    while(i) {
        curr = curr->next;
        i--;
    }
    value = curr->value;
    return true;
}

/**
 Swap method swaps the values of one linkedlist with another
 */
void Set::swap(Set& other)
{
    Node* t = head;
    head = other.head;
    other.head = t;
}

/**
 Returns size of linkedlist
 */
int Set::size() const
{
    Node* p = head;
    int size = 0;
    while(p != nullptr) {
        p = p->next;
        size++;
    }
    return size;
}

/**
 Checks if linkedlist is empty
 */
bool Set::empty() const
{
    return size() == 0;
}

/**
 Combines two Sets in sorted order
 */
void unite(const Set& s1, const Set& s2, Set& result) {
    if(s1.empty()) {
        result = s2;
        return;
    }
    else if(s2.empty()) {
        result = s1;
        return;
    }
    int maxOutputSize = s1.size() + s2.size();
    Set output;
    ItemType insertedVal;
    for(int i = 0; i<maxOutputSize; i++) {
        if(s1.get(i, insertedVal)) {
            output.insert(insertedVal);
        }
        if(s2.get(i, insertedVal)) {
            output.insert(insertedVal);
        }
    }
    result = output;
}

/**
 Returns a Set that contains the values in s1 that are not present in s2
 */
void butNot(const Set& s1, const Set& s2, Set& result) {
    if(s2.empty()) {
        result = s1;
        return;
    }
    Set output;
    ItemType comparVal;
    for(int i = 0; i<s1.size(); i++) {
        s1.get(i, comparVal);
        if(!s2.contains(comparVal)) {
            output.insert(comparVal);
        }
        else continue;
    }
    result = output;
}
