#include<raylib.h>
#include<random>
#include<vector>
#include<ctime>
#include<iostream>
using namespace std;

char level = 0;
unsigned char destroycounter = 0;

struct Block{
    Block(Vector2 d, Color c, float vy, int hp):dim(d),col(c),vely(vy),health(hp){
        pos.x = static_cast<float>(rand()% (900-static_cast<int>(dim.x)));
        pos.y = 0-d.y;
    }
    void render(){
        DrawRectangle(pos.x,pos.y,dim.x,dim.y,col);
    }
    void update(){
        pos.y += vely;
    }
    bool destroyed(){
        return pos.y > 900;
    }
private:
    Vector2 pos;
    Vector2 dim;
    Color col;
    float vely;
    int health;
};
struct DBlock:public Block{
    DBlock():Block({50,50},{224, 193, 135, 255},4,1){}
};
struct SBlock:public Block{
    SBlock():Block({10,10},{162, 135, 224, 255},2,1){}
};

vector<Block*> blocks;

void CreateBlock(){
    switch (level){
    case 0:
        if(rand() %3 == 2){
            SBlock* block = new SBlock;
            blocks.push_back(block);
        } else {
            DBlock* block = new DBlock;
            blocks.push_back(block);
        }
        break;
    
    default:
        break;
    }
}

int main(){
    InitWindow(900,900,"object generation test");
    SetTargetFPS(60);
    Vector2 startp = GetMousePosition();
    while(!IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        CreateBlock();
        BeginDrawing();
        ClearBackground({0,0,0,255});

        for(Block* block: blocks){
            block->update();
            block->render();
        }
        EndDrawing();
        for(auto it = blocks.begin(); it != blocks.end();){
            if((*it)->destroyed()){
                delete (*it);
                it=blocks.erase(it);
            }
            else{
                it++;
            }
        }
    }
    for(Block* block: blocks){
        delete block;
    }
    CloseWindow();
}