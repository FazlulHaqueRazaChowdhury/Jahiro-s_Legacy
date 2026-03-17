#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include "Map.h"
#include "Menu.h"
#include "GameState.h"
#include "Scenetransition.h"
#include <string>
#include <vector>
#include "Health.h"
#include "Leaf.h"
#include "Grass.h"
#include "Enemy2.h"
static Vector2 GetRandomSpawnPos()
{
    float x = (float)GetRandomValue(100, 2000);
    float y = (float)GetRandomValue(100, 2000);
    return {x, y};
}
static Vector2 GetRandomLeafPos()
{
    float x = (float)GetRandomValue(0, 1280);
    float y = (float)GetRandomValue(0, 720);
    return {x, y};
}
int main()
{
    // SetConfigFlags(FLAG_FULLSCREEN_MODE);
    const int windowWidth{1280};
    const int windowHeight{720};
    InitWindow(windowWidth, windowHeight, "Jahiro's Legacy");

    GameState nextState = GameState::MENU;

    InitAudioDevice();

    // bg music
    Music bgMusic = LoadMusicStream("sounds/Burn The World Waltz .mp3");
    bgMusic.looping = true;
    SetMusicVolume(bgMusic, 0.5f);
    PlayMusicStream(bgMusic);
    Sound gunShot = LoadSound("sounds/freesound_community-gun-shots-from-a-distance-7-96391.mp3");
    Sound enemyDeath = LoadSound("sounds/universfield-breeze-of-blood-122253.mp3");
    Texture2D cursor = LoadTexture("characters/cursor.png");

    Texture2D map1Tex = LoadTexture("nature_tileset/map4.png");
    Texture2D map2Tex = LoadTexture("nature_tileset/map2.png");
    Texture2D menuBackground = LoadTexture("nature_tileset/landing-2_bg.png");
    Texture2D loadingBanner = LoadTexture("nature_tileset/landing-2_bg.png");

    Map map1(map1Tex, 3.f);
    Map map2(map2Tex, 3.f);
    Texture2D fallSprite = LoadTexture("nature_tileset/spring.png");
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

    Texture2D BulletTex = LoadTexture("characters/All_Fire_Bullet_Pixel_16x16 (1).png");
    Character knight{windowWidth, windowHeight, &BulletTex};
    Texture2D redEnem = LoadTexture("characters/enem.png");

    Texture2D skelIdle = LoadTexture("characters/Skeleton/Idle.png");
    Texture2D skelWalk = LoadTexture("characters/Skeleton/Walk.png");
    Texture2D skelAttack = LoadTexture("characters/Skeleton/Attack.png");
    Texture2D skelDeath = LoadTexture("characters/Skeleton/Death.png");
    Texture2D skelHit = LoadTexture("characters/Skeleton/Take Hit.png");
    knight.setShootSound(&gunShot);


    
    Scenetransition transition(windowWidth, windowHeight);
    transition.setBanner(&loadingBanner);

    std::vector<Grass> grassPatches;

    // Adjusting for the 3.f map scale!
    // x, y, width, height
    Rectangle topLawn = {550.f, 0.f, 1800.f, 750.f};      // Pushed right to clear the left building
    Rectangle rightLawn = {2600.f, 0.f, 800.f, 800.f};    // Way off to the right
    Rectangle bottomLawn = {800.f, 950.f, 1200.f, 400.f}; // Down around the Yin-Yang

    // Plant 300 patches of grass
    for (int i = 0; i < 1000; i++)
    {
        Vector2 spawnPos;
        int lawnChoice = GetRandomValue(1, 10); // Use a 1-10 range for weighting

        // Give the massive top lawn 70% of the grass
        if (lawnChoice <= 7)
        {
            spawnPos.x = (float)GetRandomValue(topLawn.x, topLawn.x + topLawn.width);
            spawnPos.y = (float)GetRandomValue(topLawn.y, topLawn.y + topLawn.height);
        }
        else if (lawnChoice <= 8)
        { // 10% to the right courtyard
            spawnPos.x = (float)GetRandomValue(rightLawn.x, rightLawn.x + rightLawn.width);
            spawnPos.y = (float)GetRandomValue(rightLawn.y, rightLawn.y + rightLawn.height);
        }
        else
        { // 20% to the bottom Yin-Yang lawn
            spawnPos.x = (float)GetRandomValue(bottomLawn.x, bottomLawn.x + bottomLawn.width);
            spawnPos.y = (float)GetRandomValue(bottomLawn.y, bottomLawn.y + bottomLawn.height);
        }

        grassPatches.emplace_back(spawnPos);
    }
    std::vector<Enemy> enemies;
    std::vector<Enemy2> enemies2;

    const int MAX_ENEMIES = 3;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {

        Enemy e(
            GetRandomSpawnPos(),
            &redEnem, &redEnem, 100.f, 1.5f);

        e.setTarget(&knight);
        e.setDeathSound(enemyDeath);
        enemies.push_back(e);
    }

    // game state
    Menu menu(windowWidth, windowHeight);
    GameState currentState = GameState::MENU;
    Health health(&knight, nullptr, Vector2{0.f, 0.f}, 7.f);
    SetTargetFPS(60);
    SetExitKey(0);

    std::vector<Leaf> leaves;
    for (int i = 0; i < 5; i++)
    {
        // Pass the memory address (&) of the texture
        leaves.emplace_back(GetRandomLeafPos(), &fallSprite);
    }

    float updateTime = 1.f / 2.f;
    float runningTime{};

    float leafUpdateTime = 1.f / 12.f;
    float leafRunningTime{};

    while (!WindowShouldClose() && currentState != GameState::QUIT)
    {
        UpdateMusicStream(bgMusic);

        SetMusicVolume(bgMusic, menu.getMusicVolume());
        SetSoundVolume(gunShot, menu.getSfxVolume());
        SetSoundVolume(enemyDeath, menu.getSfxVolume());

        BeginDrawing();
        ClearBackground(WHITE);

        if (currentState == GameState::MENU ||
            currentState == GameState::MAP_SELECTION ||
            currentState == GameState::SETTINGS)
        { // menuscreen
            menu.render(currentState);
            GameState newState = menu.handleInput(currentState);

            if (newState == GameState::PLAYING)
            {
                // Set selected map
                currentMap = (menu.getSelectedMap() == 1) ? &map1 : &map2;
                // Start transition
                nextState = GameState::PLAYING;
                currentState = GameState::TRANSITION;
                transition.start();
            }
            else if (newState != currentState)
            {
                currentState = newState;
            }
            ShowCursor();
            if (IsKeyPressed(KEY_ESCAPE))
                currentState = GameState::PLAYING;
        }

        else if (currentState == GameState::TRANSITION)
        {

            if (nextState == GameState::PLAYING)
            {
                currentMap->render(knight, 0.0f); 
            }
            // Update and render transition
            transition.update(GetFrameTime());
            transition.render();
            DrawText("Let's Jump Into the Battle...", windowWidth / 2 , windowHeight -100, 40, YELLOW);
              
            // When fade completes -> switch to game
            if (transition.isComplete())
            {
                currentState = nextState;
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

            if (IsKeyPressed(KEY_ESCAPE))
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
            }
            else // Character is alive
            {
                std::string knightsHealth = "Health: ";
                knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
                // DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
            }
            // Tick the grass and pass the knight's world position
            for (auto &grass : grassPatches)
            {
                grass.tick(GetFrameTime(), knight.getWorldPos());
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
            for (auto &enemy : enemies2)
            {
                enemy.tick(GetFrameTime());
            }

            for (auto &leaf : leaves)
            {
                leaf.tick(GetFrameTime());
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
            for (auto &enemy : enemies2)
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
                    runningTime += GetFrameTime();
                    if (runningTime >= updateTime)
                    {
                        enemy.respawn(GetRandomSpawnPos());
                        runningTime = 0.f;
                    }
                }
            }
            for (auto &enemy : enemies2)
            {
                if (!enemy.getAlive())
                {
                    runningTime += GetFrameTime();
                    if (runningTime >= updateTime)
                    {
                        enemy.respawn(GetRandomSpawnPos());
                        runningTime = 0.f;
                    }
                }
            }
            health.tick(GetFrameTime());
        }

        EndDrawing();
    }
    UnloadMusicStream(bgMusic);
    UnloadSound(gunShot);
    UnloadSound(enemyDeath);
    CloseAudioDevice();
    CloseWindow();
}