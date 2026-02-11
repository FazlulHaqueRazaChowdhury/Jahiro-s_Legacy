#include "Enemy.h"
#include "raymath.h"

float totalCol = 9;


Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    maxFrames = 8.f;
    scale = 2.f;
    width = texture.width / maxFrames;
    height = texture.height/totalCol;
    currentCol = 3.f;
    speed = 5.f;
}

void Enemy::tick(float deltaTime)
{
    if (!getAlive() && animDone) return;
    // get toTarget
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    if(velocity.x < 0.f) rightLeft = -1.f;
    else if(velocity.x > 0.f) rightLeft = 1.f;
    DrawText(TextFormat("Velocity asdfasd: %.2f, %.2f", velocity.x, velocity.y), 10, 10, 20, RED);

    if (Vector2Length(velocity) < radius) velocity = {};
    BaseCharacter::tick(deltaTime);
    //velocity
    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
    {
        target->takeDamage(damagePerSec * deltaTime);
        currentCol = 8.f;
    }
    else if(getAlive()){
        maxFrames = 8.f;
        currentCol = 3.f;
    }
    width = texture.width / maxFrames;

}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::respawn(Vector2 pos)
{
    worldPos = pos;
    setAlive(true);
}
void Enemy::takeDamage(){
    currentCol = 7.f;
    maxFrames = 8.f;
    if(deathSound) PlaySound(*deathSound);
    setAlive(false);
}