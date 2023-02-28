#include <GL/glew.h>	
#include <GLFW/glfw3.h>
#include <iostream>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shaderProgram;

void  addShaderCode(GLuint shader, const char* code)
{
	const char* source[1];
	source[0] = code;

	GLint sourceLength[1];
	sourceLength[0] = strlen(source[0]);

	glShaderSource(shader, 1, source, sourceLength);
}

void createShaderProgram(const char* vertexCode,  const char* fragmentCode)
{
	shaderProgram = glCreateProgram();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	addShaderCode(vShader, vertexCode);
	addShaderCode(fShader, fragmentCode);

	glCompileShader(vShader);
	glCompileShader(fShader);

	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);

	glLinkProgram(shaderProgram);

}

int main()
{
	//Set up GLEW context
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW initialization failed\n";
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(glewExperimental, GL_TRUE);

	GLFWwindow* mainwindow = glfwCreateWindow(WIDTH, HEIGHT, "Practice1", NULL, NULL);
	if (!mainwindow)
	{
		std::cout << "Window creation failed\n";
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainwindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainwindow);

	//Set up GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew initialization failed\n";
		glfwDestroyWindow(mainwindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	//vertex shader code
	const char* vertexShaderCode = "\n\
		#version 330\n\
		layout (location = 0) in vec2 pos;\n\
		void main()\n\
		{\n\
			gl_Position = vec4(.4f* pos.x, 0.4f * pos.y, 1.0f, 1.0f);\n\
		}\n\
		";

	//fragment shader code
	const char* fragmentShaderCode = "\n\
		#version 330\n\
		layout (location = 0) out vec4 color;\n\
		void main()\n\
		{\n\
			color = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n\
		}\n\
		";

	//Make VAO
	glGenVertexArrays(1, &VAO);

	//Bind VAO
	glBindVertexArray(VAO);
		//Make  VBO
		glGenBuffers(1, &VBO);
		//Buffer data
		GLfloat data[] = {
			-1.0f, -1.0f,
			1.0f, -1.0f,
			0.0f, 1.0f
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(GL_FLOAT) * 2, 0);
			glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//DRAW
		createShaderProgram(vertexShaderCode, fragmentShaderCode);
		glUseProgram(shaderProgram);
			while (!glfwWindowShouldClose(mainwindow))
			{
				glfwPollEvents();

				glDrawArrays(GL_TRIANGLES, 0, 3);

				glfwSwapBuffers(mainwindow);
			}

		glUseProgram(0);

	glBindVertexArray(0);

}