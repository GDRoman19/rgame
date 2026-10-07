#pragma once
#include<raylib.h>

struct RectangleButton{
    Rectangle button;
    int mousebutton;

    RectangleButton(Rectangle,int);
    bool IsHovered();
    bool IsDown();
    bool IsPressed();
    bool IsReleased();
};

void DrawRectangleBut(RectangleButton,Color);
void DrawRectangleRoundedBut(RectangleButton,float,int,Color);

void DrawRectangleButText(RectangleButton,const char*,int,Color,Color);
void DrawRectangleRoundedButText(RectangleButton,float,int,const char*,int,Color,Color);

