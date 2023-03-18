void removeBad(list<Movie*>& li)
    {
        list<Movie*>::iterator it = li.begin();
        while(it != li.end()) {
            if((*it)->rating() < 50) {
                list<Movie*>::iterator toDelete = it;
                delete *toDelete;
                it = li.erase(it);
            }
            else it++;
        }
    }
