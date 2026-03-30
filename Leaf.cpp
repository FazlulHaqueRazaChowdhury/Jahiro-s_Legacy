#include "Leaf.h"
#include "raymath.h"
#include <math.h> 

Leaf::Leaf(Vector2 pos, Texture2D* leafTexture, Texture2D* leafTexture2) : position(pos), texture1st(*leafTexture),texture2nd(*leafTexture2)
{
    width = (float)texture1st.width / 5.f;
    height = (float)texture1st.height;
    scale = (float)GetRandomValue(10, 30) / 10.f; 

  
    velocity.x = (scale * 120.f) * -1.f; // Speed scales with size!
    velocity.y = (float)GetRandomValue(-15, 15);

    rotationSpeed = (float)GetRandomValue(-80, 80); 
    frames = (float)GetRandomValue(0, 4);
    
    swayTime = (float)GetRandomValue(0, 100); 
    swaySpeed = (float)GetRandomValue(100, 300) / 100.f; 
    swayAmount = (float)GetRandomValue(10, 40);         
    

    position.x = (float)GetRandomValue(0, 2000); 
    
    position.y = (float)GetRandomValue(-50, 770); 
}

void Leaf::tick(float deltaTime,float map)
{
    if(map == 1.f) texture = texture1st;
    else texture = texture2nd;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    swayTime += swaySpeed * deltaTime;
    float currentSwayY = sin(swayTime) * swayAmount; 
    
    angle += rotationSpeed * deltaTime;


    if (position.x < -(width * scale)) 
    {
        position.x = 1280.f + (width * scale) + (float)GetRandomValue(0, 800); 
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
        texture, 
        Rectangle{frames * width, 0.f, width, height},
        Rectangle{position.x, position.y + currentSwayY, width * scale, height * scale}, 
        Vector2{width * scale / 2.f, height * scale / 2.f},
        angle,
        WHITE);
}