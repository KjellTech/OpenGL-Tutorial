#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLShader.h"

int main() {
	// Initialize GLFW library:
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	
	// Create Window:
	GLFWwindow *window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	// Subsequent OpenGL calls applies to the context of window: 
	glfwMakeContextCurrent(window);
	
	// Initialize GLEW library:
	glewInit();
	
	GLuint shader_programme = LoadShader("vs.shd", "fs.shd");
	glUseProgram(shader_programme);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the display

		// xyz of the three vertices in the triangle:
		GLfloat vertices[] = { 0.0f, 0.5f, 0.0f, 
			                   0.5f, -0.5f, 0.0f, 
			                  -0.5f, -0.5f, 0.0f };

		// The colors of the three vertices:
		GLfloat colors[] = {
			1.0f, 0.0f, 0.0f, // red
			0.0f, 1.0f, 0.0f, // green
			0.0f, 0.0f, 1.0f  // blue
		};

		
		// Enable the first attribute in the vertex shader:
		glEnableVertexAttribArray(0);

		// Specify the location and data format of an array attribute to be used by the vertex shader:
		glVertexAttribPointer(0,          // First attribute in the vertex shader
			3,                            // number of components per vertex, here: xyz
			GL_FLOAT,                     // format   
			GL_FALSE,                     // normalized?
			0,                            // stride
			vertices); // array of vertex properties

		// Enable the second attribute in the vertex shader:
		glEnableVertexAttribArray(1);

		// Specify the location and data format of an array attribute to be used by the vertex shader:
		glVertexAttribPointer(1,          // Second attribute in the vertex shader
			3,                            // number of components per vertex, here: rgb
			GL_FLOAT,                     // format 
			GL_FALSE,                     // normalized?
			0,                            // stride
			colors); // array of vertex properties

		// draw primitives from array data:
		glDrawArrays(GL_TRIANGLES,  // 
			         0,             // starting index in array
			         3);            // number of vertices  
		
		glfwPollEvents(); // checks if the window was closed
	
		glfwSwapBuffers(window); // put the stuff we've been drawing onto the display
	}

	// Terminate GLFW library and cleanup resources:
	glfwTerminate();
	return 0;
}