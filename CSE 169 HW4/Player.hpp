//
//  Player.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Joint.hpp"
#include "Tokenizer.h"
#include "Skeleton.hpp"
#include "Animation.hpp"


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

class Player {
    
public:
    
    Player ();
    ~Player ();
    
    float Time;
    Animation * Clip;
    //Pose CurrentPose;
    Skeleton * skeleton;
    float start, end;
    
    void SetClip(Animation* c);
    //Pose & GetCurrentPose();
    void Update (float deltaTime);
};

#endif /* Player_hpp */
