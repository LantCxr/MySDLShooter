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

    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    SDL_GetTextureSize(projectilePlayerTemplate.texture, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width *= 0.3;
    projectilePlayerTemplate.height *= 0.3;
    projectilePlayerTemplate.speed = 100.0;
}

void MainScene::update(double deltaTime)
{
    keyboardControl(deltaTime);
    updateProjectiles(deltaTime);
}

void MainScene::render()
{
    renderProjectiles();
    SDL_FRect postion = {static_cast<float>(player.pos.x), static_cast<float>(player.pos.y), player.width, player.height};
    //绘制玩家
    SDL_RenderTexture(game.getRenderer(), player.texture, NULL, &postion);
}

void MainScene::clean()
{
    SDL_DestroyTexture(player.texture);
    SDL_DestroyTexture(projectilePlayerTemplate.texture);

    for (auto projectile : projectilePlayerList)
    {
        delete projectile;
    }
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
    
    
    if (keyboardState[SDL_SCANCODE_J])
    {
        auto time = SDL_GetTicks();
        if (time - player.lastShootTime > player.coolDown)
        {
            player.lastShootTime = time;
            shootPlayer();
        }
    }
    
}

void MainScene::shootPlayer()
{
   ProjectilePlayer* projectile = new ProjectilePlayer(projectilePlayerTemplate);
   projectilePlayerList.push_back(projectile);
   projectile->pos.x = player.pos.x + player.width / 2.0 - projectile->width / 2.0;
   projectile->pos.y = player.pos.y - projectile->height;
}

void MainScene::updateProjectiles(double deltaTime)
{
    for(auto it = projectilePlayerList.begin(); it != projectilePlayerList.end();)
    {
        auto projectile = *it;
        projectile->pos.y -= deltaTime * projectile->speed;
        //检查子弹是否超出屏幕
        if (projectile->pos.y < -projectile->height)
        {
            delete projectile;
            //进入下一个位置
            it = projectilePlayerList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MainScene::renderProjectiles()
{
    for (auto projectile : projectilePlayerList)
    {
        SDL_FRect postion = { static_cast<float>(projectile->pos.x), static_cast<float>(projectile->pos.y), projectile->width, projectile->height };
        SDL_RenderTexture(game.getRenderer(), projectile->texture, NULL, &postion);
    }
}
