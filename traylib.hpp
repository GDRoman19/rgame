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
void DrawRectangleButText(RectangleButton,const char*,int,Color,Color);

struct RectangleRound{
    int x;
    int y;
    int width;
    int height;
    float radius;

    RectangleRound(int, int, int, int, float);
};

Rectangle GetRect(RectangleRound);
/*
void DrawRectangleRound(int, int, int, int, float, Color);
void DrawRectangleRoundRec(RectangleRound, Color);
*/
