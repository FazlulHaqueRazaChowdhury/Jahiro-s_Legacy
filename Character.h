#ifndef CHARACTER_H
#define CHARACTER_H
#include <vector>
#include "Bullet.h"
#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character(int winWidth, int winHeight, Texture2D *bulletTexture);
    virtual void tick(float deltaTime) override;
    virtual Vector2 getScreenPos() override;
    Rectangle getWeaponCollisionRec() { return weaponCollisionRec; }
    Texture2D weapon{LoadTexture("characters/gun.png")};
    Texture2D head{LoadTexture("characters/head.png")};
    Texture2D shadow{LoadTexture("characters/shadow.png")};
    float getHealth() const { return health; }
    void takeDamage(float damage);
    std::vector<Bullet>& getBullets() { return bullets; }
    Rectangle getCharCollisionRec();
    float mainCharRL{1.f};
    void setShootSound(Sound* sound) { shootSound = sound; }
    Texture2D *BulletTex;
private:
    int windowWidth{};
    int windowHeight{};
    Rectangle weaponCollisionRec{};
    Vector2 ssPos{};
    float health{10000.f};
     std::vector<Bullet> bullets;
     Sound* shootSound{nullptr};

     float recoilOffset=0.f;
     float recoilStrength=8.f;
     float recoilReturnSpeed=25.f;
};

#endif
