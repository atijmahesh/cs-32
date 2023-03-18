#include "Set.h"
        #include <string>
        #include <iostream>
        #include <cassert>
        using namespace std;
int main() {
    Set ss;
    assert(ss.empty()); //checks if set is empty
    Set ss2(ss); //copy constructor check #1
    assert(ss2.empty()); //checks if copied set is empty
    ss.insert("alpha");
    ss.insert("beta");
    ss.insert("gamma");
    ss.insert("delta");
    ss.insert("epsilon");
    ss.insert("zeta");
    assert(!ss.insert("epsilon")); //check for duplicate insert
    assert(ss.size() == 6);  // check size and ensure duplicate not added
    string x;
    ss.get(0, x);
    assert(x == "alpha");  //testing get #1
    ss.get(2, x);
    assert(x == "delta");  // testing get #2
    ss.get(5, x);
    assert(x == "zeta");  // testing get #3
    assert(!ss.get(50, x)); //test bad argument into get
    assert(x == "zeta"); //x remains unchanged
    assert(!ss.erase("eta")); //testing erase when word isn't in it
    
    Set ss3; //inserting a bunch of values to be replaced
    ss3.insert("dummy val1");
    ss3.insert("dummy val2");
    ss3.insert("dummy val3");
    ss3.insert("dummy val4");
    ss3.insert("dummy val5");
    ss3.insert("dummy val6");
    ss3.insert("dummy val7");
    ss3.insert("dummy val8");
    ss3 = ss; //assignement operator on different sized sets
    assert(ss3.size() == 6); //double check size and contents
    ss3.get(0, x);
    assert(x == "alpha");  //testing get #1
    ss3.get(2, x);
    assert(x == "delta");  // testing get #2
    ss3.get(5, x);
    assert(x == "zeta");  // testing get #3
    assert(!ss3.contains("dummy val7")); //check if ss3 is truly equal to ss
    assert(!ss3.contains("dummy val8"));
    assert(ss3.contains("beta"));
    
    assert(ss.erase("alpha")); //check erase method
    assert(!ss.contains("alpha"));
    ss.get(0, x);
    assert(x == "beta"); //since alpha is now erased
    
    ss2.insert("one value");
    ss.swap(ss2); //test swap method and values
    assert(ss.size() == 1);
    assert(ss2.size() == 5);
    assert(!ss.contains("zeta")); //check if truly swapped
    assert(ss.contains("one value"));
    assert(ss2.contains("beta"));
    ss2.get(1, x);
    assert(x == "delta");
    
    //test unite
    Set result;
    unite(ss, ss2, result);
    assert(result.contains("one value"));
    assert(result.contains("epsilon"));
    assert(result.size() == 6);
    
    Set result2; //test result with very similar sets
    unite(ss2, ss3, result2);
    assert(result2.size() == 6);
    result2.get(0, x);
    assert(x == "alpha");
    
    //test butNot
    Set result3;
    butNot(ss3, ss2, result3);
    assert(result3.size() == 1);
    result3.get(0, x);
    assert(x == "alpha");
    
    //test destructor call
    ss.~Set();
    ss2.~Set();
    ss3.~Set();
    cerr << "All tests passed!" << endl;
}
