#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "raylib.h"
#include <iostream>

#define C_SIDE 64
#define MARGIN 10

const Color cNotActive = {128,128,128,255};
const Color cActive = {255,255,128,255};
const Color cVisited = {128,255,255,255};
const Color cStart = {0,128,0,255};
const Color cEnd = {230,0,0,255};
const Color cPath = {0,255,0,255};

class Cell
{
    public:
    bool notActive,visited,start,end,path;
    

    Cell(){
        path = end = start = visited = notActive = false;
    }
    void SetPosition(int i,int j){

    }
    void Draw(int i,int j){
        float posX = C_SIDE + (i-1)*(C_SIDE+MARGIN);
        float posY = C_SIDE + (j-1)*(C_SIDE+MARGIN);
        std::cout<<posX<<"-"<<posY<<'\n';
        if(notActive){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,3,cNotActive);
            return;
        }

        if(start){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,3,cStart);
            return;
        }

        if (end)
        {
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,3,cEnd);
            return;
        }

        if(path){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,3,cPath);
            return;
        }

        if(visited){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,3,cVisited);
        }
        
        DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,0,cActive);
        
    }
    
};

#endif