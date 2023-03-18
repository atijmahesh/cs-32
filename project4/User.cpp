#include "User.h"
using namespace std;
/*
 User constructor
 @param User's name,email,and watch history
 */
User::User(const string& full_name, const string& email,
           const vector<string>& watch_history) {
    name = full_name;
    this->email = email;
    watchHist = watch_history;
}

/*
 @return User's name
 */
string User::get_full_name() const {
    return name;
}

/*
 @return User's email
 */
string User::get_email() const {
    return email;
}

/*
 @return User's watch history
 */
vector<string> User::get_watch_history() const {
    return watchHist;
}
