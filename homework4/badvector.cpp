void removeBad(vector<Movie*>& v)
    {
        for(int i = 0; i<v.size(); i++) {
            if(v[i]->rating() < 50) {
                Movie* temp = v[i];
                delete temp;
                v.erase(v.begin()+i);
                i--;
            }
        }
    }
