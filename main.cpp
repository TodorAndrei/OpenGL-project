#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SkyBox.hpp"

#include "glm/glm.hpp" //core glm functionality
#include "glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp" //glm extension for computing inverse matrices
#include "glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib,"winmm.lib")

#include <iostream>

// window
gps::Window myWindow;

gps::Shader mySkyboxShader;
gps::SkyBox mySkybox;
std::vector<const GLchar*> mySkyboxFaces;

// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;

glm::vec3 lightDir2;
glm::vec3 lightColor2;

// shader uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;
GLint normalMatrixLoc;
GLint lightDirLoc;
GLint lightColorLoc;
GLint lightDirLoc2;
GLint lightColorLoc2;

// camera
gps::Camera myCamera(glm::vec3(0.0f, 1.0f, 10.5f), glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.08f;

GLboolean pressedKeys[1024];

// models
gps::Model3D airport;
GLfloat angleLeftRight = 0.0f;
GLfloat angleUpDown = 0.0f;
GLfloat angle;

// shaders
gps::Shader myBasicShader;

bool fog = FALSE;

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
	//TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

	if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        } else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    //TODO
}

void processMovement() {
	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		//update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

    if (pressedKeys[GLFW_KEY_E]) {
        myCamera.move(gps::MOVE_UP, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_Q]) {
        myCamera.move(gps::MOVE_DOWN, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_LEFT]) {
        angleLeftRight -= 0.02f;
        // update model matrix for airport
        myCamera.rotate(angleUpDown, angleLeftRight);
        // update normal matrix for airport
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_RIGHT]) {
        angleLeftRight += 0.02f;
        // update model matrix for airport
        myCamera.rotate(angleUpDown, angleLeftRight);
        // update normal matrix for airport
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_UP]) {
        angleUpDown += 0.01f;
        // update model matrix for airport
        myCamera.rotate(angleUpDown, angleLeftRight);
        // update normal matrix for airport
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_DOWN]) {
        angleUpDown -= 0.01f;
        // update model matrix for airport
        myCamera.rotate(angleUpDown, angleLeftRight);
        // update normal matrix for airport
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for airport
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_1]) {
        PlaySound(TEXT("jetfly.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }
    if (pressedKeys[GLFW_KEY_2]) {
        PlaySound(NULL, 0, 0);
    }

    if (pressedKeys[GLFW_KEY_Z]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (pressedKeys[GLFW_KEY_X]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (pressedKeys[GLFW_KEY_C]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if (pressedKeys[GLFW_KEY_3]) {
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        myBasicShader.useShaderProgram();
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    }

    if (pressedKeys[GLFW_KEY_4]) {
        lightColor = glm::vec3(0.0f, 0.0f, 0.0f);
        myBasicShader.useShaderProgram();
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    }

    if (pressedKeys[GLFW_KEY_5]) {
        lightColor2 = glm::vec3(1.0f, 1.0f, 1.0f);
        myBasicShader.useShaderProgram();
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightColor2"), 1, glm::value_ptr(lightColor2));
    }

    if (pressedKeys[GLFW_KEY_6]) {
        lightColor2 = glm::vec3(0.0f, 0.0f, 0.0f);
        myBasicShader.useShaderProgram();
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightColor2"), 1, glm::value_ptr(lightColor2));
    }

    if (pressedKeys[GLFW_KEY_F]) {
        fog = true;
        myBasicShader.useShaderProgram();
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "fog"), fog);
    }

    if (pressedKeys[GLFW_KEY_G]) {
        fog = false;
        myBasicShader.useShaderProgram();
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "fog"), fog);
    }

}

void initOpenGLWindow() {
    myWindow.Create(1920, 1080, "OpenGL Project Core");
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
    airport.LoadModel("models/theAirport2/theAirport2.obj");
}

void initShaders() {
	myBasicShader.loadShader(
        "shaders/basic.vert",
        "shaders/basic.frag");

    mySkyboxFaces.push_back("skybox/posx.jpg");
    mySkyboxFaces.push_back("skybox/negx.jpg");
    mySkyboxFaces.push_back("skybox/posy.jpg");
    mySkyboxFaces.push_back("skybox/negy.jpg");
    mySkyboxFaces.push_back("skybox/posz.jpg");
    mySkyboxFaces.push_back("skybox/negz.jpg");

    mySkybox.Load(mySkyboxFaces);

    mySkyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
    mySkyboxShader.useShaderProgram();

    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(mySkyboxShader.shaderProgram, "view"), 1, GL_FALSE,
        glm::value_ptr(view));

    projection = glm::perspective(glm::radians(45.0f), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(mySkyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
        glm::value_ptr(projection));
}

void initUniforms() {
	myBasicShader.useShaderProgram();

    // create model matrix for airport
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

	// get view matrix for current camera
	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
	// send view matrix to shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix for airport
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

	// create projection matrix
    projection = glm::perspective(glm::radians(45.0f), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height, 0.1f, 200.0f);
	projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
	// send projection matrix to shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));	

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
	// send light dir to shader
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
	// send light color to shader
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

    lightDir2 = glm::vec3(0.0f, 1.0f, -1.0f);
    lightDirLoc2 = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir2");
    // send light dir to shader
    glUniform3fv(lightDirLoc2, 1, glm::value_ptr(lightDir2));

    lightColor2 = glm::vec3(1.0f, 1.0f, 1.0f); //white light
    lightColorLoc2 = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor2");
    // send light color to shader
    glUniform3fv(lightColorLoc2, 1, glm::value_ptr(lightColor2));
}

void renderAirport(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    //send airport model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    //send airport normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw airport
    airport.Draw(shader);
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render the scene

	// render the airport
	renderAirport(myBasicShader);

    mySkybox.Draw(mySkyboxShader, view, projection);

}

void cleanup() {
    myWindow.Delete();
    //cleanup code for your own data
}

int main(int argc, const char * argv[]) {

    try {
        initOpenGLWindow();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
	initModels();
	initShaders();
	initUniforms();
    setWindowCallbacks();

	glCheckError();
	// application loop
	while (!glfwWindowShouldClose(myWindow.getWindow())) {
        processMovement();
	    renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}

	cleanup();

    return EXIT_SUCCESS;
}
