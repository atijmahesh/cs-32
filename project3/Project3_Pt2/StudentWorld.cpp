#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}
/*
StudentWorld Constructor, also initalizes GameWorld
@param assetPath to image folder
*/
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}

/*
Remove a square at a position
@param pixel values of square to be removed
*/
void StudentWorld::removeSquareAt(double x, double y) {
    for(int i = 0; i<container.size(); i++) {
        if(container[i]->getX() == x && container[i]->getY() == y && container[i]->isSquare()) {
            container[i]->setAlive(false);
        }
    }
}
/*
Check if there is a player on the square
@param pixel values of square to be searched
@return 0 if no player, 1 if Peach, two if Yoshi, 3 if Both
*/
int StudentWorld::isPlayerOnSquare(double x, double y) {
    if(m_Peach->getX() == x && m_Peach->getY() == y) {
        if(m_Yoshi->getX() == x && m_Yoshi->getY() == y)
            return 3;
        return 1;
    }
    else if(m_Yoshi->getX() == x && m_Yoshi->getY() == y) return 2;
    return 0;
}

/*
Check if there is a vortex and baddie overlap
@param Vortex to compare to
@return baddie that vortex hits or nullptr
*/
Actor* StudentWorld::impactableOverlap(Actor* a) {
    for(int i = 0; i<container.size(); i++) {
        if(a->complexOverlap(container[i]) && container[i]->isImpactable()) {
            return container[i];
        }
    }
    return nullptr;
}

/*
Check if there is a directionalSquare at the position
@param pixel values of square to be searched, and dir of directionalSquare
@return true if found, false if no square
*/
bool StudentWorld::isDirectionalSquareAtPosition(double x, double y, int& dir) {
    for(Actor* a:container) {
        if(a->getX() == x && a->getY() == y) {
            if(a->isDirSqr()) {
                dir = a->getDirection();
                return true;
            }
        }
    }
    return false;
}

/*
Check if the current square is a forksquare
@param pixel values of square to be checked, current direction, and potential fork directions
@return true if the square is a fork square
*/
bool StudentWorld::forkSquare(double x, double y, int curDir, int& forkDir1, int& forkDir2) {
    int forkCount = 0;
    forkDir1 = -1;
    forkDir2 = -1;
    bool isWholeSquare = (static_cast<int>(x)%16 == 0) && (static_cast<int>(y)%16 == 0);
    int prevSqrDir = (curDir + 180) % 360;
    if(!squareInDirection(x, y, prevSqrDir)) return false;
    
    if(squareInDirection(x, y, 0) && prevSqrDir != 0) {
        forkCount++;
        forkDir1 = 0;
    }
    if(squareInDirection(x, y, 180) && prevSqrDir != 180) {
        forkCount++;
        if(forkDir1 == -1) forkDir1 = 180;
        else forkDir2 = 180;
    }
    if(squareInDirection(x, y, 90) && prevSqrDir != 90) {
        forkCount++;
        if(forkDir1 == -1) forkDir1 = 90;
        else forkDir2 = 90;
    }
    if(squareInDirection(x, y, 270) && prevSqrDir != 270) {
        forkCount++;
        if(forkDir1 == -1) forkDir1 = 270;
        else forkDir2 = 270;
    }
    return (forkCount == 2 && (forkDir1 != -1) && (forkDir2 != -1) && isWholeSquare);
}
/*
Check if there is a square at the position on the board
@param integers that correspond to grids on board
@return true if there is a square
*/
bool StudentWorld::isSquare(int x, int y) {
    return (bd.getContentsOf(x, y) != Board::empty);
}

/*
Check if there is a square in a certain direction
@param pixel values of square to be searched and direction
@return true if there is a square in the direction
*/
bool StudentWorld::squareInDirection(double x, double y, double d) {
    //change pixel values to grid values
    x/=16;
    y/=16;
    if(d == 0) {
        return isSquare(x+1, y);
    }
    else if(d == 90) {
        return isSquare(x, y+1);
    }
    else if(d == 180) {
        //round up for negative directions
        x = std::ceil(x);
        return isSquare(x-1, y);
    }
    else if(d == 270) {
        //round up for negative directions
        y = std::ceil(y);
        return isSquare(x, y-1);
    }
    return false;
}

/*
 Initialize StudentWorld
 */
