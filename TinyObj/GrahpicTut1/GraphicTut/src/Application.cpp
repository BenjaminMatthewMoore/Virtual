#include "Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
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

#include "TerrainGen.h"
#include <iostream>
#include "RenderTargets.h"
#include "Particles.h"
#include "PhysicsScene.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;
using namespace std;
bool doThis;
int terrainSize;

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





Application::Application()
{
}

Application::~Application()
{
}

void Application::Startup()
{
	proj = new Project;
	windowView = new Window;
	lightsDirection = vec3(0.f, 0.f, 1.f);
	lightColour = vec3(1.f, 1.f, 1.f);
	terrainSize = 64;


	windowView->CreateWindowView();
	run = true;
	TurnOnDebugLogging();
	printOpenGLError();

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_FRONT_AND_BACK);
	printOpenGLError();

	camera = new Camera(vec3(10, 100, 10), vec3(0), vec3(0, 1, 0), glm::pi<float>() * 0.25f, windowView->m_window);
	//Grid* grid = new Grid;
	physicsScene = new PhysicsScene;
	physicsScene->SetUpPhysx((float)terrainSize);
	physicsScene->setUpVisualDebugger();
	terrain	= new TerrainGen;
	terrain->GenTerrain(terrainSize);
	physicsScene->CreateHeightMap(*terrain);
	projection = new RenderTargets;
	staff = new ImportOBJ();
	m_emitter = new ParticleEmitter();
	//ImportOBJ* dragon = new ImportOBJ;
	m_emitter->initalise(1000, 500, 0.1f, 1.0f, 1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

	
	projection->genBuffer();
	projection->genTex();
	Gizmos::create();
	Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 8, 8, vec4(1, 1, 0, 1));
	printOpenGLError();
	counter = 0;
	onceOnly = 0; 
	lastFrameTime = (float)glfwGetTime();
	doThis = true;
}

void Application::Update()
{
	deltaTime = (float)glfwGetTime() - lastFrameTime;
	physicsScene->Update(deltaTime, windowView);
	lastFrameTime = (float)glfwGetTime();
	counter += deltaTime;
	camera->Update(deltaTime);
	printOpenGLError();

	int state = (glfwGetKey(camera->p_window, GLFW_KEY_SPACE));
	if (state == GLFW_PRESS && doThis)
	{
		doThis = false;
		physicsScene->ShootCubes(camera);
		
	}
	if (state == GLFW_RELEASE)
	{
		doThis = true;
	}

	//terrain->WaterRiseFall(deltaTime);
	//if (onceOnly == 0)
	//{
	//	terrain->MountainTest();
	//	onceOnly = 1;
	//}
	
	//if (counter > .0001f)
	//{
	//	terrain->FlattenTerrain();
	//	counter = 0;
	//}
	
	//m_emitter->update(deltaTime, camera->GetProjectionView());
}

void Application::Draw()
{
	Gizmos::clear();
	camera->Update(deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection->Draw(camera->GetProjectionView(), *terrain);
	
	//staff->Render(camera, lightsDirection, lightColour);
	terrain->Draw(camera->GetProjectionView());
	physicsScene->DrawScene();
	Gizmos::draw(camera->GetProjectionView());
	counter = 0;
	glfwSwapBuffers(windowView->m_window);
	glfwPollEvents();
	//physicsScene->DrawScene();

	//grid->SetProgram(camera->GetProjectionView(), lightsDirection, lightColour);
	//grid->Draw();

	//staff->Render(camera, lightsDirection, lightColour);

	//dragon->Render(camera, lightsDirection, lightColour);

	//m_emitter->draw(camera->GetProjectionView());
	
}

void Application::Shutdown()
{
	Gizmos::destroy();
	glfwDestroyWindow(windowView->m_window);
	glfwTerminate();
	
}