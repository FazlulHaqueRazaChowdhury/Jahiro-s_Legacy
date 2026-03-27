#ifndef HEALTH_H
#define HEALTH_H

#include "raylib.h"
#include <string>
#include "Character.h"
#include "Enemy.h"

class Health {
public:
    Health(Character* character, Enemy* enemy, Vector2 pos, float scale);
    
    Character *jahiro;
    Enemy *enemy;
    
    Texture2D healthBarTexture = LoadTexture("nature_tileset/04.png");
    Texture2D head = LoadTexture("characters/Head.png");
    
    float width = healthBarTexture.width / 7.f;
    // Note: If the bottom edge of the health bar gets cut off or shows the next row, 
    // change the 14.f to 15.f or 16.f until it perfectly frames the single bar.
    float height = healthBarTexture.height / 14.f; 
    
    // Inside Health.h
    float currCol{1.f}; // Start at full health (column 1)
    float currRow{0.f}; // Use the first row style
    Vector2 pos{10.f, 10.f};
    float scale{1.f};
    
    void tick(float deltaTime);
};

#endif // HEALTH_H