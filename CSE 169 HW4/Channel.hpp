//
//  Channel.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Channel_hpp
#define Channel_hpp

#include <stdio.h>
#include "Joint.hpp"
#include "Tokenizer.h"
#include "Skeleton.hpp"
#include "Keyframe.hpp"

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

class Channel {
    
public:
    
    Channel ();
    ~Channel ();
    
    std::vector<Keyframe *> key_list;
    int index;
    string extrapolate1;
    string extrapolate2; 
    
    float Evaluate(float time);
    bool Load (Tokenizer &t);
    void preCompute_t ();
    void preCompute_c ();
    
};

#endif /* Channel_hpp */
