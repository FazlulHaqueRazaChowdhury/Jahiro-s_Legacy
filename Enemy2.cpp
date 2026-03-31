#include "Enemy2.h"
#include "raymath.h"
Enemy2::Enemy2(Vector2 pos, Texture2D *run_texture, Texture2D *attack_texture, Texture2D *hit_texture, Texture2D *death_texture, float health, float scale)
{
    worldPos = pos;
    enemtexture = *run_texture;
    runText = *run_texture;
    attackText = *attack_texture;
    hitText = *hit_texture;
    deathText = *death_texture;
    
    enemmaxFrame = 8.f;
    textureWidth = enemtexture.width / enemmaxFrame;
    textureHeight = enemtexture.height;
    
    speed = 2.5f; 
    this->health = health;
    setScale(scale);
    setAlive(true);
}

Vector2 Enemy2::getScreenPos()
{
    // 1. Get the world distance between them
    Vector2 distance = Vector2Subtract(worldPos, target->getWorldPos());
    
    // 2. ADD the player's screen position to correct the camera offset!
    return Vector2Add(distance, target->getScreenPos());
}

Rectangle Enemy2::getCollisionRec2()
{
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        textureWidth,
        textureHeight
    };
}

Rectangle Enemy2::getCollisionRec()
{
    Vector2 screenPos = getScreenPos();
    float paddingX = (textureWidth * scale) * 0.4f;  
    float paddingY = (textureHeight * scale) * 0.45f; 
    float boxWidth = (textureWidth * scale) * 0.2f;   
    float boxHeight = (textureHeight * scale) * 0.25f; 

    return Rectangle{
        screenPos.x + paddingX,
        screenPos.y + paddingY,
        boxWidth,
        boxHeight
    };
}
void Enemy2::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;
    Rectangle myBox = getCollisionRec();
    Rectangle playerBox = target->getCollisionRec();
    Vector2 myCenter = { myBox.x + (myBox.width / 2.0f), myBox.y + (myBox.height / 2.0f) };
    Vector2 playerCenter = { playerBox.x + (playerBox.width / 2.0f), playerBox.y + (playerBox.height / 2.0f) };
    Vector2 toTarget = Vector2Subtract(playerCenter, myCenter);
    velocity = Vector2{0.f, 0.f};

    bool isColliding = CheckCollisionRecs(playerBox, myBox);
    bool isPlayingHitAnim = (enemtexture.id == hitText.id);
    if (getAlive() && !isPlayingHitAnim)
    {
        if (isColliding)
        {
            enemtexture = attackText;
            enemmaxFrame = 8.f; 
            target->takeDamage(damagePerSec * deltaTime);
        }
        else 
        {
            velocity = toTarget; 
            worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
            
            enemtexture = runText;
            enemmaxFrame = 8.f;

            if (velocity.x < 0.f) rightLeft = -1.f;
            else if (velocity.x > 0.f) rightLeft = 1.f;
        }
    }
    //Animation
    runningTime += deltaTime;
    if (runningTime >= updateTime) 
    {
        runningTime = 0.f;
        
        if (!getAlive()) 
        {
            if (frame < enemmaxFrame - 1) frame++; 
        } 
        else 
        {
            frame++;
            if (frame >= enemmaxFrame) 
            {
                frame = 0;
                if (isPlayingHitAnim) 
                {
                    enemtexture = runText;
                    enemmaxFrame = 8.f;
                }
            }
        }
    }

    textureWidth = enemtexture.width / enemmaxFrame;
    Rectangle source{frame * textureWidth, 0.f, rightLeft * textureWidth, textureHeight};
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * textureWidth, scale * textureHeight};
    //for shadow
    float shadowScale = scale * 1.2f; 
    float shadowX = getScreenPos().x + ((scale * textureWidth) / 2.0f) - ((shadow.width * shadowScale) / 2.0f);
    float shadowY = getScreenPos().y + (scale * textureHeight) - (shadow.height * shadowScale) - (25.f * scale); 
    
    if (getAlive()) 
    {
        DrawTextureEx(shadow, Vector2{shadowX, shadowY}, 0.f, shadowScale, Color{255, 255, 255, 180});
    }
    DrawTexturePro(enemtexture, source, dest, Vector2{0.f, 0.f}, 0.f, WHITE);
}

void Enemy2::respawn(Vector2 pos, float newMaxHealth, float newSpeed)
{
    worldPos = pos;
    health = newMaxHealth; 
    speed = newSpeed;      
    enemtexture = runText;
    setAlive(true);
}
void Enemy2::takeDamage(int* kill)
{
    if(deathSound) PlaySound(*deathSound);
    if(hitSound && health > 0.f) PlaySound(*hitSound);
    health -= 20.f;

    if(health <= 0.f)
    {
        (*kill)++; 
        setAlive(false);
        enemtexture = deathText; 
        enemmaxFrame = 4.f;  // Set max frames to 4 for death
        frame = 0.f; 
    }
    else 
    {
        enemtexture = hitText;
        enemmaxFrame = 4.f;  // Set max frames to 4 for hit
        frame = 0.f; 
    }
}

Vector2 Enemy2::getPushVector(std::vector<Enemy2>& allEnemies)
{
    Vector2 pushVector = {0.0f, 0.0f};
    int overlapCount = 0;

    for (Enemy2& otherEnemy : allEnemies)
    {
        if (&otherEnemy == this || !otherEnemy.getAlive()) continue;

        if (CheckCollisionRecs(getCollisionRec(), otherEnemy.getCollisionRec()))
        {
            Vector2 dir = Vector2Subtract(worldPos, otherEnemy.getWorldPos());

            if (Vector2Length(dir) == 0.0f) {
                dir = { (float)GetRandomValue(-10, 10), (float)GetRandomValue(-10, 10) };
            }

            float random_dir_change = (GetRandomValue(0, 1) == 0) ? -1.0f : 1.0f;
            float rotationAngle = random_dir_change * (PI / 4.0f);
            dir = Vector2Rotate(dir, rotationAngle);

            pushVector = Vector2Add(pushVector, Vector2Normalize(dir));
            overlapCount++;
        }
    }
    
    if (overlapCount > 0) {
        pushVector = Vector2Normalize(pushVector);
    }

    return pushVector;
}