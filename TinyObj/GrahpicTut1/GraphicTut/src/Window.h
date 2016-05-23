#pragma once
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/transform.hpp"

class Window
{
public:
	Window();
	~Window();
	
	int CreateWindowView();
	GLFWwindow* m_window;
};

