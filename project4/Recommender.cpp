#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"
using namespace std;

/*
 Recommender constructor
 */
Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database) {
    udb = &user_database;
    mdb = &movie_database;
}
/*
 Recommend movies function that recommends the user a list of movies based on their watch history
 */
vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const {
    User* u = udb->get_user_from_email(user_email);
    if (u == nullptr) return vector<MovieAndRank>();
    //create an unordered map of Movie pointers and their compatability scores
    unordered_map<Movie*, int> scoreMap;
    const vector<string>& userWatchHist = u->get_watch_history();
    vector<Movie*> moviesWdirector, moviesWActor, moviesWGenre;
    //pre-compute the movies associated with directors, actors, and genres
    for (const string& movie_id : userWatchHist) {
        //movie to focus on
        const Movie* movie = mdb->get_movie_from_id(movie_id);
        //add points to movies with the same directors
        const vector<string>& dirVec = movie->get_directors();
        for (const string& dir : dirVec) {
            const vector<Movie*>& temp = mdb->get_movies_with_director(dir);
            for (int k = 0; k < temp.size(); k++){
                scoreMap[temp[k]] += 20;
            }
        }
        //add points to movies with the same actors
        const vector<string>& actVec = movie->get_actors();
        for (const string& act : actVec) {
            const vector<Movie*>& temp = mdb->get_movies_with_actor(act);
            for (int k = 0; k < temp.size(); k++){
                scoreMap[temp[k]] += 30;
            }
        }
        //add points to movies with the same genre
        const vector<string>& genreVec = movie->get_genres();
        for (const string& genre : genreVec) {
            const vector<Movie*>& temp = mdb->get_movies_with_genre(genre);
            for (int k = 0; k < temp.size(); k++){
                scoreMap[temp[k]] += 1;
            }
        }
    }
    
    //fill up reccomend with compatible movies and ones the user hasn't watched
    vector<MovieAndRank> recommend;
    for(auto itr = scoreMap.begin(); itr != scoreMap.end(); itr++) {
        MovieAndRank m = MovieAndRank(itr->first->get_id(), itr->second);
        bool userWatched = false;
        for(int i = 0; i<userWatchHist.size(); i++) {
            if(userWatchHist[i] == itr->first->get_id())
                userWatched = true;
        }
        if(!userWatched) recommend.push_back(m);
    }
    
    //sort recommend using a lambda function
    sort(recommend.begin(), recommend.end(), [this](const MovieAndRank& one, const MovieAndRank& two) {
        //sort by compatibility score
        if (one.compatibility_score != two.compatibility_score) return one.compatibility_score > two.compatibility_score;
        //sort by 5-star rating and movie name
        const Movie* movieOne = mdb->get_movie_from_id(one.movie_id);
        const Movie* movieTwo = mdb->get_movie_from_id(two.movie_id);
        return (movieOne->get_rating() > movieTwo->get_rating()) ||
               (movieOne->get_rating() == movieTwo->get_rating() && movieOne->get_title() < movieTwo->get_title());
    });
    //return up to movie_count recommend
    vector<MovieAndRank> result;
    for (int i = 0; i < recommend.size() && i < movie_count; i++) {
        result.push_back(recommend[i]);
    }
    return result;
}
