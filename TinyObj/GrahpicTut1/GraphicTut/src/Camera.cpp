#include "Camera.h"
#include <GLFW/glfw3.h>


Camera::Camera(vec3 pos, vec3 lookat, vec3 up, float fov, GLFWwindow* currentWindow)
{
	view = lookAt(pos, lookat, up);
	projection = glm::perspective(fov, 16 / 9.f, 0.1f, 1000.f);
	worldTransform = inverse(view);
	p_window = currentWindow;
}

Camera::~Camera()
{
}

mat4 Camera::GetProjection()
{
	return projection;
}

mat4 Camera::GetView()
{
	return view;
}
mat4 Camera::MoveCamera()
{
	return mat4(0);
}
mat4 Camera::GetProjectionView()
{
	return projection*view;
}
void Camera::Update(float deltaTime)
{	
	if (glfwGetKey(p_window, GLFW_KEY_RIGHT))
	{
		worldTransform = glm::translate(worldTransform, (vec3(50, 0, 0)*deltaTime));
	}
	if (glfwGetKey(p_window, GLFW_KEY_LEFT))
	{
		worldTransform = glm::translate(worldTransform, (vec3(-50, 0, 0)*deltaTime));
	}
	if (glfwGetKey(p_window, GLFW_KEY_UP))
	{
		worldTransform = glm::translate(worldTransform, (vec3(0, 50, 0)*deltaTime));
	}
	if (glfwGetKey(p_window, GLFW_KEY_DOWN))
	{
		worldTransform = glm::translate(worldTransform, (vec3(0, -50, 0)*deltaTime));
	}
	if (glfwGetKey(p_window, GLFW_KEY_S))
	{
		worldTransform = glm::translate(worldTransform, (vec3(0, 0, 50)*deltaTime));
	}
	if (glfwGetKey(p_window, GLFW_KEY_W))
	{
		worldTransform = glm::translate(worldTransform, (vec3(0, 0, -50)*deltaTime));
	}
	if (glfwGetKey(p_window, GLFW_KEY_A))
	{
		worldTransform = glm::rotate(worldTransform, 0.4f*deltaTime, glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(p_window, GLFW_KEY_D))
	{
		worldTransform = glm::rotate(worldTransform, -0.4f*deltaTime, glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(p_window, GLFW_KEY_Q))
	{
		worldTransform = glm::rotate(worldTransform, 0.4f*deltaTime, glm::vec3(1, 0, 0));
	}
	if (glfwGetKey(p_window, GLFW_KEY_E))
	{
		worldTransform = glm::rotate(worldTransform, -0.4f*deltaTime, glm::vec3(1, 0, 0));
	}
	if (glfwGetKey(p_window, GLFW_KEY_Z))
	{
		worldTransform = glm::rotate(worldTransform, 0.4f*deltaTime, glm::vec3(0, 0, 1));
	}
	if (glfwGetKey(p_window, GLFW_KEY_C))
	{
		worldTransform = glm::rotate(worldTransform, -0.4f*deltaTime, glm::vec3(0, 0, 1));
	}
	viewTransform = inverse(worldTransform);
	view = viewTransform;
}
