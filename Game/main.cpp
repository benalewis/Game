#include <iostream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

// Other includes
#include "Shader.h"
#include "Camera.h"

/*-----------Function Declarations-----------*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_move();

/*-----------Window Globals-----------*/
const GLuint WIDTH = 800, HEIGHT = 600;

/*-----------Camera Globals-----------*/
bool keys[1024]; 
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool firstMouse = true;

/*-----------Time Globals-----------*/
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
    // Init GLFW & Set all the required options for GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // GLFW Window & Current Context Set
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ice Maze", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Callback Functions
    glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

    // Initialize GLEW to setup the OpenGL Function pointers (Using Modern Approach)
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Build and compile the Shader Program
    Shader shaderProgram("Shaders/vertexShader.vert", "Shaders/fragmentShader.frag");

    // Set up vertex data (and buffer(s)) and attribute pointers
   GLfloat vertices[] = {

	/*-----------Pos & Texture Coordinates-----------*/
	//First 3 are position coords, last 2 are texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
   glm::vec3 floor[] = {
	    //Row 1
	glm::vec3( 0.0f,  0.0f, 0.0f), 
	glm::vec3( 1.0f,  0.0f, 0.0f), 
	glm::vec3( 2.0f,  0.0f, 0.0f), 
		//Row 2
	glm::vec3( 0.0f,  0.0f, 1.0f), 
	glm::vec3( 1.0f,  0.0f, 1.0f), 
	glm::vec3( 2.0f,  0.0f, 1.0f),
		//Row 3
	glm::vec3( 0.0f,  0.0f, 2.0f), 
	glm::vec3( 1.0f,  0.0f, 2.0f), 
	glm::vec3( 2.0f,  0.0f, 2.0f), 
		//Row 4
	glm::vec3( 0.0f,  0.0f, 3.0f), 
	glm::vec3( 1.0f,  0.0f, 3.0f), 
	glm::vec3( 2.0f,  0.0f, 3.0f), 
		//Row 5
	glm::vec3( 0.0f,  0.0f, 4.0f), 
	glm::vec3( 1.0f,  0.0f, 4.0f), 
	glm::vec3( 2.0f,  0.0f, 4.0f),
		//Row 6
	glm::vec3( 0.0f,  0.0f, 5.0f), 
	glm::vec3( 1.0f,  0.0f, 5.0f), 
	glm::vec3( 2.0f,  0.0f, 5.0f) 
   };

   glm::vec3 sideWalls[] = {
	   //Left Wall 1st Story
	   glm::vec3(-1.0f, 0.0f, 0.0f),
	   glm::vec3(-1.0f, 0.0f, 1.0f),
	   glm::vec3(-1.0f, 0.0f, 2.0f),
	   glm::vec3(-1.0f, 0.0f, 3.0f),
	   glm::vec3(-1.0f, 0.0f, 4.0f),
	   //Left Wall 2nd Story
	   glm::vec3(-1.0f, 1.0f, 0.0f),
	   glm::vec3(-1.0f, 1.0f, 1.0f),
	   glm::vec3(-1.0f, 1.0f, 2.0f),
	   glm::vec3(-1.0f, 1.0f, 3.0f),
	   glm::vec3(-1.0f, 1.0f, 4.0f),
	   //Left Wall 3rd Story
	   glm::vec3(-1.0f, 2.0f, 0.0f),
	   glm::vec3(-1.0f, 2.0f, 1.0f),
	   glm::vec3(-1.0f, 2.0f, 2.0f),
	   glm::vec3(-1.0f, 2.0f, 3.0f),
	   glm::vec3(-1.0f, 2.0f, 4.0f),
	   //Left Wall 4th Story
	   glm::vec3(-1.0f, 3.0f, 0.0f),
	   glm::vec3(-1.0f, 3.0f, 1.0f),
	   glm::vec3(-1.0f, 3.0f, 2.0f),
	   glm::vec3(-1.0f, 3.0f, 3.0f),
	   glm::vec3(-1.0f, 3.0f, 4.0f),
	   //Left Wall 5th Story
	   glm::vec3(-1.0f, 4.0f, 0.0f),
	   glm::vec3(-1.0f, 4.0f, 1.0f),
	   glm::vec3(-1.0f, 4.0f, 2.0f),
	   glm::vec3(-1.0f, 4.0f, 3.0f),
	   glm::vec3(-1.0f, 4.0f, 4.0f),

	   //Right Wall 1st Story
	   glm::vec3(3.0f, 0.0f, 0.0f),
	   glm::vec3(3.0f, 0.0f, 1.0f),
	   glm::vec3(3.0f, 0.0f, 2.0f),
	   glm::vec3(3.0f, 0.0f, 3.0f),
	   glm::vec3(3.0f, 0.0f, 4.0f),
	   //Right Wall 1st Story
	   glm::vec3(3.0f, 1.0f, 0.0f),
	   glm::vec3(3.0f, 1.0f, 1.0f),
	   glm::vec3(3.0f, 1.0f, 2.0f),
	   glm::vec3(3.0f, 1.0f, 3.0f),
	   glm::vec3(3.0f, 1.0f, 4.0f),
	   //Right Wall 1st Story
	   glm::vec3(3.0f, 2.0f, 0.0f),
	   glm::vec3(3.0f, 2.0f, 1.0f),
	   glm::vec3(3.0f, 2.0f, 2.0f),
	   glm::vec3(3.0f, 2.0f, 3.0f),
	   glm::vec3(3.0f, 2.0f, 4.0f),
	   //Right Wall 1st Story
	   glm::vec3(3.0f, 3.0f, 0.0f),
	   glm::vec3(3.0f, 3.0f, 1.0f),
	   glm::vec3(3.0f, 3.0f, 2.0f),
	   glm::vec3(3.0f, 3.0f, 3.0f),
	   glm::vec3(3.0f, 3.0f, 4.0f),
	   //Right Wall 1st Story
	   glm::vec3(3.0f, 4.0f, 0.0f),
	   glm::vec3(3.0f, 4.0f, 1.0f),
	   glm::vec3(3.0f, 4.0f, 2.0f),
	   glm::vec3(3.0f, 4.0f, 3.0f),
	   glm::vec3(3.0f, 4.0f, 4.0f),
   };

  glm::vec3 backWall[] = {
	   //First Story
	   glm::vec3(-1.0f, 0.0f, -1.0f),
	   glm::vec3(0.0f, 0.0f, -1.0f),
	   glm::vec3(1.0f, 0.0f, -1.0f),
	   glm::vec3(2.0f, 0.0f, -1.0f),
	   glm::vec3(3.0f, 0.0f, -1.0f),
	   //Second Story
	   glm::vec3(-1.0f, 1.0f, -1.0f),
	   glm::vec3(0.0f, 1.0f, -1.0f),
	   glm::vec3(1.0f, 1.0f, -1.0f),
	   glm::vec3(2.0f, 1.0f, -1.0f),
	   glm::vec3(3.0f, 1.0f, -1.0f),
	   //Third Story
	   glm::vec3(-1.0f, 2.0f, -1.0f),
	   glm::vec3(0.0f, 2.0f, -1.0f),
	   glm::vec3(1.0f, 2.0f, -1.0f),
	   glm::vec3(2.0f, 2.0f, -1.0f),
	   glm::vec3(3.0f, 2.0f, -1.0f),
	   //Fourth Story
	   glm::vec3(-1.0f, 3.0f, -1.0f),
	   glm::vec3(0.0f, 3.0f, -1.0f),
	   glm::vec3(1.0f, 3.0f, -1.0f),
	   glm::vec3(2.0f, 3.0f, -1.0f),
	   glm::vec3(3.0f, 3.0f, -1.0f),
	   //Roof
	   glm::vec3(-1.0f, 4.0f, -1.0f),
	   glm::vec3(0.0f, 4.0f, -1.0f),
	   glm::vec3(1.0f, 4.0f, -1.0f),
	   glm::vec3(2.0f, 4.0f, -1.0f),
	   glm::vec3(3.0f, 4.0f, -1.0f)
   };

  glm::vec3 collectable[] = {
		glm::vec3(1.0f, 2.0f, -0.5f),
  };

	/*-----------VBO & VAO-----------*/
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Texxture Coordinate attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
	// Unbind VAO
    glBindVertexArray(0); 

	/*-----------Textures-----------*/
	//Texture 1
    GLuint texture1;
    glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0); //Activate Texture (0) before binding
    glBindTexture(GL_TEXTURE_2D, texture1); 
    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //Mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    int width, height;
    unsigned char* image1 = SOIL_load_image("Textures/icewall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture 

	//Texture 2
    GLuint texture2;
    glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1); //Activate Texture (0) before binding
    glBindTexture(GL_TEXTURE_2D, texture2); 
    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //Mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    unsigned char* image2 = SOIL_load_image("Textures/icefloor.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture 

	//Texture 3
    GLuint texture3;
    glGenTextures(1, &texture3);
	glActiveTexture(GL_TEXTURE2); //Activate Texture (0) before binding
    glBindTexture(GL_TEXTURE_2D, texture3); 
    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //Mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    unsigned char* image3 = SOIL_load_image("Textures/collectable.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image3);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image3);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture 

    /*-----------Game Loop-----------*/
    while (!glfwWindowShouldClose(window))
    {
		/*-----------Time (Smooth FPS)-----------*/
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame; 

        // Event checks and movement checks
        glfwPollEvents();
		do_move();

        /*-----------Render-----------*/
		glEnable(GL_DEPTH_TEST); //Enable depth testing
        glClearColor(0.250f, 0.286f, 0.509f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //Clear color and depth

		 // Activate shader program
        shaderProgram.use();  

		/*-----------Camera & 3D-----------*/
		glm::mat4 view; //View matrix & lookAt
		view = camera.GetViewMatrix();
		glm::mat4 model; //Model view matrix
		glm::mat4 projection; //Projection matrix

		view = glm::translate(view, glm::vec3(-1.0f, -1.5f, -3.0f)); //camera start point
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f); //projection perspective

		//Get Uniform locations from the shader program
		GLint modelLoc = glGetUniformLocation(shaderProgram.Program, "model"); 
		GLint viewLoc = glGetUniformLocation(shaderProgram.Program, "view"); 
		GLint projLoc = glGetUniformLocation(shaderProgram.Program, "projection"); 

		//Pass the values to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); 
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 

		glBindVertexArray(VAO);
		/*--------Drawing---------*/
		// Bind Wall texture using texture units
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shaderProgram.Program, "Texture"), 0);
		
        // Draw Side Walls
		for (GLuint i = 0; i < 50; i++)
		{
			glm::mat4 wallModel;
			wallModel = glm::translate(wallModel, sideWalls[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(wallModel));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		// Draw Back Wall
		for (GLuint i = 0; i < 25; i++)
		{
			glm::mat4 backModel;
			backModel = glm::translate(backModel, backWall[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(backModel));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Bind Floor texture using texture units
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shaderProgram.Program, "Texture"), 0);
		
        // Draw Floor
		for (GLuint i = 0; i < 15; i++)
		{
			glm::mat4 floorModel;
			floorModel = glm::translate(floorModel, floor[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(floorModel));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw Collectable(s)
		glBindTexture(GL_TEXTURE_2D, texture3);
        glUniform1i(glGetUniformLocation(shaderProgram.Program, "Texture"), 0);
		for (GLuint i = 0; i < 1; i++)
		{
			glm::mat4 collectMe;
			collectMe = glm::translate(collectMe, collectable[i]);
			collectMe = glm::rotate(collectMe,(GLfloat)glfwGetTime() * 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(collectMe));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        glBindVertexArray(0);
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Clear up memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminate GLFW & Return
    glfwTerminate();
    return 0;
}

	/*-----------Key Callback Function-----------*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//Escape 
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

		if(action == GLFW_PRESS)
	  keys[key] = true;
		else if(action == GLFW_RELEASE)
	  keys[key] = false; 
}

	/*-----------WASD Movement-----------*/
void do_move()
{

	//WASD & Command Line I/O
	if (keys[GLFW_KEY_W]) {
         camera.ProcessKeyboard(W, deltaTime);
		 cout << "W" << endl; }
	if (keys[GLFW_KEY_S]) {
        camera.ProcessKeyboard(S, deltaTime);
		cout << "S" << endl; }
	if (keys[GLFW_KEY_A]) {
        camera.ProcessKeyboard(A, deltaTime);
		cout << "A" << endl; }
	if (keys[GLFW_KEY_D]) {
        camera.ProcessKeyboard(D, deltaTime);
		cout << "D" << endl; }
};

	/*-----------Mouse Movement (Using Eular)-----------*/
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) //Center mouse on screen when first entered
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
}