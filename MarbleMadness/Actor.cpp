#include "Actor.h"
#include "StudentWorld.h"

// ACTOR
Actor::Actor(int imageID, double startX, double startY, int dir, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, 1.0), m_world(world)
{}

bool Actor::Alive() const
{
    return m_health > 0;
}

int Actor::getHealth() const
{
    return m_health;
}
void Actor::updateHealth(int health)
{
    m_health += health;
}
void Actor::damage(int damageAmt)
{
    updateHealth(-2);
}
void Actor::setHealth(int health)
{
    m_health = health;
}
StudentWorld* Actor::getWorld() const
{
    return m_world;
}

void Actor::setdxdy(int &dx, int&dy, int direction)
{ // converts direction facing to instructions for movement
    switch (direction)
    {
        case right:
            dx = 1;
            dy = 0;
            break;
        case left:
            dx = -1;
            dy = 0;
            break;
        case up:
            dy = 1;
            dx = 0;
            break;
        case down:
            dy = -1;
            dx = 0;
            break;
    }
}

void Actor::fire(int direction, double x, double y)
{
    int dx = 0, dy = 0;
    setdxdy(dx, dy, direction);
    // creates a pea in the proper dx dy direction
    getWorld()->addActor(new Pea(IID_PEA, x+dx, y+dy, direction, getWorld()));
}

void Actor::moveActor(double x, double y, int dx, int dy)
{ // Can we move to our desired space?
    if (getWorld()->isValidPos(x+dx, y+dy, this))
    {
        moveTo(x+dx, y+dy);
        return;
    }
    else
    { // turn around if not, mainly applicable to the robots
        setDirection(getDirection() + 180);
        return;
    }
}


// AVATAR
Avatar::Avatar(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, 0, world)
{ // Initializing the Avatar
    setHealth(20);
    numPeas = 20;
    setVisible(true);
}
void Avatar::moveAvatar(int directionKey)
{
    int dx = 0;
    int dy = 0;
    Actor::setdxdy(dx, dy, directionKey);
    setDirection(directionKey);
    if (getWorld()->isValidPos(getX()+dx, getY()+dy, this))
    { // attempts to push marbles if they can be pushed
        getWorld()->pushActors(getX()+dx, getY()+dy, directionKey);
        moveTo(getX()+dx, getY()+dy);
    }
}
void Avatar::doSomething()
{
    if (!Alive())
        return;
    int ch;
    // Reading in what action the user wants the avatar to do
    if (getWorld()->getKey(ch))
    {
        switch (ch) {
            case KEY_PRESS_UP:
                moveAvatar(up);
                break;
            case KEY_PRESS_DOWN:
                moveAvatar(down);
                break;
            case KEY_PRESS_RIGHT:
                moveAvatar(right);
                break;
            case KEY_PRESS_LEFT:
                moveAvatar(left);
                break;
            case KEY_PRESS_SPACE:
                if (numPeas > 0)
                { // We can shoot a pea
                    Actor::fire(getDirection(), getX(), getY());
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    numPeas--;
                }
                break;
            case KEY_PRESS_ESCAPE:
                setHealth(0);
                break;
            default:
                break;
        }
    }
}

void Avatar::damage(int damageAmt)
{ // The avatar got hit by a pea
    updateHealth(-2);
    if (Alive())
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
}
// PEA
Pea::Pea(int imageID, double startX, double startY, int direction, StudentWorld* world): Actor(IID_PEA, startX, startY, direction, world), m_direction(direction) {
    setVisible(true);
    setHealth(100);
    firstTick = true;
}

void Pea::doSomething()
{
    if (!Alive())
        return;
    if (firstTick == true)
    { // pea rests on the first tick
        firstTick = false;
        return;
    }
    if (step2())
    { // repeat movement in a tick if we can
        int dx = 0, dy = 0;
        setdxdy(dx, dy, m_direction);
        moveTo(getX()+dx, getY()+dy);
        step2();
        return;
    }
}
bool Pea::step2()
{
    Actor* target = getWorld()->getTarget(getX(), getY(), this);
    if (target != nullptr && target->canTakeDamage())
    { // find if the pea is at the position of a target we can deal damage to e.g. marble, avatar, robot
        target->damage(-2);
        setHealth(0);
        return false;
    }
    Actor* wall = getWorld()->getActor(getX(), getY(), this);
    if (wall != nullptr && !(wall->canTakeDamage()) && wall->canAvatarOverlap() == 0  && wall->blocks() && wall != this)
    { // see if the pea overlaps with a wall or obstruction where we should remove it from the game as a result
        setHealth(0);
        return false;
    }
    return true; // we are clear to continue moving
}

// WALL
Wall::Wall(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world)
{
    setVisible(true);
    setHealth(1000);
}
    
void Wall::doSomething()
{ // Wall does not do anything
    return;
}

// MARBLE
Marble::Marble(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world)
{
    setVisible(true);
    setHealth(10);
}

