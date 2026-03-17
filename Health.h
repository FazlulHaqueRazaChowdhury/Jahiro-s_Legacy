#ifndef HEALTH_H
#define HEALTH_H
#include "raylib.h"
#include <string>
#include "Character.h"
#include "Enemy.h"

class Health {

public:
    Health(Character* character, Enemy* enemy,Vector2 pos, float scale);
    Character *jahiro;
    Enemy *enemy;
    Texture2D healthBarTexture = LoadTexture("nature_tileset/04.png");
    Texture2D head = LoadTexture("characters/Head.png");
    float width =healthBarTexture.width/7.f;
    float height = healthBarTexture.width/16.f;
    float currCol{1.f};
    float currRow{0.f};
    Vector2 pos{10.f, 10.f};
    float scale{1.f};
    void tick(float deltaTime);
};

#endif // HEALTH_H