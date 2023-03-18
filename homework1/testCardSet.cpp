//
//  main.cpp
//  cardSet
//
//  Created by Atij Mahesh on 1/24/23.
//

#include <iostream>
#include <cassert>
#include "CardSet.h"

int main() {
    CardSet ss;
    ss.add(3242);
    ss.add(3535);
    ss.add(382471538);
    ss.add(4546);
    CardSet copy = ss;
    assert(ss.size() == 4);
    assert(ss.add(4547));
    assert(!ss.add(4546));
    
    //test copy constructor and assignment operator
    assert(copy.size() == 4);
    assert(copy.add(4547));
    assert(!copy.add(4546));
    CardSet copy2;
    copy2 = ss;
    assert(copy2.size() == 5);
    assert(copy2.add(5000));
    assert(!copy2.add(4546));
    
    
    std::cerr << "Test Cases Passed" << std::endl;
    ss.print();
}
