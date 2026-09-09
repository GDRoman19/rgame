#include<raylib.h>
#include "traylib.hpp"
#include<ctime>
#include<random>
#include<string>
#define TP

using namespace std;

const Vector2 DISPLAY{1200,700};
Vector2 SCORE{0,0};
int TIMEIDLE;
int TIMEPAUSE;

struct Ball;
struct Racket;
struct Smartbot;

void RESET(Ball &p, Racket &r, Smartbot &s);

enum class State{
    IDLE,
    PLAY,
    PAUSE
};
State state = State::IDLE;

struct Racket{
    Racket(float posx, Color col){
        dim = {10,50};
        restart(posx);
        color = col;
    }
    void render(){
        DrawRectangle(pos.x-1,pos.y-1,dim.x+2,dim.y+2,{2,6,15,255});
        DrawRectangle(pos.x,pos.y,dim.x,dim.y,color);
    }
    void restart(int p){
        pos = {static_cast<float>(p),DISPLAY.y/2-(dim.y/2)};
        vel = 0;
        acc = 0;
    }
    void restart(){
        restart(pos.x);
    }
    void input(){
        if(IsKeyDown(KEY_UP)){
            acc = -2.7;
        }
        if(IsKeyReleased(KEY_UP)||IsKeyReleased(KEY_DOWN)){
            acc = 0;
        }
        if(IsKeyDown(KEY_DOWN)){
            acc = 2.7;
        }
#ifdef FOURDMOVE
        if(IsKeyDown(KEY_LEFT)){
            pos.x += -6;
        }
        if(IsKeyDown(KEY_RIGHT)){
            pos.x += 6;
        }
#endif
    }
    void update(){
        vel += acc;
        vel *= 0.70;
        if (vel > 10) {
            vel = 10;
        } else if (vel < -10) {
            vel = -10;
        }
        pos.y += vel;
        if(pos.y < 0){
            pos.y = 0;
        }
        if(pos.y + dim.y > DISPLAY.y){
            pos.y = DISPLAY.y-dim.y;
        }
    }
    Rectangle GetRectangleSide()const{
        return Rectangle{pos.x,pos.y+1,dim.x,dim.y-2};
    }
    Rectangle GetRectangleTop()const{
        return Rectangle{pos.x,pos.y,dim.x,1};
    }
    Rectangle GetRectangleBottom()const{
        return Rectangle{pos.x,pos.y+dim.y-1,dim.x,1};
    }
    Rectangle GetRectangle()const{
        return Rectangle{pos.x,pos.y,dim.y,dim.x};
    }
protected:
    Vector2 pos;
    Color color;
    Vector2 dim;
    double vel;
    double acc;
};

struct Bot:public Racket{
    Bot(float posx, Color col):Racket(posx,col){}
    void input(Vector2 bpos){
        if(bpos.y < pos.y+(dim.y/2)){
            acc = -2.7;
        } else if(bpos.y > pos.y+(dim.y/2)){
            acc = 2.7;
        } else {
            acc = 0;
        }
    }
};
struct Smartbot:public Racket{
    Smartbot(float posx, Color col):Racket(posx,col){}
    void input(Vector2 bpos, Vector2 bvel){
        int X = DISPLAY.x-(30+dim.x);
        float t = (X-13-bpos.x)/bvel.x;
        int endpos = bvel.y *t + bpos.y;
        if(endpos < pos.y+(dim.y/2)){
            acc = -2.7;
        } else if(endpos > pos.y+(dim.y/2)){
            acc = 2.7;
        } else {
            acc = 0;
        }
    }
};


