#ifndef GRASS_H
#define GRASS_H

#include "raylib.h"

class Grass
{
public:
    Grass(Vector2 pos);
    // NEW: Pass the player's position every frame
    void tick(float deltaTime, Vector2 playerWorldPos);

private:
    Vector2 worldPos; // Changed from 'position'
    
    float swayTime;
    float swaySpeed;
    float swayAmount; 
    
    float height;
    Color grassColor;
};

#endif