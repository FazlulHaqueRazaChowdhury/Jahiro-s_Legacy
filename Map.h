#ifndef MAP_H
#define MAP_H
#include "raylib.h"
#include "Character.h"
#include "Prop.h"
#include <vector>

class Map
{   
public:
    Map(Texture2D maptexture, float scale);
    
    void addProp(const Prop& prop);
    void render(Character& knight,float deltatime);
    void handleCollision(Character& knight); 
    private:
    Texture2D map;
    float mapScale{1.f};
    std:: vector<Prop> props;
   
};
#endif