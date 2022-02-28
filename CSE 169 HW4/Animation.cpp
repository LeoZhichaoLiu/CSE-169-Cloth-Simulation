//
//  Animation.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Animation.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

Animation::Animation (const char *file) {
    Load (file);
}

Animation::~Animation () {}

void Animation::Evaluate (float time, Skeleton * skeleton) {
       
        float x_translation = channel_list[0]->Evaluate(time);
        float y_translation = channel_list[1]->Evaluate(time);
        float z_translation = channel_list[2]->Evaluate(time);
    
        skeleton->root->Offset.x = x_translation;
        skeleton->root->Offset.y = y_translation;
        skeleton->root->Offset.z = z_translation;
    
    for (int i = 3; i < channel_list.size(); i=i+3) {
        float x_value = channel_list[i]->Evaluate(time);
        float y_value = channel_list[i+1]->Evaluate(time);
        float z_value = channel_list[i+2]->Evaluate(time);
        
        if (i == 6) {
            skeleton->Joint_List[(i/3)-1]->List1[0]->setValue(x_value);
            skeleton->Joint_List[(i/3)-1]->List1[1]->setValue(y_value);
            skeleton->Joint_List[(i/3)-1]->List1[2]->setValue(z_value);
        } else {
        skeleton->Joint_List[(i/3)-1]->List1[0]->setValue(x_value);
        skeleton->Joint_List[(i/3)-1]->List1[1]->setValue(y_value);
        skeleton->Joint_List[(i/3)-1]->List1[2]->setValue(z_value);
        }
    }
    
    
}
bool Animation::Load (const char *file) {
    Tokenizer token;
    char temp [256];
    
    token.Open (file);
    
    token.FindToken ("animation");
    token.FindToken ("{");
    token.FindToken ("range");
    start_time = token.GetFloat();
    end_time = token.GetFloat();
    token.FindToken ("numchannels");
    numChannel = token.GetInt();
    
    for (int i = 0; i< numChannel; i++) {
         Channel * channel = new Channel ();
         channel -> Load (token);
         channel_list.push_back (channel);
    }
    
    token.FindToken ("}");
    token.Close();
    
    return true;
}
