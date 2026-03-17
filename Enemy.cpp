#include "Enemy.h"
#include "raymath.h"
float totalCol = 9;


Enemy::Enemy(Vector2 pos, Texture2D *idle_texture, Texture2D *run_texture, float health,float scale)
{
    worldPos = pos;
    texture = *idle_texture;
    idle = *idle_texture;
    run = *run_texture;
    maxFrames = 8.f;
    width = texture.width / maxFrames;
    height = texture.height/totalCol;
    currentCol = 3.f;
    speed = 5.f;
    this->health = health;
    setScale(scale);
}

void Enemy::tick(float deltaTime)
{
    if (!getAlive()){
        damagePerSec = 0.f;
        velocity = {};
        currentCol = 7.f;
        BaseCharacter::tick(deltaTime);
    }
    else{
        damagePerSec = 10.f;
        currentCol = 3.f;
    
            // get toTarget
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    if(velocity.x < 0.f) rightLeft = -1.f;
    else if(velocity.x > 0.f) rightLeft = 1.f;
    // DrawText(TextFormat("Velocity asdfasd: %.2f, %.2f", velocity.x, velocity.y), 10, 10, 20, RED);

    if (Vector2Length(velocity) < radius) velocity = {};
    //velocity
    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
    {
        undoMovement();
        target->takeDamage(damagePerSec * deltaTime);
        currentCol = 8.f;
    }

    width = texture.width / maxFrames;
    Vector2 shadowPos = Vector2{getScreenPos().x+width*getScale()*0.5f-shadow.width,getScreenPos().y+height*getScale()-shadow.height-20.f};
    DrawTextureEx(shadow,shadowPos,0.f,2.f, WHITE);
    //drawing rectangle lines for shadows
    // DrawRectangleLines(shadowPos.x-shadow.width*0.5f,shadowPos.y-shadow.height*0.5f,shadow.width,shadow.height, RED);
    //Drawing a cirle at the feet of the enemy for debugging
    dust.Render(Vector2{
        getScreenPos().x+width*getScale()*0.5f,getScreenPos().y+height*getScale()
    }, deltaTime,rightLeft);
    //Drawing Dust particles at the feet of the enemy for debugging
    // DrawRectangleLines(getScreenPos().x,getScreenPos().y,width*getScale(),height*getScale(), RED);
    // DrawText(TextFormat("Health: %.2f", health), getScreenPos().x, getScreenPos().y - 20.f, 20, RED);
    // DrawText(TextFormat("Scale: %.2f", getScale()), getScreenPos().x, getScreenPos().y - 40.f, 20, RED);
    BaseCharacter::tick(deltaTime);
    // DrawCircleV(Vector2{getScreenPos().x+width*getScale()*0.5f,getScreenPos().y+height*getScale()}, 5.f, BLUE);
    }



}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::respawn(Vector2 pos)
{
    worldPos = pos;
    health = 100.f;
    setAlive(true);
}
void Enemy::takeDamage(){
    currentCol = 7.f;
    maxFrames = 8.f;
    if(deathSound) PlaySound(*deathSound);
    health -= 20.f;

    if(health <= 0.f)
    setAlive(false);
}