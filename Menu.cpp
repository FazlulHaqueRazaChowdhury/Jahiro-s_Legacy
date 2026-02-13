#include "Menu.h"
#include "raymath.h"
#include <string>

Menu::Menu(int winWidth, int winHeight)
    : windowWidth(winWidth), windowHeight(winHeight), 
      selectedMap(1), musicVolume(0.5f), sfxVolume(0.5f)
{
    
    // Load menu background
    menuBackground = LoadTexture("nature_tileset/landing-2_bg.png");
    customFont = LoadFont("fonts/XTypewriter-Bold.ttf");
// Load icons
    playIcon = LoadTexture("icons/play-button.png");
    settingsIcon = LoadTexture("icons/cog.png");
    quitIcon=LoadTexture("icons/exit-door.png");

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
<<<<<<< HEAD
        (float) windowWidth / 2.f - 600,
        (float) windowHeight - 100.f,
        250,
=======
        windowWidth / 2.f - 600,
        windowHeight - 100,
        100,
>>>>>>> 31548d7fe112c5eed712a40f9d35653ce3f950d2
        70
    };
    
    quitButton = {
        (float) windowWidth - 250.f,
        (float) windowHeight - 100.f,
        170,
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
    UnloadFont(customFont);
    UnloadTexture(settingsIcon);
    UnloadTexture(playIcon);
    UnloadTexture(quitIcon);
}

bool Menu::isButtonHovered(Rectangle button)
{
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, button);
}

void Menu::drawButtonWithSize(Rectangle button, const char* text, Color normalColor, Color hoverColor, int fontSize)
{
    Color currentColor = isButtonHovered(button) ? hoverColor : normalColor;

    //to made the button transparent , 0 lest,250 max
     currentColor.a = 120;

    DrawRectangleRec(button, currentColor);
    // DrawRectangleLinesEx(button, 3, BLACK);
    
    // Use custom font
    Vector2 textSize = MeasureTextEx(customFont, text, fontSize, 2);
    DrawTextEx(
        customFont,
        text,
        Vector2{
            button.x + (button.width - textSize.x) / 2,
            button.y + (button.height - fontSize) / 2
        },
        fontSize,
        2,
        WHITE
    );
}

void Menu::drawIconButton(Rectangle button, Texture2D icon, Color normalColor, Color hoverColor)
{
    Color currentColor = isButtonHovered(button) ? hoverColor : normalColor;
    currentColor.a = 120;

    DrawRectangleRec(button, currentColor);
    // DrawRectangleLinesEx(button, 3, BLACK);
    
    // Center the icon in button
    float iconSize = button.height * 0.7f;
    DrawTexturePro(
        icon,
        Rectangle{0, 0, (float)icon.width, (float)icon.height},
        Rectangle{
            button.x + (button.width - iconSize) / 2,
            button.y + (button.height - iconSize) / 2,
            iconSize,
            iconSize
        },
        Vector2{0, 0},
        0.0f,
        WHITE
    );
}
void Menu::drawButtonWithIcon(Rectangle button, const char* text, Texture2D icon, Color normalColor, Color hoverColor, int fontSize)
{
    Color currentColor = isButtonHovered(button) ? hoverColor : normalColor;
    currentColor.a = 120;

    DrawRectangleRec(button, currentColor);
    // DrawRectangleLinesEx(button, 3, BLACK);
    
    // Draw icon on the left side of button
    float iconSize = button.height * 0.8f;  // Icon is 60% of button height
    float iconX = button.x + 58;  //  padding from left
    float iconY = button.y + (button.height - iconSize) / 2;
    
    DrawTexturePro(
        icon,
        Rectangle{0, 0, (float)icon.width, (float)icon.height},
        Rectangle{iconX, iconY, iconSize, iconSize},
        Vector2{0, 0},
        0.0f,
        WHITE
    );
    
    // Draw text next to icon
    Vector2 textSize = MeasureTextEx(customFont, text, fontSize, 2);
    float textX = iconX + iconSize + 15;  // Icon + spacing
    float textY = button.y + (button.height - fontSize) / 2;
    
    DrawTextEx(
        customFont,
        text,
        Vector2{textX, textY},
        fontSize,
        2,
        WHITE
    );
}

