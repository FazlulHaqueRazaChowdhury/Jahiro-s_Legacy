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
    int windowWidth{};
    int windowHeight{};
    Texture2D* BulletTex;
    float health{100.f};
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
    void setHealth(float heal);
    //bullet
    int currentAmmo{50};
    int maxAmmo{50};
    bool isReloading{false};
    float reloadTime{1.5f}; 
    float reloadTimer{0.f};

    int getCurrentAmmo() const { return currentAmmo; }
    int getMaxAmmo() const { return maxAmmo; }
    bool getIsReloading() const { return isReloading; }
    void setReloadSound(Sound* sound) { reloadSound = sound; }
 

private:
    Rectangle weaponCollisionRec{};
    Vector2 ssPos{};
     std::vector<Bullet> bullets;
     Sound* shootSound{nullptr};

     float recoilOffset=0.f;
     float recoilStrength=8.f;
     float recoilReturnSpeed=25.f;
     Sound* reloadSound{nullptr};
};

#endif
