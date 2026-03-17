#include "Scenetransition.h"
#include <string>
Scenetransition::Scenetransition(int winWidth, int winHeight)
    : windowWidth(winWidth), windowHeight(winHeight),
      fadeAlpha(0.0f), fadeSpeed(1.0f), complete(false), active(false), blackTime(0.5f), blackTimer(0.0f), waiting(false)
{
    banner = nullptr;
}

void Scenetransition::setBanner(Texture2D *tex)
{
    banner = tex;
}

void Scenetransition::start()
{
    active = true;
    complete = false;
    fadeAlpha = 0.0f;
    blackTimer = 0.0f;
    waiting = false;
}
void Scenetransition::update(float deltaTime)
{
    if (!active || complete)
        return;

    // Fade to black
    if (!waiting)
    {
        fadeAlpha += fadeSpeed * deltaTime;

        if (fadeAlpha >= 1.0f)
        {
            fadeAlpha = 1.0f;
            waiting = true;
            blackTimer = 0.0f;
        }
    }
    // Stay black
    else
    {
        blackTimer += deltaTime;

        if (blackTimer >= blackTime)
        {
            complete = true;
        }
    }
}

void Scenetransition::render()
{
    if (!active)
        return;

    if (banner != nullptr)
    {
        DrawTexturePro(
            *banner,
            Rectangle{0, 0, (float)banner->width, (float)banner->height},
            Rectangle{0, 0, (float)windowWidth, (float)windowHeight},
            Vector2{0, 0},
            0.0f,
            WHITE);
    }
    else
    {
        DrawRectangle(0, 0, windowWidth, windowHeight, BLACK);
    }

    // fade overlay
    Color fadeColor = {0, 0, 0, (unsigned char)(fadeAlpha * 255)};
    DrawRectangle(0, 0, windowWidth, windowHeight, fadeColor);

    int totalSteps = 3;
    float stepDuration = blackTime / totalSteps;

    int count = (int)(blackTimer / stepDuration) + 1;
    if (count > totalSteps)
        count = totalSteps;

    if (count <=totalSteps)
    {
        std::string text = std::to_string(count);

        int fontSize = 80;
        int textWidth = MeasureText(text.c_str(), fontSize);

        DrawText(
            text.c_str(),
            windowWidth / 2 - textWidth / 2,
            windowHeight / 2 - fontSize / 2,
            fontSize,
            GOLD);
    }
}