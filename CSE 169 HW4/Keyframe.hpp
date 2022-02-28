//
//  Keyframe.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Keyframe_hpp
#define Keyframe_hpp

#include <stdio.h>
#include "Joint.hpp"
#include "Tokenizer.h"
#include "Skeleton.hpp"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

using namespace std;

class Keyframe {
    
public:
    
    Keyframe ();
    ~Keyframe ();
    
    float Time;
    float Value;
    float TangentIn = 0;
    float TangentOut = 0;
    string RuleIn,RuleOut;   // Tangent rule
    float A,B,C,D;   // Cubic coefficients
    
    //void calculate();
};

#endif /* Keyframe_hpp */
