#include "Actor.h"
#include "StudentWorld.h"

/*
Simple overlap between objects
@param object to compare overlap to
 @return if the objects overlap
*/
bool Actor::overlap(Actor* a){
    return (this->getX() == a->getX() && this->getY() == a->getY());
}

/*
Check is Peach and/or Yoshi make a new landing on a baddie or square
@param location for results to be copied into
*/
void Actor::checkNewLanding(bool& peachVisits, bool& yoshiVisits) {
    int situation = getWorld()->isPlayerOnSquare(getX(), getY());
    if((situation == 1 || situation == 3) && !peachVisited) {
        peachVisits = true;
        peachVisited = true;
    }
    else if((situation == 2 || situation == 3) && !yoshiVisited) {
        yoshiVisits = true;
        yoshiVisited = true;
    }
    if(situation == 0) {
        peachVisited = false;
        yoshiVisited = false;
    }
}

/*
Complex overlap between baddies and vortexes
@param object to compare overlap to
@return if the objects overlap
*/
bool Actor::complexOverlap(Actor* a) {
    double maxX = getX() + SPRITE_WIDTH / 2.0;
    double minX = getX() - SPRITE_WIDTH / 2.0;
    double maxY = getY() + SPRITE_HEIGHT / 2.0;
    double minY = getY() - SPRITE_HEIGHT / 2.0;
    double otherMaxX = a->getX() + SPRITE_WIDTH / 2.0;
    double otherMinX = a->getX() - SPRITE_WIDTH / 2.0;
    double otherMaxY = a->getY() + SPRITE_HEIGHT / 2.0;
    double otherMinY = a->getY() - SPRITE_HEIGHT / 2.0;
    if(otherMinX <= maxX && minX <= otherMaxX && otherMinY <= maxY && minY <= otherMaxY) {
        return true;
    }
    return false;
}

/*
Swap positional data, ticks, and directions between 2 actors, used between PlayerAvatars
@param object to swap data with
*/
void Actor::swapData(Actor* a) {
    double tempX = getX();
    double tempY = getY();
    int tempTicks = getTicks();
    int walkDir = getWalkDir();
    int sprDir = getDirection();
    bool tempCanMove = canMove();
    moveTo(a->getX(), a->getY());
    setTicksToMove(a->getTicks());
    setWalkDir(a->getWalkDir());
    setDirection(a->getDirection());
    setMove(a->canMove());
    a->moveTo(tempX, tempY);
    a->setTicksToMove(tempTicks);
    a->setWalkDir(walkDir);
    a->setDirection(sprDir);
    a->setMove(tempCanMove);
}

