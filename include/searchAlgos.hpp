#ifndef SEARCHALGO_HPP
#define SEARCHALGO_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include "matrixviz.hpp"
#include "raylib.h"

void BFS(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc);
void DFS(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc);
void UCS(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc);
void Astar(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc);



//number of algorithm
#define NB_ALG 4

void (*algorithm[NB_ALG])(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc)=
{
    BFS,DFS,UCS,Astar
};

char algoNames[] = "BFS\nDFS\nUCS\nA*";



struct MatrixNode{
    index2D idx;
    float cost;
    MatrixNode* parent;
};

MatrixNode* MakeMatrixNode(index2D idx,MatrixNode* parent){
    MatrixNode* out = new MatrixNode();
    out->idx = idx;
    out->parent = parent;
    return out; 
}
MatrixNode* MakeMatrixNode(index2D idx,MatrixNode* parent,int cost){
    MatrixNode* out = new MatrixNode();
    out->idx = idx;
    out->cost = cost;
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
    return out;
}

bool AlreadyIn(std::vector<MatrixNode*>& arr,MatrixNode* tofind){
    for(MatrixNode* el : arr)
        if(tofind->idx == el->idx)
            return true;
    return false;
}

void ShowSolution(MatrixNode* solution,MatrixViz& mtx){
    //show the path (to func)
    if(solution!=nullptr)
        for(MatrixNode* curr = solution->parent;curr != nullptr;curr = curr->parent){
            mtx.table[curr->idx.i][curr->idx.j].path=true;
        }
}

/*
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
*/

void BFS(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc){

    MatrixNode* solution = nullptr;
    std::vector<MatrixNode*> Queue;
    Queue.push_back(MakeMatrixNode(mtx.start,nullptr));
    

    while (animationPlaying and !Queue.empty())
    {
        //animation delay and pause 
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
        if(animationPause)
            continue;

        //animation update (algo)
        MatrixNode* curr = Queue.front(); Queue.erase(Queue.begin());
        if(curr->idx == mtx.end){
            solution = curr;
            break;
        }
        
        std::vector<MatrixNode*> Neighbors = getNeighbors(mtx,curr);
        for(MatrixNode* ngb : Neighbors)
            if(!mtx.table[ngb->idx.i][ngb->idx.j].visited 
                and !mtx.table[ngb->idx.i][ngb->idx.j].notActive 
                and !AlreadyIn(Queue,ngb)){
                    mtx.At(ngb->idx).toVisit = true;
                    Queue.push_back(ngb);

                }
        mtx.table[curr->idx.i][curr->idx.j].visited = true;

        /*
        std::cout<<"SS- "<<index2DStr(curr->idx)<<" ->";
        for(MatrixNode* mn : Queue){
            std::cout<<index2DStr(mn->idx)<<'_';
        }
        std::cout<<'\n';
        */
    }

    ShowSolution(solution,mtx);

}

void DFS(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc){
    MatrixNode* solution = nullptr;
    std::vector<MatrixNode*> Stack;
    Stack.push_back(MakeMatrixNode(mtx.start,nullptr));
    

    while (animationPlaying and !Stack.empty())
    {
        //animation delay and pause 
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
        if(animationPause)
            continue;

        //animation update (algo)
        MatrixNode* curr = Stack.back(); Stack.pop_back();
        if(curr->idx == mtx.end){
            solution = curr;
            break;
        }
        
        std::vector<MatrixNode*> Neighbors = getNeighbors(mtx,curr);
        
        for(int i = Neighbors.size()-1;i>=0;i--)
            if(!mtx.At(Neighbors[i]->idx).visited 
                and !mtx.At(Neighbors[i]->idx).notActive){
                    Stack.push_back(Neighbors[i]);
                    mtx.At(Neighbors[i]->idx).toVisit = true;
                }  
        mtx.table[curr->idx.i][curr->idx.j].visited = true;

    }

    ShowSolution(solution,mtx);

}

float EuclideanDistance(index2D curr,index2D dest){
    return sqrtf((float) (dest.i-curr.i)*(dest.i-curr.i) +  (dest.j-curr.j)*(dest.j-curr.j));
}

int ManhattanDistance(index2D curr,index2D dest){
    return abs(dest.i-curr.i) +  abs(dest.j-curr.j);
}

int CompareCost(MatrixNode* l,MatrixNode* r){
    return l->cost-r->cost;
}

int CompareCostHeuristic(MatrixNode* l,MatrixNode* r,index2D end){
    return (int)ceil((float(l->cost) + EuclideanDistance(l->idx,end)) - (float(r->cost) + EuclideanDistance(r->idx,end)));
}

MatrixNode* PrioQueuePop(std::vector<MatrixNode*>& Q,int (compare)(MatrixNode*,MatrixNode*)){
    int best = 0;
    for(int i = 0;i<Q.size();i++)
        if(compare(Q[best],Q[i]) > 0)
            best = i;
    MatrixNode* out = Q[best];
    Q.erase(Q.begin()+best);
    return out;
}

