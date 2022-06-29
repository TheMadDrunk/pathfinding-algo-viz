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

int main(int argc, char const *argv[])
{
    InitWindow(W_WIDTH,W_HEIGHT,"Best Path Search Algo Viz");
    SetTargetFPS(60);
    GuiEnable(); 
    float x=324,y=72,STEP=12;
    
    camera.target = {0,0};
    camera.offset = {x,y};
    camera.zoom = 1;
    camera.rotation = 0;
    while (!WindowShouldClose())
    {
        if(IsKeyDown(KEY_RIGHT))x-=STEP;
        if(IsKeyDown(KEY_LEFT))x+=STEP;
        if(IsKeyDown(KEY_DOWN))y-=STEP;
        if(IsKeyDown(KEY_UP))y+=STEP;
        camera.offset = {x,y};
        
        int wheel = GetMouseWheelMove();
        if(wheel>0){
            camera.zoom += 0.25;
            camera.zoom = min(camera.zoom,3.f);
        }
        if(wheel<0){
            camera.zoom -= 0.25;
            camera.zoom = max(camera.zoom,0.25f);
        }

        if(IsMouseButtonPressed(0)){
            Vector2 v = {GetMousePosition().x-camera.offset.x,GetMousePosition().y-camera.offset.y};
            cout<<v.x<<' '<<v.y<<'\n';
            
            matrix.Clicked(v);
        }

        BeginDrawing();
        ClearBackground((Color){33,33,33,1});
        
        BeginMode2D(camera);

        matrix.Draw();
        EndMode2D();

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