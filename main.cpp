#include "raylib.h"
#include "raymath.h"
#include <vector>

#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

// ray lib built in function. help to get any random value in this range. 
// added by shadman
static Vector2 GetRandomSpawnPos()
{
    float x = (float)GetRandomValue(100, 2000);
    float y = (float)GetRandomValue(100, 2000);
    return { x, y };
}

int main()
{
    const int windowWidth{700};
    const int windowHeight{700};
    InitWindow(windowWidth, windowHeight, "Stephen's Top Down");

    Texture2D map = LoadTexture("nature_tileset/map2.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight{windowWidth, windowHeight};

// added  by shadman
 int killCount = 0;
const int WIN_KILLS = 10; // we can change this number to increase the game difficulty
bool gameWon = false;


//  textures to draw enemy 

Texture2D goblinIdle = LoadTexture("characters/goblin_idle_spritesheet.png");
Texture2D goblinRun  = LoadTexture("characters/goblin_run_spritesheet.png");

Texture2D slimeIdle = LoadTexture("characters/slime_idle_spritesheet.png");
Texture2D slimeRun  = LoadTexture("characters/slime_run_spritesheet.png");


 Prop props[2]{
 Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
 Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}};

    /*Enemy goblin{
        Vector2{800.f, 300.f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};

    Enemy slime{
        Vector2{500.f, 700.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }*/

// enemy textures (load once)
   // Texture2D goblinIdle = LoadTexture("characters/goblin_idle_spritesheet.png");
   // Texture2D goblinRun  = LoadTexture("characters/goblin_run_spritesheet.png");



  // enemy container
std::vector<Enemy> enemies;

const int MAX_ENEMIES = 3;

for (int i = 0; i < MAX_ENEMIES; i++)
{
   bool isGoblin = (i % 2 == 0);

    Enemy e(
        GetRandomSpawnPos(),
        isGoblin ? goblinIdle : slimeIdle,
        isGoblin ? goblinRun  : slimeRun
    );

    e.setTarget(&knight);
    enemies.push_back(e);
}





    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // game winning script , will be utilized when game will be finished
      /*   if (gameWon)
{
   ClearBackground(RAYWHITE);
    DrawText("YOU WIN!", 200, 300, 50, GREEN);
    DrawText(TextFormat("Enemies defeated: %i", killCount), 200, 360, 20, DARKGREEN);
    EndDrawing();
    continue; //stop game logic
}*/ 


        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw the map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // draw the props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive()) // Character is not alive
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else // Character is alive
        {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        knight.tick(GetFrameTime());
        // check map bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }
        // check prop collisions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

       for (auto& enemy : enemies)
{
    enemy.tick(GetFrameTime());
}

       //bullet vs enemy . added by shadman 
for (auto& enemy : enemies)
{
    for (auto& bullet : knight.getBullets())
    {
        if (enemy.getAlive() &&
            CheckCollisionRecs(
                bullet.getCollisionRec(knight.getWorldPos()),
                enemy.getCollisionRec()))
        {
            enemy.setAlive(false);
            bullet.alive = false;
             killCount++; 
        }
    }
}
// to count the killed enemy 
if (killCount >= WIN_KILLS)
{
    gameWon = true;
}


// for enemy respawn 
for (auto& enemy : enemies)
{
    if (!enemy.getAlive())
    {
        enemy.respawn(Vector2{
            (float)GetRandomValue(200, 2000),
            (float)GetRandomValue(200, 2000)
        });
    }
}


        EndDrawing();
    }
    CloseWindow();
}