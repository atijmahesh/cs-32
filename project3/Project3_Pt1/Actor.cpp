#include "Actor.h"
#include "StudentWorld.h"

bool Actor::overlap(Actor* a){
    return (this->getX() == a->getX() && this->getY() == a->getY());
}

bool Actor::complexOverlap(Actor* a) {
    //for boo and baddie
    return false;
}

void PlayerAvatar::doSomething() {
    if(!canMove()) {
        if(!active()) {
            bool foundRandDir = false;
            int r;
            while (foundRandDir) {
                r = randInt(1, 4);
                switch (r) {
                    case 1:
                        if(getWorld()->isSquare(getX(), getY()+1)) {
                            this->setDirection(0);
                            setWalkDir(90);
                            foundRandDir = true;
                        }
                        break;
                    case 2:
                        if(getWorld()->isSquare(getX()+1, getY())) {
                            this->setDirection(0);
                            setWalkDir(0);
                            foundRandDir = true;
                        }
                        break;
                    case 3:
                        if(getWorld()->isSquare(getX()-1, getY())) {
                            this->setDirection(180);
                            setWalkDir(180);
                            foundRandDir = true;
                        }
                        break;
                    case 4:
                        if(getWorld()->isSquare(getX(), getY()-1)) {
                            this->setDirection(0);
                            setWalkDir(270);
                            foundRandDir = true;
                        }
                        break;
                }
            }
            setActive(true);
        }
        
        switch (getWorld()->getAction(playerNum)) {
            case ACTION_ROLL: {
                int die_roll = randInt(1, 10);
                roll = die_roll;
                setTicksToMove(die_roll*8);
                setMove(true);
                break;
            }
            case ACTION_FIRE: {
                if(vortex) {
                    //getWorld()->addVortex();
                    //Introduce a new Vortex projectile on the square directly in front of
                    //the Avatar in the Avatar's current walk direction.
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    vortex = false;
                }
                else {return;}
                break;
            }
            default:
                return;
        }
    }
    else if(canMove()) {
        int dir = -1;
        int forkDir1, forkDir2;
        if(getWorld()->isDirectionalSquareAtPosition(getX(), getY(), dir)) {
            setWalkDir(dir);
            if(dir == 180) this->setDirection(dir);
            else setDirection(0);
        }
        
        /*
         IMPLEMENTATION DOES NOT WORK!!!
         */
        /*else if(getWorld()->forkSquare(getX(), getY(), getDirection(), forkDir1, forkDir2)) {
            int prevSqrDir = getDirection() + 180;
            if(prevSqrDir >= 360) prevSqrDir -= 360;
            std::cout << "\n\n\nGOTHERE\n\n\n";
            switch (getWorld()->getAction(playerNum)) {
                    
                case ACTION_UP:
                    if(prevSqrDir != 90 && forkDir2 == 90) {
                        setWalkDir(90);
                        setDirection(0);
                    }
                    break;
                case ACTION_DOWN:
                    if(prevSqrDir != 270 && forkDir2 == 270) {
                        setWalkDir(270);
                        setDirection(0);
                    }
                    break;
                case ACTION_LEFT:
                    if(prevSqrDir != 190 && forkDir1 == 180) {
                        setWalkDir(180);
                        setDirection(180);
                    }
                    break;
                case ACTION_RIGHT:
                    if(prevSqrDir != 0 && forkDir1 == 0) {
                        setWalkDir(0);
                        setDirection(0);
                    }
                    break;
                case ACTION_NONE:
                    return;
            }
        }*/
        else if(!getWorld()->squareInDirection(getX(), getY(), getWalkDir())) {
            if(getWalkDir() == 0 || getWalkDir() == 180) {
                if(getWorld()->squareInDirection(getX(), getY(), 90)) {
                    setWalkDir(90);
                    setDirection(0);
                }
                else if(getWorld()->squareInDirection(getX(), getY(), 270)) {
                    setWalkDir(270);
                    setDirection(0);
                }
            }
            else if(getWalkDir() == 90 || getWalkDir() == 270) {
                if(getWorld()->squareInDirection(getX(), getY(), 0)) {
                    setWalkDir(0);
                    setDirection(0);
                }
                else if(getWorld()->squareInDirection(getX(), getY(), 180)) {
                    setWalkDir(180);
                    setDirection(180);
                }
            }
        }
        switch (getWalkDir()) {
            case 0:
                moveTo(getX()+2, getY());
                break;
            case 90:
                moveTo(getX(), getY()+2);
                break;
            case 180:
                moveTo(getX()-2, getY());
                break;
            case 270:
                moveTo(getX(), getY()-2);
        }
        setTicksToMove(getTicks()-1);
        if(getTicks() == 0) setMove(false);
    }
}

void CoinSquare::doSomething() {
    if(!alive()) return;
    int situation = getWorld()->isPlayerOnSquare(getX(), getY());
    bool canPeachMove = getWorld()->getPeach()->canMove();
    bool canYoshiMove = getWorld()->getYoshi()->canMove();
    bool peachVisits = false;
    bool yoshiVisits = false;
    if((situation == 1 || situation == 3) && !peachVisited && !canPeachMove) {
        peachVisits = true;
        peachVisited = true;
    }
    else if((situation == 2 || situation == 3) && !yoshiVisited && !canYoshiMove) {
        yoshiVisits = true;
        yoshiVisited = true;
    }
    
    if(situation == 0) {
        peachVisited = false;
        yoshiVisited = false;
    }
    
    if(peachVisits) {
        if(isBlue) {
            getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() + 3);
            getWorld()->playSound(SOUND_GIVE_COIN);
        }
        else {
            if(getWorld()->getPeach()->getCoins()<3)
                getWorld()->getPeach()->setCoins(0);
            else getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() - 3);
            getWorld()->playSound(SOUND_TAKE_COIN);
        }
    }
    if(yoshiVisits) {
        if(isBlue) {
            getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() + 3);
            getWorld()->playSound(SOUND_GIVE_COIN);
        }
        else {
            if(getWorld()->getYoshi()->getCoins()<3)
                getWorld()->getYoshi()->setCoins(0);
            else getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() - 3);
            getWorld()->playSound(SOUND_TAKE_COIN);
        }
    }
}

void BankSquare::doSomething() {
    std::cout << "";
}

void StarSquare::doSomething() {
    std::cout << "";
}

void EventSquare::doSomething() {
    std::cout << "";
}

void DroppingSquare::doSomething() {
    std::cout << "";
}

void DirectionalSquare::doSomething() {
    std::cout << "";
}

void Boo::doSomething() {
    std::cout << "";
}

void Bowser::doSomething() {
    std::cout << "";
}
