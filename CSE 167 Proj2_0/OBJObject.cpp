#include "OBJObject.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
//#include "Light.h" //
using namespace std;
float xVal, yVal, zVal, degVal = 0.0f;
float sVal = 1.0f;
//// min and max values of object's vertices
float OBJObject::minX = numeric_limits<float>::max();
float OBJObject::minY = numeric_limits<float>::max();
float OBJObject::minZ = numeric_limits<float>::max();

float OBJObject::maxX = numeric_limits<float>::min();
float OBJObject::maxY = numeric_limits<float>::min();
float OBJObject::maxZ = numeric_limits<float>::min();

int OBJObject::obj_mode = -1;
//int OBJObject::light_mode = -1;

glm::vec3 color_spec0 = glm::vec3(0.633f, 0.727811f, 0.633f); // bunny is all shiny no diffuse color
glm::vec3 color_ambi0 = glm::vec3(0.0215f, 0.1745f, 0.0215f); // emerald

glm::vec3 color_diff1 = glm::vec3(0.61424f, 0.04136f, 0.04136f); // bear is all diffuse no shininess
glm::vec3 color_ambi1 = glm::vec3(0.1745f, 0.01175f, 0.01175f); // ruby

glm::vec3 color_ambi2 = glm::vec3(0.24725f, 0.1995f, 0.0745f); // dragon should have significant diffuse and specular reflection components
glm::vec3 color_diff2 = glm::vec3(0.75164f, 0.60648f, 0.22648f);
glm::vec3 color_spec2 = glm::vec3(0.628281f, 0.555802f, 0.366065f); // gold
/*
struct Material {
    glm::vec3 color_spec;
    glm::vec3 color_diff;
    glm::vec3 color_ambi;
    
} shiny, diffuse, custom;
*/

//Light * light;

OBJObject::OBJObject(const char *filepath)
{
    //light = new Light();
    
	toWorld = glm::mat4(1.0f);
    minX = numeric_limits<float>::max();
    minY = numeric_limits<float>::max();
    minZ = numeric_limits<float>::max();
    //
    maxX = numeric_limits<float>::min();
    maxY = numeric_limits<float>::min();
    maxZ = numeric_limits<float>::min();
    
    parse(filepath);
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //
    glGenBuffers(1, &VBO2); // for normal coloring
    
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW); // 3rd param changed
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1); // Enable the usage of layout location 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // when layout=1

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW); // 3rd param changed
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


OBJObject::~OBJObject()
{
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);
}

//void OBJObject::setLight(Light * newLight) {
//    if (newLight == nullptr) {
//        newLight = new Light();
//    }
//    light = newLight;
//}

