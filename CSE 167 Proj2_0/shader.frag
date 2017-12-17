#version 330 core
// This is a sample fragment shader.
//#include "Light.h"

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normalColor; // fragNormal
in vec3 fragPos; // fragVert
//in vec4 modelview;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
//out vec4 color; // only use one?
//uniform vec3 objectColor;
//uniform vec3 lightColor;

vec3 viewPos; // the world space coordinates of the viewer

vec3 ks; // shiny
vec3 kd; // diffuse
vec3 ka; // ambient

vec3 norm; // normalized normal color
int p; // phong exponent p

vec3 cd;
vec3 cs;
vec3 ca;
vec3 result;

// vector<glm::vec3> normalColor;
mat4 model;

struct Light {
    int light_mode; // if -1, use normal coloring
    // 1 for directional, 2 for point, 3 for spot

    vec3 light_color;
    vec3 light_pos;
    vec3 light_dir;

    float cons_att;
    float linear_att;
    float quad_att;

    float cutoff_angle;
    float exponent;
};
uniform Light light;

void main()
{
    norm = normalize(normalColor);
    //
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.light_color * ka;
    
    vec3 lightDir = normalize(light.light_pos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.light_color * kd;
    
    vec3 result = ambient * ka + diffuse * kd;
    //
    /*
    cd = light.light_color * kd * norm * light.light_dir;
    
    vec3 normalizedDir = normalize(light.light_pos - fragPos); //surfaceToLight
    vec3 viewDir = normalize(viewPos - fragPos);

    cs = light.light_color * ks * pow(max(dot(normalizedDir, viewDir), 0.0), light.exponent);
    
    ca = light.light_color * ka;
    
    float distance = length(light.light_pos - fragPos);
    float attenuation = 1.0 / (light.cons_att + light.linear_att * distance
                               + light.quad_att * (distance * distance));*/
    
    //fragColor = vec4(cd + cs + ca, 1.0f);
    
    fragColor = vec4(result, 1.0f);
    
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    //color = vec4(1.0f, 0.41f, 0.7f, sampleExtraOutput);
    
//    norm = normalize(normalColor);
//
//    vec3 normalizedDir = normalize(light.light_pos - fragPos); //surfaceToLight
//    vec3 viewDir = normalize(viewPos - fragPos);
//
//    float spec = pow(max(dot(normalizedDir, viewDir), 0.0), 32);
//
//    cd = light.light_color * kd * norm; // debug color
//
//    cs = light.light_color * ks * spec;
//
//    ca = light.light_color * ka;
//
//    float distance = length(light.light_pos - fragPos);
//    float attenuation = 1.0/ (light.cons_att + light.linear_att * distance +
//                              light.quad_att * (distance * distance));
//    cd *= attenuation;
//    ca *= attenuation;
//    cs *= attenuation;
//
//    result = cd + cs + ca;
//
//    if (light.light_mode == -1) {
//        color = vec4(norm, 1.0f); // or normalColor
//    }
//    else {
//        result = cd + cs + ca;
//        fragColor = vec4(result.x, result.y, result.z, 1.0f);
//    }

    
    
    
    
    

    //fragColor = vec4(result.x, result.y, result.z, 1.0f);
    
    
    
//    vec3 result = ks * Light.light_color;
//    fragColor = vec4(result, 1.0f);
  
    //vec3 norm = normalize(normalColor);
    //Light.light_dir = normalize(Light.light_pos - fragPos);
    
//    vec3 result = (ka + kd) * Light.light_color;
//    color = vec4(result, 1.0);
    
//    float diff = max(dot(norm, Light.light_dir), 0.0);
//    vec3 diffuse = diff * Light.light_color;
    
    // Should not define light or material here
    // Read light or material info from input
    // model = mat4(normalColor, 1.0f);
    
}








