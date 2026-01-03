#include "Bullet.h"
#include "raymath.h"


constexpr int FRAME_WIDTH  = 16;
constexpr int FRAME_HEIGHT = 16;
constexpr int MAX_FRAMES   = 4;
constexpr int BULLET_ROW   =13; //  change this number to try other bullets

Bullet::Bullet(Vector2 pos, Vector2 dir)
{
    worldPos = pos;
    direction = Vector2Normalize(dir);

    texture = LoadTexture("characters/All_Fire_Bullet_Pixel_16x16 (1).png");
}

void Bullet::tick(float dt, Vector2 knightWorldPos)
{
    if (!alive) return;

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
        Vector2{
            GetScreenWidth() / 2.f,
            GetScreenHeight() / 2.f
        }
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
        screenPos.x - FRAME_WIDTH,
        screenPos.y - FRAME_HEIGHT,
        FRAME_WIDTH * 2.f,
        FRAME_HEIGHT * 2.f
    };

    DrawTexturePro(texture, source, dest, {0, 0}, 0.f, WHITE);
}

Rectangle Bullet::getCollisionRec(Vector2 knightWorldPos) const
{
    Vector2 screenPos = Vector2Add(
        Vector2Subtract(worldPos, knightWorldPos),
        Vector2{
            GetScreenWidth() / 2.f,
            GetScreenHeight() / 2.f
        }
    );

    return Rectangle{
        screenPos.x - 8,
        screenPos.y - 8,
        16,
        16
    };
}
