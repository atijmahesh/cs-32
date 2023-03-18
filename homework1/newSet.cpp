#include "newSet.h"
#include <iostream>

Set::Set() {
    maxItems = DEFAULT_MAX_ITEMS;
    numItems = 0;
    setArr = new ItemType[DEFAULT_MAX_ITEMS];
}

Set::Set(int maxItems) {
    if(maxItems < 0) {
        std::cout << "Negative Input is not ideal." << std::endl;
        exit(0);
    }
    this->maxItems = maxItems;
    numItems = 0;
    setArr = new ItemType[maxItems];
}

Set::Set(const Set &copy) {
    numItems = copy.numItems;
    maxItems = copy.maxItems;
    setArr = new ItemType[maxItems];
    for (int i = 0; i < numItems; i++)
        setArr[i] = copy.setArr[i];
}

Set &Set::operator=(const Set &copy) {
    if (this != &copy) {
        Set temp = copy;
        this->swap(temp);
    }
        return *this;
}
Set::~Set() {
    delete [] setArr;
}





    bool Set::insert(const ItemType& value) {
        if(numItems == maxItems) return false;
        for(int i = 0; i<numItems; i++) {
            if(setArr[i] == value) {
                return false;
            }
        }
        setArr[numItems] = value;
        numItems++;
        return true;
    }


      // Insert value into the set if it is not already present.  Return
      // true if the value is actually inserted.  Leave the set unchanged
      // and return false if value is not inserted (perhaps because it
      // was already in the set or because the set has a fixed capacity and
      // is full).

bool Set::erase(const ItemType& value) {
    for (int i = 0; i<numItems; i++) {
        if (setArr[i] == value) {
            for (int j = i; j < numItems - 1; j++) {
                setArr[j] = setArr[j + 1];
            }
            numItems--;
            return true;
        }
    }
    return false;
}
      // Remove the value from the set if it is present.  Return true if the
      // value was removed; otherwise, leave the set unchanged and
      // return false.
     
bool Set::contains(const ItemType& value) const {
    for(int i = 0; i<numItems; i++) {
        if(setArr[i] == value) {
            return true;
        }
    }
    return false;
}
      // Return true if the value is in the set, otherwise false.
     
bool Set::get(int i, ItemType& value) const {
    if (i < 0 || i >= numItems) return false;
    ItemType *sortedArr = new ItemType[maxItems];
    for(int z = 0; z<numItems; z++) {
        sortedArr[z] = setArr[z];
    }
    for(int j = 0; j<numItems - 1; j++) {
        int minIndex = j;
        for(int k = j+1; k<numItems; k++) {
            if(sortedArr[k] < sortedArr[minIndex]) {
                minIndex = k;
            }
        }
        ItemType temp = sortedArr[minIndex];
        sortedArr[minIndex] = sortedArr[j];
        sortedArr[j] = temp;
    }
    
    value = sortedArr[i];
    delete [] sortedArr;
    return true;
}
      // If 0 <= i < size(), copy into value the item in the set that is
      // strictly greater than exactly i items in the set and return true.
      // Otherwise, leave value unchanged and return false.

void Set::swap(Set& other) {
    int tempSize = other.numItems;
        other.numItems = this->numItems;
        this->numItems = tempSize;
        int tempMax = other.maxItems;
        other.maxItems = this->maxItems;
        this->maxItems = tempMax;
        ItemType *tempArr = other.setArr;
        other.setArr = this->setArr;
        this->setArr = tempArr;
}
      // Exchange the contents of this set with the other one.

void Set::dump() const {
    for(int i = 0; i<numItems; i++) {
        std::cerr << setArr[i] << std::endl;
    }
}
