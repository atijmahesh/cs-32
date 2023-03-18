void removeOdd(list<int>& li)
    {
        list<int>::iterator it;
        it = li.begin();
        while(it != li.end()) {
            if(*it%2 == 1) {
                it = li.erase(it);
            }
            else it++;
        }
    }
