#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <vector>

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isDirectionalSquareAtPosition(double x, double y, int& d);
    bool forkSquare(double x, double y, int currDir, int& forkDir1, int& forkDir2);
    bool isSquare(int x, int y);
    bool squareInDirection(double x, double y, double d);
    Actor* getPeach() {return m_Peach;}
    Actor* getYoshi() {return m_Yoshi;}
    int isPlayerOnSquare(double x, double y);
    int getBankVal() {return bankVal;}
    void setBankVal(int b) {bankVal = b;}
    void addDroppingSquare(double x, double y);
    void removeSquareAt(double x, double y);
    void playerVisits(double x, double y, bool& peachVisits, bool& yoshiVisits);
    Actor* impactableOverlap(Actor* a);
    void addVortex(double x, double y, int d);

private:
    std::string boardPath;
    std::vector<Actor*> container;
    Actor* m_Peach;
    Actor* m_Yoshi;
    Board bd;
    int bankVal = 0;
    
    //adding actors
    void addPeach(double x, double y);
    void addYoshi(double x, double y);
    void addBowser(double x, double y);
    void addBoo(double x, double y);
    void addBlueCoinSquare(double x, double y);
    void addRedCoinSquare(double x, double y);
    void addDirectionalSquare(double x, double y, int direction);
    void addStarSquare(double x, double y);
    void addBankSquare(double x, double y);
    void addEventSquare(double x, double y);
    void setBnkVal(int n) {bankVal = n;}
    int getBnkVal() {return bankVal;}
};

#endif // STUDENTWORLD_H_