void Marble::push(int direction, double X, double Y)
{
    int dx = 0, dy = 0;
    setdxdy(dx, dy, direction);
    if (getWorld()->isEmpty(X+dx, Y+dy))
    { // if the space in front is empty, we can move the marble
        moveTo(X+dx, Y+dy);
    }
}

// PIT
Pit::Pit(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world)
{ // NOTE: non-damagable actors like pits and goodies start with an arbitrary health that cant be decreased inorder to not get cleaned up after each tick
    setVisible(true);
    setHealth(10010);
}

void Pit::doSomething()
{ // Check if the pit is dead
    if (!Alive())
        return;
    Actor *p = getWorld()->getActor(getX(),getY(), this);
    if (p != nullptr && p->canAvatarOverlap() == 2) //marble and pit on the same space
    { // set marble and pit to dead
        setHealth(0);
        p->setHealth(0);
        p = nullptr;
    }
}
// Item
Item::Item(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Actor(imageID, startX, startY, -1, world), pointBonus(increasePoints), stolen(false)
{
    setVisible(true);
    setHealth(100);
}

void Item::setStolen(bool status)
{
    stolen = status;
}
bool Item::updateItem()
{
    if (!Alive())
        return false;
    Avatar *p = getWorld()->getAvatar();
    if (p->getX() == getX() && p->getY() == getY() && stolen == false)
    {  // Goodies and crystals increase score by different amounts
        getWorld()->increaseScore(pointBonus);
        setHealth(0);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        return true;
    } // returns false if the avatar is not on the same position as the avatar
    return false;
}

// Crystal

void Crystal::doSomething()
{
    if (updateItem())
        getWorld()->pickupCrystal();
} // decrement crystals if the avatar picks it up

// extraLife

void extraLife::doSomething()
{
    if (updateItem())
        getWorld()->incLives();
}

// restoreHealth

void restoreHealth::doSomething()
{
    if (updateItem())
        getWorld()->getAvatar()->setHealth(20);
}

// Ammo

void Ammo::doSomething()
{
    if (updateItem())
        getWorld()->getAvatar()->addAmmo(20);
}
// EXIT
Exit::Exit(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world), m_exitExposed(false)
{ // exit starts out unexposed
    setHealth(100);
    setVisible(false);
}

void Exit::doSomething()
{ //gets the avatar and checks if it is on the same space as the exit
    Avatar* p = getWorld()->getAvatar();
    if (p != nullptr && p->getX() == getX() && p->getY() == getY() && getWorld()->getCrystals() <= 0)
    { // appropraitely increase the score and finish the level
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->completedLevel();
        getWorld()->increaseScore(getWorld()->getBonus());
    }
    if (getWorld()->getCrystals() <= 0 && m_exitExposed == false)
    { // we only expose the exit if it isnt already exposed
        exposeExit();
    }
}

void Exit::exposeExit()
{
    makeVisible();
    m_exitExposed = true;
    getWorld()->playSound(SOUND_REVEAL_EXIT);
}
// ROBOT
Robot::Robot(int imageID, double startX, double startY, int startDir, int hitPoints, int score, StudentWorld* world) : Actor (imageID, startX, startY, startDir, world), m_score(score), m_ticks(0)
{
    setVisible(true);
    setHealth(hitPoints);
}

void Robot::doSomething()
{ // all robots have the same initial steps of dealing with ticks
    int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
     // level number (0, 1, 2, etc.)
    if (ticks < 3)
     ticks = 3;
    if (!Alive())
        return;
    increaseTicks();
    if (getTicks() % ticks != 0) // when the ticks arent divisible by the level ticks, the robots rest
        return;
    doDifferentSomething();
} // each robot has some differing behavior

// RAGEBOT
RageBot::RageBot(int startX, int startY, int startDir, StudentWorld* world) : Robot(IID_RAGEBOT, startX, startY, startDir, 10, 100, world)
{}

void RageBot::doDifferentSomething()
{
    int dx = 0;
    int dy = 0;
    int direction = getDirection();
    setdxdy(dx, dy, direction);
    if (getWorld()->existsClearShotToPlayer(getX(), getY(), dx, dy))
    { // RageBot sees player in line of site with no obstructions and can fire
        Actor::fire(direction, getX(), getY());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    } // Move the rage bot if we do not shoot the player
    moveActor(getX(), getY(), dx, dy);
}

void Robot::damage(int damageAmt)
{
    updateHealth(-2);
    if (Alive())
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
        return;
    }
    else
    { // the robot died
        setHealth(0);
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(m_score);
    }
}

// THIEFBOT

ThiefBot::ThiefBot(int imageID, int startX, int startY, int health, int points, StudentWorld* world) : Robot(imageID, startX, startY, right, health, points, world), goodieHeld(nullptr)
{ // random distance 1-6 before turning
    distanceBeforeTurning = randInt(1, 6);
}

