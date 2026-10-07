#include<raylib.h>
#include<random>
#include<vector>
#include<ctime>
#include<iostream>
#include<string>
#include<cstdio>
using namespace std;

char level = 0;
unsigned short int destroycounter = 28;
unsigned char cooltimer = 0;
unsigned char opacitytimer = 0;
const int fsizeleveltext = 70;

enum class BulletType{
    DEFAULT,
    SPEED,
    DAMAGE,
    COOL
};
enum class BlockType{
    DEFAULT,
    SMALL,
    ZIGZAG
};

Font dark;
BulletType type = BulletType::DEFAULT;

vector<int> ammo = {-1,20,6,16,0,0,0,0};

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

void DisplayBullets(){
    string ammotext = ammo[static_cast<int>(type)] != -1? to_string(ammo[static_cast<int>(type)]): "inf";
    DrawTextEx(dark,ammotext.c_str(),{10,10},40,5,BLUE);
}

struct Block{
    Block(Vector2 d, Color c, float vy, float hp):dim(d),col(c),vely(vy),health(hp),maxhealth(hp){
        pos.x = static_cast<float>(rand()% (900-static_cast<int>(dim.x)));
        pos.y = 0-d.y;
        dead = false;
    }
    void render(){
        DrawRectangle(pos.x,pos.y,dim.x,dim.y,col);
    }
    virtual void update(){
        pos.y += vely;
    }
    bool destroyed(){
        return pos.y > 900 || dead;
    }
    Rectangle GetRect(){
        return Rectangle{pos.x,pos.y,dim.x,dim.y};
    }
    void Hit(float damage){
        health -= damage;
        if(health <= 0){
            dead = true;
            destroycounter += maxhealth;
            cout << destroycounter;
        }
    }
protected:
    Vector2 pos;
    Vector2 dim;
    Color col;
    float vely;
    float health;
    float maxhealth;
    bool dead;
};
struct DBlock:public Block{
    DBlock():Block({50,50},{224, 193, 135, 255},4,2){}
};
struct SBlock:public Block{
    SBlock():Block({10,10},{162, 135, 224, 255},2,1){}
};

struct ZBlock:public Block{
    ZBlock():Block({30,30},{168, 252, 0, 255},3.3,1.2){
        time = rand() %2 + 5;
        maxtime = time;
        accx = rand() %4 + 3;
        accx = rand() %2 == 0? -accx:accx;
        velx = 0;
    }
    void update() override {
        Block::update();
        velx += accx;
        pos.x += velx;
        time --;
        if(time == 0){
            accx = -accx;
            time = maxtime *2;
        }
    }
private:
    unsigned char time;
    float accx;
    float velx;
    unsigned char maxtime;
};

vector<Block*> blocks;

int GetChance(vector<int> probs){
    int sum = 0;
    for(int i : probs){
        sum += i;
    }
    if(sum != 100){
        return 0;
    }
    int cumsum = 0;
    int random = rand() %100;
    int i = 0;
    for(int p: probs){
        cumsum += p;
        if(random <= cumsum && random >= cumsum-p){
            return i;
        }
        i ++;
    }
    return 0;
}

void Create(BlockType b){
    Block* block = nullptr;
    switch (b) {
        case BlockType::DEFAULT:
            block = new DBlock;
            break;
        case BlockType::SMALL:
            block = new SBlock;
            break;
        case BlockType::ZIGZAG:
            block = new ZBlock;
            break;
        default:
            block = new DBlock;
            break;
    }
    blocks.push_back(block);
}

void CreateBlock(){
    int random;
    switch (level){
    case 0:
        random = GetChance({66,34});
        Create(static_cast<BlockType>(random));
        break;
    case 1:
        random = GetChance({40,30,20,10});
        Create(static_cast<BlockType>(random));
        break;
    default:
        break;
    }
}

void DrawLevelText(){
    if(opacitytimer > 2){
        DrawTextEx(dark,"-Level up-",{450.0F-MeasureText("-Level up-",fsizeleveltext)/2 , 450-fsizeleveltext/2} ,fsizeleveltext,2,{40,40,200,opacitytimer});
        opacitytimer -= 2;
    }
}

unsigned short int GetPointsReq(){
    switch (level)
    {
    case 0:
        return 30;
    case 1:
        return 70;
    case 2:
        return 150;
    case 3:
        return 400;
    case 4:
        return 1000;
    case 5:
        return -1;
    
    default:
        return -1;
    }
}

void DrawProgressBar(){
    DrawRectangleLines(100,5+60+5+20+5,66*8,6,PURPLE);
    DrawRectangleLines(100-1,5+60+5+20+5-1,66*8+2,6+2,BLUE);
    DrawRectangle(100+1,5+60+5+20+5+1,(66*8-2)*destroycounter/GetPointsReq(),6-2,RED);
}

