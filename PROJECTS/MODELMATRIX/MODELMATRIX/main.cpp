#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint compileShader(const char* source, GLenum type)
{
	const char* code[] = { source };
	int codeLength[] = { strlen(code[0]) };
	int shader = glCreateShader(type);
	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);
	GLint result, length;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		printf("vShader compilation failed.\n");
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* buffer = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, NULL, buffer);
		printf(buffer + '\n');
		glDeleteShader(shader);
		return -1;
	}
	return shader;
}

GLuint makeShader(const char* vertexSource, const char* fragmentSource) {
	GLuint program = glCreateProgram();
	GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
	if (vertexShader != -1 && fragmentShader != -1)
	{
		glAttachShader(program, vertexShader);
		glDeleteShader(vertexShader);
		glAttachShader(program, fragmentShader);
		glDeleteShader(fragmentShader);
		glLinkProgram(program);
		GLint result, length;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			printf("Program linking failed.\n");
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* buffer = (char*)_malloca(length * sizeof(char));
			glGetProgramInfoLog(program, length, NULL, buffer);
			printf(buffer, '\n');
			glDeleteProgram(program);
			return -1;
		}
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
		if (result != GL_TRUE)
		{
			printf("Program validation failed.\n");
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* buffer = (char*)_malloca(length * sizeof(char));
			glGetProgramInfoLog(program, length, NULL, buffer);
			printf(buffer, '\n');
			glDeleteProgram(program);
			return -1;
		}
		return program;
	}
}

int main()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW init failed\n";
	}

	//set opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//set opengl profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//create window
	GLFWwindow* mainwindow = glfwCreateWindow(WIDTH, HEIGHT, "Uniform Variable", NULL, NULL);
	if (mainwindow == NULL)
	{
		std::cout << "Window creation failed!\n";
		glfwTerminate();
	}

	//get buffer size of window
	int bufferwidth, bufferheight;
	glfwGetFramebufferSize(mainwindow, &bufferwidth, &bufferheight);

	//make window current window
	glfwMakeContextCurrent(mainwindow);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed\n";
		glfwDestroyWindow(mainwindow);
		glfwTerminate();
	}

	//set field to draw
	glViewport(0, 0, bufferwidth, bufferheight);

	//set up VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//set up VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLfloat data[] = { -1.0f, -1.0f,
					1.0f, -1.0f,
					0.0f, 1.0f };
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	const char* vertexSource =
		"\n\
		#version 330\n\
		layout (location = 0) in vec2 pos;\n\
		uniform mat4 model;\n\
		out vec4 vColor;\n\
		void main()\n\
		{\n\
			gl_Position = model * vec4(pos.x, pos.y, 1.0f, 1.0f);\n\
			vColor = vec4(clamp(pos, 0.0f, 1.0f), 0.0f, 1.0f);\n\
		}\n\
		";
	const char* fragmentSource =
		"\n\
		#version 330\n\
		layout (location = 0) out vec4 color;\n\
		in vec4 vColor;\n\
		void main()\n\
		{\n\
			color = vColor;\n\
		}\n\
		";

	GLuint shader = makeShader(vertexSource, fragmentSource);
	if (shader == -1)
		return -1;

	glUseProgram(shader);
	GLint uniformModel = glGetUniformLocation(shader, "model");
	glUseProgram(0);
	bool direction = true;
	GLfloat offset = 0.0f;
	GLfloat increment = 0.00058f;
	GLfloat currIncrement = 0.000003f;
	GLfloat maxOffSet = 0.7f;
	GLfloat currAngle = 0.0f;

	glBindVertexArray(vao);
	glUseProgram(shader);
	while (!glfwWindowShouldClose(mainwindow))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::rotate(model, currAngle * 180 / glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(offset, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(mainwindow);

		if (abs(offset) >= maxOffSet)
			direction = !direction;

		if (direction)
			offset += increment;
		else
			offset -= increment;

		currAngle += currIncrement;
		if (currAngle >= 360)
			currAngle -= 360;

	}
	glUseProgram(0);
	glBindVertexArray(0);

	glDeleteProgram(shader);
}