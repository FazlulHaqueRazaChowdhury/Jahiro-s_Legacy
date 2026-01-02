#include "Bullet.h"
#include "raymath.h"

Bullet::Bullet(Vector2 pos, Vector2 dir)
{
    worldPos = pos;
    direction = Vector2Normalize(dir);
}

void Bullet::tick(float dt, Vector2 knightWorldPos)
{
    if (!alive) return;

    worldPos = Vector2Add(worldPos, Vector2Scale(direction, speed * dt));

   Vector2 screenPos = Vector2Add(
    Vector2Subtract(worldPos, knightWorldPos),
    Vector2{
        GetScreenWidth() / 2.f,
        GetScreenHeight() / 2.f
    }
);

DrawCircleV(screenPos, 4.f, RED);


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
    return Rectangle{screenPos.x - 4, screenPos.y - 4, 8, 8};
}
