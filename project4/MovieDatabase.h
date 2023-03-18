#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "treemm.h"

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    TreeMultimap<std::string, Movie*> idMap;
    TreeMultimap<std::string, Movie*> dirMap;
    TreeMultimap<std::string, Movie*> actMap;
    TreeMultimap<std::string, Movie*> genreMap;
    std::vector<std::string> split_line(const std::string& line, char delimiter);
    std::vector<Movie*> movies; //delete in destructor
    bool beenCalled;
};

#endif // MOVIEDATABASE_INCLUDED
