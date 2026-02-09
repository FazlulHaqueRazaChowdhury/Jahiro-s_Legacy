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
    //   DrawRectangleLines(getScreenPos().x, getScreenPos().y, width * scale, height * scale, RED);
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
    Rectangle source{frame * width, height, rightLeft * width, height};
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height};
    float backward = 1.f;
    // update animation frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame += backward;
        runningTime = 0.f;
        if (frame > maxFrames)
            frame = 0;
        else if (frame < 0.f)
            frame = maxFrames;
    }
        Vector2 mousePos = GetMousePosition();
        Vector2 relativePos = {600,360};
        if(velocity.x < 0.f) backward = -1.f;
        else if(velocity.x > 0.f) backward = 1.f;
    if (Vector2Length(velocity) != 0.0)
    {
        // set worldPos = worldPos + velocity
        texture = run;
        updateTime = 1.f/16.f;
        maxFrames = 8.f;
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
        // velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;    

    }
    else
    {

        texture = idle;
        maxFrames = 2.f;
        updateTime = 1.f/10.f;

    }

    velocity = {};

    // draw the character

    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}