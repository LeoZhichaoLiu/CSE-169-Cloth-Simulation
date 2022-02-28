//
//  Cloth.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Cloth.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

Cloth::Cloth () {
    initialization ();
    
    material = new Material ();
    
    material -> ambient = glm::vec3 (0.215, 0.4745, 0.215);
    material -> diffuse = glm::vec3 (0.07568, 0.71424, 0.07568);
    material -> specular = glm::vec3 (0.533, 0.727811, 0.633);
    material -> shininess = 0.6;
}

Cloth::~Cloth () {}

void Cloth::initialization() {
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Particle * P = new Particle();
            P->mass = 0.1f;
            P->num = i * height + j;
            P->position = glm::vec3(0.1f * (((float) j) - width/2.0f), 0.1f * i + 0.5f, 0.f);
            
            if (i == height-1) {
                P->fixed = true;
            } else {
                P->fixed = false;
            }
            
            particle_list.push_back (P);
            positions.push_back(P->position);
            normals.push_back(glm::vec3(0.f, 0.f, 0.f));
        }
     }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i < height - 1) {
                SpringDamper * sd = new SpringDamper();
                sd->P1 = particle_list[i*height + j];
                sd->P2 = particle_list[i*height + height + j];
                sd->RestLength = glm::length(sd->P1->position - sd->P2->position);
                spring_list.push_back(sd);
                
                if (j > 0) {
                    SpringDamper * sd2 = new SpringDamper();
                    sd2->P1 = particle_list[i*height + j];
                    sd2->P2 = particle_list[i*height + height + j - 1];
                    sd2->RestLength = glm::length(sd2->P1->position - sd2->P2->position);
                    spring_list.push_back(sd2);
                }
                
                if (j < width - 1) {
                    SpringDamper * sd3 = new SpringDamper();
                    sd3->P1 = particle_list[i*height + j];
                    sd3->P2 = particle_list[i*height + height + j + 1];
                    sd3->RestLength = glm::length(sd3->P1->position - sd3->P2->position);
                    spring_list.push_back(sd3);
                }
            }
            
            if (j < width - 1) {
                SpringDamper * sd4 = new SpringDamper();
                sd4->P1 = particle_list[i*height + j];
                sd4->P2 = particle_list[i*height + j + 1];
                sd4->RestLength = glm::length(sd4->P1->position - sd4->P2->position);
                spring_list.push_back(sd4);
            }
        }
    }
    
    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < width-1; j++) {
            int index1 = i * height + j;
            int index2 = i * height + j + 1;
            int index3 = i * height + height + j;
            indices.push_back(glm::ivec3(index1, index2, index3));
            
            Triangle * t = new Triangle();
            t->P1 = particle_list[index1];
            t->P2 = particle_list[index2];
            t->P3 = particle_list[index3];
            triangle_list.push_back (t);
            
            int index4 = i * height + height + j + 1;
            indices.push_back(glm::ivec3(index2, index3, index4));
            
            Triangle * t2 = new Triangle();
            t2->P1 = particle_list[index2];
            t2->P2 = particle_list[index4];
            t2->P3 = particle_list[index3];
            triangle_list.push_back (t2);
        }
    }
    // Calculate the normal for each particle.
    update_normal();
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
     
}

void Cloth::Update (float deltaTime) {
    
    for(int i = 0; i < 5; i++) {
        
        for (Particle * p : particle_list) {
            p->Update(deltaTime);
            positions[p->num] = p->position;
        }
        
        for (SpringDamper * sd : spring_list) {
            sd->ComputeForce();
            //cout << sd->P1->force.x << " " << sd->P1->force.y << " " << sd->P1->force.z << endl;
        }

        for (Triangle * t : triangle_list) {
            t->computeAero(air_velocity);
        }
        
        update_normal();
    }
    update_data();
}

void Cloth::Draw (const glm::mat4& viewProjMtx, GLuint shader) {
    
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    glUniform3fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(material->ambient));
    glUniform3fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(material->diffuse));
    glUniform3fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(material->specular));
    glUniform1f(glGetUniformLocation(shader, "shininess"), material->shininess);

    // Bind the VAO
    glBindVertexArray(VAO);
    
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, sizeof(glm::ivec3) * indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

// Update the bind data to draw a new sence.
void Cloth::update_data () {
    
    // Bind to the VAO.
    glBindVertexArray(VAO);
    
    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cloth::update_normal () {
    for (Particle * p : particle_list) {
        p->normal = glm::vec3(0.f, 0.f, 0.f);
        normals[p->num] = glm::vec3(0.f, 0.f, 0.f);
    }
    
    for (Triangle * t : triangle_list) {
        Particle* p1 = t->P1;
        Particle* p2 = t->P2;
        Particle* p3 = t->P3;
        glm::vec3 n1 = p2->position - p1->position;
        glm::vec3 n2 = p3->position - p1->position;
        glm::vec3 new_n = glm::normalize(glm::cross(n1, n2));
        
        p1->normal += new_n;
        p2->normal += new_n;
        p3->normal += new_n;
        
        normals[p1->num] += new_n;
        normals[p2->num] += new_n;
        normals[p3->num] += new_n;
    }
    
    for (int i = 0; i < normals.size(); i++) {
        normals[i] = glm::normalize(normals[i]);
    }
}

void Cloth::change_fixed (glm::vec3 offset) {
    for (Particle * p : particle_list) {
        if (p->fixed) {
            if (p->position.y > -0.9) {
                p->position += offset;
            } else {
                p->position -= offset;
            }
        }
    }
}
