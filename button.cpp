#include<raylib.h>
#include<traylib.hpp>
#include<iostream>

int main(){
    InitWindow(600,600,"sample window");
    SetTargetFPS(60);

    RectangleButton button{Rectangle{250,250,100,100},MOUSE_BUTTON_LEFT};

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{40,40,130,255});
        DrawRectangleBut(button,Color{10,10,180,255});
        EndDrawing();
        std::cout << button.isdown();
    }
    CloseWindow();
    return 0;
}