#ifndef ENEMY_H
#define ENEMY_H
#include "raylib.h"
#include "BaseCharacter.h"
#include "Character.h"

class Enemy : public BaseCharacter
{
public:
    Enemy(Vector2 pos, Texture2D *idle_texture, Texture2D *run_texture, float health,float scale);
    virtual void tick(float deltaTime) override;
    void setTarget(Character* character) { target = character; }
    virtual Vector2 getScreenPos() override;
    Texture2D shadow{LoadTexture("characters/shadow.png")};
    float rL{1};
    // added by shadman for respawn 
    //void setTarget(Character* character) { target = character; }
    void respawn(Vector2 pos);
    void setDeathSound(Sound& sound){deathSound=&sound;}
    virtual void takeDamage();
    float health{100.f};
    Character* target;
    float damagePerSec{10.f};
    float radius{25.f};
    Sound* deathSound{nullptr};


private:
 
};
#endif // ENEMY_H