#include "Health.h"

Health::Health(Character* character, Vector2 pos, float scale)
    : jahiro(character),pos(pos), scale(scale) {
}

void Health::tick(float deltaTime) {
    // Update the column state (restored to your correct original mapping!)
    if (jahiro->getHealth() >= 100.f) {
        currCol = 1.f;
    } 
    else if (jahiro->getHealth() >= 80.f) {
        currCol = 2.f; 
    } 
    else if (jahiro->getHealth() >= 60.f) {
        currCol = 3.f; 
    }
    else if (jahiro->getHealth() >= 40.f) {
        currCol = 4.f; 
    }
    else if (jahiro->getHealth() >= 20.f) {
        currCol = 5.f; 
    }
    else if (jahiro->getHealth() > 0.f) {
        currCol = 6.f; 
    } 
    else {
        currCol = 0.f; 
    }

    // Calculate the source and destination rectangles
    Rectangle source = {width * currCol, height * currRow, width, height};
    Rectangle dest = {pos.x, pos.y, width * scale, height * scale};
    
    // Draw the health bar and the character head icon
    DrawTexturePro(healthBarTexture, source, dest, {-95.f, -10.f}, 0.0f, WHITE);
    DrawTextureEx(head, Vector2{0.f, 0.f}, 0.f, 0.3f, WHITE);
}