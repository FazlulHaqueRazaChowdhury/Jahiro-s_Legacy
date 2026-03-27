#include "Health.h"

Health::Health(Character* character, Enemy* enemy, Vector2 pos, float scale)
    : jahiro(character), enemy(enemy), pos(pos), scale(scale) {
}

void Health::tick(float deltaTime) {
    // 1. Update the column state (restored to your correct original mapping!)
    if (jahiro->getHealth() >= 100.f) {
        currCol = 1.f; // Column 1: Full health
    } 
    else if (jahiro->getHealth() >= 80.f) {
        currCol = 2.f; // Column 2: Slightly damaged
    } 
    else if (jahiro->getHealth() >= 60.f) {
        currCol = 3.f; // Column 3
    }
    else if (jahiro->getHealth() >= 40.f) {
        currCol = 4.f; // Column 4
    }
    else if (jahiro->getHealth() >= 20.f) {
        currCol = 5.f; // Column 5
    }
    else if (jahiro->getHealth() > 0.f) {
        currCol = 6.f; // Column 6: Almost empty
    } 
    else {
        currCol = 0.f; // Column 0: Empty container (dead)
    }

    // 2. Calculate the source and destination rectangles
    Rectangle source = {width * currCol, height * currRow, width, height};
    Rectangle dest = {pos.x, pos.y, width * scale, height * scale};
    
    // 3. Draw the health bar and the character head icon
    DrawTexturePro(healthBarTexture, source, dest, {-150.f, -10.f}, 0.0f, WHITE);
    DrawTextureEx(head, Vector2{0.f, 0.f}, 0.f, 0.3f, WHITE);
}