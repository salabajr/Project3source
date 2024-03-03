#ifndef ACTOR_H_
#define ACTOR_H_

class StudentWorld;

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int dir, StudentWorld* world);
    
    //Is our actor still alive?
    bool Alive() const;
    
    //How many hitpoints does our Actor have left?
    int getHealth() const;
    
    // Typically use to deal damage from a pea
    virtual void updateHealth(int health);
    
    // Action to perform each tick
    virtual void doSomething() = 0;
    
    // gets the Actor's world
    StudentWorld* getWorld() const;
    
    virtual bool canPushMarbles() const {return false;}
    
    // 0 player cant cross  1: player can pass through in all cases 2: player can pass through sometimes
    virtual int canAvatarOverlap() const {return 0;}
    void setHealth(int health);
    
    // pushing actors we can push aka marbles
    virtual void push(int direction, double X, double Y ) {return;}
    
    // restricts interactions with other actors
    virtual bool blocks() const {return false;}
    
    // Is this actor something that is affected by a pea?
    virtual bool canTakeDamage() const {return false;}
    void makeVisible() {setVisible(true);}
    
    // Can a pit eat this actor?
    virtual bool swallows() const {return false;}
    
    virtual void damage(int damageAmt);
    virtual void setStolen(bool status) {return;}
    // Can a thiefbot pick up this actor?
    virtual bool stealable() const {return false;}
    
    // is a ThiefBot?
    virtual bool countsInFactoryCensus()  {return false;}
protected:
    // only called by derived classes
    void moveActor(double x, double y, int dx, int dy);
    
    // shoot a pea
    void fire(int direction, double x, double y);
    
    // determine movement
    void setdxdy(int &x, int&y, int direction);
private:
    int m_health;
    StudentWorld* m_world;
};

class Wall : public Actor
{
public:
    Wall(int imageID, double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocks() const {return true;}

private:

};

class Avatar : public Actor
{
public:
    Avatar(int imageID, double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    int getAvatarDirection() {return getDirection();}
    virtual bool canTakeDamage() const {return true;}
    virtual bool canPushMarbles() const {return true;}
    int getAmmo() {return numPeas;}
    void addAmmo(int amount) {numPeas += amount;}
    virtual void damage(int damageAmt);
private:
    void moveAvatar(int direction);
    int numPeas;
};
 
class Marble : public Actor
{
public:
    Marble(int imageID, double startX, double startY, StudentWorld* world);
    void push(int direction, double X, double Y);
    virtual void doSomething() {return;}
    virtual int canAvatarOverlap()  const {return 2;}
    bool canTakeDamage() const {return true;}
};

class Pit : public Actor
{
public:
    Pit(int imageID, double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool swallows() const {return true;}
};

class Item : public Actor
{
public:
    Item(int imageID, double startX, double startY, StudentWorld* world, int increasePoints);
    virtual int canAvatarOverlap() const {return 1;}
    // only goodies are stealable and it depends on if its already stolen!
    virtual bool stealable() const {return !stolen;}
    virtual void setStolen(bool status);
protected:
    bool updateItem();
private:
    bool stolen;
    int pointBonus;
};

class Crystal : public Item
{
public:
    Crystal(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Item(imageID, startX, startY, world, increasePoints) {}
    void doSomething();
    virtual bool stealable() const {return false;}
};
class extraLife : public Item
{
public:
    extraLife(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Item(imageID, startX, startY, world, increasePoints) {}
    virtual void doSomething();
};

class restoreHealth : public Item
{
public:
    restoreHealth(int imageID, double startX, double startY, StudentWorld* world, int increasePoints) : Item(imageID, startX, startY, world, increasePoints) {}
    virtual void doSomething();
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
    virtual int canAvatarOverlap() const {return 1;}
private:
    bool step2();
// This dealt with the pea skipping the first space i.e. the pea moved before appearing to the display
    bool firstTick;
    int m_direction;
};
class Exit : public Actor
{
public:
    Exit(int imageID, double startX, double startY, StudentWorld* world);
    int canAvatarOverlap() const {return 1;}
    void exposeExit();
    void doSomething();
private:
    bool m_exitExposed;
};

class Robot : public Actor
{
public:
    Robot(int imageID, double startX, double startY, int startDir, int hitPoints, int score, StudentWorld* world);
    virtual void doSomething();
    virtual bool canTakeDamage() const {return true;}
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const {return false;}
protected:
    int getTicks() {return m_ticks;}
    void increaseTicks() {m_ticks++;}
private:
    virtual void doDifferentSomething() = 0;
    int m_score;
    int m_ticks;
};

class RageBot : public Robot
{
public:
    RageBot(int startX, int startY, int startDir, StudentWorld* world);
private:
    virtual void doDifferentSomething();
};

class ThiefBot : public Robot
{
public:
    ThiefBot(int imageID, int startX, int startY, int health, int points, StudentWorld* world);
    virtual bool countsInFactoryCensus()  {return true;}
    virtual void damage(int damageAmt);
protected:
    virtual void doDifferentSomething();
private:
    int distanceBeforeTurning;
    Actor* goodieHeld;
};

class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(int imageID, int startX, int startY, int health, int points, StudentWorld* world);
    virtual void doSomething();
private:
};

class ThiefBotFactory : public Actor
{
public:
    enum ProductType { REGULAR, MEAN };
    ThiefBotFactory(int imageID, double startX, double startY, ProductType type, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocks() const {return true;}
private:
    ProductType factoryType;
};
#endif // ACTOR_H_

