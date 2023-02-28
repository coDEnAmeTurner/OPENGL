#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shader;

int main()
{
	//initialize GLFW
	if (glfwInit() != GL_TRUE) {
		printf("Initialization failed");
		glfwTerminate();
		return 1; //c++ standard return to return an error
	}

	//set up GLFW window props:
	//set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainwindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
	if (!mainwindow)
	{
		printf("GLFW window creation failed!!!");
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainwindow, &bufferWidth, &bufferHeight);

	//set context for GLEW:
	glfwMakeContextCurrent(mainwindow);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	//initialize glew
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW Init failed");
		glfwDestroyWindow(mainwindow);
		glfwTerminate();
		return 1;
	}

	//set up the viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	//loop until window closed
	while (!glfwWindowShouldClose(mainwindow))
	{
		glfwPollEvents();

		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainwindow);
	}

}