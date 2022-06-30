#ifndef SEARCHALGO_HPP
#define SEARCHALGO_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <thread>
#include <mutex>
#include <chrono>
#include "matrixviz.hpp"
#include "raylib.h"

struct MatrixNode{
    index2D idx;
    MatrixNode* parent;
};

MatrixNode* MakeMatrixNode(index2D idx,MatrixNode* parent){
    MatrixNode* out = new MatrixNode();
    out->idx = idx;
    out->parent = parent;
    return out; 
}

std::vector<MatrixNode*> getNeighbors(MatrixViz& mtx,MatrixNode* currNode){
    std::vector<MatrixNode*> out;
    index2D idx = currNode->idx;
    if(idx.i+1<mtx.size)
        out.push_back(MakeMatrixNode({idx.i+1,idx.j},currNode));
    if(idx.j+1<mtx.size)
        out.push_back(MakeMatrixNode({idx.i,idx.j+1},currNode));
    if(idx.i-1>=0)
        out.push_back(MakeMatrixNode({idx.i-1,idx.j},currNode));
    if(idx.j-1>=0)
        out.push_back(MakeMatrixNode({idx.i,idx.j-1},currNode));
    std::cout<<"Q-"<<index2DStr(idx)<<"->";
        for(MatrixNode* mn : out){
            std::cout<<index2DStr(mn->idx)<<'_';
        }
    std::cout<<'\n';
    return out;
}

bool AlreadyIn(std::vector<MatrixNode*>& arr,MatrixNode* tofind){
    for(MatrixNode* el : arr)
        if(tofind->idx == el->idx)
            return true;
    return false;
}

void TestNaiveAlgo(MatrixViz& mtx,int delay,bool& animationPlaying){
    index2D idx = mtx.start;
    
    while (animationPlaying)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
        if(mtx.table[idx.i][idx.j].end)break;
        mtx.table[idx.i][idx.j].visited = true;
        std::cout<<"modified\n";
        if(idx.i<mtx.size-1)idx.i++;
        if(idx.j<mtx.size-1)idx.j++;
    }
    
}

void BFS(MatrixViz& mtx,int delay,bool& animationPlaying,std::mutex& matrixAcc){

    MatrixNode* solution = nullptr;
    std::vector<MatrixNode*> Queue;
    Queue.push_back(MakeMatrixNode(mtx.start,nullptr));
    

    while (animationPlaying and !Queue.empty())
    {
        
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
        MatrixNode* curr = Queue.front(); Queue.erase(Queue.begin());
        if(curr->idx == mtx.end){
            solution = curr;
            break;
        }
        std::cout<<curr->idx.i<<'-'<<curr->idx.j<<'\n';
        std::vector<MatrixNode*> Neighbors = getNeighbors(mtx,curr);
        for(MatrixNode* ngb : Neighbors)
            if(!mtx.table[ngb->idx.i][ngb->idx.j].visited 
                and !mtx.table[ngb->idx.i][ngb->idx.j].notActive 
                and !AlreadyIn(Queue,ngb))
                Queue.push_back(ngb);
            
        mtx.table[curr->idx.i][curr->idx.j].visited = true;

        std::cout<<"SS- "<<index2DStr(curr->idx)<<" ->";
        for(MatrixNode* mn : Queue){
            std::cout<<index2DStr(mn->idx)<<'_';
        }
        std::cout<<'\n';

    }

    if(solution!=nullptr)
        for(MatrixNode* curr = solution->parent;curr != nullptr;curr = curr->parent){
            mtx.table[curr->idx.i][curr->idx.j].path=true;
        }
}



#endif