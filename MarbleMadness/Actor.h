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
    virtual bool canPushMarbles() const {return false;}
    virtual int canAvatarOverlap() {return 0;} // 0 walls, bots, pits 1: goodies 2: marbles
    void setHealth(int health);
    virtual void push(int direction, double X, double Y ) {return;}
    virtual bool isObstacle() {return false;}
    virtual bool blocks() {return false;}
    virtual bool canTakeDamage() {return false;}
    void makeVisible() {setVisible(true);}
    virtual void damage(int damageAmt);
protected:
    void moveActor(double x, double y, int dx, int dy);
    
    
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
    //bool isObstacle() {return true;}

private:

};

class Avatar : public Actor
{
public:
    Avatar(int imageID, double startX, double startY, StudentWorld* world);
    void doSomething();
    int getAvatarDirection() {return getDirection();}
    void moveAvatar(int direction);
    bool canTakeDamage() {return true;}
    virtual bool canPushMarbles() const {return true;}
    int getAmmo() {return numPeas;}
    void addAmmo(int amount) {numPeas += amount;}
private:
    void fire(int direction, double x, double y);
    int numPeas;
};
 
class Marble : public Actor
{
public:
    Marble(int imageID, double startX, double startY, StudentWorld* world);
    void push(int direction, double X, double Y);
    void doSomething() {return;}
    int canAvatarOverlap() {return 2;}
    bool canTakeDamage() {return true;}
private:
};

class Pit : public Actor
{
public:
    bool isPit(){return true;}
    Pit(int imageID, double startX, double startY, StudentWorld* world);
    void doSomething();
    bool isObstacle() {return true;}
private:
};

class Item : public Actor
{
public:
    Item(int imageID, double startX, double startY, StudentWorld* world, int increasePoints);
protected:
    bool updateItem();
    int canAvatarOverlap() {return 1;}
private:
    int pointBonus;
};

class Crystal : public Item
{
public:
    Crystal(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Item(imageID, startX, startY, world, increasePoints) {}
    void doSomething();
};
class extraLife : public Item
{
public:
    extraLife(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Item(imageID, startX, startY, world, increasePoints) {}
    void doSomething();
};

class restoreHealth : public Item
{
public:
    restoreHealth(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Item(imageID, startX, startY, world, increasePoints) {}
    void doSomething();
    
};

class Ammo : public Item
{
public:
    Ammo(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Item(imageID, startX, startY, world, increasePoints) {}
    void doSomething();
};

class Pea : public Actor
{
public:
    Pea (int imageID, double startX, double startY, int direction, StudentWorld* world);
    void doSomething();
    bool step2();
private:
    int m_direction;
};
class Exit : public Actor
{
public:
    Exit(int imageID, double startX, double startY, StudentWorld* world) : Actor(imageID, startX, startY, -1, world)
    {
        setHealth(1);
        setVisible(false);
        m_exitExposed = false;
    }
    int canAvatarOverlap() {return 1;}
    void exposeExit();
    void doSomething();
private:
    bool m_exitExposed;
};

class Robot : public Actor
{
public:
    Robot(int imageID, double startX, double startY, int startDir, int hitPoints, int score, StudentWorld* world);
    //virtual void doSomething() const;
    virtual bool canTakeDamage() {return true;}
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const {return false;}
//    virtual bool blocks() {return true;}
    //virtual bool needsClearShot() const;
    //virtual int shootingSound() const;

      // Does this robot shoot?
    virtual bool isShootingRobot() const {return false;}
private:
    int m_score;
};

class RageBot : public Robot
{
public:
    RageBot(int startX, int startY, int startDir, StudentWorld* world);
    virtual void doSomething();
    virtual bool isShootingRobot() const {return true;}
private:
    int m_ticks;
};

#endif // ACTOR_H_

