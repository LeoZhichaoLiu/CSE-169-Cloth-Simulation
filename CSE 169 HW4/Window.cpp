////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 169 Starter";

// Objects to render
Cube * Window::cube;
Skeleton * Window::skeleton;
Skin * Window::skin;
Animation * Window::animation;
Player * Window::player;
Cloth * Window::cloth;
ground_plane* Window::plane;

Skeleton * Window::test;
Skeleton * Window::dragon;
bool Window::solid;

string Window::file;
string Window::file2;
string Window::file3;
bool Window::isSkel;
bool Window::isSkin;
bool Window::isAnim;

bool Window::setX = true;
bool Window::setY = false;
bool Window::setZ = false;

PointLight * Window::SphereLight;
SpotLight * Window::SphereLight2;

bool Window::only_point;
bool Window::only_spot;
int Window::iterate = 0;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::lightProgram;


////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    lightProgram = LoadShaders("shaders/shader2.vert", "shaders/shader2.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
    
    if (!lightProgram) {
        std::cerr << "Failed to initialize light program" << std::endl;
        return false;
    }

	return true;
}

bool Window::initializeObjects()
{
    
    cloth = new Cloth();
    
    SphereLight = new PointLight ("OBJ/sphere.obj");
    
    SphereLight2 = new SpotLight ("OBJ/sphere.obj");
    
    plane = new ground_plane();
    
    solid = true;
    only_point = true;
    only_spot = false;
    
	/*
    // Create a cube
	cube = new Cube();
	//cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    
    player = new Player();
    
    string prefix = "SKEL/";
    
    if (isSkel) {
      const char * input = (prefix + file).c_str();
      skeleton = new Skeleton (input);
    }
    
    if (isAnim) {
      const char * input_anim = (prefix + file3).c_str();
      animation = new Animation (input_anim);
      player->SetClip (animation);
      player->skeleton = skeleton;
    }
    
    if (isSkin && isSkel) {
      const char * input_skin = (prefix + file2).c_str();
      skin = new Skin (input_skin, skeleton);
        
    } else if (isSkin && !isSkel) {
      const char * input_skin = (prefix + file2).c_str();
      skin = new Skin (input_skin, skeleton);
    }
        
    SphereLight = new PointLight ("OBJ/sphere.obj");
    
    SphereLight2 = new SpotLight ("OBJ/sphere.obj");
    
    solid = true;
    only_point = true;
    only_spot = false;
    
    if (isSkel) {
        cout << "The Joint: " << skeleton->Joint_List[iterate]->name << "    The DOF Value: "
             << skeleton->Joint_List[iterate]->List1[0]->getValue() << " " << skeleton->Joint_List[iterate]->List1[1]->getValue()
             << " " << skeleton->Joint_List[iterate]->List1[2]->getValue() << endl;
    }
    */

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
    delete test;
    delete dragon;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
    glDeleteProgram(lightProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback()
{
    Cam->Update();
	// Perform any updates as necessary.
    
    cloth->Update(glfwGetTime());
    /*
    
    if (isSkel) {
      skeleton->Update();
    }
    
    if (isSkin) {
      skin->Update();
    }
    
    if (isAnim) {
        player->Update(glfwGetTime());
    }
    */

	//cube->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the object.
	glUseProgram(lightProgram);
    glUniform1i(glGetUniformLocation(lightProgram, "only_point"), only_point);
    glUniform1i(glGetUniformLocation(lightProgram, "only_spot"), only_spot);
    
    SphereLight->draw(Cam->GetViewProjectMtx(), lightProgram);
    SphereLight2->draw(Cam->GetViewProjectMtx(), lightProgram);
    
    cloth->Draw(Cam->GetViewProjectMtx(), lightProgram);
    
    plane->Draw(Cam->GetViewProjectMtx(), lightProgram);
    
    /*
    if (isSkel && !isSkin) {
       skeleton->Draw(Cam->GetViewProjectMtx(), lightProgram, solid);
    }
    
    if (isSkin) {
      skin->Draw(Cam->GetViewProjectMtx(), lightProgram);
    }
    */
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_R:
			resetCamera();
			break;
                
        case GLFW_KEY_1:
            only_point = true;
            only_spot = false;
            break;
        
        case GLFW_KEY_2:
            only_point = false;
            only_spot = true;
            break;
        
        case GLFW_KEY_3:
            only_point = true;
            only_spot = true;
            break;
        
        case GLFW_KEY_4:
            only_point = false;
            only_spot = false;
            break;
                
        case GLFW_KEY_Z:
            setX = true;
            setY = false;
            setZ = false;
            break;
                
        case GLFW_KEY_X:
            setX = false;
            setY = true;
            setZ = false;
            break;
                
        case GLFW_KEY_C:
            setX = false;
            setY = false;
            setZ = true;
            break;
        
        case GLFW_KEY_LEFT: {
            
            if (setX) {
                cloth->air_velocity.x -= 0.3f;
            } else if (setY) {
                cloth->air_velocity.y -= 0.3f;
            } else {
                cloth->air_velocity.z -= 0.3f;
            }
            cout << cloth->air_velocity.x << " " << cloth->air_velocity.y << " " << cloth->air_velocity.z << endl;
            break;
            
        }
                
        case GLFW_KEY_RIGHT: {
        
            if (setX) {
                cloth->air_velocity.x += 0.3f;
            } else if (setY) {
                cloth->air_velocity.y += 0.3f;
            } else {
                cloth->air_velocity.z += 0.3f;
            }
            cout << cloth->air_velocity.x << " " << cloth->air_velocity.y << " " << cloth->air_velocity.z << endl;
            break;
        }
                
        case GLFW_KEY_Q: {
            cloth->change_fixed (glm::vec3(0, 0.1, 0));
            break;
        }
                
        case GLFW_KEY_E: {
            cloth->change_fixed (glm::vec3(0, -0.1, 0));
            break;
        }
                
        case GLFW_KEY_W: {
            cloth->change_fixed (glm::vec3(0, 0, 0.1));
            break;
        }
                
        case GLFW_KEY_S: {
            cloth->change_fixed (glm::vec3(0, 0, -0.1));
            break;
        }
        
        case GLFW_KEY_A: {
            glm::vec3 center = (cloth->particle_list[(cloth->height-1)*cloth->width]->position +
                                cloth->particle_list[cloth->particle_list.size()-1]->position)/2.f;
            for (Particle * p : cloth->particle_list) {
                if (p->fixed) {
                     glm::mat4 model = glm::translate(glm::mat4(1.0f), p->position) * glm::mat4(1);
                     model =glm::translate(glm::mat4(1.0f), -center) * model;
                     model = glm::rotate(glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
                     model =glm::translate(glm::mat4(1.0f), center) * model;
                     p->position = model * glm::vec4(0.0, 0.0, 0.0, 1.0);
                    
                }
            }
            break;
        }
                
        case GLFW_KEY_D: {
            glm::vec3 center = (cloth->particle_list[(cloth->height-1)*cloth->width]->position +
                      cloth->particle_list[cloth->particle_list.size()-1]->position)/2.f;
            for (Particle * p : cloth->particle_list) {
                if (p->fixed) {
                     glm::mat4 model = glm::translate(glm::mat4(1.0f), p->position) * glm::mat4(1);
                     model =glm::translate(glm::mat4(1.0f), -center) * model;
                     model = glm::rotate(glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
                     model =glm::translate(glm::mat4(1.0f), center) * model;
                     p->position = model * glm::vec4(0.0, 0.0, 0.0, 1.0);
                }
            }
            break;
        }
                
		default:
			break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}


void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //skeleton -> Scale(pow (1.1, yoffset));
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
