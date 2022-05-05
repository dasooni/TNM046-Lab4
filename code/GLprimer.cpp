/*
 * A C++ framework for OpenGL programming in TNM046 for MT1
 *
 * This is a small, limited framework, designed to be easy to use for students in an introductory
 * computer graphics course in the first year of a M Sc curriculum. It uses custom code for some
 * things that are better solved by external libraries like GLM, but the emphasis is on simplicity
 * andreadability, not generality.
 *
 * For the window management, GLFW 3.x is used for convenience.
 * The framework should work in Windows, MacOS X and Linux.
 * GLEW is used for handling platform specific OpenGL extensions.
 * This code depends only on GLFW, GLEW, and OpenGL libraries.
 * OpenGL 3.3 or higher is required.
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2013-2015
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#if defined(WIN32) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif 

// File and console I/O for logging and error reporting
#include <iostream>
// Math header for trigonometric functions
#include <cmath>

// glew provides easy access to advanced OpenGL functions and extensions
#include <GL/glew.h>

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>

#include "Utilities.hpp"

#include <vector>
#include <array>

#include "Shader.hpp"
#include "TriangleSoup.hpp"

GLuint createVertexBuffer(int location, int dimensions, const std::vector<float>& vertices) {
    GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // Tell OpenGL how the data is stored in our buffer
    // Attribute location (must match layout(location=#) statement in shader)
    // Number of dimensions (3 -> vec3 in the shader, 2-> vec2 in the shader),
    // type GL_FLOAT, not normalized, stride 0, start at element 0
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Enable the attribute in the currently bound VAO
    glEnableVertexAttribArray(location);

    return bufferID;
}

GLuint createIndexBuffer(const std::vector<unsigned int>& indices) {
    GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    // Activate (bind) the index buffer and copy data to it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);

    return bufferID;
}

//Matrix operations
std::array<float, 16> mat4mult(const std::array<float, 16>& m1, const std::array<float, 16>& m2) {
    std::array<float, 16> result;

    // Your code goes here: compute and set each element of result, e.g.:
    result[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	result[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	result[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	result[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
	
	result[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	result[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	result[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	result[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
	
	result[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	result[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	result[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	result[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
	
	result[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	result[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	result[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	result[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
	
    return result;
}
std::array<float, 16> mat4identity() {
	std::array<float, 16> result;
	
	result[0] = 1.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	
	result[4] = 0.0f;
	result[5] = 1.0f;
	result[6] = 0.0f;
	result[7] = 0.0f;
	
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = 1.0f;
	result[11] = 0.0f;
	
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}
std::array<float, 16> mat4rotx(float angle) {
	std::array<float, 16> result;
	
	result[0] = 1.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	
	result[4] = 0.0f;
	result[5] = cos(angle);
	result[6] = sin(angle);
	result[7] = 0.0f;
	
	result[8] = 0.0f;
	result[9] = -sin(angle);
	result[10] = cos(angle);
	result[11] = 0.0f;
	
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}
std::array<float, 16> mat4roty(float angle) {
	std::array<float, 16> result;
	
	result[0] = cos(angle);
	result[1] = 0.0f;
	result[2] = -sin(angle);
	result[3] = 0.0f;
	
	result[4] = 0.0f;
	result[5] = 1.0f;
	result[6] = 0.0f;
	result[7] = 0.0f;
	
	result[8] = sin(angle);
	result[9] = 0.0f;
	result[10] = cos(angle);
	result[11] = 0.0f;
	
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}
std::array<float, 16> mat4rotz(float angle) {
	std::array<float, 16> result;
	
	result[0] = cos(angle);
	result[1] = sin(angle);
	result[2] = 0.0f;
	result[3] = 0.0f;
	
	result[4] = -sin(angle);
	result[5] = cos(angle);
	result[6] = 0.0f;
	result[7] = 0.0f;
	
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = 1.0f;
	result[11] = 0.0f;
	
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}
std::array<float, 16> mat4scale(float scale) {
	std::array<float, 16> result;
	
	result[0] = scale;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	
	result[4] = 0.0f;
	result[5] = scale;
	result[6] = 0.0f;
	result[7] = 0.0f;
	
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = scale;
	result[11] = 0.0f;
	
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}
std::array<float, 16> mat4translate(float x, float y, float z) {
	std::array<float, 16> result;
	
	result[0] = 1.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	
	result[4] = 0.0f;
	result[5] = 1.0f;
	result[6] = 0.0f;
	result[7] = 0.0f;
	
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = 1.0f;
	result[11] = 0.0f;
	
	result[12] = x;
	result[13] = y;
	result[14] = z;
	result[15] = 1.0f;
	return result;
}
std::array<float, 16> mat4perspective(float vfov, float aspect, float znear, float zfar) {
	std::array<float, 16> result;
	
	float f = 1.0f / tan(vfov / 2.0f);
	
	result[0] = f / aspect;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	
	result[4] = 0.0f;
	result[5] = f;
	result[6] = 0.0f;
	result[7] = 0.0f;
	
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = -(zfar + znear) / (zfar - znear);
	result[11] = -1.0f;
	
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = -(2.0f * zfar * znear) / (zfar- znear);
	result[15] = 0.0f;
	
	return result;
	
}


void mat4print(const std::array<float, 16>& m) {
    printf("Matrix:\n");
    printf("%6.2f %6.2f %6.2f %6.2f\n", m[0], m[4], m[8], m[12]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", m[1], m[5], m[9], m[13]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", m[2], m[6], m[10], m[14]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", m[3], m[7], m[11], m[15]);
    printf("\n");
}

/*
 * main(int argc, char* argv[]) - the standard C++ entry point for the program
 */
