//
//  Grid.hpp
//  kinectExampleNew
//
//  Created by Stijn on 15/01/2019.
//

#ifndef Grid_hpp
#define Grid_hpp

#include <stdio.h>


#define gridSize 30
#define screenSize 480
#define gridElementSize (screenSize/gridSize)

class Grid{
public:
    Grid();
    void draw();
    void setColor(int,int,bool);
    
    int color[gridSize][gridSize][3];
    
};



#endif /* Grid_hpp */