void ThiefBot::doDifferentSomething()
{ // grabs a goodie if it shares the same position
    Actor *p = getWorld()->getActor(getX(), getY(), this);
    if (p != nullptr && p->stealable() == true && goodieHeld == nullptr) //goodie and isnt holding another goodie
    {
        int pickup = randInt(1, 10);
        if (pickup == 1)
        { // 1 in 10 chance of picking up the goodie
            goodieHeld = p; // remember the goodie
            p->setVisible(false);
            p->setStolen(true);
            // remove the goodie from display
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            return;
        }
    }
    int dx = 0, dy = 0;
    int direction = getDirection();
    setdxdy(dx, dy, direction);
    if (distanceBeforeTurning > 0 && getWorld()->isValidPos(getX()+dx, getY()+dy, this))
    { // movement, only moves if we can
        moveTo(getX()+dx, getY()+dy);
        distanceBeforeTurning--;
        return;
    } // decrement distance
    else
    { // choose a new distance before turning
        distanceBeforeTurning = randInt(1, 6);
        int randD = randInt(1, 4); // integers 1-4 map do different directions
        switch (randD) { // randomly choosing an initial direction
            case 1:
                randD = up;
                break;
            case 2:
                randD = down;
                break;
            case 3:
                randD = left;
                break;
            case 4:
                randD = right;
                break;
            default:
                break;
        }
        dx = 0;
        dy = 0;
        for (int i = 0; i < 4; i++)
        { // consider a new direction
            setDirection(randD);
           setdxdy(dx, dy, randD);
           if (getWorld()->isValidPos(getX()+dx, getY()+dy, this))
           { // the new direction allows to bot to move
               setDirection(randD);
               moveTo(getX()+dx, getY()+dy);
               return;
           } // try a new direction;
            randD += 90;
        } // return to original direction
        randD += 90;
        setDirection(randD);
        return;
    }
}

void ThiefBot::damage(int damageAmt)
{
    Robot::damage(damageAmt);
    if (!Alive())
    {
        if (goodieHeld != nullptr)
        { // we are a carrying a goodie
            // move the goodie to where the thiefbot died
            goodieHeld->moveTo(getX(), getY());
            goodieHeld->makeVisible();
            goodieHeld->setStolen(false);
        }
    }
}
// MeanThiefBot
MeanThiefBot::MeanThiefBot(int imageID, int startX, int startY, int health, int points, StudentWorld* world) : ThiefBot(imageID, startX, startY, health, points, world)
{}

void MeanThiefBot::doSomething()
{
    int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
     // level number (0, 1, 2, etc.)
    if (ticks < 3)
     ticks = 3;
    if (!Alive())
        return;
    increaseTicks();
    if (getTicks() % ticks != 0)
        return;
    int dx = 0;
    int dy = 0;
    int direction = getDirection();
    setdxdy(dx, dy, direction);
    if (getWorld()->existsClearShotToPlayer(getX(), getY(), dx, dy))
    {
        Actor::fire(direction, getX(), getY());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    ThiefBot::doDifferentSomething();
}

// ThiefBot Factory
ThiefBotFactory::ThiefBotFactory(int imageID, double startX, double startY, ProductType type, StudentWorld* world) :  Actor(imageID, startX, startY, -1, world), factoryType(type)
{
    setVisible(true);
    setHealth(100);
}

void ThiefBotFactory::doSomething()
{
    int count = 0;
    // accounts for a minimum x or y out of bounds
    double minX = getX()-3;
    if (minX < 0)
        minX = 0;
    
    double maxX = getX()+3;
    if (maxX >= VIEW_WIDTH)
        maxX = VIEW_WIDTH-1;
    
    double minY = getY()-3;
    if (minY < 0)
        minY = 0;
    
    double maxY = getY()+3;
    if (maxY >= VIEW_HEIGHT)
        maxY = VIEW_HEIGHT-1;
    
    for (double x = minX; x <= maxX; x++)
    { // check all squares that are within the specified range
        for (double y = minY; y <= maxY; y++)
        {
            Actor* target = getWorld()->getTarget(x, y, this);
            if (target != nullptr && target->countsInFactoryCensus())
            { // the target is a thiefbot and counts in our census
                count++;
            }
        }
    }
    if (count < 3 && getWorld()->getTarget(getX(), getY(), this) == nullptr)
    { // 1 in 50 chance of the factory creating a new thiefbot
        int rand = randInt(1, 50);
        if (rand == 1)
        { // Creating the new bots, depends on the type of factory
            if (factoryType == REGULAR)
            {
                getWorld()->addActor(new ThiefBot(IID_THIEFBOT, getX(), getY(), 5, 10, getWorld()));
                getWorld()->playSound(SOUND_ROBOT_BORN);
                return;
            }
            else if (factoryType == MEAN)
            {
                getWorld()->addActor(new MeanThiefBot(IID_MEAN_THIEFBOT, getX(), getY(), 8, 20, getWorld()));
                getWorld()->playSound(SOUND_ROBOT_BORN);
                return;
            }
        }
    }
}
