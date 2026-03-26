#ifndef ENEMY_H2
#define ENEMY_H2

#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include <vector> 

class Enemy2 
{
public:
    Enemy2(Vector2 pos, Texture2D *run_texture, Texture2D *attack_texture, Texture2D *hit_texture, Texture2D *death_texture, float health, float scale);
    
    // Core Functions
    void tick(float deltaTime);
    void takeDamage();
    void respawn(Vector2 pos);
    void undoMovement() { worldPos = worldPosLastFrame; }
    
    // Getters & Setters
    void setTarget(Character* character) { target = character; }
    void setDeathSound(Sound& sound) { deathSound = &sound; }
    void setAlive(bool isAlive) { alive = isAlive;}
    void setScale(float s) { scale = s; }   
    void setWorldPos(Vector2 pos) { worldPos = pos; }
    
    bool getAlive() const { return alive; }
    Vector2 getWorldPos() const { return worldPos; }
    Vector2 getScreenPos();
    Rectangle getCollisionRec2();
    Rectangle getCollisionRec();
    Vector2 getPushVector(std::vector<Enemy2>& allEnemies);

    // Textures & Audio
    Texture2D enemtexture;
    Texture2D runText;
    Texture2D attackText;
    Texture2D hitText;
    Texture2D deathText;
    Texture2D shadow{LoadTexture("characters/shadow.png")};
    Sound* deathSound{nullptr};

    // Public Stats
    float health{100.f};
    float damagePerSec{10.f};
    float radius{5.f};
    float enemmaxFrame{8.f};
    float textureHeight;
    float textureWidth;
    Character* target;

private:
    // Variables previously inherited from BaseCharacter
    Vector2 worldPos{0.f, 0.f};
    Vector2 worldPosLastFrame{0.f, 0.f};
    Vector2 velocity{0.f, 0.f};
    float speed{1.f};
    float scale{1.f};
    
    // Animation Variables
    float frame{0.f};
    float runningTime{0.f};
    float updateTime{1.f / 12.f};
    float rightLeft{1.f};
    bool alive{true};
};

#endif // ENEMY_H2