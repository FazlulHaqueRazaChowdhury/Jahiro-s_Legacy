#include "Grass.h"
#include "raymath.h"
#include <math.h> 

Grass::Grass(Vector2 pos) : worldPos(pos)
{
    swayTime = (float)GetRandomValue(0, 100); 
    swaySpeed = (float)GetRandomValue(100, 200) / 100.f; 
    swayAmount = (float)GetRandomValue(4, 10); 
    
    // 1. TALLER GRASS: Increased from (8, 14) so it stands out more
    height = (float)GetRandomValue(18, 28);
    
    // 2. VIBRANT COLOR: Boosted the green values so it pops off the screen
    int greenShade = GetRandomValue(150, 210); 
    grassColor = Color{ 40, (unsigned char)greenShade, 40, 255 }; 
}

void Grass::tick(float deltaTime, Vector2 playerWorldPos)
{
    swayTime += swaySpeed * deltaTime;
    float currentSway = sin(swayTime) * swayAmount; 

    Vector2 screenPos = Vector2Subtract(worldPos, playerWorldPos);
    Vector2 root = screenPos;
    
    // Slightly larger shadow to ground the thicker blades
    DrawEllipse(root.x, root.y, 5.f, 2.5f, Color{10, 20, 10, 150});

    // 3. THICKER BLADES: Increased line thickness (3.0f and 2.0f)
    Vector2 tipMid = { root.x + currentSway, root.y - height };
    DrawLineEx(root, tipMid, 3.0f, grassColor); 

    Vector2 tipLeft = { root.x + currentSway - 6.f, root.y - (height * 0.7f) };
    DrawLineEx(root, tipLeft, 2.0f, grassColor); 

    Vector2 tipRight = { root.x + currentSway + 6.f, root.y - (height * 0.8f) };
    DrawLineEx(root, tipRight, 2.0f, grassColor); 
}