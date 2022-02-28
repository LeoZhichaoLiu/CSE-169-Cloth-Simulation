//
//  Triangle.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp

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

class Triangle {

public:
    
    Particle *P1, *P2, *P3;
    glm::vec3 tri_normal;
    glm::vec3 velocity;
    float area;
    glm::vec3 Aero_force;
    
    Triangle();
    ~Triangle();
    
    void computeAero(const glm::vec3 &air_velocity);
    
};

#endif /* Triangle_hpp */
