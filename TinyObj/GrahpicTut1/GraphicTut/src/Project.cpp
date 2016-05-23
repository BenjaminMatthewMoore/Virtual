#include "Project.h"



Project::Project()
{
}


Project::~Project()
{
}

void Project::Create()
{
	windowView = new Window;
	windowView->CreateWindowView();

	
}

void Project::CreateShader(std::string vertShader, std::string fragShader, unsigned int programID)
{
	

}