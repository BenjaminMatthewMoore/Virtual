#include "Application.h"
#include "gl_core_4_4.h"
#include <cstdio>
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/transform.hpp"
#include "Camera.h"
#include "Grid.h"
#include "Window.h"
#include "tiny_obj_loader.h"
#include "ImportOBJ.h"
#include "Project.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "TerrainGen.h"
#include <iostream>
#include "RenderTargets.h"
#include "Particles.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using namespace std;

void APIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR"; 
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id;
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
}

void TurnOnDebugLogging()
{
	if (glDebugMessageCallback == nullptr) return;
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	GLuint unusedIds = 0;
	glDebugMessageControl(
		GL_DONT_CARE, // source
		GL_DONT_CARE, // type
		GL_DONT_CARE, // severity
		0,
		&unusedIds,
		true);
}


#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{

	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d\n",
			file, line);
		retCode = 1;
	}
	return retCode;
}

int main()
{

	


	TurnOnDebugLogging();
	printOpenGLError();

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_FRONT_AND_BACK);
	printOpenGLError();

	Camera* camera = new Camera(vec3(10, 100, 10), vec3(0), vec3(0, 1, 0), glm::pi<float>() * 0.25f, windowView->m_window);
	//Grid* grid = new Grid;
	TerrainGen* terrain = new TerrainGen;
	RenderTargets* projection = new RenderTargets;
	ImportOBJ* staff = new ImportOBJ();
	ParticleEmitter* m_emitter = new ParticleEmitter();
	//ImportOBJ* dragon = new ImportOBJ;
	m_emitter->initalise(1000, 500, 0.1f, 1.0f, 1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

	terrain->GenTerrain(128);
	//staff->LoadObject("./data/Models/soulspear/soulspear.obj", "./data/Models/soulspear/soulspear_diffuse.tga");
	//dragon->LoadObject("./data/Models/Dragon/Dragon.obj");

	Gizmos::create();
	Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 8, 8, vec4(1, 1, 0, 1));
	projection->genBuffer();
	projection->genTex();
	//grid->GenerateGrid(5, 5);

	printOpenGLError();
	float counter = 0;
	float onceOnly = 0;
	float lastFrameTime = (float)glfwGetTime();

	while (glfwWindowShouldClose(windowView->m_window) == false &&
		glfwGetKey(windowView->m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		float deltaTime = (float)glfwGetTime() - lastFrameTime;
		lastFrameTime = (float)glfwGetTime();
		counter += deltaTime;
		printOpenGLError();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//m_emitter->update(deltaTime, camera->GetProjectionView());
		
		//grid->SetProgram(camera->GetProjectionView(), lightsDirection, lightColour);
		//grid->Draw();
		projection->Draw(camera->GetProjectionView(), *terrain);
		//staff->Render(camera, lightsDirection, lightColour);
		terrain->Draw(camera->GetProjectionView());
		//dragon->Render(camera, lightsDirection, lightColour);
		
		//if (counter > .0001f)
		//{
		//	terrain->FlattenTerrain();
		//	counter = 0;
		//}

		//terrain->WaterRiseFall(deltaTime);
	//if (onceOnly == 0)
	//{
	//	terrain->MountainTest();
	//	onceOnly = 1;
	//}


		//m_emitter->draw(camera->GetProjectionView());
		camera->Update(deltaTime);
		
		
		counter = 0;
		// the rest of our code goes here!

		glfwSwapBuffers(windowView->m_window);
		glfwPollEvents();

	}
	
	Gizmos::destroy();



	glfwDestroyWindow(windowView->m_window);
	glfwTerminate();
	return 0;
}