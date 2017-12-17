#include "Window.h"
#include "OBJObject.h"
#include <math.h>
#include "Light.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;
GLint shaderProgram;

OBJObject * obj;
OBJObject * bunny;
OBJObject * bear;
OBJObject * dragon;
//
OBJObject * cone;
OBJObject * sphere;

int Window::obj_mode = 0;
int Window::light_mode = 0;
//Light * normalColoring; //
//Light * light;
//Light * directional_light;
//Light * point_light;
//Light * spot_light;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

bool Window::ROTATE;
bool Window::TRANSLATE_XY;
bool Window::PHONG; //
bool Window::LIGHT_ON; //

double Window::xpos;
double Window::ypos;

double Window::OLD_XPOS;
double Window::OLD_YPOS;

double Window::scrollY = 0.0;

glm::vec3 Window::A;
glm::vec3 Window::B;
float Window::angle;
glm::vec3 Window::axis;

glm::mat4 newWorld;
float angle = 25.0f;

using namespace std;

void Window::initialize_objects()
{
//    directional_light = new Light();
//    point_light = new Light();
//    spot_light = new Light();
//
//    directional_light->light_mode = 1;
//    directional_light->light_color = glm::vec3(0.0f, 0.0f, 0.502f); // navy
//
//    point_light->light_mode = 2;
//    point_light->light_color = glm::vec3(0.294f, 0.0f, 0.510f); // indigo
//    point_light->light_pos = glm::vec3(0.0f, 0.0f, 10.0f);//
//    point_light->linear_att = 0.09f; //
//
//    spot_light->light_mode = 3;
//    spot_light->light_color = glm::vec3(0.137255f, 0.556863f, 0.419608f); // sea green
//    spot_light->light_pos = glm::vec3(0.0f, 0.0f, 20.0f);//
//    spot_light->cutoff_angle = glm::cos(glm::radians(40.0f));
//    spot_light->exponent = 64.0f;
//    spot_light->quad_att = 0.1f; //
//
//    light = directional_light;
    
    obj = new OBJObject("bunny.obj");
    bunny = new OBJObject("bunny.obj");
    bear = new OBJObject("bear.obj");
    dragon = new OBJObject("dragon.obj");
    cone = new OBJObject("cone.obj");
    sphere = new OBJObject("sphere.obj");
    
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
    
    //delete(obj);
    delete(bunny);
    delete(bear);
    delete(dragon);
    //
    delete(cone);
    delete(sphere);
    
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
    obj->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
	//cube->draw(shaderProgram);
    obj->draw(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        switch (key) {
                // each time press F1 F2 F3
                // just set the toWorld= T*S*glm::mat4(1.0f)
            case GLFW_KEY_F1:
                obj = bunny;
                Window::obj_mode = 0;
                obj->draw(shaderProgram);
                obj->reset();
                break;
                
            case GLFW_KEY_F2:
                obj = bear;
                Window::obj_mode = 1;
                obj->draw(shaderProgram);
                obj->reset();
                break;
            case GLFW_KEY_F3:
                obj = dragon;
                Window::obj_mode = 2;
                obj->draw(shaderProgram);
                obj->reset();
                break;
                
            case GLFW_KEY_W:
                // scale down/up (about the model's center, not the center of the screen)
                if (mods == GLFW_MOD_SHIFT) {
                    if (angle <= 40.0f) {
                        angle += 2.0f;
                    }
                    glUniform1f(glGetUniformLocation(shaderProgram, "light.cutoff_angle"), glm::cos(glm::radians(angle)));
                } else {
                    if (angle >= 14.0f) {
                        angle -= 2.0f;
                    }
                    glUniform1f(glGetUniformLocation(shaderProgram, "light.cutoff_angle"), glm::cos(glm::radians(angle)));
                }
                break;
                
            case GLFW_KEY_N:
                if (PHONG) {
                    PHONG = false;
                    glUniform1i(glGetUniformLocation(shaderProgram, "coloring_mode"), 1);
                } else {
                    PHONG = true;
                    glUniform1i(glGetUniformLocation(shaderProgram, "coloring_mode"), 0);
                }
                break;
                
            case GLFW_KEY_S:
                if (mods == GLFW_MOD_SHIFT) {
                    obj->scale(1.5f); // was 1.5f
                } else {
                    obj->scale(0.5f); // make sure it doesn't go out of the window
                }
                break;
                
            case GLFW_KEY_0:
                LIGHT_ON = false;
                glUniform1i(glGetUniformLocation(shaderProgram, "light_mode"), 0);
                break;
                
            case GLFW_KEY_1: // directional light
                LIGHT_ON = true;
                Window::light_mode = 1;
                glUniform1i(glGetUniformLocation(shaderProgram, "light_mode"), 1);

                break;
                
            case GLFW_KEY_2: // point light
                LIGHT_ON = true;
                Window::light_mode = 2;
                glUniform1i(glGetUniformLocation(shaderProgram, "light_mode"), 2);
                glUniform1f(glGetUniformLocation(shaderProgram, "light.cons_att"), 1.0f);
                glUniform1f(glGetUniformLocation(shaderProgram, "light.linear_att"), 0.09f);
                glUniform1f(glGetUniformLocation(shaderProgram, "light.quad_att"), 0.032f);

                break;
                
            case GLFW_KEY_3:
                LIGHT_ON = true;
                Window::light_mode = 3;
                glUniform1i(glGetUniformLocation(shaderProgram, "light_mode"), 3);

                glUniform1f(glGetUniformLocation(shaderProgram, "light.cons_att"), 1.0f);
                glUniform1f(glGetUniformLocation(shaderProgram, "light.linear_att"), 0.09f);
                glUniform1f(glGetUniformLocation(shaderProgram, "light.quad_att"), 0.032f);
//
////                glUniform3f(glGetUniformLocation(shaderProgram, "light.light_pos"), cam_pos.x, cam_pos.y, cam_pos.z);
////                glUniform3f(glGetUniformLocation(shaderProgram, "light.light_dir"), cam_look_at.x, cam_look_at.y, cam_look_at.z);
                glUniform1f(glGetUniformLocation(shaderProgram, "light.cutoff_angle"), glm::cos(glm::radians(angle)));
                
                break;
                
            default:
                break;
        }
	}
}

