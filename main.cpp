#include <iostream>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "matrix.hpp"

//window height width
#define W_WIDTH 1080
#define W_HEIGHT 720

using namespace std;

//state of the animation 
// true of if its playing
bool animationPlaying = false;
//true if its on pause
bool animationPause = false;

//options
// number of cells  = matrixSize*matrixSize
int matrixSize = 15;
float animationSpeed = 1;
float zoom = 1;
int algorithmInUse = 0;

void AnimationGui();
int optionAnimationSpeed = 1;
int optionMatrixSize = 15;
bool focusedDropBox = false,focusedValueBox = false;
void OptionsGui();


MatrixViz matrix;
Camera2D camera;
#define POS 12

void ZoomPositionChanges();
void MatrixVizChanges();

int main(int argc, char const *argv[])
{
    InitWindow(W_WIDTH,W_HEIGHT,"Best Path Search Algo Viz");
    SetTargetFPS(60);
    GuiEnable(); 
    
    camera.target = {0,0};
    camera.zoom = 1;
    camera.rotation = 0;
    camera.offset = {328,72};

    while (!WindowShouldClose())
    {
        ZoomPositionChanges();
        MatrixVizChanges();

        BeginDrawing();
        ClearBackground((Color){33,33,33,1});
        
        //matrix
        BeginMode2D(camera);
        matrix.Draw();
        EndMode2D();

        //GUI
        AnimationGui();
        OptionsGui();

        EndDrawing();
    }
    
    GuiDisable();
    CloseWindow();   
    return 0;
}

void AnimationGui(){
    //to start the animation
    if(!animationPlaying){
        if(GuiButton({W_WIDTH-35,10,30,30},"#131#")){ //if clicked start animation
            animationPlaying = true;
        }
        return;
    }

    //if not paused shows pause button
    if(!animationPause){
        if(GuiButton({W_WIDTH-35,10,30,30},"#132#")){
            animationPause = true;
        }
    }
    else{
        if(GuiButton({W_WIDTH-35,10,30,30},"#131#")){
            animationPause = false;
        }
    }

    //repeat button
    if (GuiButton({W_WIDTH-70,10,30,30},"#57#"))
    {
        
    }

    //stop button
    if(GuiButton({W_WIDTH-105,10,30,30},"#133#")){
        animationPlaying = false;
    }

}

void OptionsGui(){

    if(animationPlaying)
        return;

    GuiPanel({0,0,170,720},"Options");

    //matrix size
    GuiValueBox({70,30,60,40},"Matrix size",&optionMatrixSize,5,50,focusedValueBox);
    if(IsMouseButtonReleased(0))
        if(CheckCollisionPointRec(GetMousePosition(),{70,30,60,40}))
            focusedValueBox = true;
        else focusedValueBox = false;
    

    //speed
    GuiSpinner({40,80,100,40},"Speed",&optionAnimationSpeed,1,10,false);
    GuiLabel({10,130,120,40},"Algorithm : ");
    
    GuiDropdownBox({10,170,140,40},"BFS\nDFS",&algorithmInUse,focusedDropBox);
    if(IsMouseButtonReleased(0))
        if(CheckCollisionPointRec(GetMousePosition(),{10,160,140,80}))
            focusedDropBox = true;
        else focusedDropBox = false;
        

    

}

void ZoomPositionChanges(){

        if(IsKeyDown(KEY_RIGHT))camera.offset.x-=POS;
        if(IsKeyDown(KEY_LEFT))camera.offset.x+=POS;
        if(IsKeyDown(KEY_DOWN))camera.offset.y-=POS;
        if(IsKeyDown(KEY_UP))camera.offset.y+=POS;
        
        int wheel = GetMouseWheelMove();
        if(wheel>0){
            camera.zoom += 0.25;
            camera.zoom = min(camera.zoom,3.f);
        }
        if(wheel<0){
            camera.zoom -= 0.25;
            camera.zoom = max(camera.zoom,0.25f);
        }
}

void MatrixVizChanges(){


    if(animationPlaying)
        return;

    if(IsMouseButtonPressed(0)){
        Vector2 v = {GetMousePosition().x-camera.offset.x,GetMousePosition().y-camera.offset.y};  
        matrix.Clicked(v);
    }

    if(IsKeyPressed(KEY_S)){
        Vector2 p = GetMousePosition();
        p.x -= camera.offset.x;
        p.y -= camera.offset.y;
        if(p.x>=0 and p.y>=0)
            matrix.SetStart(matrix.getCell(p));
    }

    if(IsKeyPressed(KEY_E)){
        Vector2 p = GetMousePosition();
        p.x -= camera.offset.x;
        p.y -= camera.offset.y;
        if(p.x>=0 and p.y>=0)
            matrix.SetEnd(matrix.getCell(p));
    }

}