int StudentWorld::init() {
    //setup Board
    std:string boardName;
    boardPath = assetPath()+ "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(boardPath);
    if (result == Board::load_fail_file_not_found || result == Board::load_fail_bad_format) {
        return GWSTATUS_BOARD_ERROR;
    }
    //add actors
    for(int x = 0; x<BOARD_WIDTH; x++) {
        for(int y = 0; y<BOARD_HEIGHT; y++) {
            Board::GridEntry pt = bd.getContentsOf(x,y);
            switch (pt) {
                case Board::player:
                    addPeach(x, y);
                    addYoshi(x, y);
                    break;
                case Board::blue_coin_square:
                    addBlueCoinSquare(x, y);
                    break;
                case Board::red_coin_square:
                    addRedCoinSquare(x, y);
                    break;
                case Board::up_dir_square:
                    addDirectionalSquare(x, y, 90);
                    break;
                case Board::down_dir_square:
                    addDirectionalSquare(x, y, 270);
                    break;
                case Board::left_dir_square:
                    addDirectionalSquare(x, y, 180);
                    break;
                case Board::right_dir_square:
                    addDirectionalSquare(x, y, 0);
                    break;
                case Board::event_square:
                    addEventSquare(x, y);
                    break;
                case Board::bank_square:
                    addBankSquare(x, y);
                    break;
                case Board::star_square:
                    addStarSquare(x, y);
                    break;
                case Board::boo:
                    addBoo(x, y);
                    break;
                case Board::bowser:
                    addBowser(x, y);
                    break;
                default:
                    break;
            }
        }
    }
	startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

/*
 Move Actors in StudentWorld
 */
int StudentWorld::move() {
    if(timeRemaining() <= 0) { //game is over and declare winner
        playSound(SOUND_GAME_FINISHED);
        int gameWinner = -1; //1 if peach, two if yoshi
        if(m_Peach->getStars() > m_Yoshi->getStars()) {
            gameWinner = 1;
        }
        else if(m_Peach->getStars() == m_Yoshi->getStars()) {
            if(m_Peach->getCoins() > m_Yoshi->getCoins()) gameWinner = 1;
            else if(m_Peach->getCoins() < m_Yoshi->getCoins()) gameWinner = 2;
            else gameWinner = randInt(1, 2);
        }
        else gameWinner = 2;
        if(gameWinner == 1) {
            setFinalScore(m_Peach->getStars(), m_Peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else {
            setFinalScore(m_Yoshi->getStars(), m_Yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
    }
    //have all actors dosomething
    m_Peach->doSomething();
    m_Yoshi->doSomething();
    for(Actor* a:container) {
        a->doSomething();
    }
    //display game text
    int p1_roll = m_Peach->getRoll();
    int p1_stars = m_Peach->getStars();
    int p1_money = m_Peach->getCoins();
    bool p1_has_vortex = m_Peach->hasVortex();
    int time = timeRemaining();
    int bank = bankVal;
    int p2_roll = m_Yoshi->getRoll();
    int p2_stars = m_Yoshi->getStars();
    int p2_money = m_Yoshi->getCoins();
    bool p2_has_vortex = m_Yoshi->hasVortex();
    
    //remove dead actors
    for(int i = 0; i<container.size(); i++) {
        if(!container[i]->alive()) {
            delete container[i];
            container.erase(container.begin()+i);
            i--;
        }
    }
    std::ostringstream oss;
    //print game text
    oss << "P1 Roll: " << p1_roll << " Stars: " << p1_stars << " $$: " << p1_money;
    if (p1_has_vortex) {
        oss << " VOR | ";
    } else {
        oss << " | ";
    }
    oss  << "Time: " << time << " | Bank: " << bank << " | P2 Roll: " << p2_roll << " Stars: " << p2_stars << " $$: " << p2_money;
    if (p2_has_vortex) {
        oss << " VOR";
    }
    string output = oss.str();
    setGameStatText(output);
	return GWSTATUS_CONTINUE_GAME;
}

//deallocate objects in StudentWorld
StudentWorld::~StudentWorld() {
    cleanUp();
}
//delete all actors in StudentWorld
void StudentWorld::cleanUp() {
    for (Actor* a:container)
        delete a;
    container.clear();
    if(m_Peach) delete m_Peach;
    m_Peach = nullptr;
    if(m_Yoshi) delete m_Yoshi;
    m_Yoshi = nullptr;
}

void StudentWorld::addPeach(double x, double y) {
    m_Peach = new Peach(this, x, y);
    container.push_back(new BlueCoinSquare(this, x, y));
}
void StudentWorld::addYoshi(double x, double y) {
    m_Yoshi = new Yoshi(this, x, y);
}
void StudentWorld::addBlueCoinSquare(double x, double y) {
    container.push_back(new BlueCoinSquare(this, x, y));
}
void StudentWorld::addRedCoinSquare(double x, double y) {
    container.push_back(new RedCoinSquare(this, x, y));
}
void StudentWorld::addBowser(double x, double y) {
    container.push_back(new Bowser(this, x, y));
    container.push_back(new BlueCoinSquare(this, x, y));
}
void StudentWorld::addBoo(double x, double y) {
    container.push_back(new Boo(this, x, y));
    container.push_back(new BlueCoinSquare(this, x, y));
}
void StudentWorld::addDirectionalSquare(double x, double y, int direction) {
    container.push_back(new DirectionalSquare(this, x, y, direction));
}
void StudentWorld::addStarSquare(double x, double y) {
    container.push_back(new StarSquare(this, x, y));
}
void StudentWorld::addBankSquare(double x, double y) {
    container.push_back(new BankSquare(this, x, y));
}
void StudentWorld::addEventSquare(double x, double y) {
    container.push_back(new EventSquare(this, x, y));
}
void StudentWorld::addDroppingSquare(double x, double y) {
    x/=16;
    y/=16;
    container.push_back(new DroppingSquare(this, x, y));
}
void StudentWorld::addVortex(double x, double y, int d) {
    x/=16;
    y/=16;
    container.push_back(new Vortex(this, x, y, d));
}
