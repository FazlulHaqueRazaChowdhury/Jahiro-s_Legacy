#include "Character.h"
#include "raymath.h"
#include <string>


Character::Character(int winWidth, int winHeight,Texture2D *bulletTexture) 
    : windowWidth(winWidth), windowHeight(winHeight), BulletTex(bulletTexture)
{
    width = texture.width / maxFrames;
    height = texture.height;
    
    worldPos = {
        static_cast<float>(windowWidth) / 2.f + 300.f,
        static_cast<float>(windowHeight) / 2.f + 300.f
    };
    
    ssPos = getScreenPos();
    mouseMovement = true;
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
    if (!getAlive())
        return;

    // MOVEMENT 
    if (IsKeyDown(KEY_A)) velocity.x -= 1.0;
    if (IsKeyDown(KEY_D)) velocity.x += 1.0;
    if (IsKeyDown(KEY_W)) velocity.y -= 1.0;
    if (IsKeyDown(KEY_S)) velocity.y += 1.0;

    std::string debugText2 = "Velocity Coord: " + std::to_string((int)GetMousePosition().x) +
                             ", " + std::to_string((int)GetMousePosition().y);
    // DrawText(debugText2.c_str(), 55, 80, 20, GREEN);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        isAttacking = true;
    else
        isAttacking = false;
       // PLAYER CENTER (SCREEN)
    Vector2 playerScreenCenter = Vector2Add(
        getScreenPos(),
        {(width * scale) / 2.f, (height * scale) / 2.f}
    );
    //Shadows
    DrawTextureEx(shadow,Vector2{playerScreenCenter.x-(float)shadow.width,playerScreenCenter.y-(float)shadow.height},0.f,2.f, WHITE);
    BaseCharacter::tick(deltaTime);

    if(runFlag){
        dust.Render(Vector2{
        playerScreenCenter.x,
        playerScreenCenter.y-20.f
    }, deltaTime,rightLeft);
    }

    // FROM THIS PART WAS DONE BY AZWAD HUSSAIN SHADMAN shooting mechanism
    // AIM DIRECTION 
    Vector2 mouseScreen = GetMousePosition();
    Vector2 shootDir = Vector2Subtract(mouseScreen,getScreenPos());
    
    float rotation = atan2f(shootDir.y, shootDir.x) * RAD2DEG;
    
    // Normalized gun direction for bullet spawning
    Vector2 gunDir = {
        cosf(rotation * DEG2RAD),
        sinf(rotation * DEG2RAD)
    };
    

    //Flip
    float flip{1.f};
    if (rotation > 90.f || rotation < -90.f)
    {
        flip = -1.f;
        rightLeft = -1.f;
    }
    else
    {
        flip = 1.f;
        rightLeft = 1.f;
    }

    // draw the head
    float headScale = 0.04f;
    Rectangle source2{
        (float)(1.f * head.width),
        (float)(1.f * head.height),
        (float)head.width, // flip when facing left
        (float)head.height * flip
    };

    Vector2 headOffset = {
        2.f * flip, // move gun forward (right)
        -25.f       // move gun UP (negative y)
    };

    Rectangle dest2{
        playerScreenCenter.x + headOffset.x,
        playerScreenCenter.y + headOffset.y,
        weapon.width * headScale,
        weapon.height * headScale
    };
    
    float headRotate = rotation;

    if (rotation > 30.f && rotation < 150.f)
    {
        headRotate = 30.f;
        if (rotation >= 90.f)
            headRotate = 150.f;
    }
    else if (rotation < -30.f && rotation > -150.f)
    {
        headRotate = -30.f;
        if (rotation <= -90.f)
            headRotate = -150.f;
    }

    Vector2 origin2{
        (head.width * headScale) / 2.f, // closer to grip
        (head.height * headScale) / 2.f
    };
    
    DrawTexturePro(head, source2, dest2, origin2, headRotate, WHITE);

    // GUN 
    float gunScale = 0.08f;
    
    Rectangle source{
        (float)(1.f * weapon.width),
        (float)(1.f * weapon.height),
        (float)weapon.width, // flip when facing left
        (float)weapon.height * flip
    };

    Vector2 gunOffset = {
        -10.f * rightLeft, // move gun forward (right)
        -15.f              // move gun UP (negative y)
    };

    recoilOffset=Lerp(recoilOffset,0.f,recoilReturnSpeed*deltaTime);
    Vector2 recoilVec=Vector2Scale(gunDir,-recoilOffset);

    Rectangle dest{
        playerScreenCenter.x + gunOffset.x+recoilVec.x,
        playerScreenCenter.y + gunOffset.y+recoilVec.y,
        weapon.width * gunScale,
        weapon.height * gunScale
    };
    Vector2 origin{
        0.f, // closer to grip
        (weapon.height * gunScale) / 2.f
    };
    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);

    

    //Muzzle postion 
    // Hand position in screen space
    Vector2 handScreenPos = Vector2Add(playerScreenCenter, gunOffset);
    
    // Distance from hand to muzzle tip
    float handToMuzzle = weapon.width * gunScale*.8f; // adjust as needed
    
    // Muzzle position in screen space
    handScreenPos.y -= 5.f; // adjust for better alignment
    Vector2 muzzleScreenPos = Vector2Add(
        handScreenPos,
        Vector2Scale(gunDir, handToMuzzle)
    );

    // Shoot & Reload
    if (isReloading)
    {
        int missingBullets = maxAmmo - currentAmmo;   
        if (missingBullets <= 15) 
        {
            reloadTime = 0.5f; 
        } 
        else if (missingBullets <= 35) 
        {
            reloadTime = 1.2f; 
        } 
        else 
        {
            reloadTime = 2.0f; 
        }
        reloadTimer += deltaTime;
        if (reloadTimer >= reloadTime)
        {
            currentAmmo = maxAmmo;
            isReloading = false;
            reloadTimer = 0.f;
        }
    }
    if (IsKeyPressed(KEY_R) && currentAmmo < maxAmmo && !isReloading)
    {
        isReloading = true;
        if(reloadSound) PlaySound(*reloadSound);
    }
    
    // Shooting Logic
    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) && !isReloading)
    {
        if (currentAmmo > 0) 
        {
            bullets.emplace_back(BulletTex, muzzleScreenPos, Vector2Subtract(mouseScreen, muzzleScreenPos));
            if (shootSound) PlaySound(*shootSound); 
            recoilOffset = recoilStrength;
            currentAmmo--; 
        }
        else 
        {
            isReloading = true;
                 if(reloadSound) PlaySound(*reloadSound);
        }
    }
    // Bullets
    for (int i = 0; i < bullets.size(); i++)
    {
        bullets[i].tick(deltaTime, worldPos, rightLeft, muzzleScreenPos);
        
        if (!bullets[i].alive)
        {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
    
    mainCharRL = rightLeft;
}

void Character::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0.f)
        setAlive(false);
}

Rectangle Character::getCharCollisionRec()
{
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale
    };
}
void Character::setHealth(float heal) {
    health = heal;
}