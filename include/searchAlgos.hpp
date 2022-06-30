#ifndef SEARCHALGO_HPP
#define SEARCHALGO_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include "matrixviz.hpp"
#include "raylib.h"

void TestNaiveAlgo(MatrixViz& mtx,int delay,bool& animationPlaying){
    std::cout<<"here\n";
    double LastT = GetTime();
    index2D idx = mtx.start;
    
    while (animationPlaying)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds((int)delay));
        if(mtx.table[idx.i][idx.j].end)break;
        mtx.table[idx.i][idx.j].visited = true;
        std::cout<<"modified\n";
        if(idx.i<mtx.size-1)idx.i++;
        if(idx.j<mtx.size-1)idx.j++;
        LastT = GetTime();
        
    }
    std::cout<<"here\n";
}

#endif