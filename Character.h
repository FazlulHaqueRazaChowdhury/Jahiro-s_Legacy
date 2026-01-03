#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"
// added by shadman
#include "Bullet.h"
#include <vector>

class Character : public BaseCharacter
{
public:
    Character(int winWidth, int winHeight);
    virtual void tick(float deltaTime) override;
    virtual Vector2 getScreenPos() override;
    Rectangle getWeaponCollisionRec() { return weaponCollisionRec; }
    float getHealth() const { return health; }
    void takeDamage(float damage);
    std::vector<Bullet>& getBullets() { return bullets; }

private:
    int windowWidth{};
    int windowHeight{};
    Texture2D weapon{LoadTexture("")};
    Rectangle weaponCollisionRec{};
    float health{100.f};
    std::vector<Bullet> bullets;
      // added by shadman
    Vector2 facingDir{1.f, 0.f}; // default: facing right

};

#endif
