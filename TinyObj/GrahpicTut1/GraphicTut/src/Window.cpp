#include "Window.h"



Window::Window()
{
}


Window::~Window()
{
}

int Window::CreateWindowView()
{
	if (glfwInit() == false)
		return -1;

	m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
	if (m_window == nullptr) {
		glfwTerminate();
		return -2;
	}


	glfwMakeContextCurrent(m_window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return -3;
	}
	return 4;
}