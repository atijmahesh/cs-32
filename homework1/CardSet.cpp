//
//  CardSet.cpp
//  homework1
//
//  Created by Atij Mahesh on 1/23/23.
//

#include "CardSet.h"
#include <iostream>

CardSet::CardSet() {}

bool CardSet::add(ItemType cardNumber) {return m_set.insert(cardNumber);}

int CardSet::size() const {return m_set.size();}

void CardSet::print() const {
    for (int i = 0; i < m_set.size(); i++) {
        ItemType assignedItem;
        m_set.get(i, assignedItem);
        std::cout << assignedItem << std::endl;
    }
}
