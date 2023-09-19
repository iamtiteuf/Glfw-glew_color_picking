#ifndef GLEW_INITIALIZE
#define GLEW_INITIALIZE
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texUV;

};
GLFWwindow* MainWindow = nullptr;

class Glew_Initialization
{
public:

	
	float width = 1920;
	float height = 1080;
	int Init_Glfw()
	{
		if (!glfwInit())
		{
			return EXIT_FAILURE;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		MainWindow = glfwCreateWindow(width, height, "GLEW_COLOR_PICKING", NULL, NULL);
		
		if (!MainWindow)
		{
			glfwTerminate();
			return EXIT_FAILURE;
		}
		glfwMakeContextCurrent(MainWindow);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			glfwDestroyWindow(MainWindow);
			glfwTerminate();

			std::cout << "failed to initialize glew";
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

};
#endif
