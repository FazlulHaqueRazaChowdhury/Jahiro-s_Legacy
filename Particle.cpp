#include "Particle.h"
#include "raymath.h"


void Particles::Render(Vector2 knightPos, float deltaTime, float rightLeft)
{
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrames)
        {

            frame = 0;
        }    
    }
    Vector2 screenPos = knightPos;
    // DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
    Rectangle source{frame * width , height, width*rightLeft, height};
    Rectangle dest{screenPos.x - (width * scale * 0.5f), screenPos.y - (height * scale * 0.5f) , scale * width , scale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}

