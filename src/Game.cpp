#include "Game.h"
#include "MainScene.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_Log.h>
#include <SDL3_image/SDL_image.h>

Game::Game()
{
}

Game::~Game()
{
    Clear();
}

void Game::run()
{
    while (isRunning)
    {
        if (currentScene)
        {
            auto frameStart = SDL_GetTicksNS();

            SDL_Event event;
            handleEvent(&event);
            update();
            render();
    
            auto frameEnd = SDL_GetTicksNS();
            Uint64 diff = frameEnd - frameStart;
            if (diff < frameTime)
            {
                SDL_DelayNS(frameTime - diff);
                deltaTime = frameTime / 100000000.0f;
            }
            else
            {
                deltaTime = diff / 100000000.0f;
            }
        }
        
    }
    
}

void Game::init()
{
    //初始化SDL
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
       SDL_Log("SDL_Init Error: %s\n" ,SDL_GetError());
       isRunning = false;
    }

    window = SDL_CreateWindow("SDL3", windowWidth, windowHeight, 0);
    if (!window)
    {
       SDL_Log("SDL_InitWindow Error: %s\n" ,SDL_GetError());
       isRunning = false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_Log("SDL_InitRenderer Error: %s\n" ,SDL_GetError());
        isRunning = false;
    }

    //初始化SDL_Mixer
    if (Mix_Init(MIX_INIT_MP3) < 0)
    {
        SDL_Log("SDL_InitMix Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    //打开音频设备
    SDL_AudioSpec spec;
    spec.freq = 44100;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;

    if (Mix_OpenAudio(0, &spec) < 0)
    {
        SDL_Log("SDL_InitMix Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    // 设置音效channel数量
    Mix_AllocateChannels(32);

    // 设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);
    
    currentScene = new MainScene();
    currentScene->init();
}

void Game::Clear()
{
    currentScene->clean();
    delete currentScene;

    //清理SDL_Mixer
    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::ChangeScene(SceneBase *scene)
{
    if (scene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }

    currentScene = scene;
    currentScene->init();
    
}

void Game::handleEvent(SDL_Event *event)
{
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_EVENT_QUIT)
        {
            isRunning = false;
        }
    }
    currentScene->handleEvent(event);
}

void Game::update()
{
    currentScene->update(deltaTime);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    currentScene->render();
    SDL_RenderPresent(renderer);

}