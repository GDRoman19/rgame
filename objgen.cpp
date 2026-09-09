#include<raylib.h>
#include<random>
#include<vector>
#include<ctime>
#include<iostream>
using namespace std;

char level = 0;
unsigned char destroycounter = 0;
unsigned char cooltimer = 0;
enum class BulletType{
    DEFAULT,
    SPEED
};
BulletType type = BulletType::DEFAULT;

vector<int> ammo = {-1,20};

bool CanShoot(){
    if(ammo[static_cast<int>(type)]!=0){
        return true;
    }
    return false;
}

void UseBullet(){
    if(ammo[static_cast<int>(type)] > 0){
        ammo[static_cast<int>(type)]--;
    }
}

struct Block{
    Block(Vector2 d, Color c, float vy, int hp):dim(d),col(c),vely(vy),health(hp){
        pos.x = static_cast<float>(rand()% (900-static_cast<int>(dim.x)));
        pos.y = 0-d.y;
        dead = false;
    }
    void render(){
        DrawRectangle(pos.x,pos.y,dim.x,dim.y,col);
    }
    void update(){
        pos.y += vely;
    }
    bool destroyed(){
        return pos.y > 900 || dead;
    }
    Rectangle GetRect(){
        return Rectangle{pos.x,pos.y,dim.x,dim.y};
    }
    void Hit(){
        health --;
        if(health == 0){
            dead = true;
        }
        
    }
private:
    Vector2 pos;
    Vector2 dim;
    Color col;
    float vely;
    int health;
    bool dead;
};
struct DBlock:public Block{
    DBlock():Block({50,50},{224, 193, 135, 255},4,2){}
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

struct Player{
    Player(){
        pos = {430,880};
        dim = {40,10};
    }
    void render(){
        DrawRectangle(pos.x,pos.y,dim.x,dim.y,RED);
        DrawRectangle(pos.x+(dim.x/2-5),pos.y-10,10,8,RED);
    }
    void update(){
        pos.x = GetMouseX()-dim.x/2;
    }
    Vector2 GetPos(){
        return {pos.x+(dim.x/2-5),pos.y-10};
    }
private:
    Vector2 pos;
    Vector2 dim;
};

struct Bullet{
    Bullet(Vector2 p,Vector2 d,int a,float s,int o, Color c):pos(p),dim(d),damage(a),speed(s),cooldown(o),color(c){dead=false;}
    void render() const{
        DrawRectangle(pos.x,pos.y,dim.x,dim.y,color);
    }
    void update() {
        pos.y -= speed;
        for(Block* block: blocks){
            if(CheckCollisionRecs({pos.x,pos.y,dim.x,dim.y},block->GetRect())){
                dead = true;
                block->Hit();
            };
        }
    }
    int GetCool(){
        return cooldown;
    }
    bool destroyed(){
        return pos.y <= 0-dim.y || dead;
    }
protected:
    Vector2 pos;
    Vector2 dim;
    int damage;
    float speed;
    int cooldown;
    Color color;
    bool dead;
}; 

struct DBullet:public Bullet{ 
    explicit DBullet(Vector2 p):Bullet(p,{8,12},1,4,56,{163,36,36,255}){}
};
struct SBullet:public Bullet{ 
    explicit SBullet(Vector2 p):Bullet(p,{8,12},1,6,29,{129,150,23,255}){}
};

vector<Bullet*> bullets;

void CreateBullet(Player p){
    if(type == BulletType::DEFAULT){
        DBullet* b = new DBullet(p.GetPos());
        bullets.push_back(b);
        cooltimer = b->GetCool();
    }
    if(type == BulletType::SPEED){
        SBullet* b = new SBullet(p.GetPos());
        bullets.push_back(b);
        cooltimer = b->GetCool();
    }
}

int main(){
    InitWindow(900,900,"object generation test");
    SetTargetFPS(60);
    Player p;
    short int timeblock = rand()%122+99;
    srand(time(0));
    while(!WindowShouldClose() || IsKeyDown(KEY_ESCAPE)){
        timeblock --;
        if(timeblock == 0){
            CreateBlock();
            timeblock = rand()%122+99;
        }
        if(cooltimer != 0){
            cooltimer --;
        }
        BeginDrawing();
        ClearBackground({0,0,0,255});

        for(Block* block: blocks){
            block->update();
            block->render();
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cooltimer == 0 && CanShoot()){
            CreateBullet(p);
            UseBullet();
        }
        for(Bullet* bullet: bullets){
            bullet->update();
            bullet->render();
        }

        p.render();
        p.update();

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
        for(auto it = bullets.begin(); it != bullets.end();){
            if((*it)->destroyed()){
                delete (*it);
                it=bullets.erase(it);
            }
            else{
                it++;
            }
        if(IsKeyPressed(KEY_ONE)){
                type = BulletType::DEFAULT;
            }
        if(IsKeyPressed(KEY_TWO)){
                type = BulletType::SPEED;
            }
        }
    }
    for(Block* block: blocks){
        delete block;
    }
    for(Bullet* bul: bullets){
        delete bul;
    }
    CloseWindow();
}