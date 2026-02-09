#ifndef PROP_H
#define PROP_H
#include "raylib.h"

class Prop
{
public:
    Prop(Vector2 pos, Texture2D tex, float maxF, float r, float sc);
    void Render(Vector2 knightPos, float deltaTime);
    Rectangle getCollisionRec(Vector2 knightPos);
private:
Vector2 worldPos{};
Texture2D texture{};
    float scale{1.5f};
    float frame{0.f};
    float row{0.f};
    float maxFrames{};
    float rowMax{};
    float updateTime{1.f / 12.f};
    float runningTime{};
    float width{ texture.width / maxFrames };   
    float height{ texture.height / (rowMax + 1)};
};
#endif