/*
Change Direction automatically at a corner by detecting if there is square in the current
 direction
*/
void MovingActor::changeDirectionAtCorner() {
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

/*
Teleport MovingActors to a random valid square, then putting their walk direction,
 active, and move states as invalid
*/
void MovingActor::teleport() {
    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
    int randX, randY;
    while (true) {
        randX = randInt(0, 15);
        randY = randInt(0, 15);
        if(getWorld()->isSquare(randX, randY)) break;
    }
    moveTo(randX*SPRITE_WIDTH, randY*SPRITE_HEIGHT);
    setWalkDir(-1);
    setActive(false);
    setMove(false);
}

/*
Simple move of 2 pixels in a MovingActor's current walking direction
*/
void MovingActor::simpleMove() {
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
}


/*
Setting a random valid (there is a square in the direction)
 direction for a MovingActor to move in
*/
void MovingActor::setRandDirection() {
    while(true) {
        int rand2 = randInt(1, 4);
        if(rand2 == 1) {
            if(!getWorld()->squareInDirection(getX(), getY(), 0))
                continue;
            setWalkDir(0);
            setDirection(0);
            break;
        }
        else if(rand2 == 2) {
            if(!getWorld()->squareInDirection(getX(), getY(), 90))
                continue;
            setWalkDir(90);
            setDirection(0);
            break;
        }
        else if(rand2 == 3) {
            if(!getWorld()->squareInDirection(getX(), getY(), 180))
                continue;
            setWalkDir(180);
            setDirection(180);
            break;
        }
        else if(rand2 == 4) {
                if(!getWorld()->squareInDirection(getX(), getY(), 270))
                continue;
            setWalkDir(270);
            setDirection(0);
            break;
        }
    }
}

/*
PlayerAvatar's doSomething method
*/
void PlayerAvatar::doSomething() {
    //check if waiting for roll
    if(!canMove()) {
        //if recently teleported
        if(!active()) {
            setRandDirection();
            setActive(true);
        }
        //player rolls the dice or fires a vortex
        switch (getWorld()->getAction(playerNum)) {
            case ACTION_ROLL: {
                int die_roll = randInt(1, 10);
                roll = die_roll;
                setTicksToMove(die_roll*8);
                setMove(true);
                break;
            }
            case ACTION_FIRE: {
                if(hasVortex()) {
                    switch (getWalkDir()) {
                        case 0:
                            getWorld()->addVortex(getX()+SPRITE_WIDTH, getY(), getWalkDir());
                            break;
                        case 180:
                            getWorld()->addVortex(getX()-SPRITE_WIDTH, getY(), getWalkDir());
                            break;
                        case 90:
                            getWorld()->addVortex(getX(), getY()+SPRITE_WIDTH, getWalkDir());
                            break;
                        case 270:
                            getWorld()->addVortex(getX(), getY()-SPRITE_WIDTH, getWalkDir());
                            break;
                    }
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
    
    //player has rolled Dice and is walking
    else if(canMove()) {
        int dir = -1;
        int forkDir1 = -1, forkDir2 = -1;
        //if the player meets a directionalSquare (has to change direction)
        if(getWorld()->isDirectionalSquareAtPosition(getX(), getY(), dir)) {
            setWalkDir(dir);
            if(dir == 180) this->setDirection(dir);
            else setDirection(0);
        }
        //if the player is at a fork square
        else if(getWorld()->forkSquare(getX(), getY(), getWalkDir(), forkDir1, forkDir2)) {
            switch (getWorld()->getAction(playerNum)) {
                case ACTION_UP:
                    if(forkDir1 == 90 || forkDir2 == 90) {
                        setWalkDir(90);
                        setDirection(0);
                    }
                    else return;
                    break;
                case ACTION_DOWN:
                    if(forkDir1 == 270 || forkDir2 == 270) {
                        setWalkDir(270);
                        setDirection(0);
                    }
                    else return;
                    break;
                case ACTION_LEFT:
                    if(forkDir1 == 180 || forkDir2 == 180) {
                        setWalkDir(180);
                        setDirection(180);
                    }
                    else return;
                    break;
                case ACTION_RIGHT:
                    if(forkDir1 == 0 || forkDir2 == 0) {
                        setWalkDir(0);
                        setDirection(0);
                    }
                    else return;
                    break;
                default:
                    return;
            }
        }
        //if the player is at a corner
        else if(!getWorld()->squareInDirection(getX(), getY(), getWalkDir())) {
            changeDirectionAtCorner();
        }
        simpleMove();
        setTicksToMove(getTicks()-1);
        if(getTicks() == 0) setMove(false);
    }
}

/*
Teleport baddies, same as MovingActor teleport
 but walkDir must be zero, and pause set to 180
*/
void Baddie::teleport() {
    MovingActor::teleport();
    setWalkDir(0);
    setDirection(0);
    setPause(180);
}

/*
Movning baddies
*/
void Baddie::move() {
    int forkDir1 = -1, forkDir2 = -1;
    //check if baddie is at a fork square
    if(getWorld()->forkSquare(getX(), getY(), getWalkDir(), forkDir1, forkDir2)) {
        setRandDirection();
    }
    //check if baddie is at a corner square
    else if(!getWorld()->squareInDirection(getX(), getY(), getWalkDir())) {
        changeDirectionAtCorner();
    }
    simpleMove();
    setDist(getDist()+2);
    setTicksToMove(getTicks()-1);
    if(getTicks() == 0) {
        setMove(false);
        setPause(180);
    }
}

/*
Teleport baddie when it's hit by a vortex
*/
void Baddie::hitByVortex() {
    teleport();
}

/*
Bowser doSomething
*/
void Bowser::doSomething() {
    //in a pause state
    if(!canMove()) {
        bool peachVisits = false;
        bool yoshiVisits = false;
        checkNewLanding(peachVisits, yoshiVisits);
        //check if peach and Bowser intersect and peach is in waiting to roll state
        if(overlap(getWorld()->getPeach()) && !getWorld()->getPeach()->canMove() && peachVisits) {
            int rand = randInt(1, 2);
            //50% chance of losing all coins and stars
            if(rand == 1) {
                getWorld()->getPeach()->setCoins(0);
                getWorld()->getPeach()->setStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
        }
        //check if yoshi and bowser itersect and yoshi is wating to roll
        if(overlap(getWorld()->getYoshi()) && !getWorld()->getYoshi()->canMove() && yoshiVisits) {
            int rand = randInt(1, 2);
            if(rand == 1) {
                getWorld()->getYoshi()->setCoins(0);
                getWorld()->getYoshi()->setStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
        }
        setPause(getPause()-1);
        if(getPause() == 0) {
            int squares_to_move = randInt(1, 10);
            setTicksToMove(squares_to_move*8);
            setRandDirection();
            setMove(true);
        }
    }
    //bowser can move
    else if(canMove()) {
        move();
        if(getTicks() == 0) {
            int rand = randInt(1, 4);
            //25% chance of dropping
            if(rand == 1) {
                getWorld()->removeSquareAt(getX(), getY());
                getWorld()->addDroppingSquare(getX(), getY());
                getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
            }
        }
        
    }
}

/*
Boo doSomething
*/
void Boo::doSomething() {
    //paused state
    if(!canMove()) {
        bool peachVisits = false;
        bool yoshiVisits = false;
        checkNewLanding(peachVisits, yoshiVisits);
        //check if Boo and peach intersect and peach is waiting to roll
        if(overlap(getWorld()->getPeach()) && !getWorld()->getPeach()->canMove() && peachVisits) {
            int rand = randInt(1, 2);
            //50% chance to swap coins
            if(rand == 1) {
                int temp = getWorld()->getPeach()->getCoins();
                getWorld()->getPeach()->setCoins(getWorld()->getYoshi()->getCoins());
                getWorld()->getYoshi()->setCoins(temp);
            }
            //50% chance to swap stars
            else {
                int temp = getWorld()->getPeach()->getStars();
                getWorld()->getPeach()->setStars(getWorld()->getYoshi()->getStars());
                getWorld()->getYoshi()->setStars(temp);
            }
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
        }
        //check if boo and peach intersect and peach is waiting to roll
        if(overlap(getWorld()->getYoshi()) && !getWorld()->getYoshi()->canMove() && yoshiVisits) {
            int rand = randInt(1, 2);
            if(rand == 1) {
                int temp = getWorld()->getPeach()->getCoins();
                getWorld()->getPeach()->setCoins(getWorld()->getYoshi()->getCoins());
                getWorld()->getYoshi()->setCoins(temp);
            }
            else {
                int temp = getWorld()->getPeach()->getStars();
                getWorld()->getPeach()->setStars(getWorld()->getYoshi()->getStars());
                getWorld()->getYoshi()->setStars(temp);
            }
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
        }
        setPause(getPause()-1);
        if(getPause() == 0) {
            int squares_to_move = randInt(1, 3);
            setTicksToMove(squares_to_move*8);
            setRandDirection();
            setMove(true);
        }
    }
    else if(canMove()) {
        move();
    }
}


/*
Vortex doSomething
*/
void Vortex::doSomething() {
    //vortex not alive
    if(!alive()) {return;}
    simpleMove();
    //vortex off screen
    if(getX()> VIEW_WIDTH || getY()> VIEW_HEIGHT ||
       getX()< 0 || getY() < 0) {
        setAlive(false);
        return;
    }
    //vortex and baddies intersect
    Actor* a = getWorld()->impactableOverlap(this);
    if(a != nullptr) {
        a->teleport();
        setAlive(false);
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}

/*
Coin Square doSomething
*/
void CoinSquare::doSomething() {
    if(!alive()) return; //been destroyed by Bowser
    bool peachVisits = false;
    bool yoshiVisits = false;
    checkNewLanding(peachVisits, yoshiVisits);
    //check if peach lands
    if(peachVisits && !getWorld()->getPeach()->canMove()) {
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
    //check if yoshi lands
    if(yoshiVisits && !getWorld()->getYoshi()->canMove()) {
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

/*
BankSquare doSomething
*/
void BankSquare::doSomething() {
    if(!alive()) return; //destroyed by bowser
    bool peachVisits = false;
    bool yoshiVisits = false;
    checkNewLanding(peachVisits, yoshiVisits);
    if(peachVisits) {
        //peach lands on square
        if(!getWorld()->getPeach()->canMove()) {
            getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins()+getWorld()->getBankVal());
            getWorld()->setBankVal(0);
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
        }
        //peach passes over square
        else {
            int bankAddition = 5;
            if(getWorld()->getPeach()->getCoins() < 5) {
                bankAddition = getWorld()->getPeach()->getCoins();
                getWorld()->getPeach()->setCoins(0);
            }
            else getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins()-5);
            getWorld()->setBankVal(getWorld()->getBankVal() + bankAddition);
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
        }
    }
    if(yoshiVisits) {
        //yoshi lands on square
        if(!getWorld()->getYoshi()->canMove()) {
            getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins()+getWorld()->getBankVal());
            getWorld()->setBankVal(0);
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
        }
        else {
            //yoshi passes over square
            int bankAddition = 5;
            if(getWorld()->getYoshi()->getCoins() < 5) {
                bankAddition = getWorld()->getYoshi()->getCoins();
                getWorld()->getYoshi()->setCoins(0);
            }
            else getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins()-5);
            getWorld()->setBankVal(getWorld()->getBankVal() + bankAddition);
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
        }
    }
}

/*
StarSquare doSomething
*/
void StarSquare::doSomething() {
    if(!alive()) return; //destroyed by bowser
    bool peachVisits = false;
    bool yoshiVisits = false;
    checkNewLanding(peachVisits, yoshiVisits);
    //if peach passes or lands
    if(peachVisits) {
        if(getWorld()->getPeach()->getCoins()<20) return;
        else {
            getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins()-20);
            getWorld()->getPeach()->setStars(getWorld()->getPeach()->getStars()+1);
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
    //if yoshi passes or lands
    if(yoshiVisits) {
        if(getWorld()->getYoshi()->getCoins()<20) return;
        else {
            getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins()-20);
            getWorld()->getYoshi()->setStars(getWorld()->getYoshi()->getStars()+1);
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
}

/*
EventSquare doSomething
*/
void EventSquare::doSomething() {
    if(!alive()) return; //destroyed by bowser
    bool peachVisits = false;
    bool yoshiVisits = false;
    checkNewLanding(peachVisits, yoshiVisits);
    //check for landing and if the players haven't been just swapped
    if(peachVisits && !getWorld()->getPeach()->canMove() && !justSwapped) {
        int rand = randInt(1, 3);
        switch (rand) {
            //1/3 chance to be teleported
            case 1:
                getWorld()->getPeach()->teleport();
                break;
            //1/3 chance to swap w other player
            case 2:
                getWorld()->getPeach()->swapData(getWorld()->getYoshi());
                justSwapped = true;
                break;
            //1/3 chance to get vortex power
            case 3:
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                if(!getWorld()->getPeach()->hasVortex())
                    getWorld()->getPeach()->setVortex(true);
                break;
        }
    }
    if(yoshiVisits && !getWorld()->getYoshi()->canMove() && !justSwapped) {
        int rand = randInt(1, 3);
        switch (rand) {
            case 1:
                getWorld()->getYoshi()->teleport();
                break;
            case 2:
                getWorld()->getYoshi()->swapData(getWorld()->getPeach());
                justSwapped = true;
                break;
            case 3:
                if(!getWorld()->getYoshi()->hasVortex())
                    getWorld()->getYoshi()->setVortex(true);
                break;
        }
    }
}

/*
DroppingSquare doSomething
*/
void DroppingSquare::doSomething() {
    if(!alive()) return;
    bool peachVisits = false;
    bool yoshiVisits = false;
    checkNewLanding(peachVisits, yoshiVisits);
    //check if peach lands
    if(peachVisits && !getWorld()->getPeach()->canMove()) {
        int rand = randInt(1, 2);
        //50% chance to lose 10 coins
        if(rand == 1) {
            getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins()-10);
            if(getWorld()->getPeach()->getCoins() < 0) getWorld()->getPeach()->setCoins(0);
        }
        //50% chance to lose a star
        else if(getWorld()->getPeach()->getStars() >= 1) {
            getWorld()->getPeach()->setStars(getWorld()->getPeach()->getStars() - 1);
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
    //check if yoshi lands
    if(yoshiVisits&& !getWorld()->getYoshi()->canMove()) {
        int rand = randInt(1, 2);
        if(rand == 1) {
            getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins()-10);
            if(getWorld()->getYoshi()->getCoins() < 0) getWorld()->getYoshi()->setCoins(0);
        }
        else if(getWorld()->getYoshi()->getStars() >= 1) {
            getWorld()->getYoshi()->setStars(getWorld()->getYoshi()->getStars() - 1);
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
}
