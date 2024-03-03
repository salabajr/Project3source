#include "Actor.h"
#include "StudentWorld.h"

void Actor::moveActor(double x, double y, int dx, int dy)
{
    if (getWorld()->isValidPos(x+dx, y+dy, this))
    {
        moveTo(x+dx, y+dy);
        return;
    }
    else
    {
        setDirection(getDirection() + 180);
        return;
    }
}
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
StudentWorld* Actor::getWorld()
{

    return m_world;
}

void Actor::fire(int direction, double x, double y)
{
    int dx = 0, dy = 0;
    setdxdy(dx, dy, direction);
    getWorld()->addActor(new Pea(IID_PEA, x+dx, y+dy, direction, getWorld()));
}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// AVATAR
Avatar::Avatar(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, 0, world)
{
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
    {
        getWorld()->pushActors(getX()+dx, getY()+dy, directionKey);
        moveTo(getX()+dx, getY()+dy);
    }
}
void Avatar::doSomething()
{
    if (!Alive())
        return;
    int ch;
    if(getWorld()->getKey(ch))
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
                {
                    Actor::fire(getDirection(), getX(), getY());
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    numPeas--;
                }
                break;
            case KEY_PRESS_ESCAPE:
                setHealth(0);
                break;
            default:
                //moveAvatar(ch);
                break;
        }
    }
}

void Avatar::damage(int damageAmt)
{
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
    {
        firstTick = false;
        return;
    }
    // TODO ACCOUNT FOR OVERLAP OF ROBOT AND FACTORY
    if (step2())
    {
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
    {

        target->damage(-2);
        setHealth(0);
        return false;
    }
    Actor* wall = getWorld()->getActor(getX(), getY(), this);
    if (wall != nullptr && !(wall->canTakeDamage()) && wall->canAvatarOverlap() == 0  && wall->blocks() && wall != this)
    {
        setHealth(0);
        return false;
        // TODO ACCOUNT FOR OVERLAP OF ROBOT AND FACTORY
    }
    return true;
}

// WALL
Wall::Wall(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world)
{
    setVisible(true);
    setHealth(9999);
}
    
void Wall::doSomething()
{
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
    {
        moveTo(X+dx, Y+dy);
        return;
    }
}

// PIT
Pit::Pit(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world)
{
    setVisible(true);
    setHealth(99999);
}

void Pit::doSomething()
{
    if (!Alive())
        return;
    Actor *p = getWorld()->getActor(getX(),getY(), this);
    if (p != nullptr && p->canAvatarOverlap() == 2) //marble
    {
        setHealth(0);
        p->setHealth(0);
        p = nullptr;
    }
}

// Item
Item::Item(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Actor(imageID, startX, startY, -1, world), pointBonus(increasePoints)
{
    setVisible(true);
    setHealth(9999);
}
bool Item::updateItem()
{
    if (!Alive())
        return false;
    Avatar *p = getWorld()->getAvatar();
    if (p->getX() == getX() && p->getY() == getY())
    {
        getWorld()->increaseScore(pointBonus);
        setHealth(0);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        return true;
    }
    return false;
}

// Crystal

void Crystal::doSomething()
{
    if (updateItem())
        getWorld()->pickupCrystal();
}

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

void Exit::doSomething()
{
    Avatar* p = getWorld()->getAvatar();
    if (p != nullptr && p->getX() == getX() && p->getY() == getY() && getWorld()->getCrystals() <= 0)
    {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->completedLevel();
        getWorld()->increaseScore(getWorld()->getBonus());
    }
    if (getWorld()->getCrystals() <= 0 && m_exitExposed == false)
    {
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
    doDifferentSomething();
}

void Actor::setdxdy(int &dx, int&dy, int direction)
{
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
    {
        Actor::fire(direction, getX(), getY());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
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
    {
        setHealth(0);
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(m_score);
    }
}

// THIEFBOT

ThiefBot::ThiefBot(int imageID, int startX, int startY, int health, int points, StudentWorld* world) : Robot(imageID, startX, startY, right, health, points, world), memory(0)
{
    distanceBeforeTurning = randInt(1, 6);
}

void ThiefBot::doDifferentSomething()
{
    Actor *p = getWorld()->getActor(getX(),getY(), this);
    if (p != nullptr && p->stealable() > 0 && memory == 0) //goodie
    {
        int pickup = randInt(1, 10);
        if (pickup == 1)
        {
            memory = p->stealable();
            p->setHealth(0);
            p = nullptr;
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            return;
        }
    }
    int dx = 0;
    int dy = 0;
    int direction = getDirection();
    setdxdy(dx, dy, direction);
    if (distanceBeforeTurning > 0 && getWorld()->isValidPos(getX()+dx, getY()+dy, this))
    {
        moveTo(getX()+dx, getY()+dy);
        distanceBeforeTurning--;
        return;
    }
    else
    {
        distanceBeforeTurning = randInt(1, 6);
        int randD = randInt(1, 4);
        switch (randD) {
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
        {
           setdxdy(dx, dy, randD);
           if (getWorld()->isValidPos(getX()+dx, getY()+dy, this))
           {
               setDirection(randD);
               return;
           }
            randD += 90;
        }
        randD += 90;
        return;
    }
}

void ThiefBot::damage(int damageAmt)
{
    Robot::damage(damageAmt);
    if (!Alive())
    {
        if (memory > 1)
           switch (memory)
           {
               case 1:
                   getWorld()->addActor(new extraLife(IID_EXTRA_LIFE, getX(), getY(), getWorld(), 1000));
                   break;
               case 2:
                   getWorld()->addActor(new restoreHealth(IID_RESTORE_HEALTH, getX(), getY(), getWorld(), 500));
                   break;
               case 3:
                   getWorld()->addActor(new Ammo(IID_AMMO, getX(), getY(), getWorld(), 100));
                   break;
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
ThiefBotFactory::ThiefBotFactory(int imageID, double startX, double startY, int type, StudentWorld* world) :  Actor(imageID, startX, startY, -1, world), factoryType(type)
{    
    setVisible(true);
    setHealth(999);
}

void ThiefBotFactory::doSomething()
{
    int count = 0;
    
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
    {
        for (double y = minY; y <= maxY; y++)
        {
            Actor* target = getWorld()->getTarget(x, y, this);
            if (target != nullptr && target->countsInFactoryCensus())
            {
                count++;
            }
        }
    }
    if (count < 3 && getWorld()->getTarget(getX(), getY(), this) == nullptr)
    {
        int rand = randInt(1, 50);
        if (rand == 1)
        {
            if (factoryType == 0)
            {
                getWorld()->addActor(new ThiefBot(IID_THIEFBOT, getX(), getY(), 5, 10, getWorld()));
                getWorld()->playSound(SOUND_ROBOT_BORN);
                return;
            }
            else if (factoryType == 1)
            {
                getWorld()->addActor(new MeanThiefBot(IID_MEAN_THIEFBOT, getX(), getY(), 8, 20, getWorld()));
                getWorld()->playSound(SOUND_ROBOT_BORN);
                return;
            }
        }
    }
}
