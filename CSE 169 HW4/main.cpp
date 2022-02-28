#include "main.h"


////////////////////////////////////////////////////////////////////////////////

void error_callback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(error_callback);
	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, Window::resizeCallback);
	
	// Set the key callback.
	glfwSetKeyCallback(window, Window::keyCallback);

	// Set the mouse and cursor callbacks
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) 
		<< std::endl;

	//If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " << 
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}



////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[] )
{
    
	// Create the GLFW window.
	GLFWwindow* window = Window::createWindow(800, 600);
    
    Window::isSkel = true;
    Window::isSkin = true;
    Window::isAnim = true;
    
    if (argc == 4) {
        Window::file = argv[1];
        Window::file2 = argv[2];
        Window::file3 = argv[3];
        
    } else if (argc == 2) {
        
        string temp = argv[1];
        size_t dot = temp.find(".");
        
        if (temp.substr(dot+1) == "skel") {
            Window::file = argv[1];
            Window::isSkin = false;
            Window::isAnim = false;
        }
        if (temp.substr(dot+1) == "skin") {
            Window::file2 = argv[1];
            Window::isSkel = false;
            Window::isAnim = false;
        }
        
    } else if (argc == 3) {
        string temp1 = argv[1];
        size_t dot1 = temp1.find(".");
        
        string temp2 = argv[2];
        size_t dot2 = temp2.find(".");
        
        if (temp1.substr(dot1+1) == "skel") {
            Window::file = argv[1];
            
            if (temp2.substr(dot2+1) == "skin") {
                Window::file2 = argv[2];
                Window::isAnim = false;
            }
        }
        
        if (temp1.substr(dot1+1) == "skel") {
            Window::file = argv[1];
            
            if (temp2.substr(dot2+1) == "anim") {
                Window::file3 = argv[2];
                Window::isSkin = false;
            }
        }
        
    } else {
       Window::file = "wasp.skel";
       Window::file2 = "wasp.skin";
       Window::file3 = "wasp_walk.anim";

    }
    
	if (!window) exit(EXIT_FAILURE);

	// Print OpenGL and GLSL versions.
	print_versions();
	// Setup callbacks.
	setup_callbacks(window);
	// Setup OpenGL settings.
	setup_opengl_settings();

	// Initialize the shader program; exit if initialization fails.
	if (!Window::initializeProgram()) exit(EXIT_FAILURE);
	// Initialize objects/pointers for rendering; exit if initialization fails.
	if (!Window::initializeObjects()) exit(EXIT_FAILURE);
	
	// Loop while GLFW window should stay open.
	while (!glfwWindowShouldClose(window))
	{
        //window->file = argv[1];
        
		// Main render display callback. Rendering of objects is done here.
		Window::displayCallback(window);

		// Idle callback. Updating objects, etc. can be done here.
		Window::idleCallback();
	}

	Window::cleanUp();
	// Destroy the window.
	glfwDestroyWindow(window);
	// Terminate GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
