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
void Avatar::doSomething()
{
    if (!Alive())
        return;
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch) {
            case KEY_PRESS_LEFT:
                if (getWorld()->isValidPos(getX()-1, getY()))
                {
                    setDirection(left);
                    moveTo(getX()-1, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                if (getWorld()->isValidPos(getX()+1, getY())){
                    setDirection(right);
                    moveTo(getX()+1, getY());
                }
                break;
            case KEY_PRESS_UP:
                if (getWorld()->isValidPos(getX(), getY()+1)){
                    setDirection(up);
                    moveTo(getX(), getY()+1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getWorld()->isValidPos(getX(), getY()-1)){
                    setDirection(down);
                    moveTo(getX(), getY()-1);
                }
                break;
            case KEY_PRESS_SPACE:
                // add pea DO Later
                break;
            case KEY_PRESS_ESCAPE:
                setHealth(0);
                break;
            default:
                break;
        }
    }
        
}

// WALL
Wall::Wall(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world)
{
    setVisible(true);
}
void Wall::doSomething()
{
    return;
}

