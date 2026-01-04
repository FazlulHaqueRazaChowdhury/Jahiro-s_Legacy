#include "Character.h"
#include "raymath.h"

constexpr int TILE_SIZE = 16;
constexpr int GUN_COL = 6;   // change if needed
constexpr int GUN_ROW = 4;   // change if needed


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

Vector2 moveDir{};
if (IsKeyDown(KEY_A)) moveDir.x -= 1.f;
if (IsKeyDown(KEY_D)) moveDir.x += 1.f;
if (IsKeyDown(KEY_W)) moveDir.y -= 1.f;
if (IsKeyDown(KEY_S)) moveDir.y += 1.f;

if (Vector2Length(moveDir) > 0.f)
{
    moveDir = Vector2Normalize(moveDir);
    velocity = moveDir;
    facingDir = moveDir;   // bullet follow facing 
}

    BaseCharacter::tick(deltaTime);

   /* Vector2 origin{};
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
    }*/

// player center in SCREEN space
Vector2 playerScreenCenter = Vector2Add(
    getScreenPos(),
    { (width * scale) / 2.f, (height * scale) / 2.f }
);

// mouse position in SCREEN space
Vector2 mouseScreen = GetMousePosition();

// direction from player → mouse (SCREEN SPACE)
Vector2 shootDir = Vector2Subtract(mouseScreen, playerScreenCenter);

// gun rotation (degrees)
float rotation = atan2f(shootDir.y, shootDir.x) * RAD2DEG;

// gun texture source
Rectangle source{
    (float)(GUN_COL * TILE_SIZE),
    (float)(GUN_ROW * TILE_SIZE),
    (float)TILE_SIZE,
    (float)TILE_SIZE
};

Vector2 gunOffset = {
    4.f * scale,    // move gun forward (right)
   2.f * scale     // move gun UP (negative y)
};

float gunScale = scale * 0.8f;  // gun size

// gun destination 
Rectangle dest{
    playerScreenCenter.x + gunOffset.x,
    playerScreenCenter.y + gunOffset.y,
    TILE_SIZE * gunScale,
    TILE_SIZE * gunScale
};

Vector2 origin{
    3.f * scale,    // closer to grip
    (TILE_SIZE * scale) / 2.f
};


// draw the gun
DrawTexturePro(
    weapon,
    source,
    dest,
    origin,
    rotation,
    WHITE
);



    // draw the sword
 //   Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
   // Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
    //DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);

    //SHOOTING 
if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) || IsKeyDown(KEY_B))
{
    // Player center in WORLD space
    Vector2 playerWorldCenter = {
        worldPos.x + (width * scale) / 2.f,
        worldPos.y + (height * scale) / 2.f
    };


    // Vector2 dir = Vector2Subtract(mouseWorld, playerWorldCenter);
    //bullets.emplace_back(playerWorldCenter, dir);

   bullets.emplace_back(playerWorldCenter, shootDir);


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
