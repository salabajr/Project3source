#include "Actor.h"
#include "StudentWorld.h"


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
                // add pea DO Later
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
    Actor *p = getWorld()->getActor(getX(),getY());
    if (p != nullptr && p->canAvatarOverlap() == 2)
    {
        setHealth(0);
        p->setHealth(0);
    }
        
}
