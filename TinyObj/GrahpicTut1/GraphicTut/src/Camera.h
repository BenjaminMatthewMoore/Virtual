#ifndef CAMERA_H
#define CAMERA_H
#include "gl_core_4_4.h"
//#include <cstdio>
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/transform.hpp"
using namespace glm;

class Camera
{
public:
	Camera(vec3 pos, vec3 lookat, vec3 up, float fov, GLFWwindow* currentWindow);
	~Camera();

	mat4 GetView();
	mat4 GetProjection();
	mat4 MoveCamera();
	mat4 GetProjectionView();
	void Update(float deltaTime);

private:
	mat4 view;
	mat4 projection;
	mat4 worldTransform;
	mat4 viewTransform;
	GLFWwindow* p_window;

	
};



#endif
