#pragma once
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
#include <string>

// all rendered objects inherit from project
class Project
{
public:
	Project();
	~Project();

	void Create();
	//takes in a vertex shader Fragment Shader and an ID to create unique shader program for rendered objects
	void CreateShader(std::string vertShader, std::string fragShader, unsigned int programID);
	Window* windowView;
	Camera* camera;
	Grid* grid;
	ImportOBJ* staff;
	const char* vsSource;
	const char* fsSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
};

