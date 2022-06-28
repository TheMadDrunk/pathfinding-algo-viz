#include <iostream>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


//window height width
#define W_WIDTH 720
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
void OptionsGui();


int main(int argc, char const *argv[])
{
    InitWindow(W_WIDTH,W_HEIGHT,"Best Path Search Algo Viz");
    SetTargetFPS(60);
    GuiEnable(); 
    
    while (!WindowShouldClose())
    {
        AnimationGui();
        OptionsGui();

        BeginDrawing();
        EndDrawing();
    }
    


    GuiDisable();
    CloseWindow();   
    return 0;
}

void AnimationGui(){
    //to start the animation
    if(!animationPlaying){
        if(GuiButton({685,10,30,30},"#131#")){ //if clicked start animation
            animationPlaying = true;
        }
        return;
    }

    //if not paused shows pause button
    if(!animationPause){
        if(GuiButton({685,10,30,30},"#132#")){
            animationPause = true;
        }
    }
    else{
        if(GuiButton({685,10,30,30},"#131#")){
            animationPause = false;
        }
    }

    //repeat button
    if (GuiButton({700,10,30,30},"#57#"))
    {
        
    }
    //stop button
    if(GuiButton({},"")){
        
    }


}
void OptionsGui(){
    if(animationPlaying or animationPause)
        return;
    

}