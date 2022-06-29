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

struct index2D
{
    int i,j;
};


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
    index2D start,end;

    MatrixViz(){
        size = 15;
        table.resize(15,std::vector<Cell>(15));
        SetStart({0,0});
        SetEnd({14,14});
    }

    MatrixViz(int size){
        this->size = size;
        table.resize(size,std::vector<Cell>(size));
        SetStart({0,0});
        SetEnd({size-1,size-1});
    }

    void SetStart(index2D idx){
        table[start.i][start.j].start = false;
        table[idx.i][idx.j].start = true;
        start = idx;
    }

    void SetEnd(index2D idx){
        table[end.i][end.j].end = false;
        table[idx.i][idx.j].end = true;
        end = idx;
    }

    void Draw(){
        for(int i = 0;i<size;i++)
            for(int j = 0;j<size;j++){
                table[i][j].Draw(i,j);
            }
    }

    void Clicked(Vector2 pos){
        if(pos.x<0 or pos.y<0)
            return;
        index2D index = getCell(pos);
        table[index.i][index.j].notActive = ! table[index.i][index.j].notActive;
    }

    index2D getCell(Vector2 pos){
        int i = floor(pos.x)/(C_SIDE+MARGIN);
        int j = floor(pos.y)/(C_SIDE+MARGIN); 
        return {std::min(i,size-1),std::min(j,size-1)};
    }  
};




#endif