int main(int argc, char* argv[]) {
    // Vertex coordinates (x,y,z) for three vertices

	//A cube has 6 faces with 2 triangles each (12 edges). Each face has 3 vertices. Each vertex has 3 coordinates. 

	GLfloat color1[] = {255, 0, 0};
    GLfloat color2[] = {0, 255, 0};
    GLfloat color3[] = {0, 0, 255};
    GLfloat color4[] = {255, 255, 0};
    GLfloat color5[] = {0, 255, 255};
    GLfloat color6[] = {255, 255, 255};
	
    const std::vector<GLfloat> vertexArrayData = {
        //-1.0f, -1.0f, 0.0f,  // First vertex, xyz
        //1.0f,  -1.0f, 0.0f,  // Second vertex, xyz
        //0.0f,  1.0f,  0.0f   // Third vertex, xyz
		
		// front
  //      -1.0, -1.0, 1.0, 
		//1.0, -1.0, 1.0, 
		//1.0, 1.0, 1.0, 
		//-1.0, 1.0, 1.0,
  //      // back
  //      -1.0, -1.0, -1.0, 
		//1.0, -1.0, -1.0, 
		//1.0, 1.0, -1.0, 
		//-1.0, 1.0, -1.0

		        // top (+z)
        -1, -1,  1,
         1, -1,  1,
        -1,  1,  1,
        -1,  1,  1,
         1, -1,  1,
         1,  1,  1, 

        // bottom (-z)
        -1, -1, -1,
        -1,  1, -1,
         1, -1, -1,
         1, -1, -1,
        -1,  1, -1,
         1,  1, -1,

        // right (+x)
         1, -1, -1, 
         1,  1, -1, 
         1, -1,  1, 
         1, -1,  1, 
         1,  1, -1, 
         1,  1,  1, 

        // left (-x)
        -1, -1, -1, 
        -1, -1,  1, 
        -1,  1, -1, 
        -1,  1, -1, 
        -1, -1,  1, 
        -1,  1,  1, 

        // front (+y)
        -1, -1, -1,
         1, -1, -1,
        -1, -1,  1,
        -1, -1,  1,
         1, -1, -1,
         1, -1,  1,

        // back (-y)
        -1,  1, -1,
        -1,  1,  1,
         1,  1, -1,
         1,  1, -1,
        -1,  1,  1,
         1,  1,  1,
    };
    const std::vector<GLfloat> colorArrayData = {
        //1.0f, 0.0f, 0.0f,  // Red
        //0.0f, 1.0f, 0.0f,  // Green
        //0.0f, 0.0f, 1.0f,  // Blue

		//// front
		//1.0f, 0.0f, 0.0f, // red
		//0.0f, 1.0f, 0.0f, // green
		//0.0f, 0.0f, 1.0f, // blue
		//1.0f, 1.0f, 1.0f, // white

		//// back
		//1.0f, 0.0f, 0.0f, // red
		//0.0f, 1.0f, 0.0f, // green
		//0.0f, 0.0f, 1.0f, // blue
		//1.0f, 1.0f, 1.0f // white

		color1[0], color1[1], color1[2], color1[0], color1[1], color1[2],
        color1[0], color1[1], color1[2], color1[0], color1[1], color1[2],
        color1[0], color1[1], color1[2], color1[0], color1[1], color1[2],

        color2[0], color2[1], color2[2], color2[0], color2[1], color2[2],
        color2[0], color2[1], color2[2], color2[0], color2[1], color2[2],
        color2[0], color2[1], color2[2], color2[0], color2[1], color2[2],

        color3[0], color3[1], color3[2], color3[0], color3[1], color3[2],
        color3[0], color3[1], color3[2], color3[0], color3[1], color3[2],
        color3[0], color3[1], color3[2], color3[0], color3[1], color3[2],

        color4[0], color4[1], color4[2], color4[0], color4[1], color4[2],
        color4[0], color4[1], color4[2], color4[0], color4[1], color4[2],
        color4[0], color4[1], color4[2], color4[0], color4[1], color4[2],

        color5[0], color5[1], color5[2], color5[0], color5[1], color5[2],
        color5[0], color5[1], color5[2], color5[0], color5[1], color5[2],
        color5[0], color5[1], color5[2], color5[0], color5[1], color5[2],

        color6[0], color6[1], color6[2], color6[0], color6[1], color6[2],
        color6[0], color6[1], color6[2], color6[0], color6[1], color6[2],
        color6[0], color6[1], color6[2], color6[0], color6[1], color6[2],
    };
    const std::vector<GLuint> indexArrayData = {
		//0, 1, 2,
		//2, 3, 0,
		//
		//1, 5, 6,
		//6, 2, 1,
		//
		//7, 6, 5,
		//5, 4, 7,
		//
		//4, 0, 3,
		//3, 7, 4,
		//
		//4, 5, 1,
		//1, 0, 4,
		//
		//3, 2, 6,
		//6, 7, 3

        0,  1,  2, 
        3,  4,  5,

        6,  7,  8,  
        9,  10, 11,

        12, 13, 14, 
        15, 16, 17,

        18, 19, 20, 
        21, 22, 23,

        24, 25, 26, 
        27, 28, 29,

        30, 31, 32, 
        33, 34, 35
	};
	
	int width, height;  // Get window size.
	
    float time;
	
    GLint locationTime, locationP, locationR, locationMV;
	
    Shader myShader;

    TriangleSoup myShape;
	
    // Initialise GLFW
    glfwInit(); 

    const GLFWvidmode* vidmode;  // GLFW struct to hold information about the display
    GLFWwindow* window;
    // Determine the desktop size
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor()); 

    // Make sure we are getting a GL context of at least version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable the OpenGL core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a square window (aspect 1:1) to fill half the screen height
    window = glfwCreateWindow(vidmode->height / 2, vidmode->height / 2, "GLprimer", nullptr, nullptr);
    if (!window) {
        std::cout << "Unable to open window. Terminating.\n";
        glfwTerminate();  // No window was opened, so we can't continue in any useful way
        return -1;
    }

    // Make the newly created window the "current context" for OpenGL
    // (This step is strictly required or things will simply not work)
    glfwMakeContextCurrent(window);

    // Initialize glew
    GLenum err = glewInit();
	
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return -1;
    }

    // Generate 1 Vertex array object, put the resulting identifier in vertexArrayID
    GLuint vertexArrayID = 0;
    glGenVertexArrays(1, &vertexArrayID);
    // Activate the vertex array object
    glBindVertexArray(vertexArrayID);

    // Create the vertex buffer objects for attribute locations 0 and 1
    // (the list of vertex coordinates and the list of vertex colors).
    GLuint vertexBufferID = createVertexBuffer(0, 3, vertexArrayData);
    GLuint colorBufferID = createVertexBuffer(1, 3, colorArrayData);
    // Create the index buffer object (the list of triangles).
    GLuint indexBufferID = createIndexBuffer(indexArrayData);

    // Deactivate the vertex array object again to be nice
    glBindVertexArray(0);

    // Show some useful information on the GL context
    std::cout << "GL vendor:       " << glGetString(GL_VENDOR)
              << "\nGL renderer:     " << glGetString(GL_RENDERER)
              << "\nGL version:      " << glGetString(GL_VERSION)
              << "\nDesktop size:    " << vidmode->width << " x " << vidmode->height << "\n";

	// Do not wait for screen refresh between frames
    glfwSwapInterval(0);  
    // Enable back face culling
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //Shaders
    myShader.createShader("vertex.glsl", "fragment.glsl");

    locationTime = glGetUniformLocation(myShader.id(), "time");
    locationR = glGetUniformLocation(myShader.id(), "R");
    locationP = glGetUniformLocation(myShader.id(), "P");
    locationMV = glGetUniformLocation(myShader.id(), "MV");

	
    glUseProgram(myShader.id());  // Activate the shader to set its variables
	
    if (locationTime == -1) {  // If the variable is not found, -1 is returned
        std::cout << "Unable to locate variable 'time' in shader!\n"
                  << indexArrayData.size() * sizeof(unsigned int);
    }

	// Lab 3 & 4
    //myShape.createSphere(1.0, 200);
    myShape.createBox(0.2, 0.2, 1.0);
	
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Set the clear color to a dark gray (RGBA)
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);  // Set viewport. This is the pixel rectangle we want to draw into
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers for drawing
        
		util::displayFPS(window);
		
        /* ---- Rendering code should go here ---- */
        time = static_cast<float>(glfwGetTime());  // Number of seconds since the program was started
        
		glUseProgram(myShader.id());
       

		//auto composition = mat4identity();
		auto R = mat4identity();
        auto P = mat4identity();
		auto MV = mat4identity();
        
		
		auto S = mat4scale(0.5);
        auto V = mat4rotx(M_PI / 10);
        auto T = mat4translate(0.0, 0.0, 3.0);
		

        auto orbit = mat4roty( time * M_PI / 8); //R1
		auto spin = mat4rotx( time * M_PI / 2); // R2

        P = mat4perspective( M_PI / 2, 1, 0.1, 100.0);
		
		//composition = mat4mult(V, orbit);
		//composition = mat4mult(composition, T);
		//composition = mat4mult(composition, spin);
  //      R = mat4mult(spin, R);
  //      MV = mat4mult(V, orbit);
  //      MV = mat4mult(MV, T);
		//MV = mat4mult(MV, spin);
        MV = spin;
		
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // rendering as lines or filled
		
        glUniformMatrix4fv(locationR, 1, GL_FALSE, R.data());
        glUniformMatrix4fv(locationMV, 1, GL_FALSE, MV.data()); 
		glUniformMatrix4fv(locationP, 1, GL_FALSE, P.data()); 
        glUniform1f(locationTime, time); 
        
		myShape.render();
		
		// Draw the triangles
		
        /*glBindVertexArray(vertexArrayID);
		
        // Draw our triangle with 3 vertices.
        // When the last argument of glDrawElements is nullptr, it means
        // "use the previously bound index buffer". (This is not obvious.)
        // The index buffer is part of the VAO state and is bound with it.
		
        glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);*/

        // Swap buffers, display the image and prepare for next frame
        glfwSwapBuffers(window);

        // Poll events (read keyboard and mouse input)
        glfwPollEvents();

        // Exit if the ESC key is pressed (and also if the window is closed)
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    // Close the OpenGL window and terminate GLFW
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &colorBufferID);
    glDeleteBuffers(1, &indexBufferID);

    glfwDestroyWindow(window);
    glfwTerminate();

}
