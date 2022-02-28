//
//  ground_plane.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/24.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef ground_plane_hpp
#define ground_plane_hpp

#include <stdio.h>

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

class ground_plane {
  
public:
    
    struct Material {
        public :
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
    
    Material * material;
    
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f, 0.f, 0.f);
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    
    ground_plane();
    ~ground_plane();
    
    void Draw (const glm::mat4& viewProjMtx, GLuint shader);
    
};

#endif /* ground_plane_hpp */
