#include<raylib.h>
#include<vector>
#include<cmath>
#include<iostream>
using namespace std;

enum class TileType{
    SOLID,
    LINESMIDDLE,
    LINESCORNER,
    LINESFLAT,
    LINESFILLERCORNER,
    LINESPILLAR,
    LINESENDPILLAR
};

struct Tile{
    Tile(Vector2 p,TileType t):pos(p),type(t){
        switch (type)
        {
        case TileType::SOLID:
            issolid = true;
            break;
        default:
            break;
        }
    }
    void render() const{
        if(type == TileType::SOLID){
            DrawRectangle(pos.x,pos.y,24,24,BLUE);
        }
    }
    bool getsolid() const{
        return issolid;
    }
    Vector2 getpos() const{
        return pos;
    }
private:
    Vector2 pos;
    TileType type;
    bool issolid;
};

vector<Tile> tiles;

int main(){
    InitWindow(960,720,"window");
    SetTargetFPS(60);

    Vector2 block;
    int i = 0;

    while (!WindowShouldClose() || IsKeyDown(KEY_ESCAPE)){
        BeginDrawing();
        ClearBackground({0,0,0,255});

        for(Tile b : tiles){
            b.render();
        }

        EndDrawing();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            block = {floor(GetMousePosition().x / 24) * 24,floor(GetMousePosition().y / 24) * 24};
            for(Tile b : tiles){
                if (b.getpos().x == floor(GetMousePosition().x / 24) * 24 &&
                b.getpos().y == floor(GetMousePosition().y / 24) * 24) {
                    tiles.erase(tiles.begin()+i);
                }
                i++;
            }
            i = 0;
            tiles.push_back({{block.x,block.y},TileType::SOLID});
            cout << tiles.size();
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            for(Tile b : tiles){
                if (b.getpos().x == floor(GetMousePosition().x / 24) * 24 &&
                b.getpos().y == floor(GetMousePosition().y / 24) * 24) {
                    tiles.erase(tiles.begin()+i);
                }
                i++;
            }
            i = 0;
            cout << tiles.size();
        }
    } 
    
    CloseWindow();
    return 0;
}
