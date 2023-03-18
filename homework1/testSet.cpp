#include "Set.h"
    #include <iostream>
    #include <string>
    #include <cassert>
    using namespace std;

int main() {
    Set ss;
    assert(ss.empty());
        ss.insert("lavash");
        ss.insert("roti");
        ss.insert("chapati");
        ss.insert("injera");
        ss.insert("roti");
        ss.insert("matzo");
        ss.insert("injera");
        assert(ss.size() == 5);  // duplicate "roti" and "injera" were not added
        string x;
        ss.get(0, x);
        assert(x == "chapati");  // "chapati" is greater than exactly 0 items in ss
        ss.get(4, x);
        assert(x == "roti");  // "roti" is greater than exactly 4 items in ss
        ss.get(2, x);
        assert(x == "lavash");  // "lavash" is greater than exactly 2 items in ss
        assert(!ss.erase("tortilla"));
    
        //test copy constructor and assignment operator
    Set copy = ss;
    assert(copy.size() == 5);  // duplicate "roti" and "injera" were not added
    copy.get(0, x);
    assert(x == "chapati");  // "chapati" is greater than exactly 0 items in ss
    copy.get(4, x);
    assert(x == "roti");  // "roti" is greater than exactly 4 items in ss
    copy.get(2, x);
    assert(x == "lavash");  // "lavash" is greater than exactly 2 items in ss
    assert(!copy.erase("tortilla"));
    
    Set copy2;
    copy2 = ss;
    assert(copy2.size() == 5);  // duplicate "roti" and "injera" were not added
    copy2.get(0, x);
    assert(x == "chapati");  // "chapati" is greater than exactly 0 items in ss
    copy2.get(4, x);
    assert(x == "roti");  // "roti" is greater than exactly 4 items in ss
    copy2.get(2, x);
    assert(x == "lavash");  // "lavash" is greater than exactly 2 items in ss
    assert(!copy2.erase("tortilla"));

    
    
        ss.erase("chapati");
        assert(!ss.contains("chapati"));
    assert(ss.contains("roti"));
        Set ss1;
        ss1.insert("laobing");
        Set ss2;
        ss2.insert("matzo");
        ss2.insert("pita");
        ss1.swap(ss2);
        assert(ss1.size() == 2  &&  ss1.contains("matzo")  &&  ss1.contains("pita")  &&
               ss2.size() == 1  &&  ss2.contains("laobing"));
    cerr << "All tests passed!" << endl;
    }
