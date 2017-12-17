#include "Window.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

#include "Light.h"

Light::Light() {
    
    light_mode = -1;
    
    light_color = glm::vec3();
    light_pos = glm::vec3();
    light_dir = glm::vec3();
    
    cons_att = 0.0f;
    linear_att = 0.0f;
    quad_att = 0.0f;
    
    cutoff_angle = 0.0f;
    exponent = 0.0f;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Light::~Light() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void Light::draw(GLuint shaderProgram) {
    
    glUniform1i(glGetUniformLocation(shaderProgram, "light.light_mode"), light_mode);
 
    // Pass Light info to Light struct in fragment shader
    glUniform3f(glGetUniformLocation(shaderProgram, "light.light_color"), light_color.x,
                light_color.y, light_color.z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "light.light_pos"), light_pos.x,
                light_pos.y, light_pos.z);
    
    //glUniform3f(glGetUniformLocation(shaderProgram, "light.light_dir"), light->light_dir.x,
    //light->light_dir.y, light->light_dir.z);
    
    glUniform1i(glGetUniformLocation(shaderProgram, "light.light_mode"), light_mode);
    glUniform1f(glGetUniformLocation(shaderProgram, "light.const_att"), 1.0f); //
    glUniform1f(glGetUniformLocation(shaderProgram, "light.linear_att"), 0.09f); //
    glUniform1f(glGetUniformLocation(shaderProgram, "light.quad_att"), 0.032f); //
    //glUniform1f(glGetUniformLocation(shaderProgram, "light.cutoff_angle"), cutoff_angle);
    glUniform1f(glGetUniformLocation(shaderProgram, "light.exponent"), exponent);

}






