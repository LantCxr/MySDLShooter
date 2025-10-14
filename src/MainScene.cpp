#include "MainScene.h"
#include "Object.h"
#include "Game.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

MainScene::MainScene()
    :game(Game::getInstance())
{
}

MainScene::~MainScene()
{
}

void MainScene::init()
{
    //加载图片
    player.texture = IMG_LoadTexture(game.getRenderer(),"assets/image/SpaceShip.png");
    SDL_GetTextureSize(player.texture, &player.width, &player.height);
    player.width *= 0.3;
    player.height *= 0.3;
    player.speed = 50.0;
    //设置初始位置
    player.pos.x = game.getWindowWidth() / 2.0 - player.width / 2.0;
    player.pos.y = game.getWindowHeight() - player.height;

    SDL_Log("MainScene init x:%d  y:%d speed:%f", player.pos.x, player.pos.y, player.speed);
}

void MainScene::update(double deltaTime)
{
    keyboardControl(deltaTime);
}

void MainScene::render()
{
    SDL_FRect postion = {static_cast<float>(player.pos.x), static_cast<float>(player.pos.y), player.width, player.height};
    //绘制玩家
    SDL_RenderTexture(game.getRenderer(), player.texture, NULL, &postion);
}

void MainScene::clean()
{
}

void MainScene::handleEvent(SDL_Event* event)
{
    
}

void MainScene::keyboardControl(double deltaTime)
{
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_D])
    {
        player.pos.x += deltaTime * player.speed;   
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        player.pos.x -= deltaTime * player.speed;  
    }
    if (keyboardState[SDL_SCANCODE_W])
    {
        player.pos.y -= deltaTime * player.speed;   
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        player.pos.y += deltaTime * player.speed;  
    }

    if (player.pos.x < 0)
    {
        player.pos.x = 0;
    }
    if (player.pos.x > game.getWindowWidth() - player.width)
    {
        player.pos.x = game.getWindowWidth() - player.width;
    }
    if (player.pos.y < 0)
    {
        player.pos.y = 0;
    }
    if (player.pos.y > game.getWindowHeight() - player.height)
    {
        player.pos.y = game.getWindowHeight() - player.height;
    }
    
}
