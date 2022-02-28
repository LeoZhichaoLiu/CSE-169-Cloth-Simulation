//
//  Channel.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Channel.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

Channel::Channel () {}

Channel::~Channel () {}

float Channel::Evaluate(float time) {
    
    int pre = 0;
    int post = 0;
    float initial = 0.0;
    
    if (time < key_list[0]->Time) {
        if (extrapolate1 == "constant") {
            return key_list[0]->Value;
            
        } else if (extrapolate1 == "cycle"){
            while (time < key_list[0]->Time) {
                time = time + (key_list[key_list.size()-1]->Time - key_list[0]->Time);
            }
            
        } else if (extrapolate1 == "cycle_offset") {
            while (time < key_list[0]->Time) {
                time = time + (key_list[key_list.size()-1]->Time - key_list[0]->Time);
                initial = initial - (key_list[key_list.size()-1]->Value - key_list[0]->Value);
            }
            
        } else if (extrapolate1 == "linear") {
            
            return (key_list[0]->Value - (key_list[0]->TangentOut) * (key_list[0]->Time - time));
            
        } else if (extrapolate1 == "bounce") {
            while (time < key_list[0]->Time) {
                time = time + 2 * (key_list[key_list.size()-1]->Time - key_list[0]->Time);
            }
            if (time > key_list[key_list.size()-1]->Time) {
                time = key_list[key_list.size()-1]->Time - (time - key_list[key_list.size()-1]->Time);
            }
        }
    }
    
    if (time > key_list[key_list.size()-1]->Time) {
        
        
        if (extrapolate2 == "constant") {
            return key_list[key_list.size()-1]->Value;
            
        } else if (extrapolate2 == "cycle"){
            while (time > key_list[key_list.size()-1]->Time) {
                time = time - (key_list[key_list.size()-1]->Time - key_list[0]->Time);
            }
            
        } else if (extrapolate2 == "cycle_offset") {
            while (time > key_list[key_list.size()-1]->Time) {
                time = time - (key_list[key_list.size()-1]->Time - key_list[0]->Time);
                initial = initial + (key_list[key_list.size()-1]->Value - key_list[0]->Value);
            }
        } else if (extrapolate1 == "linear") {
            return (key_list[key_list.size()-1]->Value -(key_list[key_list.size()-1]->TangentIn) * (time - key_list[key_list.size()-1]->Time));
            
        } else if (extrapolate1 == "bounce") {
            while (time > key_list[key_list.size()-1]->Time) {
                time = time - 2 * (key_list[key_list.size()-1]->Time - key_list[0]->Time);
            }
            if (time < key_list[0]->Time) {
                time = key_list[0]->Time + (key_list[0]->Time - time);
            }
        }
    }
    
    for (int i = 0; i < key_list.size(); i++) {
        if (key_list[i]->Time == time) {
            return (initial + key_list[i]->Value);
            
        } else if (time > key_list[i]->Time && time < key_list[i+1]->Time) {
            pre = i;
            post = i+1;
            break;
        }
    }
    
    float res ;
    Keyframe * cube = key_list[pre];
    Keyframe * cube2 = key_list[post];
    float u = (time - cube->Time) / (cube2->Time - cube->Time);
    
    res = initial + cube->A * u * u * u + cube->B * u * u + cube->C * u + cube->D;
    
    return res;
    
}

bool Channel::Load (Tokenizer &t) {
    
    char temp [256];
    t.FindToken("channel");
    index = t.GetInt();
    t.FindToken("{");
    t.FindToken("extrapolate");
    t.GetToken (temp);
    extrapolate1 = temp;
    t.GetToken (temp);
    extrapolate2 = temp;
    
    t.FindToken("keys");
    int numKeys = t.GetInt();
    t.FindToken("{");
    
    for (int i = 0; i < numKeys; i++) {
        Keyframe * key = new Keyframe ();
        key->Time = t.GetFloat();
        key->Value = t.GetFloat();
        t.GetToken (temp);
        key->RuleIn = temp;
        t.GetToken (temp);
        key->RuleOut = temp;
        key_list.push_back (key);
    }
        
    t.FindToken("}");
    t.FindToken("}");
    
    preCompute_t();
    preCompute_c();
    
    return true;
}

void Channel::preCompute_t() {
    
    for (int i = 0; i < key_list.size(); i++) {
        
        Keyframe * key = key_list[i];
        
        if (key->RuleIn == "Flat") {
            key->TangentIn = 0;
        
        } else if (key->RuleIn == "Linear") {
            if (i == 0) {
              key->TangentIn = 0;
            } else {
              key->TangentIn = (key->Value - key_list[i-1]->Value) / (key->Time - key_list[i-1]->Time);
            }
            
        
        } else if (key->RuleIn == "Smooth") {
            if (i == 0) {
                 key->TangentIn = 0;
            } else if (i == key_list.size()-1) {
                 key->TangentIn = (key->Value - key_list[i-1]->Value) / (key->Time - key_list[i-1]->Time);
            } else {
                 key->TangentIn = (key_list[i+1]->Value - key_list[i-1]->Value) / (key_list[i+1]->Time - key_list[i-1]->Time);
            }
            
        } else {
            
             key->TangentIn = std::stof(key->RuleIn);
        }
        
        if (key->RuleOut == "Flat") {
            key->TangentOut = 0;
        
        } else if (key->RuleOut == "Linear") {
            if (i == key_list.size()-1) {
                key->TangentOut = 0;
            } else {
              key->TangentOut = (key_list[i+1]->Value - key->Value) / (key_list[i+1]->Time - key->Time);
            }
        
        } else if (key->RuleOut == "Smooth") {
            if (i == 0) {
                 key->TangentOut = (key_list[i+1]->Value - key->Value) / (key_list[i+1]->Time - key->Time);
            } else if (i == key_list.size()-1) {
                 key->TangentOut = 0;
            } else {
                 key->TangentOut = (key_list[i+1]->Value - key_list[i-1]->Value) / (key_list[i+1]->Time - key_list[i-1]->Time);
            }
            
        } else {
            
            key->TangentOut = std::stof(key->RuleOut);
        }
    }
}

void Channel::preCompute_c() {
    
    for (int i = 0; i < key_list.size()-1; i++) {
    
        Keyframe * key = key_list[i];
        Keyframe * key2 = key_list[i+1];
        
        key->A = 2 * key->Value  - 2 * key2->Value + key->TangentOut * (key2->Time - key->Time)
                  +  key2->TangentIn * (key2->Time - key->Time);
        key->B = -3 * key->Value  + 3 * key2->Value - 2 * key->TangentOut * (key2->Time - key->Time)
                  -  key2->TangentIn * (key2->Time - key->Time);
        key->C = key->TangentOut * (key2->Time - key->Time);
        key->D = key->Value;
    }
}