struct Ball{
    Ball(){
        restart();
        radius = 13;
        color = {170,30,30,255};
    }
    bool reqestrestart(){
        if (pos.x <= radius) {
            SCORE.y ++;
            return true;
        } else if (pos.x >= DISPLAY.x-radius) {
            SCORE.x ++;
            return true;
        } else {
            return false;
        }
    }
    void restart(){
        pos = {DISPLAY.x/2,DISPLAY.y/2};
        do{
            vel.x = rand()%11-5;
            vel.y = rand()%11-5;
        }while(vel.x == 0 || vel.y == 0);
    }
    void render() const{
        DrawCircle(pos.x,pos.y,radius+1,{0,0,0,225});
        DrawCircle(pos.x,pos.y,radius,color);
    }
    Vector2 getpos(){
        return pos;
    }
    Vector2 getvel(){
        return vel;
    }
    void update(){
        pos.x += vel.x;
        pos.y += vel.y;
        if (pos.y <= radius || pos.y >= DISPLAY.y-radius) {
            vel.y = -vel.y;
            if (pos.y <= radius) {
                pos.y = radius;
            } else if (pos.y >= DISPLAY.y-radius) {
                pos.y = DISPLAY.y-radius;
            }
        }
        
#ifdef TP
    if(IsMouseButtonPressed(0)){
        pos = GetMousePosition();
    }
#endif
    }
    void collision(const Racket p){
        if (CheckCollisionCircleRec(pos,radius,p.GetRectangleTop())&&vel.y > 0) {
            vel.y = -abs(vel.y);
            while(CheckCollisionCircleRec(pos,radius,p.GetRectangle())){
                pos.y --;
            }
        } else if (CheckCollisionCircleRec(pos,radius,p.GetRectangleBottom())&&vel.y < 0) {
            vel.y = abs(vel.y);

            while(CheckCollisionCircleRec(pos,radius,p.GetRectangle())){
                pos.y ++;
            }
        } else if (CheckCollisionCircleRec(pos,radius,p.GetRectangleSide())) {
            vel.x = -vel.x;
            vel.x *= 1.1;
        }
    }
private:
    Vector2 pos;
    Vector2 vel;
    float radius;
    Color color;
};

void RESET(Ball &p, Racket &r, Smartbot &s) {
    p.restart();
    r.restart();
    s.restart();
    TIMEIDLE = 67;
    state = State::IDLE;
}

int main() {
    srand(time(0));

    InitWindow(DISPLAY.x,DISPLAY.y,"pang pong");
    SetTargetFPS(60);

    Ball ball;
    Racket p {30,{100,50,6,255}};
    Smartbot b {DISPLAY.x-40,{100,50,6,255}};
    RectangleButton ply {{DISPLAY.x/2-100,300,200,40},MOUSE_BUTTON_LEFT};
    RectangleButton ext {{DISPLAY.x/2-100,350,200,40},MOUSE_BUTTON_LEFT};

    while(!WindowShouldClose() || IsKeyDown(KEY_ESCAPE)) {
        if(IsKeyPressed(KEY_ESCAPE)&&state == State::PLAY){
            state = State::PAUSE;
            TIMEPAUSE = 5;
        }
        BeginDrawing();
        ClearBackground({30,32,68,255});
        ball.render();
        p.render();
        b.render();
        string scoretextx = to_string(static_cast<int>(SCORE.x));
        string scoretexty = to_string(static_cast<int>(SCORE.y));
        DrawText(scoretextx.c_str(),DISPLAY.x/2-40,13,28,DARKBLUE);
        DrawText(scoretexty.c_str(),DISPLAY.x/2+40,13,28,DARKBLUE);
#ifdef DEBUG
    DrawRectangleRec(p.GetRectangleSide(),RAYWHITE);
    DrawRectangleRec(p.GetRectangleTop(),RAYWHITE);
    DrawRectangleRec(p.GetRectangleBottom(),RAYWHITE);
#endif

        if (state == State::PLAY) {
            ball.update();
            ball.collision(p);
            ball.collision(b);
            if(ball.reqestrestart()){
                RESET(ball,p,b);
            }
            p.update();
            p.input();
            b.update();
            b.input(ball.getpos(),ball.getvel());
        } else if (state == State::IDLE){
            TIMEIDLE --;
            if (TIMEIDLE <= 0){
                state = State::PLAY;
            }
        } else if (state == State::PAUSE){
            DrawText("Pang Pong",DISPLAY.x/2-MeasureText("Pang Pong",70)/2,30,70,RAYWHITE);
            if(IsKeyPressed(KEY_ESCAPE) && TIMEPAUSE <= 0){
                state = State::PLAY;
            }
            TIMEPAUSE --;
            DrawRectangle(0,0,DISPLAY.x,DISPLAY.y,{0,0,0,100});
            if (!ply.IsHovered()){DrawRectangleButText(ply,"resume",20,DARKBLUE,BLACK);}
            if (ply.IsHovered()){DrawRectangleButText(ply,"resume",20,BLUE,{10,10,10,255});}

            if (!ext.IsHovered()){DrawRectangleButText(ext,"quit",20,DARKBLUE,BLACK);}
            if (ext.IsHovered()){DrawRectangleButText(ext,"quit",20,BLUE,{10,10,10,255});}
            
            if (ply.IsPressed()){
                state = State::PLAY;
            }
            if (ext.IsPressed()){
                break;
            }
        }
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}