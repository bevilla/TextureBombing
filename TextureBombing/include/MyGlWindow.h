#pragma once

#include <vector>

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

#include "Model3D.h"
#include "Viewer.h"
#include "ModelView.h"
#include "WindowGUI.h"
#include "Light.h"

class MyGlWindow : public Fl_Gl_Window
{
private:
	WindowGUI _gui;
	bool _isInitialized;
	Viewer *_viewer;
	Model _model;
	Model3D *_mainObject;
	std::vector<Object3D *> _objs;
	Light light;

public:
	MyGlWindow(int x, int y, int w, int h);
	~MyGlWindow();

	bool init();

private:
	void draw();
	int handle(int);
};
