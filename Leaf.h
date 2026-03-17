#ifndef LEAF_H
#define LEAF_H

#include "raylib.h"

class Leaf
{
public:
    Leaf(Vector2 pos, Texture2D* leafTexture); 
    void tick(float deltaTime);

private:
    Texture2D* texture;
    Vector2 position;
    Vector2 velocity;
    
    float frames{0.f};
    float maxFrames{4.f};
    float width;
    float height;
    float updateTime{1.f / 12.f}; 
    float runningTime{0.f};
    
    float angle{0.f};
    float rotationSpeed;
    
    float swayTime;
    float swaySpeed;
    float swayAmount;
    
    // NEW: Give each leaf its own scale for fake 3D depth
    float scale; 
};

#endif