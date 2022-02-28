//
//  Particle.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Particle.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

Particle::Particle() {}

Particle::~Particle() {}


void Particle::ComputeGravity() {
    this->force += mass * glm::vec3(0.f, -9.8f, 0.f);
}

void Particle::Update (float deltaTime) {
    
    if (fixed == false) {
        ComputeGravity();
        //cout << force.x << " " << force.y << " " << force.z << endl;
        acceleration = this->force / mass;
        velocity = velocity + acceleration * 1.f/600.f;
        position = position + velocity * 1.f/600.f;
        
        if (position.y <= -0.9) {
            position.y = -1.8 - position.y;
            velocity.x = velocity.x * 0.15f;
            velocity.y = velocity.y * -0.1f;
            velocity.z =  velocity.z * 0.15f;
        }
        
        force = glm::vec3(0.f, 0.f, 0.f);
    }
    
}
