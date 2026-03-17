#ifndef ENEMY2_H
#define ENEMY2_H

#include "Enemy.h"

class Enemy2 : public Enemy
{
public:
    // We pass in all the separate animation strips
    Enemy2(Vector2 pos, Texture2D *idleTex, Texture2D *walkTex, Texture2D *attackTex, Texture2D *deathTex, Texture2D *hitTex, float health, float scale);
    
    virtual void tick(float deltaTime) override;
    virtual void takeDamage() override;

private:
    Texture2D *skelIdle;
    Texture2D *skelWalk;
    Texture2D *skelAttack;
    Texture2D *skelDeath;
    Texture2D *skelHit;

    float hitTimer{0.f}; // Used to play the "Take Hit" animation briefly
};

#endif