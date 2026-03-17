#include "Health.h"

Health::Health(Character* character, Enemy* enemy, Vector2 pos, float scale)
    : jahiro(character), enemy(enemy), pos(pos), scale(scale) {
}
void Health::tick(float deltaTime) {
    Rectangle source = {width*currCol, height*currRow, width, height};
    Rectangle dest = {pos.x, pos.y, width*scale, height*scale};
    
if (jahiro->getHealth() <= 0) {
    currCol = 0;
} 
else if (jahiro->getHealth() >= 100.f) {
    currCol = 1;
} 
else if (jahiro->getHealth() >= 80.f) {
    currCol = 2;
}
else if (jahiro->getHealth() >= 60.f) {
    currCol = 3;}
else if (jahiro->getHealth() >= 40.f) {
    currCol = 4;}
else if (jahiro->getHealth() >= 20.f) {
    currCol = 5;}
else if (jahiro->getHealth() > 10.f) {
    currCol = 6;}
    DrawTexturePro(healthBarTexture, source, dest, {-130.f, -10.f}, 0.0f, WHITE);
    DrawTextureEx(head, Vector2{0.f, 0.f}, 0.f, 0.3f, WHITE);
}