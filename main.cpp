#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include "Map.h"
#include "Menu.h"
#include "GameState.h"
#include <string>
#include <vector>

static Vector2 GetRandomSpawnPos()
{
    float x = (float)GetRandomValue(100, 2000);
    float y = (float)GetRandomValue(100, 2000);
    return {x, y};
}
int main()
{
    // SetConfigFlags(FLAG_FULLSCREEN_MODE);
    const int windowWidth{1280};
    const int windowHeight{720};
    InitWindow(windowWidth, windowHeight, "Jahiro's Legacy");

    InitAudioDevice();

    // bg music
    Music bgMusic = LoadMusicStream("sounds/Burn The World Waltz .mp3");
    bgMusic.looping = true;
    SetMusicVolume(bgMusic, 0.5f);
    PlayMusicStream(bgMusic);
    Sound gunShot = LoadSound("sounds/freesound_community-gun-shots-from-a-distance-7-96391.mp3");
    Sound enemyDeath = LoadSound("sounds/universfield-breeze-of-blood-122253.mp3");
    Texture2D cursor = LoadTexture("characters/cursor.png");

    Texture2D map1Tex = LoadTexture("nature_tileset/map3.png");
    Texture2D map2Tex = LoadTexture("nature_tileset/map2.png");
    Texture2D menuBackground = LoadTexture("nature_tileset/landing-2_bg.png");

    Map map1(map1Tex, 3.f);
    Map map2(map2Tex, 3.f);

    // MAP 1 PROPS
    map1.addProp(Prop({750.f, 500.f}, LoadTexture("nature_tileset/tree.png"), 30, 0, 4.f));
    map1.addProp(Prop({800.f, 600.f}, LoadTexture("nature_tileset/tree2.png"), 16, 0, 1.5f));
    map1.addProp(Prop({2200.f, 850.f}, LoadTexture("nature_tileset/tree3.png"), 25, 0, 1.2f));
    map1.addProp(Prop({2300.f, 800.f}, LoadTexture("nature_tileset/tree.png"), 30, 0, 3.9f));
    // MAP 2 PROPS
    map2.addProp(Prop({800.f, 600.f}, LoadTexture("nature_tileset/tree2.png"), 16, 0, 1.5f));
    map2.addProp(Prop({2200.f, 850.f}, LoadTexture("nature_tileset/tree3.png"), 25, 0, 1.2f));
    map2.addProp(Prop({230.f + 446.f, 200.f + 256.f}, LoadTexture("nature_tileset/Torch.png"), 4, 1, 1.5));
    map2.addProp(Prop({1160.f, 730.f}, LoadTexture("nature_tileset/Torch.png"), 4, 1, 1.5));
    map2.addProp(Prop({2020.f, 720.f}, LoadTexture("nature_tileset/Torch.png"), 4, 1, 1.5));
    map2.addProp(Prop({2340.f, 370.f}, LoadTexture("nature_tileset/Torch.png"), 4, 1, 1.5));
    Map *currentMap = &map1;

    Character knight{windowWidth, windowHeight};

    Texture2D goblinIdle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblinRun = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slimeIdle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slimeRun = LoadTexture("characters/slime_run_spritesheet.png");

    Texture2D enem = LoadTexture("characters/enem.png");
    knight.setShootSound(&gunShot);

    std::vector<Enemy> enemies;

    const int MAX_ENEMIES = 4;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        bool isGoblin = (i % 2 == 0);

        Enemy e(
            GetRandomSpawnPos(),
            enem,enem);

        e.setTarget(&knight);
        e.setDeathSound(enemyDeath);
        enemies.push_back(e);
    }

    // game state
    Menu menu(windowWidth, windowHeight);
    GameState currentState = GameState::MENU;

    SetTargetFPS(60);

    while (!WindowShouldClose()&& currentState != GameState::QUIT)
    {
        UpdateMusicStream(bgMusic);

       SetMusicVolume(bgMusic,menu.getMusicVolume());
     SetSoundVolume(gunShot, menu.getSfxVolume());
        SetSoundVolume(enemyDeath, menu.getSfxVolume());

        BeginDrawing();
        ClearBackground(WHITE);

        if (currentState == GameState::MENU ||
             currentState == GameState::MAP_SELECTION || 
             currentState == GameState::SETTINGS
        )
        {   //menuscreen
            menu.render(currentState);
            GameState newState = menu.handleInput(currentState);
            
            if (newState == GameState::PLAYING)
            {
                // Set selected map
                currentMap = (menu.getSelectedMap() == 1) ? &map1 : &map2;
                currentState = GameState::PLAYING;
            }
            else if (newState != currentState)
            {
                currentState = newState;
            }
        }
        else if (currentState == GameState::PLAYING)
        {

            currentMap->render(knight, GetFrameTime());
            currentMap->handleCollision(knight);

            if (IsKeyPressed(KEY_E))
                currentMap = &map1;

            if (IsKeyPressed(KEY_C))
                currentMap = &map2;

                if (IsKeyPressed(KEY_M))
                currentState = GameState::MENU;

            HideCursor();
            DrawTexturePro(
                cursor,
                Rectangle{0.f, 0.f, (float)cursor.width, (float)cursor.height},
                Rectangle{(float)GetMouseX() - (float)cursor.width * 0.05f * 0.5f, (float)GetMouseY() - (float)cursor.height * 0.05f * 0.5f, (float)cursor.width * 0.05f, ((float)cursor.height * 0.05f)},
                Vector2{0.f, 0.f},
                0.f,
                WHITE);

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
            if (
                knight.getWorldPos().y <= 0.f || knight.getWorldPos().x <= 49.f ||
                (knight.getWorldPos().x <= 466.f && knight.getWorldPos().y >= 654.f) || knight.getWorldPos().y >= 822.f || knight.getWorldPos().x > 1443.f || (knight.getWorldPos().x >= 1210.f && knight.getWorldPos().y >= 636.f))
            {
                knight.undoMovement();
            }

            for (auto &enemy : enemies)
            {
                enemy.tick(GetFrameTime());
            }

            for (auto &enemy : enemies)
            {
                for (auto &bullet : knight.getBullets())
                {
                    if (enemy.getAlive() &&
                        CheckCollisionRecs(
                            bullet.getCollisionRec(knight.getWorldPos()),
                            enemy.getCollisionRec()))
                    {
                        enemy.takeDamage();
                        bullet.alive = false;
                    }
                }
            }
            for (auto &enemy : enemies)
            {
                if (!enemy.getAlive())
                {
                    enemy.respawn(Vector2{
                        (float)GetRandomValue(100, 2000),
                        (float)GetRandomValue(200, 2000)});
                }
            }
        }
        EndDrawing();
    }
    UnloadMusicStream(bgMusic);
    UnloadSound(gunShot);
    UnloadSound(enemyDeath);
    CloseAudioDevice();
    CloseWindow();
}