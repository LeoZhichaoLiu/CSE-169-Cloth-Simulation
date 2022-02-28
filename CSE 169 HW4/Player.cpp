//
//  Player.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Player.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

Player::Player () {}

Player::~Player() {}

void Player::SetClip(Animation* c) {
    Clip=c;
    start = c->start_time;
    end = c->end_time;
}

void Player::Update (float deltaTime) {
    //std::cout << deltaTime << endl;
    if (deltaTime <= end && deltaTime >= start) {
        Clip->Evaluate(deltaTime, skeleton);
        
    } else if (deltaTime < start) {
        while (deltaTime <start) {
           deltaTime = deltaTime + end - start;
        }
        Clip->Evaluate(deltaTime, skeleton);
        
    } else if (deltaTime > end ){
        while (deltaTime > end) {
           deltaTime = deltaTime - end + start;
        }
        Clip->Evaluate(deltaTime, skeleton);
    }
}
