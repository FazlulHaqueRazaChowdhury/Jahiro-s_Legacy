#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

class Bullet
{
public:
    Bullet(Vector2 pos, Vector2 dir);
    void tick(float dt, Vector2 knightWorldPos, float mainCharRL, Vector2 origin);
    Vector2 gunPos{};
    Rectangle getCollisionRec(Vector2 knightWorldPos) const;
    bool alive{true};

private:
    Vector2 worldPos{};
    Vector2 direction{};
    float speed{700.f};
// for bullet texture
  Texture2D texture{};
  int frame{0};
  float runningTime{0.f};
 float frameTime{0.08f};

};

#endif
