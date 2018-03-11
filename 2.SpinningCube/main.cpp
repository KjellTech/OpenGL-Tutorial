#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLShader.h"
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "lodepng.h"



int main() {
	// Initialize GLFW library:
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	
	// Create Window:
	GLFWwindow *window = glfwCreateWindow(640, 640, "Spinning Cube", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	// Subsequent OpenGL calls applies to the context of window: 
	glfwMakeContextCurrent(window);
	
	// Initialize GLEW library:
	glewInit();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	
	GLuint shader_programme = LoadShader("vs.shd", "fs.shd");
	glUseProgram(shader_programme);

	GLuint texid;
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);

	glActiveTexture(GL_TEXTURE0); // first texture unit
	GLint sampler2dLoc = glGetUniformLocation(shader_programme, "tex");
	glUniform1i(sampler2dLoc, 0); // index of the texture unit = 0 since we use GL_TEXTURE0


	


	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "wooden-crate.png");
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 
		         0, // lod level, 0 = full resolution 
				 4, // number of color components in the texture 
				 width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);


	int i = 0;
	glm::vec3 eye(4.0f, 4.0f, 4.0f);
	glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the display

		glm::mat4 perspectiveMatrix = glm::perspective(45.0f,  // fovy
			                                           1.0f,  // aspect
			                                           0.1f,  // znear
			                                         100.0f);  // zfar
		
		eye = glm::rotate(eye, 0.1f/3.14159f, rotationAxis);
		
		glm::vec3 target(0.0f, 0.0f, 0.0f);
		glm::vec3 up(0.0, 1.0f, 0.0f);

		glm::mat4 viewMatrix = glm::lookAt(eye, target, up);

		glm::mat4 PVMatrix = perspectiveMatrix * viewMatrix;

		GLint PVMatrixLocation = glGetUniformLocation(shader_programme, "PVMatrix");
		glUniformMatrix4fv(PVMatrixLocation, 1, GL_FALSE, &PVMatrix[0][0]);

		// xyz of the three vertices in the triangle:
		GLfloat vertices[] = { 
							   // x = -1
							   -1.0f, -1.0f, -1.0f,
							   -1.0f, +1.0f, -1.0f,
							   -1.0f, +1.0f, +1.0f,
							   -1.0f, -1.0f, +1.0f,
							   // x = +1
							   +1.0f, -1.0f, -1.0f,
							   +1.0f, +1.0f, -1.0f,
							   +1.0f, +1.0f, +1.0f,
							   +1.0f, -1.0f, +1.0f,
							   // y = -1
							   -1.0f, -1.0f, -1.0f,
							   +1.0f, -1.0f, -1.0f,
							   +1.0f, -1.0f, +1.0f,
							   -1.0f, -1.0f, +1.0f,
							   // y = +1
							   -1.0f, +1.0f, -1.0f,
							   +1.0f, +1.0f, -1.0f,
							   +1.0f, +1.0f, +1.0f,
							   -1.0f, +1.0f, +1.0f,
							   // z = -1
							   -1.0f, -1.0f, -1.0f,
							   +1.0f, -1.0f, -1.0f,
							   +1.0f, +1.0f, -1.0f,
							   -1.0f, +1.0f, -1.0f,
							   // z = +1
							   -1.0f, -1.0f, +1.0f,
							   +1.0f, -1.0f, +1.0f,
							   +1.0f, +1.0f, +1.0f,
							   -1.0f, +1.0f, +1.0f
		};

		// Texture coordinates:
		GLfloat UV[] = {
			// x = -1
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};


		unsigned int indices[] = 
		{
		4 * 0 + 0, 4 * 0 + 1, 4 * 0 + 2, 4 * 0 + 0, 4 * 0 + 2, 4 * 0 + 3,
		4 * 1 + 0, 4 * 1 + 1, 4 * 1 + 2, 4 * 1 + 0, 4 * 1 + 2, 4 * 1 + 3,
		4 * 2 + 0, 4 * 2 + 1, 4 * 2 + 2, 4 * 2 + 0, 4 * 2 + 2, 4 * 2 + 3,
		4 * 3 + 0, 4 * 3 + 1, 4 * 3 + 2, 4 * 3 + 0, 4 * 3 + 2, 4 * 3 + 3,
		4 * 4 + 0, 4 * 4 + 1, 4 * 4 + 2, 4 * 4 + 0, 4 * 4 + 2, 4 * 4 + 3,
		4 * 5 + 0, 4 * 5 + 1, 4 * 5 + 2, 4 * 5 + 0, 4 * 5 + 2, 4 * 5 + 3,
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
			2,                            // number of components per vertex, here: uv
			GL_FLOAT,                     // format 
			GL_FALSE,                     // normalized?
			0,                            // stride
			UV); // array of vertex properties

		// draw primitives from indices:
		glDrawElements(GL_TRIANGLES,      // primitive type
			           6*2*3,             // number of vertices
					   GL_UNSIGNED_INT,   // format of indices
			           indices);            
		
		glfwPollEvents(); // checks if the window was closed
	
		glfwSwapBuffers(window); // put the stuff we've been drawing onto the display
		i++;
	}

	// Terminate GLFW library and cleanup resources:
	glfwTerminate();
	return 0;
}