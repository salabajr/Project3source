#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void addActor(Actor* actor);
    bool isValidPos(double x, double y, Actor* p);
    bool isEmpty(double x, double y);
    void pushActors(double x, double y, int direction);
    Actor* getActor(double x, double y, Actor* p);
    Actor* getTarget(double x, double y, Actor* p);
    void removeDeadGameObjects();
    void updateDisplayText();
    Avatar* getAvatar() {return m_avatar;}
    bool noCrystals() {return m_crystals == 0;}
    int getCrystals() {return m_crystals;}
    void pickupCrystal() {m_crystals--;}
    void completedLevel() {m_levelDone = true;}
    int getBonus() {return m_bonus;}
    bool existsClearShotToPlayer(int x, int y, int dx, int dy);

private:
    std::list<Actor*> m_actors;
    Avatar* m_avatar;
    int m_bonus;
    int m_crystals;
    bool m_levelDone;

};

#endif // STUDENTWORLD_H_