void Menu::renderMainMenu()
{
    // Title
    const char* title = "JAHIRO'S LEGACY";
     Vector2 titleSize = MeasureTextEx(customFont, title, 80, 3);
    DrawTextEx(
        customFont,
        title,
        Vector2{windowWidth / 2.f - titleSize.x / 2, 100},
        80,  // size
        3,   // spacing
        GOLD
    );
    
    // Draw main buttons
     drawButtonWithIcon(playButton, "PLAY", playIcon, DARKGREEN, GREEN, 45);
     drawButtonWithSize(mapSelectButton, "MAP SELECT", DARKBLUE, BLUE, 35);
     drawIconButton(settingsButton, settingsIcon, DARKGRAY, PURPLE);
     drawIconButton(quitButton, quitIcon, DARKGRAY, RED);

    // Instructions
    const char* instructions = "Use WASD to move, Mouse to aim and shoot";
    Vector2 instSize = MeasureTextEx(customFont, instructions, 20, 1);
    DrawTextEx(
        customFont,
        instructions,
        Vector2{windowWidth / 2.f - instSize.x / 2, (float)windowHeight - 80.f},
        20,
        1,
        LIGHTGRAY
    );
}

void Menu::renderMapSelection()
{
    // Title
    const char* title = "SELECT MAP";
    int titleWidth = MeasureText(title, 70);
  Vector2 titleSize = MeasureTextEx(customFont, title, 70, 3);
    DrawTextEx(
        customFont,
        title,
        Vector2{windowWidth / 2.f - titleSize.x / 2, 120},
        70,
        3,
        GOLD);
    
    // Map buttons with selection highlight
    Color map1Color = (selectedMap == 1) ? BLUE : DARKBLUE;
    Color map1Hover = (selectedMap == 1) ? SKYBLUE : BLUE;
    drawButtonWithSize(map1Button, "ORIGINAL", map1Color, map1Hover, 38);
    
    Color map2Color = (selectedMap == 2) ? BLUE : DARKBLUE;
    Color map2Hover = (selectedMap == 2) ? SKYBLUE : BLUE;
    drawButtonWithSize(map2Button, "APOCALYPTIC", map2Color, map2Hover, 38);
    
    // Display current selection
    std::string selectionText = "Current: ";
    selectionText += (selectedMap == 1) ? "ORIGINAL" : "APOCALYPTIC";
    Vector2 selSize = MeasureTextEx(customFont, selectionText.c_str(), 30, 2);
    DrawTextEx(
        customFont,
        selectionText.c_str(),
        Vector2{windowWidth / 2.f - selSize.x / 2, windowHeight / 2.f + 60},
        30,
        2,
        YELLOW
    );
    
    // Back button
  drawButtonWithSize(backFromMapButton, "BACK", DARKGRAY, GRAY, 35);
}

void Menu::renderSettings()
{
    // Title
    const char* title = "SETTINGS";
     Vector2 titleSize = MeasureTextEx(customFont, title, 70, 3);
    DrawTextEx(
        customFont,
        title,
        Vector2{windowWidth / 2.f - titleSize.x / 2, 120},
        40,
        3,
        GOLD
    );
    
    // Music Volume
    const char* musicLabel = "Music";
    DrawTextEx(
        customFont,
        musicLabel,
        Vector2{windowWidth / 2.f - 280, windowHeight / 2.f - 70},
        30,
        2,
        WHITE
    );
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
    DrawTextEx(
        customFont,
        musicPercent.c_str(),
        Vector2{windowWidth / 2.f - 20, windowHeight / 2.f - 60},
        30,
        2,
        WHITE
    );
    
    drawButtonWithSize(musicUpButton, "+", DARKBLUE, BLUE, 40);
    
    // SFX Volume
    const char* sfxLabel = "SFX";
    DrawTextEx(
        customFont,
        sfxLabel,
        Vector2{windowWidth / 2.f - 260, windowHeight / 2.f + 40},
        30,
        2,
        WHITE
    );
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
    DrawTextEx(
        customFont,
        sfxPercent.c_str(),
        Vector2{windowWidth / 2.f - 20, windowHeight / 2.f + 40},
        30,
        2,
        WHITE
    );
    
    drawButtonWithSize(sfxUpButton, "+", DARKBLUE, BLUE, 40);
    
    // Back button
   drawButtonWithSize(backFromSettingsButton, "BACK", DARKGRAY, GRAY, 35);

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