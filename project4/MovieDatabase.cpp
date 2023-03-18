#include "MovieDatabase.h"
#include "Movie.h"

using namespace std;

/*
 MovieDatabase constructor
 */
MovieDatabase::MovieDatabase() {
    beenCalled = false;
}
/*
 MovieDatabase destructor
 */
MovieDatabase::~MovieDatabase() {
    for (int i = 0; i < movies.size(); i++) delete movies[i];
}

/*
 Load a movie text file
 When adding movies to a map, all of the Movie's data members are
 converted to uppercase letters
 @param movie text filename
 @return bool if the file could be loaded
 */
bool MovieDatabase::load(const string& filename) {
    //Open the input file
    ifstream input(filename);
    if (!input || beenCalled) {
        return false;
    }
    beenCalled = true;

    //Read the movie records from the input file
    string line;
    while (getline(input, line)) {
        //Read the movie ID
        string id = line;
        //Read the movie title
        getline(input, line);
        string title = line;
        //Read the movie release year
        getline(input, line);
        string release_year = line;
        //Read the movie directors
        getline(input, line);
        vector<string> directors = split_line(line, ',');
        //Read the movie actors
        getline(input, line);
        vector<string> actors = split_line(line, ',');
        //Read the movie genres
        getline(input, line);
        vector<string> genres = split_line(line, ',');
        //Read the movie rating
        getline(input, line);
        float rating = stof(line);
        
        //Create a new Movie object
        Movie* movie = new Movie(id, title, release_year, directors, actors, genres, rating);
        movies.push_back(movie);
        //Add the movie to the ID map
        transform(id.begin(), id.end(), id.begin(), ::toupper); //only uppercase values in map
        idMap.insert(id, movie);

        //Add the movie to the director map
        for (string& director : directors) {
            transform(director.begin(), director.end(), director.begin(), ::toupper);
            dirMap.insert(director, movie);
        }
        //Add the movie to the actor map
        for (string& actor : actors) {
            transform(actor.begin(), actor.end(), actor.begin(), ::toupper);
            actMap.insert(actor, movie);
        }

        //Add the movie to the genre map
        for (string& genre : genres) {
            transform(genre.begin(), genre.end(), genre.begin(), ::toupper);
            genreMap.insert(genre, movie);
        }
        //Skip the empty line between movie records
        getline(input, line);
    }
    //Close input file
    input.close();
    return true;
}

/*
 Split line function splits a string into a vector using delimiters
 */
vector<string> MovieDatabase::split_line(const string& line, char delimiter) {
    vector<string> result;
    string item;
    istringstream iss(line);
    while (getline(iss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

//Converts input to upper case letters beforehand
Movie* MovieDatabase::get_movie_from_id(const string& id) const {
    string tempId = "";
    for (char c:id) tempId += toupper(c);
    if (!idMap.find(tempId).is_valid()) return nullptr;
    return idMap.find(tempId).get_value();
}

//Converts input to upper case letters beforehand
vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string tempDir = "";
    for (char c:director) tempDir += toupper(c);
    vector<Movie*> m;
    TreeMultimap<string, Movie*>::Iterator it = dirMap.find(tempDir);
    while (it.is_valid()){
        m.push_back(it.get_value());
        it.advance();
    }
    return m;
}

//Converts input to upper case letters beforehand
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string tempAct = "";
    for (char c:actor) tempAct += toupper(c);
    vector<Movie*> m;
    TreeMultimap<string, Movie*>::Iterator it = actMap.find(tempAct);
    while (it.is_valid()){
        m.push_back(it.get_value());
        it.advance();
    }
    return m;
}

//Converts input to upper case letters beforehand
vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string tempGenre = "";
    for (char c:genre) tempGenre += toupper(c);
    vector<Movie*> m;
    TreeMultimap<string, Movie*>::Iterator it = genreMap.find(tempGenre);
    int count = 0;
    while (it.is_valid()){
        count++;
        m.push_back(it.get_value());
        it.advance();
    }
    return m;
}

