//
//  ground_plane.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/24.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "ground_plane.hpp"


ground_plane::ground_plane() {
    
    material = new Material ();
    
    material -> ambient = glm::vec3 (0.415, 0.1745, 0.615);
    material -> diffuse = glm::vec3 (0.3568, 0.071424, 0.7568);
    material -> specular = glm::vec3 (0.533, 0.27811, 0.633);
    material -> shininess = 0.3;
    
    positions = {
        glm::vec3(-8.f, -1.f, 8.f),
        glm::vec3(-8.f, -1.f, -8.f),
        glm::vec3(8.f, -1.f, -8.f),
        glm::vec3(8.f, -1.f, 8.f)
        
    };
    
    normals = {
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f)
    };
    
    indices = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(0, 2, 3)
    };
    
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

ground_plane::~ground_plane() {}

void ground_plane::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
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
