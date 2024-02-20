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
    virtual bool blocks() = 0;
protected:
    void setHealth(int health);
private:
    int m_health;
    StudentWorld* m_world;
};

class Wall : public Actor
{
public:
    Wall(int imageID, double startX, double startY, StudentWorld* world);
    void doSomething();
    bool blocks() {return true;}
private:

};


class Avatar : public Actor
{
public:
    // need to add PEAS
    Avatar(int imageID, double startX, double startY, StudentWorld* world);
    void doSomething();
    bool blocks() {return true;}
private:
};
 
#endif // ACTOR_H_

