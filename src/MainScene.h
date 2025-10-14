#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "SceneBase.h"
#include "Object.h"
#include <list>

class Game;

class MainScene: public SceneBase
{
public:
    MainScene();
    ~MainScene();

    virtual void init() override;
    virtual void update(double deltaTime) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event* event) override;

    void keyboardControl(double deltaTime);

    void shootPlayer();
    void updateProjectiles(double deltaTime);
    void renderProjectiles();

private:
    Player player;

    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer*> projectilePlayerList;
    Game &game;

};

#endif