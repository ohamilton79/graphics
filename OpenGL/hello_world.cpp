#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "hello_world.h"
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

//Callback for each time a window is resized
//void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//Provide input control
void processInput(GLFWwindow* window)
{
	//Close window if escape key pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {

	/*//TESTING GLM
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 1.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;*/


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	//Register callback on every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	//Define size of rendering window
	glViewport(0, 0, 800, 600);

	//Setup vertex data 
	//-------------------------

	//Define rectangle vertices
	float vertices1[150] = {};

	//Create vertices of graph of y = x^2
	int count = 0;
	for (float x = 0.0f; x <= 2.0f; x += 0.04f) {
		float y{ std::pow(x, 3.0f) };
		float normalisedX = x;
		vertices1[count] = normalisedX ;
		count++;
		float normalisedY = y;
		vertices1[count] = normalisedY;

		std::cout << normalisedX << std::endl << normalisedY;
		
		count++;
		vertices1[count] = 0.0f;
		count++;
	}

	//Reset counter 
	count = 0;

	//Create indices for the vertices arrays, so that the graph can be drawn from individual lines
	unsigned int indices[300] = {};

	for (int i = 0; i < 145; i+=3) {
		indices[count] = i;
		std::cout << indices[count];
		indices[count + 1] = i + 1;
		std::cout << indices[count + 1];
		indices[count + 2] = i + 2;
		std::cout << indices[count + 2];
		count += 3;

		indices[count] = i + 3;
		std::cout << indices[count];
		indices[count + 1] = i + 4;
		std::cout << indices[count + 1];
		indices[count + 2] = i + 5;
		std::cout << indices[count + 2];
		count += 3;
	}

	//Create EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//Generate vertex buffer object and VAO
	unsigned int firstVBO, firstVAO;
	glGenVertexArrays(1, &firstVAO);
	glGenBuffers(1, &firstVBO);
	//Bind Vertex Array Object
	glBindVertexArray(firstVAO);

	//Bind to GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, firstVBO);

	//Copy vertex data into GPU memory to draw
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	//Bind the EBO and copy the indices into the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Tell OpenGL how to interpret vertex and colour data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind buffers for later use
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//Get shaders for drawing graph
	Shader firstShader("..\\shader_vs.txt", "..\\shader_fs_1.txt");

	//Render loop to keep window open and handling input
	while (!glfwWindowShouldClose(window)) 
	{
		//Process any input
		processInput(window);

		//Rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use the first shader program
		firstShader.use();
		glBindVertexArray(firstVAO);

		//Iterate over a range of angles to rotate the graph about
		for (float angle = 0.0f; angle < 360.0f; angle+=10.0f) {
			glm::mat4 trans = glm::mat4(1.0f);	//Initialised to have 1s on leading diagonal

			//Pass translation matrix to vertex shader
			int vertexTransformLocation = glGetUniformLocation(firstShader.ID, "transform");
			//glUniform1f(vertexAngleLocation, glm::radians(angle));

			//Transformation matrix to rotate graph about x axis
			trans = glm::rotate(trans, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

			//Pass translation matrix to vertex shader
			glUniformMatrix4fv(vertexTransformLocation, 1, GL_FALSE, glm::value_ptr(trans));

			//Draw lines of curve
			glDrawElements(GL_LINES, 95, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Goodbye!";
	glfwTerminate();
	return 0;
}