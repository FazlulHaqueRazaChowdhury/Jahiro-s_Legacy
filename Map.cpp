#include "Map.h"
#include "raymath.h"

Map::Map(Texture2D mapTexture, float scale){
    map=mapTexture;
    mapScale=scale;
}

void Map::addProp(const Prop& prop){
    props.push_back(prop);
}
void Map::render(Character& knight,float deltatime){
    Vector2 mapPos=Vector2Scale(knight.getWorldPos(),-1.f);

    //drawing map

    DrawTextureEx(map,mapPos,0.f,mapScale, WHITE);

    //drawing Props
    for(auto& prop:props){
        prop.Render(knight.getWorldPos(),deltatime);
    }
}
void Map:: handleCollision(Character& knight){
    for(auto& prop:props){
        if(CheckCollisionRecs(
            prop.getCollisionRec(knight.getWorldPos())
        ,knight.getCharCollisionRec())){
        knight.undoMovement();
        }
    }
}
