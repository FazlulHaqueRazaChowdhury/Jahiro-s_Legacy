#include "Menu.h"
#include "raymath.h"
#include <string>

Menu::Menu(int winWidth, int winHeight)
    : windowWidth(winWidth), windowHeight(winHeight), 
      selectedMap(1), musicVolume(0.5f), sfxVolume(0.5f)
{
    // Load menu background
    menuBackground = LoadTexture("nature_tileset/landing-2_bg.png");
    
    // main menu buttons
    playButton = {
        windowWidth / 2.f - 150,
        windowHeight / 2.f - 120,
        300,
        70
    };
    
    mapSelectButton = {
        windowWidth / 2.f - 150,
        windowHeight / 2.f - 30,
        300,
        70
    };
    
    settingsButton = {
        windowWidth / 2.f - 600,
        windowHeight - 100,
        300,
        70
    };
    
    quitButton = {
        windowWidth -250,
        windowHeight - 100,
        200,
        70
    };
    
    // map selection buttons
    map1Button = {
        windowWidth / 2.f - 350,
        windowHeight / 2.f - 50,
        300,
        80
    };
    
    map2Button = {
        windowWidth / 2.f + 50,
        windowHeight / 2.f - 50,
        350,
        80
    };
    
    backFromMapButton = {
        windowWidth / 2.f - 150,
        windowHeight / 2.f + 120,
        300,
        60
    };
    
    // setting button
    musicUpButton = {
        windowWidth / 2.f + 100,
        windowHeight / 2.f - 80,
        80,
        60
    };
    
    musicDownButton = {
        windowWidth / 2.f - 180,
        windowHeight / 2.f - 80,
        80,
        60
    };
    
    sfxUpButton = {
        windowWidth / 2.f + 100,
        windowHeight / 2.f + 20,
        80,
        60
    };
    
    sfxDownButton = {
        windowWidth / 2.f - 180,
        windowHeight / 2.f + 20,
        80,
        60
    };
    
    backFromSettingsButton = {
        windowWidth / 2.f - 150,
        windowHeight / 2.f + 150,
        300,
        60
    };
}

Menu::~Menu()
{
    UnloadTexture(menuBackground);
}

bool Menu::isButtonHovered(Rectangle button)
{
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, button);
}

void Menu::drawButton(Rectangle button, const char* text, Color normalColor, Color hoverColor)
{
    Color currentColor = isButtonHovered(button) ? hoverColor : normalColor;
    DrawRectangleRec(button, currentColor);
    DrawRectangleLinesEx(button, 3, BLACK);
    
    int textWidth = MeasureText(text, 40);
    DrawText(text,
             button.x + (button.width - textWidth) / 2,
             button.y + (button.height - 40) / 2,
             40,
             WHITE);
}

void Menu::drawButtonWithSize(Rectangle button, const char* text, Color normalColor, Color hoverColor, int fontSize)
{
    Color currentColor = isButtonHovered(button) ? hoverColor : normalColor;
    DrawRectangleRec(button, currentColor);
    DrawRectangleLinesEx(button, 3, BLACK);
    
    int textWidth = MeasureText(text, fontSize);
    DrawText(text,
             button.x + (button.width - textWidth) / 2,
             button.y + (button.height - fontSize) / 2,
             fontSize,
             WHITE);
}

void Menu::renderMainMenu()
{
    // Title
    const char* title = "JAHIRO'S LEGACY";
    int titleWidth = MeasureText(title, 80);
    DrawText(title, windowWidth / 2 - titleWidth / 2, 100, 80, GOLD);
    
    // Draw main buttons
    drawButton(playButton, "PLAY", DARKGREEN, GREEN);
    drawButton(mapSelectButton, "MAP SELECT", DARKBLUE, BLUE);
    drawButton(settingsButton, "SETTINGS", DARKPURPLE, PURPLE);
    drawButton(quitButton, "QUIT", DARKGRAY, RED);
    
    // Instructions
    DrawText("Use WASD to move, Mouse to aim and shoot",
             windowWidth / 2 - 250,
             windowHeight - 80,
             20,
             LIGHTGRAY);
}

void Menu::renderMapSelection()
{
    // Title
    const char* title = "SELECT MAP";
    int titleWidth = MeasureText(title, 70);
    DrawText(title, windowWidth / 2 - titleWidth / 2, 120, 70, GOLD);
    
    // Map buttons with selection highlight
    Color map1Color = (selectedMap == 1) ? BLUE : DARKBLUE;
    Color map1Hover = (selectedMap == 1) ? SKYBLUE : BLUE;
    drawButton(map1Button, "ORIGINAL", map1Color, map1Hover);
    
    Color map2Color = (selectedMap == 2) ? BLUE : DARKBLUE;
    Color map2Hover = (selectedMap == 2) ? SKYBLUE : BLUE;
    drawButton(map2Button, "APOCALYPTIC", map2Color, map2Hover);
    
    // Display current selection
    std::string selectionText = "Current: ";
    selectionText += (selectedMap == 1) ? "ORIGINAL" : "APOCALYPTIC";
    int selWidth = MeasureText(selectionText.c_str(), 30);
    DrawText(selectionText.c_str(), 
             windowWidth / 2 - selWidth / 2, 
             windowHeight / 2 + 60, 
             30, 
             YELLOW);
    
    // Back button
    drawButton(backFromMapButton, "BACK", DARKGRAY, GRAY);
}

