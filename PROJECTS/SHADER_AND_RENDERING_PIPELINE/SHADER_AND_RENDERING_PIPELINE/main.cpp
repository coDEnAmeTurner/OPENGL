#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <malloc.h>
#include <iostream>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shaderProgram;

void addShaderSource(GLuint shader, const char* source)
{
	const GLchar* code[1];
	code[0] = source;

	GLint codeLength[1];
	codeLength[0] = strlen(code[0]);

	glShaderSource(shader, 1, code, codeLength);
}
void makeShaderProgram(const char* vSource, const char* fSource)
{
	shaderProgram = glCreateProgram();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result;
	GLint length;

	addShaderSource(vShader, vSource);
	addShaderSource(fShader, fSource);

	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		printf("vShader compilation failed.\n");
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &length);
		char* buffer = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(vShader, length, NULL, buffer);
		printf(buffer + '\n');
		glDeleteShader(vShader);
		return;
	}
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		printf("fShader compilation failed.\n");
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &length);
		char* buffer = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(fShader, length, NULL, buffer);
		printf(buffer + '\n');
		glDeleteShader(fShader);
		return;
	}

	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		printf("Program linking failed.\n");
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
		char* buffer = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(shaderProgram, length, NULL, buffer);
		printf(buffer, '\n');
		glDeleteProgram(shaderProgram);
		return;
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
	if (result != GL_TRUE)
	{
		printf("Program validation failed.\n");
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
		char* buffer = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(shaderProgram, length, NULL, buffer);
		printf(buffer, '\n');
		glDeleteProgram(shaderProgram);
		return;
	}
}

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

	//working with VAO
	//make a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
		//working with VBO
		//make a VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			GLfloat data[] = { -1.0f, -1.0f, 0.0f,
								1.0f, -1.0f, 0.0f,
								0.0f, 1.0f, 0.0f }; //the data only contans 1 attribute: position
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
			glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		const char* vertexShaderSource = "\n\
		#version 330 \n\
		layout (location = 0) in vec3 pos; \n\
		\n\
		void main() \n\
		{\n\
			gl_Position = vec4(0.4f * pos.x, 0.4f * pos.y, pos.z, 1.0);\n\
		}\n\
		";

		const char* fragmentShaderSource = "\n\
		#version 330\n\
		layout (location = 0) out vec4 color;\n\
		\n\
		void main()\n\
		{\n\
			color = vec4(1.0, 0.0, 0.0, 1);\n\
		}\n\
		";

		makeShaderProgram(vertexShaderSource, fragmentShaderSource);
		glUseProgram(shaderProgram);

		//loop until window closed
		while (!glfwWindowShouldClose(mainwindow))
		{
			glfwPollEvents();

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(mainwindow);
		}

		glUseProgram(0);
		glDeleteProgram(shaderProgram);

	glBindVertexArray(0);

	std::cin.get();
}