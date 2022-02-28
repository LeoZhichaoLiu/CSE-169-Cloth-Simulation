//
//  SpringDamper.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "SpringDamper.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>


SpringDamper::SpringDamper() {}

SpringDamper::~SpringDamper() {}

void SpringDamper::ComputeForce() {
    glm::vec3 e_old = P2->position - P1->position;
    float length = glm::length (e_old);
    glm::vec3 e;
    
    if (length == 0.f) {
      return;
    } else {
      e = e_old / length;
    }
    
    float v_close = glm::dot ((P1->velocity - P2->velocity), e);
    float final_force = -SpringConstant * (RestLength - length) - DampingConstant * v_close;
    
    glm::vec3 f1 = final_force * e;
    glm::vec3 f2 = -f1;
    
    P1->force += f1;
    P2->force += f2;
}
