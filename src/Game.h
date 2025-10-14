#ifndef GAME_H
#define GAME_H

#include "SceneBase.h"
#include <SDL3/SDL.h>

class Game
{
public:
    ~Game();

    void run();
    void init();
    void Clear();
    void ChangeScene(SceneBase* scene);

    void handleEvent(SDL_Event* event);
    void update();
    void render();

    inline static Game& getInstance()
    {
        static Game instance;
        return instance;
    }

    inline SDL_Window* getWindow() { return window; }
    inline SDL_Renderer* getRenderer() { return renderer; }

    inline int getWindowWidth() { return windowWidth; }
    inline int getWindowHeight() { return windowHeight; }

private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    SceneBase* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
    bool isRunning = true;

    int FPS = 60;
    Uint64 frameTime = 100000000 / FPS;
    double deltaTime = 0;

};


#endif
