#include<raylib.h>

using namespace std;

int main() {
    InitWindow(200,200,"test");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        EndDrawing();
    }
    
    while(!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        EndDrawing();
    }
    
    CloseWindow();
}