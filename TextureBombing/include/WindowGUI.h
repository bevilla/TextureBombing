#pragma once

#include <vector>

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Check_Button.H>

#include "Object3D.h"
#include "Light.h"

#include "ShaderEditGUI.h"

#include "Model3D.h"
#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"
#include "Torus.h"

#define FLOAT_TO_CROP_STRING(n) (std::to_string(n).substr(0, std::to_string(n).find('.') + 3))

class WindowGUI
{
private:
	struct Vector3GUI
	{
		Vector3GUI(Fl_Input *a, Fl_Input *b, Fl_Input *c) : a(a), b(b), c(c) {}
		Fl_Input *a;
		Fl_Input *b;
		Fl_Input *c;
	};

	Light &_light;
	std::vector<Object3D *> &_objs;
	Object3D *_selectedObject;
	int _selectedObjectIndex;

	std::vector<Fl_Widget *> _widgets;
	Fl_File_Chooser	_fileChooser;
	Fl_Tabs		*_tabs;
	Fl_Group	*_objectsGroup;
	Fl_Group	*_lightGroup;

	// Objects Widgets
	Fl_Hold_Browser	*_objBrowser;
	Fl_Input	*_objName;
	Vector3GUI	*_objPosition;
	Vector3GUI	*_objRotation;
	Vector3GUI	*_objScale;

	ShaderEditGUI	_shaderGUI;

public:
	static Fl_Window	*win;

	WindowGUI(Fl_Window *win, Light &light, std::vector<Object3D *> &objs);
	~WindowGUI();

	void init();
	void update();

private:
	void initObjectsWidgets();
	void updateObjectsWidgets();
	void initLightWidgets();

	Vector3GUI *addVector3(int x, int y, const char *label, const char *label1, const char *label2, const char *label3, glm::vec3 *vec);
	void updateVector3(Vector3GUI *vec3gui, glm::vec3 *vec);

	static void addVectorCallback(Fl_Widget *widget, void *param);
	static void addObjectCallback(Fl_Widget *widget, void *param);

	template <typename T>
	T *addWidget(T *widget)
	{
		_widgets.push_back(widget);
		return widget;
	}
};

