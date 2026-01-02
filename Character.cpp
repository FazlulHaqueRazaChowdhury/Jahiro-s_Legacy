#include "Character.h"
#include "raymath.h"

Character::Character(int winWidth, int winHeight) :
    windowWidth(winWidth),
    windowHeight(winHeight)
{
    width = texture.width / maxFrames;
    height = texture.height;
}

Vector2 Character::getScreenPos()
{
    return Vector2{
        static_cast<float>(windowWidth) / 2.0f - scale * (0.5f * width),
        static_cast<float>(windowHeight) / 2.0f - scale * (0.5f * height)
    };
} 

void Character::tick(float deltaTime)
{
    if (!getAlive()) return;

    /*if (IsKeyDown(KEY_A))
        velocity.x -= 1.0;
    if (IsKeyDown(KEY_D))
        velocity.x += 1.0;
    if (IsKeyDown(KEY_W))
        velocity.y -= 1.0;
    if (IsKeyDown(KEY_S))
        velocity.y += 1.0;*/
        Vector2 moveDir{};

if (IsKeyDown(KEY_A)) moveDir.x -= 1.f;
if (IsKeyDown(KEY_D)) moveDir.x += 1.f;
if (IsKeyDown(KEY_W)) moveDir.y -= 1.f;
if (IsKeyDown(KEY_S)) moveDir.y += 1.f;

if (Vector2Length(moveDir) > 0.f)
{
    moveDir = Vector2Normalize(moveDir);
    velocity = moveDir;
    facingDir = moveDir;   // THIS LINE MAKES BULLETS FOLLOW FACING
}

    BaseCharacter::tick(deltaTime);

    Vector2 origin{};
    Vector2 offset{};
    float rotation{};
    if (rightLeft > 0.f)
    {
        origin = {0.f, weapon.height * scale};
        offset = {35.f, 55.f};
        weaponCollisionRec = {
            getScreenPos().x + offset.x,
            getScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale
        };
        rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 35.f : 0.f;
    }
    else
    {
        origin = {weapon.width * scale, weapon.height * scale};
        offset = {25.f, 55.f};
        weaponCollisionRec = {
            getScreenPos().x + offset.x - weapon.width * scale,
            getScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale
        };
        rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -35.f : 0.f;
    }

    // draw the sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
    Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);

    //SHOOTING 
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
{
    // Player center in WORLD space
    Vector2 playerWorldCenter = {
        worldPos.x + (width * scale) / 2.f,
        worldPos.y + (height * scale) / 2.f
    };

    // Mouse position in WORLD space
   // Vector2 mouseWorld = Vector2Add(GetMousePosition(), worldPos);
   Vector2 mouseWorld = Vector2Add(
    GetMousePosition(),
    getWorldPos()
);


   // Vector2 dir = Vector2Subtract(mouseWorld, playerWorldCenter);
    //bullets.emplace_back(playerWorldCenter, dir);
    bullets.emplace_back(playerWorldCenter, facingDir);

}
for (int i = 0; i < bullets.size(); i++)
{
    bullets[i].tick(deltaTime, worldPos);
    if (!bullets[i].alive)
    {
        bullets.erase(bullets.begin() + i);
        i--;
    }
}


}

void Character::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0.f)
    {
        setAlive(false);
    }
}
