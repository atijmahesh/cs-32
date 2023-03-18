#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}



//return 0 if no player, 1 if Peach, two if Yoshi, 3 if Both
int StudentWorld::isPlayerOnSquare(double x, double y) {
    if(m_Peach->getX() == x && m_Peach->getY() == y) {
        if(m_Yoshi->getX() == x && m_Yoshi->getY() == y)
            return 3;
        return 1;
    }
    else if(m_Yoshi->getX() == x && m_Yoshi->getY() == y) return 2;
    return 0;
}


bool StudentWorld::isDirectionalSquareAtPosition(double x, double y, int& dir) {
    for(Actor* a:container) {
        if(a->getX() == x && a->getY() == y) {
            if(a->getType() == DIRSQRTYPE) {
                dir = a->getDirection();
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::forkSquare(double x, double y, int curDir, int& forkDir1, int& forkDir2) {
    curDir += 180;
    if(curDir >= 360) curDir -= 360;
    if(isSquare(x+1, y) && isSquare(x, y-1) && (curDir!=0 || curDir !=270)) {
        forkDir1 = 0;
        forkDir2 = 270;
        return true;
    }
    else if(isSquare(x+1, y) && isSquare(x, y+1) && (curDir!=0 || curDir !=90)) {
        forkDir1 = 0;
        forkDir2 = 90;
        return true;
    }
    else if(isSquare(x-1, y) && isSquare(x, y-1)  && (curDir!=180 || curDir !=270)) {
        forkDir1 = 180;
        forkDir2 = 270;
        return true;
    }
    else if(isSquare(x-1, y) && isSquare(x, y+1)  && (curDir!=180 || curDir !=90)) {
        forkDir1 = 180;
        forkDir2 = 90;
        return true;
    }
    return false;
}

bool StudentWorld::isSquare(int x, int y) {
    return (bd.getContentsOf(x, y) != Board::empty);
}

bool StudentWorld::squareInDirection(double x, double y, double d) {
    x/=16;
    y/=16;
    if(d == 0) {
        return isSquare(x+1, y);
    }
    else if(d == 90) {
        return isSquare(x, y+1);
    }
    else if(d == 180) {
        x = std::ceil(x);
        y = std::ceil(y);
        return isSquare(x-1, y);
    }
    else if(d == 270) {
        x = std::ceil(x);
        y = std::ceil(y);
        return isSquare(x, y-1);
    }
    return false;
}



int StudentWorld::init() {
    std:string boardName;
    boardPath = assetPath()+ "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(boardPath);
    if (result == Board::load_fail_file_not_found || result == Board::load_fail_bad_format) {
        return GWSTATUS_BOARD_ERROR;
    }
    for(int x = 0; x<16; x++) {
        for(int y = 0; y<16; y++) {
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

int StudentWorld::move() {
    if(timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        int gameWinner = -1; //1 if peach, two if yoshi
        if(m_Peach->getStars() > m_Yoshi->getStars()) {
            gameWinner = 1;
        }
        else if(m_Peach->getStars() == m_Yoshi->getStars()) {
            if(m_Peach->getCoins() > m_Yoshi->getCoins()) {
                gameWinner = 1;
            }
            else if(m_Peach->getCoins() < m_Yoshi->getCoins()) {
                gameWinner = 2;
            }
            else {
                gameWinner = randInt(1, 2);
            }
        }
        else {
            gameWinner = 2;
        }
        
        if(gameWinner == 1) {
            setFinalScore(m_Peach->getStars(), m_Peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else {
            setFinalScore(m_Yoshi->getStars(), m_Yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
            
    }
    m_Peach->doSomething();
    m_Yoshi->doSomething();
    for(Actor* a:container) {
        a->doSomething();
    }
    
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
    
    for(int i = 0; i<container.size(); i++) {
        if(!container[i]->alive()) {
            delete container[i];
            container.erase(container.begin()+i);
            i--;
        }
    }
    std::ostringstream oss;

    oss << "P1 Roll: " << p1_roll << " Stars: " << p1_stars << " $$: " << p1_money << " | Time: " << time << " | Bank: " << bank;
    if (p1_has_vortex) {
        oss << " VOR | ";
    } else {
        oss << " | ";
    }
    oss << "P2 Roll: " << p2_roll << " Stars: " << p2_stars << " $$: " << p2_money;
    if (p2_has_vortex) {
        oss << " VOR";
    }

    string output = oss.str();
    setGameStatText(output);
    
	return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

void StudentWorld::cleanUp() {
    for (Actor* a:container)
        delete a;
    container.clear();
    delete m_Peach;
    m_Peach = nullptr;
    delete m_Yoshi;
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
    container.push_back(new DroppingSquare(this, x, y));
}