void Menu::renderSettings()
{
    // Title
    const char* title = "SETTINGS";
    int titleWidth = MeasureText(title, 70);
    DrawText(title, windowWidth / 2 - titleWidth / 2, 120, 70, GOLD);
    
    // Music Volume
    DrawText("Music", windowWidth / 2 - 280, windowHeight / 2 - 70, 30, WHITE);
    drawButtonWithSize(musicDownButton, "-", DARKBLUE, BLUE, 40);
    
    // Volume bar for music
    Rectangle musicBar = {
        windowWidth / 2.f - 80,
        windowHeight / 2.f - 70,
        160,
        40
    };
    DrawRectangleRec(musicBar, DARKGRAY);
    Rectangle musicFill = {
        musicBar.x,
        musicBar.y,
        musicBar.width * musicVolume,
        musicBar.height
    };
    DrawRectangleRec(musicFill, GREEN);
    DrawRectangleLinesEx(musicBar, 2, WHITE);
    
    // Display percentage
    std::string musicPercent = std::to_string((int)(musicVolume * 100)) + "%";
    DrawText(musicPercent.c_str(), windowWidth / 2 - 20, windowHeight / 2 - 60, 30, WHITE);
    
    drawButtonWithSize(musicUpButton, "+", DARKBLUE, BLUE, 40);
    
    // SFX Volume
    DrawText("SFX", windowWidth / 2 - 260, windowHeight / 2 + 40, 30, WHITE);
    drawButtonWithSize(sfxDownButton, "-", DARKBLUE, BLUE, 40);
    
    // Volume bar for SFX
    Rectangle sfxBar = {
        windowWidth / 2.f - 80,
        windowHeight / 2.f + 30,
        160,
        40
    };
    DrawRectangleRec(sfxBar, DARKGRAY);
    Rectangle sfxFill = {
        sfxBar.x,
        sfxBar.y,
        sfxBar.width * sfxVolume,
        sfxBar.height
    };
    DrawRectangleRec(sfxFill, GREEN);
    DrawRectangleLinesEx(sfxBar, 2, WHITE);
    
    // Display percentage
    std::string sfxPercent = std::to_string((int)(sfxVolume * 100)) + "%";
    DrawText(sfxPercent.c_str(), windowWidth / 2 - 20, windowHeight / 2 + 40, 30, WHITE);
    
    drawButtonWithSize(sfxUpButton, "+", DARKBLUE, BLUE, 40);
    
    // Back button
    drawButton(backFromSettingsButton, "BACK", DARKGRAY, GRAY);
}

void Menu::render(GameState currentState)
{
    ClearBackground(Color{20, 20, 40, 255});
    
    // Draw background
    DrawTexturePro(
        menuBackground,
        Rectangle{0, 0, (float)menuBackground.width, (float)menuBackground.height},
        Rectangle{0, 0, (float)windowWidth, (float)windowHeight},
        Vector2{0, 0},
        0.f,
        WHITE
    );
    
    // Render appropriate screen based on state
    if (currentState == GameState::MENU)
    {
        renderMainMenu();
    }
    else if (currentState == GameState::MAP_SELECTION)
    {
        renderMapSelection();
    }
    else if (currentState == GameState::SETTINGS)
    {
        renderSettings();
    }
}

GameState Menu::handleInput(GameState currentState)
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return currentState;
    
    // main menu
    if (currentState == GameState::MENU)
    {
        if (isButtonHovered(playButton))
            return GameState::PLAYING;
        
        if (isButtonHovered(mapSelectButton))
            return GameState::MAP_SELECTION;
        
        if (isButtonHovered(settingsButton))
            return GameState::SETTINGS;
        
        if (isButtonHovered(quitButton))
            return GameState::QUIT;
    }
    
    // map selection
    else if (currentState == GameState::MAP_SELECTION)
    {
        if (isButtonHovered(map1Button))
            selectedMap = 1;
        
        if (isButtonHovered(map2Button))
            selectedMap = 2;
        
        if (isButtonHovered(backFromMapButton))
            return GameState::MENU;
    }
    
    //settings
    else if (currentState == GameState::SETTINGS)
    {
        if (isButtonHovered(musicUpButton))
        {
            musicVolume += 0.1f;
            if (musicVolume > 1.0f) musicVolume = 1.0f;
        }
        
        if (isButtonHovered(musicDownButton))
        {
            musicVolume -= 0.1f;
            if (musicVolume < 0.0f) musicVolume = 0.0f;
        }
        
        if (isButtonHovered(sfxUpButton))
        {
            sfxVolume += 0.1f;
            if (sfxVolume > 1.0f) sfxVolume = 1.0f;
        }
        
        if (isButtonHovered(sfxDownButton))
        {
            sfxVolume -= 0.1f;
            if (sfxVolume < 0.0f) sfxVolume = 0.0f;
        }
        
        if (isButtonHovered(backFromSettingsButton))
            return GameState::MENU;
    }
    
    return currentState;
}