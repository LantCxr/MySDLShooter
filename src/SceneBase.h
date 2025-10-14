#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <SDL3/SDL_events.h>

class SceneBase
{
public:
    SceneBase() = default;
    virtual ~SceneBase() = default;

    virtual void init() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event* event) = 0;
};

#endif