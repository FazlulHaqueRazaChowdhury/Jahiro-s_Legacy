#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "GameState.h"

class Menu
{
public:
    Menu(int windowWidth, int windowHeight);
    ~Menu();
    
    void render(GameState currentState);
    GameState handleInput(GameState currentState);
    int getSelectedMap() const { return selectedMap; }
    float getMusicVolume() const { return musicVolume; }
    float getSfxVolume() const { return sfxVolume; }
    
private:
    float windowWidth;
    float windowHeight;
    
    // Textures
    Texture2D menuBackground;
    Font customFont;

    // Main Menu Buttons
    Rectangle playButton;
    Rectangle mapSelectButton;
    Rectangle settingsButton;
    Rectangle quitButton;
    
    // Map Selection Buttons
    Rectangle map1Button;
    Rectangle map2Button;
    Rectangle backFromMapButton;
    
    // Settings Buttons
    Rectangle musicUpButton;
    Rectangle musicDownButton;
    Rectangle sfxUpButton;
    Rectangle sfxDownButton;
    Rectangle backFromSettingsButton;
    
    // State
    int selectedMap;
    float musicVolume;
    float sfxVolume;
    
    // Helper functions
    bool isButtonHovered(Rectangle button);
    void drawButtonWithSize(Rectangle button, const char* text, Color normalColor, Color hoverColor, int fontSize);
    void drawIconButton(Rectangle button, Texture2D icon, Color normalColor, Color hoverColor);
   void drawButtonWithIcon(Rectangle button, const char* text, Texture2D icon, Color normalColor, Color hoverColor, int fontSize);
    // Render different screens
    void renderMainMenu();
    void renderMapSelection();
    void renderSettings();

    // Icons
    Texture2D playIcon;
    Texture2D settingsIcon;
    Texture2D quitIcon;
};

#endif