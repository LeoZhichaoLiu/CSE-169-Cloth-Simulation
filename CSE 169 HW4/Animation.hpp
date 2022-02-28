//
//  Animation.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>
#include "Joint.hpp"
#include "Tokenizer.h"
#include "Skeleton.hpp"
#include "Channel.hpp"

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

class Animation {
    
public:
    
    Animation (const char *file);
    ~Animation ();
    
    std::vector<Channel *> channel_list;
    float start_time;
    float end_time;
    int numChannel;
    
    //Skeleton * skeleton;
    
    void Evaluate (float time, Skeleton * skeleton);
    bool Load (const char *filename);
};

#endif /* Animation_hpp */
