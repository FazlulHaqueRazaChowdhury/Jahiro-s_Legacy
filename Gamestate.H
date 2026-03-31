#ifndef GAMESTATE_H
#define GAMESTATE_H

enum class GameState
{
    INTRO,
    MENU,
    MAP_SELECTION,
    SETTINGS,
    TRANSITION,
    PLAYING,
    PAUSED,
    STATS,
    GAME_OVER,
    QUIT
};

#endif