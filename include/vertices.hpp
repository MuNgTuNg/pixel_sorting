#pragma once
#include <vector>
namespace shb{
std::vector<float> defaultQuadVerts{
    // positions         // texture coords
    // positions         // texture coords
    -1.f,  1.f, 0.f,       0.f, 1.f, //top left front
    -1.f, -1.f, 0.f,       0.f, 0.f, //bottom left front
     1.f, -1.f, 0.f,       1.f, 0.f, //bottom right front
    -1.f,  1.f, 0.f,       0.f, 1.f, //top left front
     1.f, -1.f, 0.f,       1.f, 0.f, //bottom right front
     1.f,  1.f, 0.f,       1.f, 1.f,  //top right front 
};
std::vector<unsigned int> defaultQuadIndices{
    1,2,3,
    2,5,0
};

}//namespace shb