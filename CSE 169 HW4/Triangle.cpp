//
//  Triangle.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Triangle.hpp"

Triangle::Triangle() {
    
}

Triangle::~Triangle() {}


void Triangle::computeAero (const glm::vec3 &air_velocity) {
    
    this->velocity = (P1->velocity + P2->velocity + P3->velocity) / 3.0f - air_velocity;
    
    glm::vec3 temp_normal = glm::cross ((P2->position -P1->position), (P3->position -P1->position));
    
    if (glm::length (temp_normal) == 0.f) {
        return;
    }
    
    this->tri_normal = temp_normal / glm::length (temp_normal);
    
    float temp_area = 0.5 * glm::length (temp_normal);
    
    if (glm::length(this->velocity) == 0.f) {
        return;
    }
    
    area = temp_area * glm::dot ((this->velocity / glm::length(this->velocity)), this->tri_normal );
    
    Aero_force = -0.5f * 1225.f * glm::length(this->velocity) * glm::length(this->velocity)
                    * 1.28f * area * this->tri_normal;
    
    P1->force += Aero_force / 3.0f;
    P2->force += Aero_force / 3.0f;
    P3->force += Aero_force / 3.0f;
}


