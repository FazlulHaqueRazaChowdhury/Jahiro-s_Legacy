#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
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
#include <fstream>

Vector2 spawnPositions[] = {
    {700.f, 1.f},
    {1442.f, 500.f},
    {50.f, 500.f}};

static Vector2 GetRandomSpawnPos()
{
    int index = GetRandomValue(0, 2);
    return spawnPositions[index];
}
static Vector2 GetRandomLeafPos()
{
    float x = (float)GetRandomValue(0, 1280);
    float y = (float)GetRandomValue(0, 720);
    return {x, y};
}
int main()
{
    //  SetConfigFlags(FLAG_FULLSCREEN_MODE);
    const int windowWidth{1280};
    const int windowHeight{720};
    InitWindow(windowWidth, windowHeight, "Jahiro's Legacy");

    GameState nextState = GameState::MENU;

    InitAudioDevice();

    // Intro page things
    Texture2D introPage = LoadTexture("nature_tileset/Game_intro Page.png");
    float introAlpha = 1.f;
    float introTimer = 0.f;
    float introDuration = 5.f;

    // game ending bg
    Texture2D gameOverBg = LoadTexture("nature_tileset/Game_ending_bg.png");

    // bg music
    Music bgMusic = LoadMusicStream("sounds/Burn The World Waltz .mp3");
    bgMusic.looping = true;
    SetMusicVolume(bgMusic, 0.5f);
    PlayMusicStream(bgMusic);
    Sound gunShot = LoadSound("sounds/freesound_community-gun-shots-from-a-distance-7-96391.mp3");
    Sound enemyDeath = LoadSound("sounds/universfield-breeze-of-blood-122253.mp3");
    Sound reloadSound = LoadSound("sounds/reload.mp3");
    Sound bulletHitSound = LoadSound("sounds/enemy_hit.mp3");
    Sound gameOverSound = LoadSound("sounds/game_over.mp3");
    Sound gameOverSoundWin = LoadSound("sounds/win.mp3");
    Texture2D cursor = LoadTexture("characters/cursor.png");

    Texture2D map1Tex = LoadTexture("nature_tileset/map4.png");
    Texture2D map2Tex = LoadTexture("nature_tileset/map2.png");
    Texture2D menuBackground = LoadTexture("nature_tileset/landing-2_bg.png");
    Texture2D loadingBanner = LoadTexture("nature_tileset/landing-2_bg.png");

    Map map1(map1Tex, 3.f);
    Map map2(map2Tex, 3.f);
    Texture2D fallSprite = LoadTexture("nature_tileset/spring.png");
    Texture2D fallSpriteAkra = LoadTexture("nature_tileset/fall.png");

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
    Texture2D gobRun = LoadTexture("characters/Goblin/run.png");
    Texture2D gobAttk = LoadTexture("characters/Goblin/Attack.png");
    Texture2D gobDeath = LoadTexture("characters/Goblin/Death.png");
    Texture2D gobHit = LoadTexture("characters/Goblin/hit.png");

    // Enemy2 goblin({1500.f, 1500.f}, &gobRun, &gobAttk, &gobHit, &gobDeath, 100.f, 1.5f);
    // goblin.setTarget(&knight);
    // goblin.setDeathSound(enemyDeath);
    knight.setShootSound(&gunShot);
    knight.setShootSound(&gunShot);
    knight.setReloadSound(&reloadSound);

    Texture2D eyeRun = LoadTexture("characters/FlyingEye/run.png");
    Texture2D eyeAttk = LoadTexture("characters/FlyingEye/Attack.png");
    Texture2D eyeDeath = LoadTexture("characters/FlyingEye/Death.png");
    Texture2D eyeHit = LoadTexture("characters/FlyingEye/hit.png");

    Texture2D mushRun = LoadTexture("characters/Mushroom/Run.png");
    Texture2D mushAttk = LoadTexture("characters/Mushroom/Attack.png");
    Texture2D mushDeath = LoadTexture("characters/Mushroom/Death.png");
    Texture2D mushHit = LoadTexture("characters/Mushroom/hit.png");

    // Texture2D skelRun = LoadTexture("characters/Skeleton/Walk.png");
    // Texture2D skelAttk = LoadTexture("characters/Skeleton/Attack.png");
    // Texture2D skelDeath = LoadTexture("characters/Skeleton/Death.png");
    // Texture2D skelHit = LoadTexture("characters/Skeleton/hit.png");

    std::vector<Enemy2> enemies2;
    const int MAX_ENEMIES = 8;
    int enemiesKilled = 0;
    int lvl = 1;
    int highScore = 0;
    std::ifstream loadFile("savegame.dat");
    if (loadFile.is_open())
    {
        loadFile >> highScore;
        loadFile.close();
    }
    int currentScore = 0;
    int preLvl = 1;
    int inc = 5;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        // 0 = Goblin, 1 = Flying Eye
        int randomType = i % 3;
        if (randomType == 0)
        {
            Enemy2 e(spawnPositions[i], &gobRun, &gobAttk, &gobHit, &gobDeath, 100.f, 1.5f);
            e.setTarget(&knight);
            e.setDeathSound(enemyDeath);
            e.setHitSound(bulletHitSound);
            enemies2.push_back(e);
        }
        else if (randomType == 1)
        {
            Enemy2 e(spawnPositions[i], &mushRun, &mushAttk, &mushHit, &mushDeath, 100.f, 1.5f);
            e.setTarget(&knight);
            e.setDeathSound(enemyDeath);
            e.setHitSound(bulletHitSound);
            enemies2.push_back(e);
        }
        else
        {
            // Note: Fixed the typo here to pass &eyeDeath at the end!
            Enemy2 e(spawnPositions[i], &eyeRun, &eyeAttk, &eyeHit, &eyeDeath, 100.f, 1.5f);
            e.setTarget(&knight);
            e.setDeathSound(enemyDeath);
            e.setHitSound(bulletHitSound);
            enemies2.push_back(e);
        }
    }

    Scenetransition transition(windowWidth, windowHeight);
    transition.setBanner(&loadingBanner);

    // game state
    Menu menu(windowWidth, windowHeight);
    menu.setHighScore(highScore);
    GameState currentState = GameState::INTRO;
    Health health(&knight, Vector2{0.f, 0.f}, 7.f);
    SetTargetFPS(60);
    SetExitKey(0);

    std::vector<Leaf> leaves;
    for (int i = 0; i < 15; i++)
    {
        // Pass the memory address (&) of the texture
        leaves.emplace_back(GetRandomLeafPos(), &fallSprite, &fallSpriteAkra);
    }

    float updateTime = 1.f / 2.f;
    float runningTime{};

    while (!WindowShouldClose() && currentState != GameState::QUIT)
    {
        UpdateMusicStream(bgMusic);

        SetMusicVolume(bgMusic, menu.getMusicVolume() * 0.2f);
        SetSoundVolume(enemyDeath, menu.getSfxVolume() * 1.0f);
        SetSoundVolume(reloadSound, menu.getSfxVolume() * 0.7f);
        SetSoundVolume(bulletHitSound, menu.getSfxVolume() * 0.5f);
        SetSoundVolume(gunShot, menu.getSfxVolume() * 0.3f);
        SetSoundVolume(gameOverSound, menu.getSfxVolume());

        BeginDrawing();
        ClearBackground(WHITE);

        if (currentState == GameState::INTRO)
        {
            DrawTexturePro(
                introPage,
                Rectangle{0, 0, (float)introPage.width, (float)introPage.height},
                Rectangle{0, 0, (float)windowWidth, (float)windowHeight},
                Vector2{0, 0}, 0.f, WHITE);

            introTimer += GetFrameTime();

            // fade out
            if (introTimer >= introDuration)
            {
                introAlpha -= GetFrameTime();
                Color fadeColor = {0, 0, 0, (unsigned char)((1.f - introAlpha) * 255)};
                DrawRectangle(0, 0, windowWidth, windowHeight, fadeColor);

                if (introAlpha <= 0.f)
                    currentState = GameState::MENU;
            }

            // skip with any key
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                currentState = GameState::MENU;
        }

        else if (currentState == GameState::MENU ||
                 currentState == GameState::MAP_SELECTION ||
                 currentState == GameState::SETTINGS || currentState == GameState::STATS)
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
            DrawText("Let's Jump Into the Battle...", windowWidth / 2, windowHeight - 100, 40, YELLOW);

            // When fade completes -> switch to game
            if (transition.isComplete())
            {
                currentState = nextState;
            }
        }

        else if (currentState == GameState::PLAYING && knight.getAlive())
        {
            if (IsKeyPressed(KEY_ESCAPE))
                currentState = GameState::MENU;

            currentMap->render(knight, GetFrameTime());
            currentMap->handleCollision(knight);
            HideCursor();
            DrawTexturePro(
                cursor,
                Rectangle{0.f, 0.f, (float)cursor.width, (float)cursor.height},
                Rectangle{(float)GetMouseX() - (float)cursor.width * 0.05f * 0.5f, (float)GetMouseY() - (float)cursor.height * 0.05f * 0.5f, (float)cursor.width * 0.05f, ((float)cursor.height * 0.05f)},
                Vector2{0.f, 0.f},
                0.f,
                WHITE);

            // Enemy push logic
            for (auto &enemy : enemies2)
            {
                enemy.tick(GetFrameTime());
                if (enemy.getAlive())
                {
                    Vector2 push = enemy.getPushVector(enemies2);
                    float pushForce = 6.0f; 
                    push = Vector2Scale(push, pushForce);
                    enemy.setWorldPos(Vector2Add(enemy.getWorldPos(), push));
                }
            }
            // Bullet & Enemy collision detection system
            for (auto &enemy : enemies2)
            {
                for (auto &bullet : knight.getBullets())
                {
                    if (enemy.getAlive() &&
                        CheckCollisionRecs(
                            bullet.getCollisionRec(knight.getWorldPos()),
                            enemy.getCollisionRec()))
                    {
                        enemy.takeDamage(&enemiesKilled);
                        bullet.alive = false;
                    }
                }
            }
            // enemy respawning && game core logics
            lvl = 1 + (enemiesKilled / inc);
            currentScore = enemiesKilled * lvl;
            if (currentScore > highScore)
            {
                highScore = currentScore;
                menu.setHighScore(highScore);
                std::ofstream saveFile("savegame.dat");
                if (saveFile.is_open())
                {
                    saveFile << highScore;
                    saveFile.close();
                }
            }
            menu.setHighScore(highScore);
            menu.setCurrentScore(currentScore);
            if (lvl > preLvl)
            {
                knight.setHealth(100.f);
                preLvl = lvl;
            }
            float currentMaxHealth = 50 + (lvl * 20.f * 0.5);
            float currentSpeed = 2.0f + (lvl * 0.08f);
            float respawnDelay = 1.0f - (lvl * 0.1f);
            if (respawnDelay < 0.2f)
                respawnDelay = 0.2f;
            runningTime += GetFrameTime();

            if (runningTime >= respawnDelay)
            {
                for (auto &enemy : enemies2)
                {
                    if (!enemy.getAlive())
                    {
                        enemy.respawn(GetRandomSpawnPos(), currentMaxHealth, currentSpeed);
                        runningTime = 0.f;
                        break;
                    }
                }
            }
            // check map bounds
            if (
                knight.getWorldPos().y <= 10.f || knight.getWorldPos().x <= 49.f ||
                (knight.getWorldPos().x <= 466.f && knight.getWorldPos().y >= 654.f) || knight.getWorldPos().y >= 822.f || knight.getWorldPos().x > 1443.f || (knight.getWorldPos().x >= 1210.f && knight.getWorldPos().y >= 636.f))
            {
                knight.undoMovement();
            }
            knight.tick(GetFrameTime());
            float currMap = menu.getSelectedMap();
            for (auto &leaf : leaves)
            {
                leaf.tick(GetFrameTime(), currMap);
            }
            // hud
            int topBoxX = 1290 - 340;
            int topBoxY = 20;
            DrawRectangle(topBoxX, topBoxY, 320, 80, Fade(BLACK, 0.8f));
            DrawRectangleLines(topBoxX, topBoxY, 320, 80, WHITE);
            DrawText(TextFormat("JAHIRO's IJJOT: %d", enemiesKilled * lvl), topBoxX + 15, topBoxY + 15, 20, WHITE);
            DrawText(TextFormat("Level: %d", lvl), topBoxX + 15, topBoxY + 45, 20, WHITE);
            int bottomBoxX = 1280 - 280;
            int bottomBoxY = 720 - 70;
            DrawRectangle(bottomBoxX, bottomBoxY, 260, 50, Fade(BLACK, 0.8f));
            DrawRectangleLines(bottomBoxX, bottomBoxY, 260, 50, WHITE);
            health.tick(GetFrameTime());
            // Reloading Animation
            if (knight.getIsReloading())
            {
                DrawText("RELOADING...", bottomBoxX + 15, bottomBoxY + 15, 20, RED);
            }
            else
            {
                DrawText(TextFormat("AMMO: %d / %d", knight.getCurrentAmmo(), knight.getMaxAmmo()), bottomBoxX + 15, bottomBoxY + 15, 20, WHITE);
            }
        }
        else if (!knight.getAlive() && currentState == GameState::PLAYING)
        {
            currentState = GameState::GAME_OVER;
            if(highScore > currentScore){
                PlaySound(gameOverSound);
            }
            else {
                PlaySound(gameOverSoundWin);
            }
        }

        else if (currentState == GameState::GAME_OVER)
        {
            // Background
            DrawTexturePro(gameOverBg,
                           Rectangle{0, 0, (float)gameOverBg.width, (float)gameOverBg.height},
                           Rectangle{0, 0, (float)windowWidth, (float)windowHeight},
                           Vector2{0, 0}, 0.f, WHITE);
            DrawRectangle(0, 0, windowWidth, windowHeight, Fade(BLACK, 0.5f));

            // Modal box
            int modalW = 560, modalH = 400;
            int modalX = windowWidth / 2 - modalW / 2;
            int modalY = windowHeight / 2 - modalH / 2;
            DrawRectangle(modalX, modalY, modalW, modalH, Color{20, 20, 20, 220});
            DrawRectangleLinesEx(Rectangle{(float)modalX, (float)modalY, (float)modalW, (float)modalH}, 3, RED);

            // Title
            if(highScore > currentScore)
            DrawText("DO NOT PLAY WITH JAHIRO'S IJJOT!", modalX+10, modalY + 30, 28, RED);
            else
            {

                DrawText("JAHIRO SAVED HIS LEGACY.", modalX + 110, modalY + 30, 28, RED);
                DrawText("THANK YOU!", modalX + 170, modalY + 60, 28, RED);
            }

            // Score display
            DrawText(TextFormat("Score: %d", currentScore), modalX + 190, modalY + 100, 28, WHITE);
            DrawText(TextFormat("High Score: %d", highScore), modalX + 160, modalY + 140, 28, GOLD);

            // REPLAY button
            Rectangle replayBtn = {(float)modalX + 30, (float)modalY + 300, 150, 55};
            bool replayHovered = CheckCollisionPointRec(GetMousePosition(), replayBtn);
            DrawRectangleRec(replayBtn, replayHovered ? GREEN : DARKGREEN);
            DrawText("REPLAY", modalX + 50, modalY + 318, 22, WHITE);

            // MENU button
            Rectangle menuBtn = {(float)modalX + 205, (float)modalY + 300, 150, 55};
            bool menuHovered = CheckCollisionPointRec(GetMousePosition(), menuBtn);
            DrawRectangleRec(menuBtn, menuHovered ? BLUE : DARKBLUE);
            DrawText("MENU", modalX + 238, modalY + 318, 22, WHITE);

            // QUIT button
            Rectangle quitBtn = {(float)modalX + 380, (float)modalY + 300, 150, 55};
            bool quitHovered = CheckCollisionPointRec(GetMousePosition(), quitBtn);
            DrawRectangleRec(quitBtn, quitHovered ? Color{220, 50, 50, 255} : Color{150, 20, 20, 255});
            DrawText("QUIT", modalX + 418, modalY + 318, 22, WHITE);

            ShowCursor();

            if (replayHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                knight.setHealth(100.f);
                knight.setAlive(true);
                enemiesKilled = 0;
                currentScore = 0;
                lvl = 1;
                preLvl = 1;
                runningTime = 0.f;
                for (auto &enemy : enemies2)
                    enemy.respawn(GetRandomSpawnPos(), 50.f, 2.5f);
                currentState = GameState::PLAYING;
            }
            if (menuHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                knight.setHealth(100.f);
                knight.setAlive(true);
                enemiesKilled = 0;
                currentScore = 0;
                lvl = 1;
                preLvl = 1;
                runningTime = 0.f;
                for (auto &enemy : enemies2)
                    enemy.respawn(GetRandomSpawnPos(), 50.f, 2.5f);
                currentState = GameState::MENU;
            }
            if (quitHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                currentState = GameState::QUIT;
        }

        EndDrawing();
    }
    UnloadMusicStream(bgMusic);
    UnloadSound(gunShot);
    UnloadSound(enemyDeath);
    UnloadTexture(introPage);
    UnloadTexture(gameOverBg);
    UnloadSound(reloadSound);
    UnloadSound(bulletHitSound);
    UnloadSound(gameOverSound);
    CloseAudioDevice();
    CloseWindow();
}