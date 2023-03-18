#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "GameConstants.h"

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
            Vortex
        Square
            Coin Square
                Red Coin Square
                Blue Coin Square
            Directional Square
            Star Square
            Bank Square
            Event Square
            Dropping Square
*/

class Actor:public GraphObject {
private:
    StudentWorld* m_world; //student world pointer
    bool isActive = true; //for moving actors who are teleported
    bool isAlive = true; //for vortex and squares
    int actorType = -1; //ID actors
    bool peachVisited = false; //for baddies and squares
    bool yoshiVisited = false; //for baddies and squares

public:
    Actor(StudentWorld* world, int img, double x, double y, int depth) :
            GraphObject(img, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, depth),
            m_world(world){}
    virtual void doSomething() = 0;
    virtual bool isSquare() {return false;} //used to detect when square is removed
    virtual bool canMove() {return false;} //paused/walking state for moving actors
    virtual int getCoins() {return 0;}
    virtual void setCoins(int c) {return;}
    virtual int getStars() {return 0;}
    virtual void setStars(int s) {return;}
    virtual int getRoll() {return 0;} //player roll
    virtual bool hasVortex() {return false;}
    StudentWorld* getWorld() {return m_world;}
    void setActive(bool active){isActive = active;}
    bool active() {return isActive;}
    void setAlive(bool alive){isAlive = alive;}
    bool alive() {return isAlive;}
    virtual bool isDirSqr() {return false;}
    virtual void teleport() {return;}
    bool overlap(Actor* a);
    bool complexOverlap(Actor* a);
    void checkNewLanding(bool& peachVisits, bool& yoshiVisits);
    virtual void swapData(Actor* a);
    virtual void setVortex(bool b) {return;}
    virtual bool isImpactable() {return false;}
    virtual void setTicksToMove(int t) {return;}
    virtual int getTicks() {return -1;}
    virtual void setWalkDir(int d) {return;}
    virtual int getWalkDir() {return -1;}
    virtual void setMove(bool m) {return;}
};

class MovingActor:public Actor {
private:
    bool move = false;
    int ticks_to_move = 0;
    int walkDirection = 0;
public:
    MovingActor(StudentWorld* world, int img, double x, double y):Actor(world, img, x, y, 0){}
    virtual void doSomething() = 0;
    virtual bool canMove() {return move;}
    virtual void setMove(bool m) {move = m;}
    virtual void setTicksToMove(int t) {ticks_to_move = t;}
    virtual int getTicks() {return ticks_to_move;}
    virtual void setWalkDir(int d) {walkDirection = d;}
    virtual int getWalkDir() {return walkDirection;}
    void changeDirectionAtCorner();
    void teleport();
    void setRandDirection();
    void simpleMove();
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
    virtual void setVortex(bool b) {vortex = b;}
    virtual int getCoins() {return coins;}
    virtual void setCoins(int c) {coins = c;}
    virtual int getStars() {return stars;}
    virtual void setStars(int s) {stars = s;}
    virtual int getRoll() {return roll;}
};

class Peach:public PlayerAvatar {
public:
    Peach(StudentWorld* world, double x, double y):PlayerAvatar(world, IID_PEACH, x, y, 1) {}
};

class Yoshi:public PlayerAvatar {
public:
    Yoshi(StudentWorld* world, double x, double y):PlayerAvatar(world, IID_YOSHI, x, y, 2) {}
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
    virtual void move();
    virtual bool isImpactable() {return true;}
    void hitByVortex();
    virtual void teleport();
};

class Bowser:public Baddie {
public:
    Bowser(StudentWorld* world, double x, double y):Baddie(world, IID_BOWSER, x, y){}
    virtual void doSomething();
};

class Boo:public Baddie {
public:
    Boo(StudentWorld* world, double x, double y):Baddie(world, IID_BOO, x, y){}
    virtual void doSomething();
};

class Vortex:public MovingActor {
public:
    Vortex(StudentWorld* world, double x, double y, int firedDirection):MovingActor(world, IID_VORTEX, x, y){
        setWalkDir(firedDirection);
    }
    virtual void doSomething();
};

class Square:public Actor {
public:
    Square(StudentWorld* world, int img, double x, double y):Actor(world, img, x, y, 0){}
    virtual void doSomething() = 0;
    virtual bool isSquare() {return true;}
};

class CoinSquare:public Square {
private:
    bool isBlue;
public:
    CoinSquare(StudentWorld* world, int img, double x, double y, bool blue):Square(world, img, x, y), isBlue(blue) {}
   virtual void doSomething();
};

class RedCoinSquare:public CoinSquare {
public:
    RedCoinSquare(StudentWorld* world, double x, double y):CoinSquare(world, IID_RED_COIN_SQUARE, x, y, false) {}
};

class BlueCoinSquare:public CoinSquare {
public:
    BlueCoinSquare(StudentWorld* world, double x, double y):CoinSquare(world, IID_BLUE_COIN_SQUARE, x, y, true) {}
};

class StarSquare:public Square {
public:
   StarSquare(StudentWorld* world, double x, double y):Square(world, IID_STAR_SQUARE, x, y) {}
   virtual void doSomething();
};

class DirectionalSquare:public Square {
private:
    int forcedDirection;
public:
    DirectionalSquare(StudentWorld* world, double x, double y, int forcedDirection):Square(world, IID_DIR_SQUARE, x, y), forcedDirection(forcedDirection) {
        setDirection(forcedDirection);
    }
    virtual bool isDirSqr() {return true;}
    virtual void doSomething() {return;}
};

class BankSquare:public Square {
public:
    BankSquare(StudentWorld* world, double x, double y):Square(world, IID_BANK_SQUARE, x, y){}
    virtual void doSomething();
};

class EventSquare:public Square {
public:
    EventSquare(StudentWorld* world, double x, double y):Square(world, IID_EVENT_SQUARE, x, y) {}
    virtual void doSomething();
private:
    bool justSwapped = false;
};

class DroppingSquare:public Square {
public:
    DroppingSquare(StudentWorld* world, double x, double y):Square(world, IID_DROPPING_SQUARE, x, y) {}
    virtual void doSomething();
};
#endif // ACTOR_H_
