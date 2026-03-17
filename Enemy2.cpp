#include "Enemy2.h"
#include "raymath.h"

Enemy2::Enemy2(Vector2 pos, Texture2D *idleTex, Texture2D *walkTex, Texture2D *attackTex, Texture2D *deathTex, Texture2D *hitTex, float health, float scale)
    : Enemy(pos, idleTex, walkTex, health, scale) // Call the parent constructor
{
    skelIdle = idleTex;
    skelWalk = walkTex;
    skelAttack = attackTex;
    skelDeath = deathTex;
    skelHit = hitTex;

    // Set initial state
    texture = *skelIdle;
    maxFrames = 4.f; // Idle has 4 frames
    width = texture.width / maxFrames;
    height = texture.height; // Single row, so height is just the texture height!
    currentCol = 0.f; // Always 0 for strip sprites
}

void Enemy2::takeDamage()
{
    Enemy::takeDamage(); // Do the normal health subtraction from Enemy
    hitTimer = 0.3f;     // Play the hit animation for 0.3 seconds
}

void Enemy2::tick(float deltaTime)
{
    if (!getAlive())
    {
        // DEAD STATE
        texture = *skelDeath;
        maxFrames = 4.f;
        velocity = {};
    }
    else
    {
        // Get velocity towards target
        velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
        
        // Face the correct direction
        if(velocity.x < 0.f) rightLeft = -1.f;
        else if(velocity.x > 0.f) rightLeft = 1.f;

        // Decrease hit timer
        if (hitTimer > 0.f) hitTimer -= deltaTime;

        // Determine current animation state based on actions
        if (hitTimer > 0.f)
        {
            // TAKE HIT STATE
            texture = *skelHit;
            maxFrames = 4.f;
            velocity = {}; // Stop moving while hit
        }
        else if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
        {
            // ATTACK STATE
            undoMovement();
            target->takeDamage(damagePerSec * deltaTime); // Make sure damagePerSec is protected in Enemy.h!
            texture = *skelAttack;
            maxFrames = 8.f; // Attack sheet has 8 frames
        }
        else if (Vector2Length(velocity) > radius)
        {
            // WALK STATE
            texture = *skelWalk;
            maxFrames = 4.f;
        }
        else
        {
            // IDLE STATE
            velocity = {};
            texture = *skelIdle;
            maxFrames = 4.f;
        }
    }

    // Standardize sizes for the current texture
    width = texture.width / maxFrames;
    height = texture.height;
    currentCol = 0.f; // Always top row

    // Draw shadow underneath (inherited from Enemy)
    Vector2 shadowPos = Vector2{getScreenPos().x + width * getScale() * 0.5f - shadow.width, getScreenPos().y + height * getScale() - shadow.height - 40.f};
    DrawTextureEx(shadow, shadowPos, 0.f, 2.f, WHITE);
    
    // Call the base character tick to handle the actual frame counting and drawing!
    BaseCharacter::tick(deltaTime);
}