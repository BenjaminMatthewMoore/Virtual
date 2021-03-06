#pragma once
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
#include <iostream>

class ImportOBJ;
class Project;
class TerrainGen;
class RenderTargets;
class ParticleFireEmitter;
class PhysicsScene;
//class PhysicsScene;

class Application
{
public:
	Application();
	~Application();

	void Startup();
	void Update();
	void Draw();
	void Shutdown();
	bool run;
	Window* windowView;
private:

	PhysicsScene* physicsScene;
	Camera* camera;
		//Grid* grid = new Grid;
	TerrainGen* terrain;
	RenderTargets* projection;
	ImportOBJ* staff;
	ParticleFireEmitter* m_emitter;
	Project* proj;
	vec3 lightsDirection;
	vec3 lightColour;
	float lastFrameTime;
	float deltaTime;
	float counter;
	float onceOnly;
//	PhysicsScene* physicsScene;
};

