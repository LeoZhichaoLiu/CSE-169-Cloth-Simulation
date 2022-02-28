//
//  SpringDamper.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef SpringDamper_hpp
#define SpringDamper_hpp

#include <stdio.h>
#include "Particle.hpp"

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

#include <stdio.h>

class SpringDamper {
   
public:
    
    float SpringConstant = 3000.5f;
    float DampingConstant = 10.f;
    float RestLength;
    Particle *P1, *P2;
    
    SpringDamper();
    ~SpringDamper();
    
    void ComputeForce();
};

#endif /* SpringDamper_hpp */
