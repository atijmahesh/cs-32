//
//  Set.h
//  homework1
//
//  Created by Atij Mahesh on 1/19/23.
//

#ifndef Set_h
#define Set_h
#include <string>

const int DEFAULT_MAX_ITEMS = 160;
using ItemType = std::string;

class Set
{
  public:
    Set() {numItems = 0;}

    bool empty() const {return (size() == 0);}
    
    int size() const {return numItems;}   // Return the number of items in the set.

    bool insert(const ItemType& value);
      // Insert value into the set if it is not already present.  Return
      // true if the value is actually inserted.  Leave the set unchanged
      // and return false if value is not inserted (perhaps because it
      // was already in the set or because the set has a fixed capacity and
      // is full).

    bool erase(const ItemType& value);
      // Remove the value from the set if it is present.  Return true if the
      // value was removed; otherwise, leave the set unchanged and
      // return false.
     
    bool contains(const ItemType& value) const;
      // Return true if the value is in the set, otherwise false.
     
    bool get(int i, ItemType& value) const;
      // If 0 <= i < size(), copy into value the item in the set that is
      // strictly greater than exactly i items in the set and return true.
      // Otherwise, leave value unchanged and return false.

    void swap(Set& other);
      // Exchange the contents of this set with the other one.
    void dump() const;
private:
    int numItems;
    ItemType setArr[DEFAULT_MAX_ITEMS];
};

#endif
