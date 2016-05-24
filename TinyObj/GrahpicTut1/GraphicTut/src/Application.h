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
#include "ImportOBJ.h"
#include "Project.h"

#include "TerrainGen.h"
#include <iostream>
#include "RenderTargets.h"
#include "Particles.h"

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

	Camera* camera;
		//Grid* grid = new Grid;
	TerrainGen* terrain;
	RenderTargets* projection;
	ImportOBJ* staff;
	ParticleEmitter* m_emitter;
	Project* proj;
	vec3 lightsDirection;
	vec3 lightColour;
	float lastFrameTime;
	float deltaTime;
	float counter;
	float onceOnly;
};