MatrixNode* PrioQueuePop(std::vector<MatrixNode*>& Q,index2D end,int (compare)(MatrixNode*,MatrixNode*,index2D)){
    int best = 0;
    for(int i = 0;i<Q.size();i++)
        if(compare(Q[best],Q[i],end) > 0)
            best = i;
    MatrixNode* out = Q[best];
    Q.erase(Q.begin()+best);
    return out;
}

bool AlreadyIn(std::vector<MatrixNode*>& arr,MatrixNode* tofind,int (compare)(MatrixNode*,MatrixNode*)){
    for(MatrixNode* el : arr)
        if(tofind->idx == el->idx and compare(tofind,el)>0)
            return true;
    return false;
}

bool AlreadyIn(std::vector<MatrixNode*>& arr,MatrixNode* tofind,index2D end,int (compare)(MatrixNode*,MatrixNode*,index2D)){
    for(MatrixNode* el : arr)
        if(tofind->idx == el->idx and compare(tofind,el,end)>0)
            return true;
    return false;
}

void UpdateCost(std::vector<MatrixNode*>& Q,MatrixNode* toUpdate){
    for(MatrixNode* el : Q)
        if(toUpdate->idx == el->idx){
            el->cost = toUpdate->cost;
            el->parent = toUpdate->parent;
            return;
        }     
}

std::vector<MatrixNode*> getNeighborsCost(MatrixViz& mtx,MatrixNode* currNode){
    std::vector<MatrixNode*> out;
    index2D idx = currNode->idx;
    if(idx.i+1<mtx.size)
        out.push_back(MakeMatrixNode({idx.i+1,idx.j},currNode,currNode->cost+1));
    if(idx.j+1<mtx.size)
        out.push_back(MakeMatrixNode({idx.i,idx.j+1},currNode,currNode->cost+1));
    if(idx.i-1>=0)
        out.push_back(MakeMatrixNode({idx.i-1,idx.j},currNode,currNode->cost+1));
    if(idx.j-1>=0)
        out.push_back(MakeMatrixNode({idx.i,idx.j-1},currNode,currNode->cost+1));
    return out;
}

void UCS(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc){
    
    MatrixNode* solution = nullptr;
    std::vector<MatrixNode*> Pqueue;
    Pqueue.push_back(MakeMatrixNode(mtx.start,nullptr,0));
    

    while (animationPlaying and !Pqueue.empty())
    {
        //animation delay and pause 
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
        if(animationPause)
            continue;

        //animation update (algo)
        MatrixNode* curr = PrioQueuePop(Pqueue,CompareCost);
        if(curr->idx == mtx.end){
            solution = curr;
            break;
        }
        
        std::vector<MatrixNode*> Neighbors = getNeighborsCost(mtx,curr);
        for(MatrixNode* ngb : Neighbors)
            if(!mtx.table[ngb->idx.i][ngb->idx.j].visited 
                and !mtx.table[ngb->idx.i][ngb->idx.j].notActive 
                and !AlreadyIn(Pqueue,ngb)){
                    mtx.At(ngb->idx).toVisit = true;
                    Pqueue.push_back(ngb);
                }
            else if(AlreadyIn(Pqueue,ngb,CompareCost))
                UpdateCost(Pqueue,ngb);
        mtx.table[curr->idx.i][curr->idx.j].visited = true;
    }

    ShowSolution(solution,mtx);

}

void Astar(MatrixViz& mtx,int delay,bool& animationPlaying,bool& animationPause,std::mutex& matrixAcc){
    
    MatrixNode* solution = nullptr;
    std::vector<MatrixNode*> Pqueue;
    Pqueue.push_back(MakeMatrixNode(mtx.start,nullptr,0));
    

    while (animationPlaying and !Pqueue.empty())
    {
        //animation delay and pause 
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
        if(animationPause)
            continue;

        //animation update (algo)
        MatrixNode* curr = PrioQueuePop(Pqueue,mtx.end,CompareCostHeuristic);
        if(curr->idx == mtx.end){
            solution = curr;
            break;
        }
        
        std::vector<MatrixNode*> Neighbors = getNeighborsCost(mtx,curr);
        for(MatrixNode* ngb : Neighbors)
            if(!mtx.table[ngb->idx.i][ngb->idx.j].visited 
                and !mtx.table[ngb->idx.i][ngb->idx.j].notActive 
                and !AlreadyIn(Pqueue,ngb)){
                    mtx.At(ngb->idx).toVisit = true;
                    Pqueue.push_back(ngb);
                }
            else if(AlreadyIn(Pqueue,ngb,mtx.end,CompareCostHeuristic))
                UpdateCost(Pqueue,ngb);
        mtx.table[curr->idx.i][curr->idx.j].visited = true;
    }

    ShowSolution(solution,mtx);
}


#endif