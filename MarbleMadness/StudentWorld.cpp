#include "StudentWorld.h"
#include "GameConstants.h"
#include <sstream>
#include <iostream>
#include <string>
#include <list>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_avatar = nullptr;
    m_actors.clear();
    
}

void StudentWorld::addActor(Actor* actor)
{
    m_actors.push_back(actor);  // Add the pointer to the end of the list
}



int StudentWorld::init()
{
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel("level00.txt");
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Could not find level00.txt data file\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level\n";
        for (int x = 0; x <= VIEW_WIDTH; x++)
        {
            for (int y = 0; y <= VIEW_HEIGHT; y++)
            {
                Level::MazeEntry ge = lev.getContentsOf(x,y);
                switch (ge) {
                    case Level::player:
                        m_avatar = new Avatar(IID_PLAYER, x, y, this); // adding the avatar to the maze
                        break;
                    case Level::wall:
                        addActor(new Wall(IID_WALL, x, y, this));
                        break;
                    case Level::marble:
                        addActor(new Marble(IID_MARBLE, x, y, this));
                        break;
                    case Level::pit:
                        addActor(new Pit(IID_PIT, x, y, this));
                    default:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{

    // Update the Game Status Line

    //NEED TO BE IMPLEMENTED
    //updateDisplayText(); // update the score/lives/level text at screen top

     // The term "actors" refers to all robots, the player, Goodies,
     // Marbles, Crystals, Pits, Peas, the exit, etc.
     // Give each actor a chance to do something



    m_avatar->doSomething();
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->Alive())
        {
         // ask each actor to do something (e.g. move)
         (*it)->doSomething();
        }
        if (!m_avatar->Alive())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }

    // IMPLEMENT LATER
    //if (thePlayerCompletedTheCurrentLevel())
    //{
    //increaseScoreAppropriately();
    //return GWSTATUS_FINISHED_LEVEL;
    }


    // Remove newly-dead actors after each tick
    removeDeadGameObjects(); // delete dead game objects
    // Reduce the current bonus for the Level by one
    //increaseScore(-1);
    // If the player has collected all of the crystals on the level, then we
    // must expose the exit so the player can advance to the next level
    //if (thePlayerHasCollectedAllOfTheCrystalsOnTheLevel())
    //exposeTheExitInTheMaze(); // make the exit Active
    // return the proper result
    //if (thePlayerDiedDuringThisTick())
    //return GWSTATUS_PLAYER_DIED;
    //if (thePlayerCompletedTheCurrentLevel())
    //    return GWSTATUS_FINISHED_LEVEL;
    //{
    //increaseScoreAppropriately();

    ostringstream oss;
    oss.fill('0');
    oss << "Score: " << setw(7) << getScore() << "  Level: " << setw(2) << getLevel();
    oss.fill(' ');
    oss << "  Lives: " << setw(2) << getLives() << "  Health: " << setw(3) << m_avatar->getHealth()/0.2 << "%";
    oss << "  Ammo: " << setw(3); // << m_avatar->getAmmo();
    oss << "  Bonus: " << setw(4);

    setGameStatText(oss.str());
    // This code is here merely to allow the game to build, run, and terminate after you type q
    //setGameStatText("Game will end when you type q");

    return GWSTATUS_CONTINUE_GAME;

}



void StudentWorld::cleanUp()
{
    delete m_avatar;
    m_avatar = nullptr;
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
    {
        delete *it;
    }
    m_actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::isValidPos(double x, double y)
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
        {
            if ((*it)->canAvatarOverlap() == 0)
                return false;
            if ((*it)->canAvatarOverlap() == 1)
                return true;
            if ((*it)->canAvatarOverlap() == 2)
            {
                int direction = m_avatar->getDirection();
                if (direction == Actor::down)
                {
                    if (isEmpty(x, y-1))
                        return true;
                }
                else if (direction == Actor::up)
                {
                    if (isEmpty(x, y+1))
                        return true;
                }
                else if (direction == Actor::right)
                {
                    if (isEmpty(x+1, y))
                        return true;
                }
                else if (direction == Actor::left)
                {
                    if (isEmpty(x-1, y))
                        return true;
                }
                return false;
            }
        }
        
    }
    return true;
}
bool StudentWorld::isEmpty(double x, double y)
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->isObstacle())
            return true;
        else if ((*it)->getX() == x && (*it)->getY() == y)
            return false;
    }
    return true;
}

void StudentWorld::pushActors(double x, double y, int direction)
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
            (*it)->push(direction, x, y);
    }
    return;
}
Actor* StudentWorld::getActor(double x, double y)
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
            return *it;
    }
    return nullptr;
}

void StudentWorld::removeDeadGameObjects()
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (!(*it)->Alive())
        {
            delete (*it);
            it = m_actors.erase(it);
        }
            
    }
}
