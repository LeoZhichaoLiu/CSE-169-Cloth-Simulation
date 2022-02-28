//
//  Particle.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>

//#include "SpringDamper.hpp"
//#include "Triangle.hpp"

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

class Particle {

public:
    
    glm::vec3 position;
    glm::vec3 velocity = glm::vec3 (0.f, 0.f, 0.f);
    float mass;
    glm::vec3 force = glm::vec3 (0.f, 0.f, 0.f);
    glm::vec3 acceleration;
    glm::vec3 normal;
    int num;
    
    bool fixed = false;
    
    Particle();
    ~Particle();
    
    void ComputeGravity();
    void Update (float deltaTime);
};

#endif /* Particle_hpp */
