
#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H
#include "raylib.h"
#include "Particle.h"
class BaseCharacter
{
public:
    BaseCharacter();
    Vector2 getWorldPos() { return worldPos; }
    void undoMovement();
    Rectangle getCollisionRec();
    virtual void tick(float deltaTime);
    virtual Vector2 getScreenPos() = 0;
    bool getAlive() { return alive; }
    void setAlive(bool isAlive) { alive = isAlive; }
    Particles dust;
    float runFlag{false};
    float runningTime{};
    float updateTime{1.f / 15.f};
    bool animDone{false};
    float rightLeft{1.f};
    void setScale(float s){scale=s;}
    float getScale(){return scale;}
protected:
    Texture2D texture{LoadTexture("characters/idle.png")};
    Texture2D idle{LoadTexture("characters/idle.png")};
    Texture2D run{LoadTexture("characters/walk.png")};
    Texture2D attack{LoadTexture("characters/idle.png")};
    Texture2D runattack{LoadTexture("characters/idle.png")};
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};
    // 1 : facing right, -1 : facing left

    // animation variables
    float frame{};
    float maxFrames{2.f};
    float speed{4.f};
    float  maxIdleFrame{1.f};
    float width{(float)texture.width/maxFrames};
    float height{(float)texture.height};
    float scale{.08f};
    Vector2 velocity{};
    bool isAttacking{false};
    bool mouseMovement{false};
    float currentCol{1.f};
private:
    bool alive{true};
};

#endif