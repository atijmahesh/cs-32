#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include "treemm.h"

class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;
  private:
    TreeMultimap<std::string, User*> map;
    std::vector<User*> users; //users to delete later
    bool beenCalled;
};

#endif // USERDATABASE_INCLUDED
