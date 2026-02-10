#ifndef PARTICLE_H
#define PARTICLE_H
#include "raylib.h"

class Particles
{
public:
    void Render(Vector2 knightPos, float deltaTime, float rightLeft);
private:
Texture2D texture{LoadTexture("nature_tileset/dust.png")};
    float scale{3.5f};
    float frame{0.f};
    float maxFrames{8.f};
    float rowMax{1.f};
    float updateTime{1.f / 12.f};
    float runningTime{};
    float width{ (float) texture.width / maxFrames };   
    float height{(float) texture.height};
};
#endif