// Map 2D cursor position to 3D
glm::vec3 Window::trackBallMapping(GLFWwindow* window, double xpos, double ypos) {
    glm::vec3 v;
    float d;
    int w = Window::width;
    int h = Window::height;
    
    v.x = (2.0 * xpos - w)/w;
    v.y = (h - 2.0 * ypos)/h;
    v.z = 0.0;
    //d = v.length();
    d = glm::length(v);
    d = (d < 1) ? d : 1.0; // if d is smaller than 1, set d to 1
    v.z = sqrtf(1.0001 - d*d); // or pow(d, 2.0)
    v = glm::normalize(v);
    return v;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    
    if (action == GLFW_PRESS) {
        
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT: 
               /* if (!LIGHT_ON) */ROTATE = true;
                break;
                
            case GLFW_MOUSE_BUTTON_RIGHT:
                /*if (!LIGHT_ON)*/ TRANSLATE_XY = true;
                break;
                
            default:
                break;
        }
    }
    if (action == GLFW_RELEASE) {
        ROTATE = false;
        TRANSLATE_XY = false;
    }
    scrollY = 0.0;
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    A = trackBallMapping(window, OLD_XPOS, OLD_YPOS);
    //glfwGetCursorPos(window, &xpos, &ypos);
    B = trackBallMapping(window, xpos, ypos);

    if (A == B) {
        return;
    }
    
    float velocity = length(B-A);
    if (velocity < 0.0001) return;
    
//    if (glm::dot(A, B) > 1) {
//        angle = acos(1);
//    } else {
//        angle = velocity /** acos(glm::dot(A, B))*/; // if > 1, let equal to 1
//    }
    
    angle = velocity;
    axis = glm::cross(A, B);
//    R = glm
    if (ROTATE) obj->orbit(angle, axis);
    
    if (TRANSLATE_XY) {

        obj->translateXY((xpos - OLD_XPOS), (-1)*(ypos - OLD_YPOS)); //
    }
    
    OLD_XPOS = xpos;
    OLD_YPOS = ypos;
}

// implement scroll_callback
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scrollY += yoffset;
    float zoomSensitivity = -0.018f; // change
    obj->translateZ(zoomSensitivity * (float)scrollY);
    scrollY = 0.0;
}



