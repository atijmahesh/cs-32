#include "Movie.h"
using namespace std;

/*
 Movie Constructor
 */
Movie::Movie(const string& id, const string& title, const string& release_year,
             const vector<string>& directors, const vector<string>& actors,
             const vector<string>& genres, float rating) {
    this->id = id;
    this->title = title;
    this->release_year = release_year;
    this->directors = directors;
    this->actors = actors;
    this->genres = genres;
    this->rating = rating;
}


string Movie::get_id() const {
    return id;
}

string Movie::get_title() const {
    return title;
}

string Movie::get_release_year() const {
    return release_year;
}

float Movie::get_rating() const {
    return rating;
}

vector<string> Movie::get_directors() const {
    return directors;
}

vector<string> Movie::get_actors() const {
    return actors;
}

vector<string> Movie::get_genres() const {
    return genres;
}
