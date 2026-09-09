#include<raylib.h>
#include<vector>
#include<cmath>
using namespace std;

vector<Vector2> tiles;

int main(){
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(900,900,"window");
    SetTargetFPS(60);

    Vector2 block;

    while (!IsKeyDown(KEY_F1)){
        BeginDrawing();
        ClearBackground({0,0,0,255});

        for(Vector2 b : tiles){
            DrawRectangle(b.x,b.y,floor((floor((GetScreenHeight())/(GetScreenWidth()/40))))*GetScreenWidth()/40/
                (GetScreenHeight()/40),GetScreenHeight()/40,BLUE);
        }

        EndDrawing();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            block = {floor((floor((GetMousePosition().x)/(GetScreenWidth()/40))*GetScreenWidth()/40)/
                (GetScreenHeight()/40))*GetScreenHeight()/40,
                floor((GetMousePosition().y)/(GetScreenHeight()/40))*GetScreenHeight()/40};
            tiles.push_back({block.x,block.y});
        }
    } 
}
