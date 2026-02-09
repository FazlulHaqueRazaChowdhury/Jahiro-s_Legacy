#include "Character.h"
#include "raymath.h"
#include <string>

Character::Character(int winWidth, int winHeight) 
    : windowWidth(winWidth), windowHeight(winHeight)
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
    DrawText(debugText2.c_str(), 55, 80, 20, GREEN);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        isAttacking = true;
    else
        isAttacking = false;

    BaseCharacter::tick(deltaTime);

    // FROM THIS PART WAS DONE BY AZWAD HOSSAIN SHADMAN shooting mechanism
    // PLAYER CENTER (SCREEN)
    Vector2 playerScreenCenter = Vector2Add(
        getScreenPos(),
        {(width * scale) / 2.f, (height * scale) / 2.f}
    );

    // AIM DIRECTION 
    Vector2 mouseScreen = GetMousePosition();
    Vector2 shootDir = Vector2Subtract(mouseScreen, playerScreenCenter);
    shootDir.y -= 25.f; // adjust for gun position
    float rotation = atan2f(shootDir.y, shootDir.x) * RAD2DEG;
    
    // Normalized gun direction for bullet spawning
    Vector2 gunDir = {
        cosf(rotation * DEG2RAD),
        sinf(rotation * DEG2RAD)
    };
    
    std::string rotationText = std::to_string(rotation);
    DrawText(rotationText.c_str(), 10.f, 80.f, 20, RED);

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

    Rectangle dest{
        playerScreenCenter.x + gunOffset.x,
        playerScreenCenter.y + gunOffset.y,
        weapon.width * gunScale,
        weapon.height * gunScale
    };
    //Drawing the text for dest
    std::string destText = "Gun Dest: " + std::to_string((int)dest.x) + ", " + std::to_string((int)dest.y);
    DrawText(destText.c_str(), 10.f, 110.f, 20, RED);
    //Converting gun screen position to world position for bullet spawning
    Vector2 origin{
        0.f, // closer to grip
        (weapon.height * gunScale) / 2.f
    };
    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);
    // DrawRectangleLines(dest.x, dest.y, dest.width*flip, dest.height, RED); // debug gun dest

     DrawCircleV(mouseScreen, 5.f, GREEN); // debug mouse position
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
        // muzzleScreenPos.y -= (15.f); // adjust for better alignment
        // muzzleScreenPos.x += 5.f * flip; // adjust for better alignment
        //  muzzle position to world space for bullet spawning

    // DrawCircleV(muzzleScreenPos, 5.f, RED); // debug player center
    std::string muzzleText = "Muzzle Screen Pos: " + std::to_string((int)muzzleScreenPos.x) + ", " + std::to_string((int)muzzleScreenPos.y);   
    DrawText(muzzleText.c_str(), 100.f, 640.f, 20, RED);
    // DEBUG 
    std::string debugText = "Rotation: " + std::to_string(headRotate);
    DrawText(debugText.c_str(), 155, 280, 20, GREEN);

    // Shoot
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_B))
    {
        bullets.emplace_back(muzzleScreenPos, gunDir);
        if (shootSound) PlaySound(*shootSound); 
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