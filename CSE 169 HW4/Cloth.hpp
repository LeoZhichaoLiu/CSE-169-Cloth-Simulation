//
//  Cloth.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/2/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Cloth_hpp
#define Cloth_hpp

#include <stdio.h>

#include "Particle.hpp"
#include "SpringDamper.hpp"
#include "Triangle.hpp"
#include "Material.hpp"

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

class Cloth {

public:
    
    std::vector <Particle *> particle_list;
    std::vector <SpringDamper *> spring_list;
    std::vector <Triangle *> triangle_list;
    
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
    
    int height = 20;
    int width = 20;
    glm::vec3 air_velocity = glm::vec3 (0.1f, 0.1f, 0.1f);
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    
    Cloth();
    ~Cloth();
    
    void initialization ();
    void Update (float deltaTime);
    void update_data();
    void update_normal();
    void change_fixed (glm::vec3 offset);
    
    void Draw (const glm::mat4& viewProjMtx, GLuint shader);
    
};

#endif /* Cloth_hpp */
