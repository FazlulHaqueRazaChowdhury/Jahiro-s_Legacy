#include "Bullet.h"
#include "raymath.h"


constexpr int FRAME_WIDTH  = 16;
constexpr int FRAME_HEIGHT = 16;
constexpr int MAX_FRAMES   = 4;
constexpr int BULLET_ROW   =2; //  change this number to try other bullets

Bullet::Bullet(Vector2 pos, Vector2 dir)
{
    worldPos = pos;
    direction = Vector2Normalize(dir);

    texture = LoadTexture("characters/All_Fire_Bullet_Pixel_16x16 (1).png");
}

void Bullet::tick(float dt, Vector2 knightWorldPos, float mainCharRL, Vector2 origin)
{
    if (!alive) return;

    gunPos= origin;
    // movement
    worldPos = Vector2Add(worldPos, Vector2Scale(direction, speed * dt));

    // animation timing
    runningTime += dt;
    if (runningTime >= frameTime)
    {
        runningTime = 0.f;
        frame++;
        if (frame >= MAX_FRAMES) frame = 0;
    }

    Vector2 screenPos = Vector2Add(
        Vector2Subtract(worldPos, knightWorldPos),
        gunPos
    );

    // source (from BIG sprite sheet)
    Rectangle source{
        (float)(frame * FRAME_WIDTH),
        (float)(BULLET_ROW * FRAME_HEIGHT),
        (float)FRAME_WIDTH,
        (float)FRAME_HEIGHT
    };

    // destination , to draw on screen 
    Rectangle dest{
        screenPos.x,
        screenPos.y,
        FRAME_WIDTH *.5f,
        FRAME_HEIGHT *.5f
    };
    if(screenPos.x < -FRAME_WIDTH || screenPos.x > GetScreenWidth()+FRAME_WIDTH ||
       screenPos.y < -FRAME_HEIGHT || screenPos.y > GetScreenHeight()+FRAME_HEIGHT){
        UnloadTexture(texture);
        alive = false;
    }
    DrawTexturePro(texture, source, dest, {}, 0.f, WHITE);
}

Rectangle Bullet::getCollisionRec(Vector2 knightWorldPos) const
{
    Vector2 screenPos = Vector2Add(
        Vector2Subtract(worldPos, knightWorldPos),
        Vector2{GetScreenWidth()/2.f, GetScreenHeight()/2.f}
    );

    return Rectangle{
        screenPos.x - FRAME_WIDTH,
        screenPos.y - FRAME_HEIGHT,
          FRAME_WIDTH *.5f,
        FRAME_HEIGHT *.5f
    };
}
