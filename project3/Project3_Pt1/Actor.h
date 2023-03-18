#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "GameConstants.h"
#include <vector>
#include <algorithm>

class StudentWorld;
/*
 Inheritance Hirearchy
 GraphObject
    Actor
        Moving Actor
            PlayerAvatar
                Peach
                Yoshi
            Baddie
                Bowser
                Boo
        Square
            Coin Square
                Red Coin Square
                Blue Coin Square
            Directional Square
            Star Square
            Bank Square
            Event Square
            Dropping Square
        Vortex
 */

const int PEACHTYPE = 0;
const int YOSHITYPE = 1;
const int BOWSERTYPE = 2;
const int BOOTYPE = 3;
const int BCSTYPE = 4;
const int RCSTYPE = 5;
const int STRSQRTYPE = 6;
const int DIRSQRTYPE = 7;
const int BNKSQRTYPE = 8;
const int DRPSQRTYPE = 9;
const int EVTSQRTYPE = 9;
const int VRTXTYPE = 11;

class Actor:public GraphObject {
private:
    StudentWorld* m_world;
    bool isActive = true; //for moving actors who are teleported
    bool isAlive = true; //for vortex and squares
    int actorType = -1; //ID actors
    
public:
    Actor(StudentWorld* world, int img, double x, double y, int depth) :
            GraphObject(img, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, depth),
            m_world(world){}
    virtual void doSomething() = 0;
    virtual bool isSquare() {return false;}
    virtual bool isAvatar() {return false;}
    virtual bool isBaddie() {return false;}
    virtual bool canMove() {return false;}
    virtual int getCoins() {return 0;}
    virtual void setCoins(int c) {return;}
    virtual int getStars() {return 0;}
    virtual void setStars(int s) {return;}
    virtual int getRoll() {return 0;}
    virtual bool hasVortex() {return false;}
    StudentWorld* getWorld() {return m_world;}
    void setActive(bool active){isActive = active;}
    bool active() {return true;}
    void setAlive(bool alive){isAlive = alive;}
    bool alive() {return true;}
    void setType(int t) {actorType = t;}
    int getType() {return actorType;}
    bool overlap(Actor* a);
    bool complexOverlap(Actor* a);
};

class MovingActor:public Actor {
private:
    bool move = false;
    int ticks_to_move = 0;
    int walkDirection = 0;
public:
    MovingActor(StudentWorld* world, int img, double x, double y):Actor(world, img, x, y, 0){}
    virtual void doSomething() = 0;
    bool canMove() {return move;}
    void setMove(bool m) {move = m;}
    void setTicksToMove(int t) {ticks_to_move = t;}
    int getTicks() {return ticks_to_move;}
    void setWalkDir(int d) {walkDirection =d;}
    int getWalkDir() {return walkDirection;}
};

class PlayerAvatar:public MovingActor {
private:
    bool vortex = false;
    int playerNum;
    int coins = 0;
    int stars = 0;
    int roll = 0;
public:
    PlayerAvatar(StudentWorld* world, int img, double x, double y, int playerNum):MovingActor(world, img, x, y), playerNum(playerNum){
        setTicksToMove(0);
    }
    virtual void doSomething();
    virtual bool hasVortex() {return vortex;}
    virtual int getCoins() {return coins;}
    virtual void setCoins(int c) {coins = c;}
    virtual int getStars() {return stars;}
    virtual void setStars(int s) {stars = s;}
    virtual int getRoll() {return roll;}
};

class Peach:public PlayerAvatar {
public:
    Peach(StudentWorld* world, double x, double y):PlayerAvatar(world, IID_PEACH, x, y, 1) {
        setType(PEACHTYPE);
    }
};

class Yoshi:public PlayerAvatar {
public:
    Yoshi(StudentWorld* world, double x, double y):PlayerAvatar(world, IID_YOSHI, x, y, 2) {
        setType(YOSHITYPE);
    }
};

class Square:public Actor {
private:
    
public:
    Square(StudentWorld* world, int img, double x, double y):Actor(world, img, x, y, 0){}
    virtual void doSomething() = 0;
};

class CoinSquare:public Square {
private:
    bool peachVisited = false;
    bool yoshiVisited = false;
    bool isBlue;
public:
    CoinSquare(StudentWorld* world, int img, double x, double y, bool blue):Square(world, img, x, y), isBlue(blue) {}
   virtual void doSomething();
};

class RedCoinSquare:public CoinSquare {
public:
    RedCoinSquare(StudentWorld* world, double x, double y):CoinSquare(world, IID_RED_COIN_SQUARE, x, y, false) {
        setType(RCSTYPE);
    }
    
};

class BlueCoinSquare:public CoinSquare {
public:
    BlueCoinSquare(StudentWorld* world, double x, double y):CoinSquare(world, IID_BLUE_COIN_SQUARE, x, y, true) {setType(BCSTYPE);}
};


 
class Baddie:public MovingActor {
private:
    int distTrav = 0;
    int pauseCounter = 180;
public:
    Baddie(StudentWorld* world, int img, double x, double y):MovingActor(world, img, x, y){}
    int getDist() {return distTrav;}
    int getPause() {return pauseCounter;}
    void setDist(int d) {distTrav = d;}
    void setPause(int p) {pauseCounter = p;}
};

class Bowser:public Baddie {
public:
    Bowser(StudentWorld* world, double x, double y):Baddie(world, IID_BOWSER, x, y){
        setType(BOWSERTYPE);
    }
    void doSomething();
};

class Boo:public Baddie {
public:
    Boo(StudentWorld* world, double x, double y):Baddie(world, IID_BOO, x, y){
        setType(BOOTYPE);
    }
    void doSomething();
};

class StarSquare:public Square {
private:
public:
   StarSquare(StudentWorld* world, double x, double y):Square(world, IID_STAR_SQUARE, x, y) {
       setType(STRSQRTYPE);
   }
   virtual void doSomething();
};

class DirectionalSquare:public Square {
private:
    int forcedDirection;
public:
    DirectionalSquare(StudentWorld* world, double x, double y, int forcedDirection):Square(world, IID_DIR_SQUARE, x, y), forcedDirection(forcedDirection) {
        setDirection(forcedDirection);
        setType(DIRSQRTYPE);
    }
    virtual void doSomething();
};

class BankSquare:public Square {
private:
    int balance;
public:
    BankSquare(StudentWorld* world, double x, double y):Square(world, IID_BANK_SQUARE, x, y), balance(0) {setType(BNKSQRTYPE);}
    virtual void doSomething();
};

class EventSquare:public Square {
public:
    EventSquare(StudentWorld* world, double x, double y):Square(world, IID_EVENT_SQUARE, x, y) {
        setType(EVTSQRTYPE);
    }
    virtual void doSomething();
};

class DroppingSquare:public Square {
public:
    DroppingSquare(StudentWorld* world, double x, double y):Square(world, IID_DROPPING_SQUARE, x, y) {
        setType(DRPSQRTYPE);
    }
    virtual void doSomething();
};

 /*
 class Vortex:public MovingActor {
     
 };
*/
 
#endif // ACTOR_H_
