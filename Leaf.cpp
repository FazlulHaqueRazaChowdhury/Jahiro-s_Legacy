#include "Leaf.h"
#include "raymath.h"
#include <math.h> 

Leaf::Leaf(Vector2 pos, Texture2D* leafTexture) : position(pos), texture(leafTexture)
{
    width = (float)texture->width / 5.f;
    height = (float)texture->height;

    // 1. DEPTH (PARALLAX EFFECT)
    // Random size between 1.0 and 3.0
    scale = (float)GetRandomValue(10, 30) / 10.f; 

    // Bigger leaves (closer to camera) move much faster. Smaller leaves drift in the background.
    velocity.x = (scale * 120.f) * -1.f; // Speed scales with size!
    velocity.y = (float)GetRandomValue(-15, 15);

    rotationSpeed = (float)GetRandomValue(-80, 80); 
    frames = (float)GetRandomValue(0, 4);
    
    swayTime = (float)GetRandomValue(0, 100); 
    swaySpeed = (float)GetRandomValue(100, 300) / 100.f; 
    swayAmount = (float)GetRandomValue(10, 40);         
    
    // 2. FIX INITIAL SPAWN CLUMPING
    // Spread them randomly across the ENTIRE screen and way off-screen to the right
    position.x = (float)GetRandomValue(0, 2000); 
    // Spread them across the entire height of the screen (0 to 720)
    position.y = (float)GetRandomValue(-50, 770); 
}

void Leaf::tick(float deltaTime)
{
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    swayTime += swaySpeed * deltaTime;
    float currentSwayY = sin(swayTime) * swayAmount; 
    
    angle += rotationSpeed * deltaTime;

    // 3. FIX THE "SINGLE POINT" RESET
    if (position.x < -(width * scale)) 
    {
        // Don't just reset to 1280! Add a random delay distance (up to 800 pixels) 
        // so they stagger back onto the screen at different times.
        position.x = 1280.f + (width * scale) + (float)GetRandomValue(0, 800); 
        
        // Pick a new random height for its next pass
        position.y = (float)GetRandomValue(-50, 770); 
    }

    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        runningTime = 0.f;
        frames++;
        if (frames > maxFrames) frames = 0.f;
    }

    // Draw using the leaf's unique scale
    DrawTexturePro(
        *texture, 
        Rectangle{frames * width, 0.f, width, height},
        Rectangle{position.x, position.y + currentSwayY, width * scale, height * scale}, 
        Vector2{width * scale / 2.f, height * scale / 2.f},
        angle,
        WHITE);
}