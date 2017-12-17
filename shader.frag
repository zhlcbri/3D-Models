#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 fragNormal;
in vec3 objNormal;
in vec3 fragPos; // fragVert

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

uniform vec3 ks; // specular
uniform vec3 kd; // diffuse
uniform vec3 ka; // ambient

struct Light {
    //int light_mode; // if -1, use normal coloring
    // 1 for directional, 2 for point, 3 for spot

    vec3 light_color; // cl
    vec3 light_pos;
    vec3 light_dir; // L

    float cons_att;
    float linear_att;
    float quad_att;

    float cutoff_angle;
    float exponent;
};
uniform Light light;

struct Material {
    vec3 surface_normal; // no need
    vec3 diffuse; // kd*(n*L)
    vec3 specular; // ks*(R*e)^p
    vec3 ambient; //
    float p; // shininess; Phong exponent; greater p sharper highlight
};
uniform Material material;

uniform int coloring_mode; // if 1, use normal coloring
uniform int light_mode; // 1 for directional, 2 for point, 3 for spot

void main()
{
    if (coloring_mode == 1) {
        vec3 norm = (normalize(objNormal)+1.0f)/2.0f; // only for normal coloring
        fragColor = vec4(norm, 1.0f); // normal coloring
    } else {
        
        //vec3 objectColor = vec3(1.0f, 0.41f, 0.7f); // cl; hot pink
        vec3 viewPos = vec3(0.0f, 0.0f, 20.0f); // e, default camera position
       
        vec3 lightPos = vec3(1.2f, 1.0f, 2.0f); // make it user-configurable parameter
        
        // Specular
        vec3 viewDir = normalize(viewPos - fragPos); // e;
        vec3 reflectDir = reflect((1) * light.light_dir, normalize(fragNormal)); // R = -L;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // (e * R)^p
        vec3 specular = ks * spec * light.light_color; // ks * cl * (e * R) ^ p
        
        
        // Ambient
        vec3 ambient = ka * light.light_color; // ka * ca
        
        
        // Diffuse
        // the light's direction vector is the difference vector between light's position vector and fragment's position vector
        //vec3 lightDir = normalize(light.light_dir - fragPos); // L
        float diff = max(dot(normalize(fragNormal), (-1)*light.light_dir), 0.0f);
        vec3 diffuse = diff * light.light_color * kd; // cl * (n*L)
        vec3 result = vec3(1.0f);
        
        result = ambient + diffuse + specular;
        
        if (light_mode == 1) {
            result = ambient + diffuse + specular;
        }
        else if (light_mode == 2) {
            float dist = length(light.light_pos - fragPos);
            float attenuation = 1.0 / (light.cons_att + light.linear_att * dist + light.quad_att * (dist * dist));
            //fragColor = vec4(attenuation*(ambient+diffuse+specular), 1.0);
            result = attenuation * (ambient+diffuse+specular);
        }
        else if (light_mode == 3) {
            
            float theta = dot(normalize(light.light_pos - fragPos), normalize(light.light_dir)); // - light dir?
            if (theta > light.cutoff_angle) {
                float dist = length(light.light_pos - fragPos);
                float attenuation = 1.0 / (light.cons_att + light.linear_att * dist + light.quad_att * (dist * dist));
                result = attenuation * (ambient+diffuse+specular);
            } else {
                result = ambient * diffuse; // TexCoords?
            }
        }
        else {
            result = ambient + diffuse + specular;
        }
        
        fragColor = vec4(result, 1.0);
    }

}








