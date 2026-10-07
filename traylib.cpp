#include<raylib.h>
#include "traylib.hpp"
#include<iostream>
#include<random>
#include<ctime>


RectangleButton::RectangleButton(Rectangle b, int m){
    button = b;
    mousebutton = m;
}
bool RectangleButton::IsHovered(){
    if (GetMouseX() > button.x && GetMouseY() > button.y && GetMouseX() < button.x + button.width && GetMouseY() < button.y + button.height) {
        return true;
    }
    return false;
}
bool RectangleButton::IsDown(){
    if (GetMouseX() > button.x && GetMouseY() > button.y && GetMouseX() < button.x + button.width && GetMouseY() < button.y + button.height && IsMouseButtonDown(mousebutton)) {
        return true;
    }
    return false;
}
bool RectangleButton::IsReleased(){
    if (GetMouseX() > button.x && GetMouseY() > button.y && GetMouseX() < button.x + button.width && GetMouseY() < button.y + button.height && IsMouseButtonReleased(mousebutton)) {
        return true;
    }
    return false;
}
bool RectangleButton::IsPressed(){
    if (GetMouseX() > button.x && GetMouseY() > button.y && GetMouseX() < button.x + button.width && GetMouseY() < button.y + button.height && IsMouseButtonPressed(mousebutton)) {
        return true;
    }
    return false;


    
};

void DrawRectangleBut(RectangleButton button, Color color) {
    DrawRectangleRec(button.button,color);
}
void DrawRectangleRoundedBut(RectangleButton button, float roundness,int segments, Color color) {
    DrawRectangleRounded(button.button,roundness,segments,color);
}

void DrawRectangleButText(RectangleButton button, const char* text, int fontsize, Color color, Color textcolor) {
    DrawRectangleRec(button.button,color);
    int textw = MeasureText(text,fontsize);
    DrawText(text,(button.button.width/2+button.button.x)-textw/2, 
            (button.button.height/2+button.button.y)-fontsize/2,fontsize,textcolor);
}
void DrawRectangleRoundedButText(RectangleButton button, float roundness, int segments, const char* text, int fontsize, Color color, Color textcolor) {
    DrawRectangleRounded(button.button,roundness,segments,color);
    int textw = MeasureText(text,fontsize);
    DrawText(text,(button.button.width/2+button.button.x)-textw/2, 
            (button.button.height/2+button.button.y)-fontsize/2,fontsize,textcolor);
}


