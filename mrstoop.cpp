#include<raylib.h>
#include "traylib.hpp"
#include<vector>
#include<iostream>
#define DRAW

using namespace std;

enum class State{
    GAME,
    PAUSE,
    MAIN_MENU
};

State state = State::MAIN_MENU;

struct player {
    player(){
        rect = {450,450,20,23};
        vely = 0;
        acc = 0;
    }
    void render() const{
        DrawRectangleRec(rect,{209,53,25,255});
    }
    void update(){
        if(IsKeyDown(KEY_RIGHT)){
            acc = 4.7;
        } else if(IsKeyDown(KEY_LEFT)){
             acc = -4.7;
        } else {
            acc = 0;
        }
        if (vely < 18){
            vely ++;
        }
        rect.y += vely;
        rect.x += acc;
    }
private:
    Rectangle rect;
    int vely;
    float acc;
};

int main(){
    RectangleButton newgame {{300,400,300,100},MOUSE_BUTTON_LEFT};
    player p;
    Vector2 pos1;
    vector<Rectangle> platforms;
    InitWindow(900,900,"MrStoopid");
    SetTargetFPS(60);
    while(!IsKeyDown(KEY_ESCAPE) && !WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (state == State::MAIN_MENU){
            if(newgame.IsHovered()) {DrawRectangleBut(newgame,{122,178,255,255});
            } else {DrawRectangleBut(newgame,{0,140,255,255});}
            DrawText("new game", 330, 430, 50, BLACK);
            DrawText("Mr. Stoopid", 240, 30, 70, BLACK);
            if (newgame.IsPressed()){state = State::GAME;}
        } else if(state == State::GAME){
            p.render();
            p.update();
#ifdef DRAW
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                pos1 = GetMousePosition();
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)){
                platforms.push_back(Rectangle{pos1.x,pos1.y,GetMousePosition().x-pos1.x,GetMousePosition().y-pos1.y});
            }
            for(int i = 0; i < platforms.size(); i++){
                DrawRectangleRec(platforms[i],BLACK);
            }
#endif
        }
        EndDrawing();
    }
}