#include "Prop.h"
#include "raymath.h"

Prop::Prop(Vector2 pos, Texture2D* tex, float maxF,float r, float sc):
    worldPos(pos),
    texture(*tex),
    maxFrames(maxF),
    rowMax(r),
    scale(sc)
{

}

void Prop::Render(Vector2 knightPos, float deltaTime)
{
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrames)
        {
            row++;
            frame = 0;
        }    
        if(row > rowMax){
            row = 0;
        }
        
    }
    Vector2 screenPos{ Vector2Subtract(worldPos, knightPos) };
    // DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
    Rectangle source{frame * width, height*row, width, height};
    Rectangle dest{screenPos.x, screenPos.y, scale * width, scale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}

Rectangle Prop::getCollisionRec(Vector2 knightPos)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    // DrawRectangleLines(screenPos.x, screenPos.y,width, height, RED);
    return Rectangle{
        screenPos.x,
        screenPos.y,
        width ,
        height 
    };
}