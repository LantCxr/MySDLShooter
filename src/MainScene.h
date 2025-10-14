#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "SceneBase.h"
#include "Object.h"

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

private:
    Player player;
    Game &game;

};

#endif