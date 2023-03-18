#include "UserDatabase.h"
#include "User.h"
using namespace std;

/*
 UserDatabase Constructor
 */
UserDatabase::UserDatabase() {
    beenCalled = false;
}

/*
 UserDatabase Destructor
 */
UserDatabase::~UserDatabase() {
    for (int i = 0; i < users.size(); i++) delete users[i];
}


/*
 Load method to load users.txt file
 @return if loading was succesfull
 @param users filename
 */
bool UserDatabase::load(const string& filename) {
    if (beenCalled) return false;
    //input filename
    ifstream infile(filename);
    if (!infile) return false;
    string line;
    string name, email;
    int numMovies;
    while (getline(infile, line)) {
        if (line.empty()) continue; //Skip empty lines
        //Get the user's name
        name = line;
        //Get the user's email
        getline(infile, line);
        email = line;
        //Get the number of movies for this user
        infile >> numMovies;
        infile.ignore(10000, '\n');
        // Get the user's movie IDs
        vector<string> movieIds;
        for (int i = 0; i < numMovies; ++i) {
            getline(infile, line);
            movieIds.push_back(line);
        }

        //Create the User object and insert into TMM
        User* user = new User(name, email, movieIds);
        users.push_back(user);
        map.insert(email, user);

        // Skip the empty line between users
        getline(infile, line);
    }
    beenCalled = true;
    return true;
}

/*
 Using TMM to get user from email
 @param user's email
 @output user or nullptr if not in database
 */
User* UserDatabase::get_user_from_email(const string& email) const {
    auto a = map.find(email);
    if (!a.is_valid()) return nullptr;
    return a.get_value();
}
