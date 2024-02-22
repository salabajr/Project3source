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
    virtual ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    void addActor(Actor* actor);
    bool isValidPos(double x, double y);
    bool isEmpty(double x, double y);
    void pushActors(double x, double y, int direction);
    Actor* getActor(double x, double y);
    void removeDeadGameObjects();
    



private:
    std::list<Actor*> m_actors;
    Avatar* m_avatar;
};

#endif // STUDENTWORLD_H_
