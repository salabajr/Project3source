#ifndef ACTOR_H_
#define ACTOR_H_

class StudentWorld;

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int dir, StudentWorld* world)
    : GraphObject(imageID, startX, startY, dir, 1.0), m_world(world)
    {}
    bool Alive() const;
    int getHealth() const;
    virtual void updateHealth(int health);
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    virtual int canAvatarOverlap() {return 0;} // 0 walls, bots, pits 1: goodies 2: marbles
    virtual bool isMarble() {return false;}
    void setHealth(int health);
    virtual void push(int direction, double X, double Y ) {return;}
private:
    int m_health;
    StudentWorld* m_world;
};

class Wall : public Actor
{
public:
    Wall(int imageID, double startX, double startY, StudentWorld* world);
    void doSomething();
    virtual int blocks() {return 1;}

private:

};



class Avatar : public Actor
{
public:
    // need to add PEAS
    Avatar(int imageID, double startX, double startY, StudentWorld* world);
    void doSomething();
    bool blocks() {return true;}
    int getAvatarDirection() {return getDirection();}
    void moveAvatar(int direction);
private:
};
 
class Marble : public Actor
{
public:
    Marble(int imageID, double startX, double startY, StudentWorld* world);
    void push(int direction, double X, double Y);
    bool isMarble() {return true;}
    void doSomething() {return;}
    int canAvatarOverlap() {return 2;}
private:
};
#endif // ACTOR_H_

