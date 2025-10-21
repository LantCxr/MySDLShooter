#include "Game.h"
#include "MainScene.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_Log.h>
#include <SDL3_image/SDL_image.h>

void Game::backgroundUpdate(float deltaTime)
{
     nearStars.offset += nearStars.speed * deltaTime;
    if (nearStars.offset >= 0)
    {
        nearStars.offset -= nearStars.height;
    }

    farStars.offset += farStars.speed * deltaTime;
    if (farStars.offset >= 0){
        farStars.offset -= farStars.height;
    }
}

void Game::backgroundRender()
{
    // 渲染远处的星星
    for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height){
        for (int posX = 0; posX < getWindowWidth(); posX += farStars.width){
            SDL_FRect ds = {posX, posY, farStars.width, farStars.height};
            SDL_RenderTexture(renderer, farStars.texture, NULL, &ds);
        }
    }
    // 渲染近处的星星
    for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
    {
        for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
        {
            SDL_FRect dstRect = {posX, posY, nearStars.width, nearStars.height};
            SDL_RenderTexture(renderer, nearStars.texture, nullptr, &dstRect);
        }   
    }
}

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

    //初始化背景卷轴
    nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
    SDL_GetTextureSize(nearStars.texture, &nearStars.width, &nearStars.height);
    nearStars.width /= 2;
    nearStars.height /= 2;

    farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
    SDL_GetTextureSize(farStars.texture, &farStars.width, &farStars.height);
    farStars.width /= 2;
    farStars.height /= 2;
    farStars.speed = 20; //远处移动较慢
    
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

    //清理背景资源
    if (nearStars.texture != nullptr)
    {
       SDL_DestroyTexture(nearStars.texture);
    }
    
    if (farStars.texture != nullptr)
    {
        SDL_DestroyTexture(farStars.texture);
    }

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
    backgroundUpdate(deltaTime);
    currentScene->update(deltaTime);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // 渲染远处的星星
    backgroundRender();
    currentScene->render();
    SDL_RenderPresent(renderer);

}