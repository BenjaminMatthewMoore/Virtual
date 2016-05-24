#include "Application.h"
int main()
{
	Application* app = new Application;
	app->Startup();


	//staff->LoadObject("./data/Models/soulspear/soulspear.obj", "./data/Models/soulspear/soulspear_diffuse.tga");
	//dragon->LoadObject("./data/Models/Dragon/Dragon.obj");




	

	while (glfwWindowShouldClose(app->windowView->m_window) == false &&
		glfwGetKey(app->windowView->m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		app->Update();
		app->Draw();
	}
	
	app->Shutdown();
	return 0;
}