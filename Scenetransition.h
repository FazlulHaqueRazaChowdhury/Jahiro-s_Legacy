#ifndef SCENE_TRANSITION_H
#define SCENE_TRANSITION_H
 #include "raylib.h"

 class Scenetransition{

    public: 
    Scenetransition(int windowWidth,int windowHeight);
    
    void start();
    void update(float deltaTime);
    void render();
    bool isComplete() const { return complete;}
    void setBanner(Texture2D* tex);

    private: 
    int windowWidth;
    int windowHeight;
    float fadeAlpha;
    float fadeSpeed;
    bool complete;
    bool active;
    float blackTime;
    float blackTimer;
    bool waiting;
    Texture2D* banner;
 };
 #endif