//
//  Grid.cpp
//  kinectExampleNew
//
//  Created by Stijn on 15/01/2019.
//

#include "Grid.hpp"
#include "ofMain.h"

Grid::Grid(){
    for(int i = 0; i<3 ; i++)
        for(int j = 0; j<(gridSize); j++)
            for(int k = 0; k<(gridSize) ; k++){
                color[j][k][i] = std::rand()%255;
    }
}

void Grid::draw(){
    for(int i = 0; i<gridSize ; i++) for(int j = 0; j<gridSize ; j++){
        ofSetColor(color[i][j][0], color[i][j][1], color[i][j][2]);
        ofDrawRectangle(gridElementSize*gridSize-i*gridElementSize, j*gridElementSize, gridElementSize, gridElementSize);
    }
}

void Grid::setColor(int x, int y, bool on){
    for(int i = 0; i<3 ; i++){
        if(on){
        color[x][y][i] = 255;
        }else{
          color[x][y][i] = 0;
        }
    }
}
