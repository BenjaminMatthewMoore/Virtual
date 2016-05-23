#pragma once
#include <string>
#include "gl_core_4_4.h"
#include <cstdio>
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/transform.hpp"
#include "Camera.h"
#include "Window.h"
#include "tiny_obj_loader.h"
#include <string>
class Program
{
public:
	Program();
	~Program();

	bool create(std::string vertShader, std::string fragShader);

	//void LoadShader(const char* vertexShader, const char* fragShader);
	unsigned int m_programID;
	const char* vsSource;
	const char* fsSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
};

