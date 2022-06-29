#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "raylib.h"
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#define C_SIDE 32
#define MARGIN 5

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
    
    void Draw(int i,int j){
        float posX = i*(C_SIDE+MARGIN);
        float posY = j*(C_SIDE+MARGIN);
        
        if(notActive){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,0,cNotActive);
            return;
        }

        if(start){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,0,cStart);
            return;
        }

        if (end)
        {
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,0,cEnd);
            return;
        }

        if(path){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,0,cPath);
            return;
        }

        if(visited){
            DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,0,cVisited);
        }
        
        DrawRectangleRounded({posX,posY,C_SIDE,C_SIDE},0.5,0,cActive);
        
    } 
};

class MatrixViz
{
public:
    int size;
    std::vector<std::vector<Cell>> table;

    MatrixViz(){
        size = 15;
        table.resize(15,std::vector<Cell>(15));
    }

    MatrixViz(int size){
        this->size = size;
        table.resize(size,std::vector<Cell>(size));
    }

    void Draw(){
        
        for(int i = 0;i<size;i++)
            for(int j = 0;j<size;j++){
                
                table[i][j].Draw(i,j);
            }
        
        
    }

    void Clicked(Vector2 pos){
        std::pair<int,int> indxs = getCell(pos);
        table[indxs.first][indxs.second].notActive != table[indxs.first][indxs.second].notActive;
    }

    std::pair<int,int> getCell(Vector2 pos){
        int i = floor(pos.x)/(C_SIDE+MARGIN);
        int j = floor(pos.y)/(C_SIDE+MARGIN); 
        return std::pair<int,int>{i,j};
    }
    
};




#endif