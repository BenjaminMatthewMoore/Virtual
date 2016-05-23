#pragma once
struct GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

	int window_width;
	int window_height;

};
