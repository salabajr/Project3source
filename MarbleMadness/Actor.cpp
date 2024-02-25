#include "Actor.h"
#include "StudentWorld.h"

void Actor::moveActor(int direction)
{
    return;
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
void Actor::setHealth(int health)
{
    m_health = health;
}
StudentWorld* Actor::getWorld()
{

    return m_world;
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
    switch (directionKey) {
        case left:
            setDirection(left);
            if (getWorld()->isValidPos(getX()-1, getY()))
            {
                getWorld()->pushActors(getX()-1, getY(), left);
                moveTo(getX()-1, getY());
            }
            break;
        case right:
            setDirection(right);
            if (getWorld()->isValidPos(getX()+1, getY()))
            {
                getWorld()->pushActors(getX()+1, getY(), right);
                moveTo(getX()+1, getY());
            }
            break;
        case up:
            setDirection(up);
            if (getWorld()->isValidPos(getX(), getY()+1))
            {
                getWorld()->pushActors(getX(), getY()+1, up);
                moveTo(getX(), getY()+1);
            }
            break;
        case down:
            setDirection(down);
            if (getWorld()->isValidPos(getX(), getY()-1))
            {
                getWorld()->pushActors(getX(), getY()-1, down);
                moveTo(getX(), getY()-1);
            }
            break;
        default:
            break;
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
                fire(getDirection(), getX(), getY());
                break;
            case KEY_PRESS_ESCAPE:
                setHealth(0);
                break;
            default:
                moveAvatar(ch);
                break;
        }
    }
}
void Avatar::fire(int direction, double x, double y)
{
    switch (direction)
    {
        case right:
            getWorld()->addActor(new Pea(IID_PEA, x+1, y, right, getWorld()));
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            break;
        case left:
            getWorld()->addActor(new Pea(IID_PEA, x-1, y, left, getWorld()));
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            break;
        case up:
            getWorld()->addActor(new Pea(IID_PEA, x, y+1, up, getWorld()));
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            break;
        case down:
            getWorld()->addActor(new Pea(IID_PEA, x, y-1, down, getWorld()));
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            break;
        default:
            return;
    }
    numPeas--;
}
// PEA
Pea::Pea(int imageID, double startX, double startY, int direction, StudentWorld* world): Actor(IID_PEA, startX, startY, -1, world), m_direction(direction) {
    setVisible(true);
    setHealth(100);
}

void Pea::doSomething()
{
    if (!Alive())
        return;

    // TODO ACCOUNT FOR OVERLAP OF ROBOT AND FACTORY
    if (step2())
    {
        switch (m_direction) {
            case right:
                moveTo(getX()+1, getY());
                step2();
                break;
            case left:
                moveTo(getX()-1, getY());
                step2();
                break;
            case up:
                moveTo(getX(), getY()+1);
                step2();
                break;
            case down:
                moveTo(getX(), getY()-1);
                step2();
                break;
            default:
                return;
        }
    }
    
    
}
bool Pea::step2()
{

    Actor* target = getWorld()->getActor(getX(), getY(), this);
    if (target != nullptr && target->canTakeDamage())
    {

        target->updateHealth(-2);
        setHealth(0);
        return false;
    }
    else if (target != nullptr && !(target->canTakeDamage()) && target->canAvatarOverlap() == 0 && target->blocks() && target != this)
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
    switch (direction) {
        case right:
            if (getWorld()->isEmpty(X+1, Y))
            {
                moveTo(X+1, Y);
                return;
            }
            break;
        case left:
            if (getWorld()->isEmpty(X-1, Y))
            {
                moveTo(X-1, Y);
                return;
            }
            break;
        case up:
            if (getWorld()->isEmpty(X, Y+1))
            {
                moveTo(X, Y+1);
                return;
            }
            break;
        case down:
            if (getWorld()->isEmpty(X, Y-1))
            {
                moveTo(X, Y-1);
                return;
            }
            break;
        default:
            break;
    }
    return;
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
        std::cerr << "hello1";
        exposeExit();
    }
}


void Exit::exposeExit()
{
    std::cerr << "hello";
    makeVisible();
    m_exitExposed = true;
    getWorld()->playSound(SOUND_REVEAL_EXIT);
}