void DrawSelectBlock(){
    Block* winner = nullptr;
    float windistance = 200.0F;
    Vector2 temppos;
    for(Block* b: blocks){
        temppos.x = abs(b->GetRect().x + b->GetRect().width/2 - GetMousePosition().x);
        temppos.y = abs(b->GetRect().y + b->GetRect().height/2 - GetMousePosition().y);
        if(temppos.x + temppos.y < windistance){
            windistance = temppos.x + temppos.y;
            winner = b;
        }
    }
    if(!winner){
            return;
    }
    DrawRectangleLinesEx(winner->GetRect(),3,RED);
}

void DrawInventory(){
    //Drawing inventory UI
    for(int i = 0; i < 12; i++){
        if(i < 8){
            DrawRectangleRoundedLinesEx({static_cast<float>(100+(66*i)),5,60,60},0.1,10,4,BLUE);
        } else {
            DrawRectangleRoundedLinesEx({static_cast<float>(106+(66*i)),5,60,60},0.1,10,4,RED);
        }

    }
    //Drawing ammo
    for(int i = 0; i < 8; i++){
        if(i == static_cast<int>(type)){
            DrawRectangleRoundedLinesEx({static_cast<float>(100+(66*i)),5,60,60},0.1,10,4,RAYWHITE);
        }
        string strammo = to_string(ammo[i]);
        int fsize = 20;
        if(ammo[i] == -1){
            DrawTextEx(dark,"Infinite",{static_cast<float>(100+(66*i))+60/2-(MeasureText("Infinite",fsize)/2),70},fsize,2,BLUE);
        }else{
            if(ammo[i] != 0){
                DrawTextEx(dark,strammo.c_str(),{static_cast<float>(100+(66*i))+60/2-(MeasureText(strammo.c_str(),fsize)/2),70},fsize,2,BLUE);
            } else {
                DrawTextEx(dark,strammo.c_str(),{static_cast<float>(100+(66*i))+60/2-(MeasureText(strammo.c_str(),fsize)/2),70},fsize,9,RED);
            }
        }
    }

    //Drawing bullets
    DrawRectangleGradientV(120,20,20,30,RED,{100,10,10,255});
    DrawRectangleGradientV(120+66,20,20,30,{129,150,23,255},{78,102,11,255});
    DrawRectangleGradientV(120+66*2,20,20,30,{120,6,131,255},{150,31,161,255});
    DrawRectangleGradientV(120+66*3,20,20,30,{12,117,112,255},{100,193,193,255});
    
    DrawProgressBar();
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
    Bullet(Vector2 p,Vector2 d,float a,float s,int o, Color c):pos(p),dim(d),damage(a),speed(s),cooldown(o),color(c){dead=false;}
    void render() const{
        DrawRectangle(pos.x,pos.y,dim.x,dim.y,color);
    }
    void update() {
        pos.y -= speed;
        for(Block* block: blocks){
            if(CheckCollisionRecs({pos.x,pos.y,dim.x,dim.y},block->GetRect())){
                dead = true;
                block->Hit(damage);
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
struct ABullet:public Bullet{ 
    explicit ABullet(Vector2 p):Bullet(p,{8,12},1.4,4,44,{150,31,161,255}){}
};
struct CBullet:public Bullet{ 
    explicit CBullet(Vector2 p):Bullet(p,{8,12},1,1,19,{82,209,192,255}){}
};

vector<Bullet*> bullets;

void CreateBullet(Player p){
    Bullet* b;
    if(type == BulletType::DEFAULT){
        b = new DBullet(p.GetPos());
    }
    if(type == BulletType::SPEED){
        b = new SBullet(p.GetPos());
    }
    if(type == BulletType::DAMAGE){
        b = new ABullet(p.GetPos());
    }
    if(type == BulletType::COOL){
        b = new CBullet(p.GetPos());
    }
    bullets.push_back(b);
    cooltimer = b->GetCool();
    
}

int main(){
    InitWindow(900,900,"BlockBuster");
    SetTargetFPS(60);

    dark = LoadFontEx("./dark.ttf",72,nullptr,0);
    
    Player p;
    short int timeblock = rand()%122+99;
    srand(time(0));

    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    while(!WindowShouldClose() || IsKeyDown(KEY_ESCAPE)){
        timeblock --;
        DisplayBullets();
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

        if(destroycounter >= GetPointsReq()){
            opacitytimer = 255;
            destroycounter = 0;
            level++;
        }

        DrawLevelText();

        if(IsKeyDown(KEY_X)){
            DrawInventory();
        }
        if(IsKeyDown(KEY_Z)){
            DrawSelectBlock();
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
        for(auto it = bullets.begin(); it != bullets.end();){
            if((*it)->destroyed()){
                delete (*it);
                it=bullets.erase(it);
            }
            else{
                it++;
            }
        }
        if(IsKeyPressed(KEY_ONE)){
                type = BulletType::DEFAULT;
            }
        if(IsKeyPressed(KEY_TWO)){
                type = BulletType::SPEED;
            }
        if(IsKeyPressed(KEY_THREE)){
                type = BulletType::DAMAGE;
            }
        if(IsKeyPressed(KEY_FOUR)){
                type = BulletType::COOL;
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