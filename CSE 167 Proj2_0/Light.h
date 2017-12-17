#ifndef Light_h
#define Light_h

#define GLFW_INCLUDE_GLEXT //
/*#ifdef __APPLE__
 #include <OpenGL/gl3.h>
 #include <OpenGL/glext.h>
 #include <OpenGL/gl.h> // Remove this line in future projects
 #else
 #include <GL/glew.h>
 #endif*/
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OBJObject.h"

using namespace std;

class Light {
//    int mode; // 0 for directional; 1 for point; 2 for spotlight
    //OBJObject* obj;

    // void sendToShader(); // send update to shader
public:
    Light();
    ~Light();
    
    OBJObject* obj;
    
    // These variables are needed for the shader program
    GLuint VAO, VBO;
    
    int light_mode;
    
    glm::vec3 light_color;
    glm::vec3 light_pos;
    glm::vec3 light_dir;
    
    float cons_att;
    float linear_att;
    float quad_att;
    
    float cutoff_angle;
    float exponent;
    
    // These variables are needed for the shader program
//    GLuint VAO, VBO, VBO2, EBO;
    void draw(GLuint shaderProgram);
    //void setObj(const char *filepath);
};

#endif /* Light_h */
