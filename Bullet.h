#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector2 pos, Vector2 dir);
    void tick(float dt, Vector2 knightWorldPos);

    Rectangle getCollisionRec(Vector2 knightWorldPos) const;
    bool alive{true};

private:
    Vector2 worldPos{};
    Vector2 direction{};
    float speed{700.f};
};

#endif