void OBJObject::parse(const char *filepath)
{
    ifstream ifs;
    float x,y,z; // vertex coordinates
    float r = 0.0;
    float g = 0.0;
    float b = 0.0; // vertex color
    char c1, c2;
    char buf[255]; // buffer for reading line for filepath
    string strbuf;
    
    unsigned int x1, y1, z1, x2, y2, z2;
    
    ifs.open(filepath);
    if (!ifs.is_open()) {
        cerr << "error loading the file" << endl;
        exit(-1);
    } // in case the file can't be found or is corrupt
    
    while (!ifs.eof()) {
        ifs.get(c1);
        ifs.get(c2);
        getline(ifs, strbuf);
        strbuf.copy(buf, strbuf.length());
        
        if ((c1 == 'v') && (c2 == ' ')) {
            sscanf(buf, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            if (x < minX) minX = x;
            if (y < minY) minY = y;
            if (z < minZ) minZ = z;
            
            if (x > maxX) maxX = x;
            if (y > maxY) maxY = y;
            if (z > maxZ) maxZ = z;
            
            vertices.push_back(glm::vec3(x, y, z)); // do not store the rgb values here
            
        } else if ((c1 == 'v') && (c2 == 'n')) {
            //cout << "vn line read" << endl;
            sscanf(buf, " %f %f %f", &x, &y, &z);
            
            // map the vertex normals in the range of 0 to 1
            // and store as rgb value
//            glm::vec3 normalized = glm::normalize(glm::vec3(x, y, z)); // normalize the vertex normals
//            normals.push_back(glm::vec3((normalized.x+1.0f)/2.0f, (normalized.y+1.0f)/2.0f, (normalized.z+1.0f)/2.0f));
            normals.push_back(glm::vec3((x+1.0f)/2.0f, (y+1.0f)/2.0f, (z+1.0f)/2.0f));
            
        } else if ((c1 == 'f') && (c2 == ' ')) {
            // 3 sets of indices to vertices and normals
            // define the 3 corners of a triangle
            
            sscanf(buf, "%u//%u %u//%u %u//%u", &x1, &x2, &y1, &y2, &z1, &z2);
            // make sure that the number you parsed is 3 * num_faces
            // offset all indices by -1
            indices.push_back(x1 - 1);
            indices.push_back(y1 - 1);
            indices.push_back(z1 - 1);
        }
    }
    ifs.close(); // close the file when done parsing
}

void OBJObject::update()
{
    //spin(1.0f);
}

void OBJObject::draw(GLuint shaderProgram)
{
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * toWorld;

    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    //toWorld
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
    // viewPos
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, GL_FALSE, &Window::V[0][0]);
    
//    if (obj_mode == -1) {
//        //
//        cout << "using normal coloring" << endl;
//        return;
//    }
    
    // Send Material properties to shader program
    if (obj_mode == 0) { // bunny
        glUniform3f(glGetUniformLocation(shaderProgram, "ks"), color_spec0.x, color_spec0.y, color_spec0.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "kd"), 0.0f, 0.0f, 0.0f);
//        glUniform3f(glGetUniformLocation(shaderProgram, "ka"), color_ambi0.x, color_ambi0.y, color_ambi0.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "ka"), 0.0f, 0.0f, 0.0f);
    } else if (obj_mode == 1) { // bear
        glUniform3f(glGetUniformLocation(shaderProgram, "ks"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "kd"), color_diff1.x, color_diff1.y, color_diff1.z);
//        glUniform3f(glGetUniformLocation(shaderProgram, "ka"), color_ambi1.x, color_ambi1.y, color_ambi1.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "ka"), 0.0f, 0.0f, 0.0f);
    } else if (obj_mode == 2) {
        glUniform3f(glGetUniformLocation(shaderProgram, "ks"), color_spec2.x, color_spec2.y, color_spec2.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "kd"), color_diff2.x, color_diff2.y, color_diff2.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "ka"), color_ambi2.x, color_ambi2.y, color_ambi2.z);
    }
    
    // Pass Light info to Light struct in fragment shader
 /*   glUniform3f(glGetUniformLocation(shaderProgram, "light.light_color"), light->light_color.x,
                light->light_color.y, light->light_color.z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "light.light_pos"), light->light_pos.x,
                light->light_pos.y, light->light_pos.z);
    
    //glUniform3f(glGetUniformLocation(shaderProgram, "light.light_dir"), light->light_dir.x,
                //light->light_dir.y, light->light_dir.z);
    
    glUniform1i(glGetUniformLocation(shaderProgram, "light.light_mode"), light->light_mode);
    glUniform1f(glGetUniformLocation(shaderProgram, "light.const_att"), 1.0f); //
    glUniform1f(glGetUniformLocation(shaderProgram, "light.linear_att"), 0.09f); //
    glUniform1f(glGetUniformLocation(shaderProgram, "light.quad_att"), 0.032f); //
    glUniform1f(glGetUniformLocation(shaderProgram, "light.cutoff_angle"), light->cutoff_angle);
    glUniform1f(glGetUniformLocation(shaderProgram, "light.exponent"), light->exponent);*/
    
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

// Whenever you click on F1, F2 and F3 all models need to be centered and scaled

void OBJObject::toCenter() {
    this->toWorld[3][0] = 0.0f;
    this->toWorld[3][1] = 0.0f;
    this->toWorld[3][2] = 0.0f;
    
    /*midX = (maxX + minX)/2.0f;
    midY = (maxY + minY)/2.0f;
    midZ = (maxZ + minZ)/2.0f;*/
    
   // translateMat = glm::translate(glm::mat4(1.0f), glm::vec3((-1.0f)*midX, (-1.0f)*midY, (-1.0f)*midZ)); // correct
    
    //toWorld = toWorld * translateMat;
}

void OBJObject::scale() {
    float diffX = maxX - minX;
    float diffY = maxY - minY;
    float diffZ = maxZ - minZ;
    
    float maxRange = fmax(diffX, fmax(diffY, diffZ));
    float factor = 1.0f/(maxRange/2.0f);
    
    scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(factor));
    // toWorld = toWorld * scaleMat;
}

void OBJObject::reset() { //
    toCenter();
    scale();
   // set the toWorld= T*S*glm::mat4(1.0f)
    //toWorld = translateMat * scaleMat * glm::mat4(1.0f);
    toWorld = scaleMat * glm::mat4(1.0f);
}

void OBJObject::orbit(float angle, glm::vec3 axis) {
    this->toWorld = glm::rotate(glm::mat4(1.0f), angle*70/180 * glm::pi<float>(), axis) * this->toWorld; // can change value to less or greater than 50
    // this->toWorld = this->toWorld * glm::rotate(glm::mat4(1.0f), angle*80/180 * glm::pi<float>(), axis);
}

void OBJObject::translateXY(float xVal, float yVal) {
    translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(xVal*(0.008f), yVal*(0.008f), 0.0f)); //
    toWorld = translateMat * toWorld;
}

void OBJObject::translateZ(float zVal) {
    this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zVal)) * this->toWorld;
}

void OBJObject::scale(float val) {
    this->toWorld = this->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(val));
}








