#include<iostream>
#include<vector>
#include<raylib.h>
using namespace std;

struct Ball{
    Color color;
    
    Ball(Vector2 SpawnPos, int elasticity, Color col){
        radius = 13;
        color = col;
        pos = SpawnPos;
        vel = {0,0};
        elastic = elasticity;
        acc = 0.3;
    }
    void Render() const{
        DrawCircle(pos.x,pos.y,radius,{color});
    }
    void Update(vector<Vector4> lin){
        if(vel.y > -28){
            vel.y -= 1;
        }
        pos.y -= vel.y;
        for(int i = 0; i < lin.size(); i++){
            if(CheckCollisionCircleLine(pos,radius,{lin[i].x,lin[i].y},{lin[i].z,lin[i].w})){
                vel.y = -vel.y - elastic;
                while(CheckCollisionCircleLine(pos,radius,{lin[i].x,lin[i].y},{lin[i].z,lin[i].w})){
                    if(vel.y>=0){
                        pos.y --;
                    } else {
                        pos.y ++;
                    }
                }
            }
        }
        
        if(IsKeyDown(KEY_A)&&vel.x<10){
            vel.x += acc;
        }
        if(IsKeyDown(KEY_D)&&vel.x>-10){
            vel.x -= acc;
        }
        pos.x -= vel.x;
    }

private:
    float radius;
    Vector2 vel;
    Vector2 pos;
    int elastic;
    float acc;
};

int main(){
    InitWindow(800,800,"Bounceball");
    SetTargetFPS(60);

    Vector2 pos;
    Vector2 pos2;
    bool set = false;
    vector<Vector4> lines;
    Ball b{{400,400},0,BLUE};
    bool sim = false;

    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground({15,15,15,255});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            pos = GetMousePosition();
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            if(pos.x <= GetMouseX() && pos.y < GetMouseY()) {
                if(GetMouseX()-pos.x>GetMouseY()-pos.y){
                    DrawLine(pos.x,pos.y,GetMouseX(),pos.y,{230,230,230,255});
                } else {
                    DrawLine(pos.x,pos.y,pos.x,GetMouseY(),{230,230,230,255});
                }
            } else if(pos.x <= GetMouseX() && pos.y > GetMouseY()) {
                if(GetMouseX()-pos.x>pos.y-GetMouseY()){
                    DrawLine(pos.x,pos.y,GetMouseX(),pos.y,{230,230,230,255});
                } else {
                    DrawLine(pos.x,pos.y,pos.x,GetMouseY(),{230,230,230,255});
                }
            } else if(pos.x >= GetMouseX() && pos.y < GetMouseY()) {
                if(pos.x-GetMouseX()>GetMouseY()-pos.y){
                    DrawLine(pos.x,pos.y,GetMouseX(),pos.y,{230,230,230,255});
                } else {
                    DrawLine(pos.x,pos.y,pos.x,GetMouseY(),{230,230,230,255});
                }
            } else if(pos.x >= GetMouseX() && pos.y > GetMouseY()) {
                if(pos.x-GetMouseX()>pos.y-GetMouseY()){
                    DrawLine(pos.x,pos.y,GetMouseX(),pos.y,{230,230,230,255});
                } else {
                    DrawLine(pos.x,pos.y,pos.x,GetMouseY(),{230,230,230,255});
                }
            }
        }
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            set = true;
            pos2 = GetMousePosition();
        }
        if(set){
            if(pos.x <= pos2.x && pos.y < pos2.y) {
                if(pos2.x-pos.x>pos2.y-pos.y){
                    lines.push_back({pos.x,pos.y,pos2.x,pos.y});
                } else {
                    lines.push_back({pos.x,pos.y,pos.x,pos2.y});
                }
            } else if(pos.x <= pos2.x && pos.y > pos2.y) {
                if(pos2.x-pos.x>pos.y-pos2.y){
                    lines.push_back({pos.x,pos.y,pos2.x,pos.y});
                } else {
                    lines.push_back({pos.x,pos.y,pos.x,pos2.y});
                }
            } else if(pos.x >= pos2.x && pos.y < pos2.y) {
                if(pos.x-pos2.x>pos2.y-pos.y){
                    lines.push_back({pos.x,pos.y,pos2.x,pos.y});
                } else {
                    lines.push_back({pos.x,pos.y,pos.x,pos2.y});
                }
            } else if(pos.x >= pos2.x && pos.y > pos2.y) {
                if(pos.x-pos2.x>pos.y-pos2.y){
                    lines.push_back({pos.x,pos.y,pos2.x,pos.y});
                } else {
                    lines.push_back({pos.x,pos.y,pos.x,pos2.y});
                }
            }
            set = false;
        }

        for(int i = 0; i < lines.size(); i++){
            DrawLine(lines[i].x,lines[i].y,lines[i].z,lines[i].w,{230,230,230,255});
        }

        b.Render();
        EndDrawing();
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            sim = true;
        }
        if(sim){
            b.Update(lines);
        }
        
    }

    CloseWindow();
    return 0;
}