#define _USE_MATH_DEFINES
#include "MainScene.h"
#include "Object.h"
#include "Game.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <math.h>

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

    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-1.png");
    SDL_GetTextureSize(enemyTemplate.texture, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width *= 0.3;
    enemyTemplate.height *= 0.3;
    enemyTemplate.speed = 20.0;

    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0, 1);

    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    SDL_GetTextureSize(projectileEnemyTemplate.texture, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width *= 0.3;
    projectileEnemyTemplate.height *= 0.3;
    projectileEnemyTemplate.speed = 50.0;
}

void MainScene::update(double deltaTime)
{
    keyboardControl(deltaTime);
    updateProjectiles(deltaTime);
    //绘制敌人
    updateEnemies(deltaTime);
    updateProjectileEnemies(deltaTime);
    updatePlayer(deltaTime);
}

void MainScene::render()
{
    renderProjectiles();
    renderProjectileEnemies();


    if (!isDead)
    {
        SDL_FRect postion = {static_cast<float>(player.pos.x), static_cast<float>(player.pos.y), player.width, player.height};
        //绘制玩家
        SDL_RenderTexture(game.getRenderer(), player.texture, NULL, &postion);
    }

    //生成敌人
    spawnEnemy();
    renderEnemies();

}

void MainScene::clean()
{
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
     if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }

    for (auto projectile : projectilePlayerList)
    {
        delete projectile;
    }

    if (enemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    //清理敌人
    for (auto enemy : enemyList)
    {
        delete enemy;
    }

    if (projectileEnemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }
    
    //清理敌人子弹
    for (auto projectile : projectileEnemyList)
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
    if (isDead)
    {
        return;
    }
    
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
            bool isHit = false;
            for (auto enemy : enemyList)
            {
                SDL_Rect enemyRect = 
                { 
                    static_cast<int>(enemy->pos.x),
                    static_cast<int>(enemy->pos.y),
                    static_cast<int>(enemy->width),
                    static_cast<int>(enemy->height) 
                };
                SDL_Rect projectileRect ={
                    static_cast<int>(projectile->pos.x),
                    static_cast<int>(projectile->pos.y),
                    static_cast<int>(projectile->width),
                    static_cast<int>(projectile->height)
                };
                if (SDL_HasRectIntersection(&enemyRect, &projectileRect))
                {
                    enemy->hp -= projectile->damage;
                    delete projectile;
                    it = projectilePlayerList.erase(it);
                    isHit = true;
                    break;
                }
            }
            if (!isHit)
            {
               ++it;
            }
        }
    }
}

void MainScene::enemyExplode(Enemy *enemy)
{
    delete enemy;
}

void MainScene::updatePlayer(float deltaTime)
{
    if(isDead){
        return;
    }

    if (player.hp <= 0)
    {
        isDead = true;
    }

    for(auto enemy : enemyList)
    {
        SDL_Rect enemyRect = 
        { 
            static_cast<int>(enemy->pos.x),
            static_cast<int>(enemy->pos.y),
            static_cast<int>(enemy->width),
            static_cast<int>(enemy->height) 
        };
        SDL_Rect playerRect ={
            static_cast<int>(player.pos.x),
            static_cast<int>(player.pos.y),
            static_cast<int>(player.width),
            static_cast<int>(player.height)
        };
        if (SDL_HasRectIntersection(&enemyRect, &playerRect))
        {
            player.hp -= 1;
            enemy->hp = 0; //敌人死亡
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

void MainScene::spawnEnemy()
{
    if (dis(gen) > 1 / 1000.f)
    {
        return;
    }
    
    Enemy* enemy = new Enemy(enemyTemplate);
    enemy->pos.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->pos.y = -enemy->height;
    enemyList.push_back(enemy);
}

void MainScene::updateEnemies(double deltaTime)
{
    for (auto it = enemyList.begin(); it != enemyList.end();)
    {
        auto enemy = *it;
        enemy->pos.y += deltaTime * enemy->speed;
        if (enemy->pos.y > game.getWindowHeight())
        {
            delete enemy;
            it = enemyList.erase(it);
        }
        else
        {
            if ((SDL_GetTicks() - enemy->lastShootTime) > enemy->coolDown && !isDead)
            {
                spawnProjectileEnemy(enemy);
                enemy->lastShootTime = SDL_GetTicks();
            }

            if (enemy-> hp <= 0)
            {
                enemyExplode(enemy);
                it = enemyList.erase(it);
            }
            else
            {
                ++it;
            }
            
            
        }
    }
}

void MainScene::renderEnemies()
{
    for (auto enemy : enemyList)
    {
        SDL_FRect postion = { static_cast<float>(enemy->pos.x), static_cast<float>(enemy->pos.y), enemy->width, enemy->height };
        SDL_RenderTexture(game.getRenderer(), enemy->texture, NULL, &postion);
    }
}

void MainScene::spawnProjectileEnemy(Enemy* enemy)
{
    ProjectileEnemy * projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectileEnemyList.push_back(projectile);
    projectile->pos.x = enemy->pos.x + enemy->width / 2.0 - projectile->width / 2.0;
    projectile->pos.y = enemy->pos.y + enemy->height;

    float x = (player.pos.x + player.width / 2.0) - (enemy->pos.x + enemy->width / 2.0);
    float y = (player.pos.y + player.height / 2.0) - (enemy->pos.y + enemy->height / 2.0);
    auto length = sqrt(x * x + y * y);
    
    if (length != 0) 
    {
        x /= length;
        y /= length;
        projectile->direction = {x, y};
    } else {
        projectile->direction = {0, 1}; // 默认向下
    }

}

void MainScene::updateProjectileEnemies(double deltaTime)
{
   for (auto it = projectileEnemyList.begin(); it != projectileEnemyList.end();)
    {
        auto projectile = *it;
        projectile->pos.y += deltaTime * projectile->speed * projectile->direction.y;
        projectile->pos.x += deltaTime * projectile->speed * projectile->direction.x;
        if (projectile->pos.x < 0 ||
            projectile->pos.x > game.getWindowWidth() - projectile->width ||
            projectile->pos.y > game.getWindowHeight() - projectile->height ||
            projectile->pos.y < 0
        )
        {
            delete projectile;
            it = projectileEnemyList.erase(it);
        }
        else
        {
            SDL_Rect enemyRect = {
                static_cast<int>(projectile->pos.x),
                static_cast<int>(projectile->pos.y),
                static_cast<int>(projectile->width),
                static_cast<int>(projectile->height)
            };
            SDL_Rect playerRect = {
                static_cast<int>(player.pos.x),
                static_cast<int>(player.pos.y),
                static_cast<int>(player.width),
                static_cast<int>(player.height)
            };
            if (SDL_HasRectIntersection(&enemyRect, &playerRect))
            {
                player.hp -= projectile->damage;
                delete projectile;
                it = projectileEnemyList.erase(it);
            }
            else{
                ++it;
            }
        }
    } 
}

void MainScene::renderProjectileEnemies()
{
   for (auto ProjectileEnemy : projectileEnemyList)
    {
        SDL_FRect postion = { 
            static_cast<float>(ProjectileEnemy->pos.x),
            static_cast<float>(ProjectileEnemy->pos.y), 
            ProjectileEnemy->width, 
            ProjectileEnemy->height 
        };
        float angle = atan2f(ProjectileEnemy->direction.y, ProjectileEnemy->direction.x) * 180 / M_PI - 90;
        SDL_RenderTextureRotated(game.getRenderer(), ProjectileEnemy->texture, NULL, &postion, angle, NULL, SDL_FLIP_NONE);
    }
}
