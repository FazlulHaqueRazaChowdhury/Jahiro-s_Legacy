#include "BaseCharacter.h"
#include "raymath.h"
#include <string>

BaseCharacter::BaseCharacter()
{
    
}

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;

}

Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale
    };
}

void BaseCharacter::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;
    Rectangle source{frame * width, currentCol * height, rightLeft * width, height};
    Rectangle dest{getScreenPos().x , getScreenPos().y , scale * width, scale * height};
    float backward = 1.f;
    // update animation frame
    
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        runningTime = 0.f;
        
        if (!getAlive()) 
        {
            // If dead, play the animation but freeze on the final frame
            if (frame < maxFrames - 1.f) 
            {
                frame += 1.f;
            }
        } 
        else 
        {
            // Normal looping logic for when alive
            frame += backward;
            if (frame > maxFrames)
                frame = 0;
            else if (frame < 0.f)
                frame = maxFrames;
        }
    }

        if(velocity.x < 0.f) backward = -1.f;
        else if(velocity.x > 0.f) backward = 1.f;
    if (Vector2Length(velocity) != 0.0)
    {
        texture = run;
        updateTime = 1.f/12.f;
        maxFrames = 8.f;
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
        runFlag = true;

    }
    else
    {
        runFlag = false;
        texture = idle;
        updateTime = 1.f/10.f; 
    }

    velocity = {};

    // draw the character

    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}