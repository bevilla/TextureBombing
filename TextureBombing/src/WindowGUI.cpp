#include <iostream>
#include <sstream>
#include <string>

#include "WindowGUI.h"

Fl_Window *WindowGUI::win = NULL;

WindowGUI::WindowGUI(Fl_Window *win, Light &light, std::vector<Object3D *> &objs) :
	_light(light),
	_objs(objs),
	_selectedObject(NULL),
	_fileChooser(".", "*", 0, "Select a file"),
	_tabs(NULL),
	_objectsGroup(NULL),
	_lightGroup(NULL),
	_shaderGUI(535)
{
	WindowGUI::win = win;
}


WindowGUI::~WindowGUI()
{
	delete _tabs;
}

void WindowGUI::init()
{
	_tabs = addWidget(new Fl_Tabs(700, 10, 310, 680));

	// Objects tab GUI init
	{
		_objectsGroup = addWidget(new Fl_Group(700, 40, 310, 650, "Objects"));

		Fl_Group *kek = addWidget(new Fl_Scroll(703, 42, 304, 647));
		initObjectsWidgets();
		kek->end();

		_objectsGroup->end();
	}

	// Light tab GUI init
	{
		_lightGroup = addWidget(new Fl_Group(700, 40, 310, 650, "Light"));

		initLightWidgets();

		_lightGroup->end();
	}

	_tabs->end();
}

void WindowGUI::update()
{
	updateObjectsWidgets();
	if (_selectedObject)
		_objBrowser->select(_selectedObjectIndex + 1);
}

void WindowGUI::initObjectsWidgets()
{
	Object3D *obj = _selectedObject;
	int y = 65;

	Fl_Box *addObjectFrame = addWidget(new Fl_Box(710, y, 270, 110));
	addObjectFrame->box(FL_BORDER_FRAME);
	addObjectFrame->color(FL_DARK2);
	addObjectFrame->align(FL_ALIGN_TOP_LEFT);
	addObjectFrame->label("Add object");
	_fileChooser.callback([](Fl_File_Chooser *fileChooser, void *param) {
		WindowGUI *gui = (WindowGUI *)param;

		if (!fileChooser->shown())
		{
			Model3D *model = new Model3D(fileChooser->value());

			if (model->isLoaded())
			{
				gui->_objs.push_back(model);
				gui->update();
				gui->win->redraw();
			}
			else
			{
				fl_alert("Invalid object file");
				delete model;
			}
		}
	}, this);
	addWidget(new Fl_Button(715, y + 5, 260, 30, "Load 3D model"))->callback(&addObjectCallback, this);
	addWidget(new Fl_Button(715, y + 40, 128, 30, "Plane"))->callback(&addObjectCallback, this);
	addWidget(new Fl_Button(715 + 132, y + 40, 128, 30, "Cube"))->callback(&addObjectCallback, this);
	addWidget(new Fl_Button(715, y + 75, 128, 30, "Sphere"))->callback(&addObjectCallback, this);
	addWidget(new Fl_Button(715 + 132, y + 75, 128, 30, "Torus"))->callback(&addObjectCallback, this);

	_objBrowser = addWidget(new Fl_Hold_Browser(710, y += 140, 270, 100));
	_objBrowser->callback([](Fl_Widget *widget, void *param) {
		WindowGUI *gui = (WindowGUI *)param;
		Fl_Hold_Browser *browser = (Fl_Hold_Browser *)widget;
		int index = browser->value();

		if (index > 0 && index <= (int)gui->_objs.size())
		{
			gui->_selectedObject = gui->_objs[index - 1];
			gui->_selectedObjectIndex = index - 1;
			gui->_shaderGUI.setObject(gui->_selectedObject);
		}
		gui->update();
	}, this);
	_objBrowser->align(FL_ALIGN_TOP_LEFT);
	_objBrowser->label("Object list");

	Fl_Button *deleteButton = addWidget(new Fl_Button(710, y += 115, 270, 30, "Delete selected object"));
	deleteButton->callback([](Fl_Widget *widget, void *param) {
		WindowGUI *gui = (WindowGUI *)param;

		if (gui->_selectedObject)
		{
			delete *(gui->_objs.begin() + gui->_selectedObjectIndex);
			gui->_objs.erase(gui->_objs.begin() + gui->_selectedObjectIndex);
			gui->_selectedObject = NULL;
			gui->_shaderGUI.setObject(NULL);
			gui->update();
			gui->win->redraw();
		}
	}, this);
	deleteButton->color(FL_RED);

	_objName = addWidget(new Fl_Input(710, y += 55, 270, 20));
	_objName->callback([](Fl_Widget *widget, void *param) {
		WindowGUI *gui = (WindowGUI *)param;
		const char *name = ((Fl_Input *)widget)->value();

		if (gui->_selectedObject)
			gui->_selectedObject->setName(name);
		gui->update();
	}, this);
	_objName->align(FL_ALIGN_TOP_LEFT);
	_objName->label("Name");

	Fl_Box *transformFrame = addWidget(new Fl_Box(710, y += 45, 270, 90));
	transformFrame->box(FL_BORDER_FRAME);
	transformFrame->color(FL_DARK2);
	transformFrame->align(FL_ALIGN_TOP_LEFT);
	transformFrame->label("Transform");
	_objPosition = addVector3(725, y += 10, "Position", "X", "Y", "Z", obj ? &obj->getPosition() : NULL);
	_objRotation = addVector3(725, y += 25, "Rotation", "X", "Y", "Z", obj ? &obj->getRotation() : NULL);
	_objScale = addVector3(725, y += 25, "Scale", "X", "Y", "Z", obj ? &obj->getScale() : NULL);

	_shaderGUI.init();
}

void WindowGUI::updateObjectsWidgets()
{
	Object3D *obj = _selectedObject;

	_objBrowser->clear();
	for (auto &e : _objs)
		_objBrowser->insert(_objs.size(), e->getName().c_str());

	_objName->value(_selectedObject ? _selectedObject->getName().c_str() : "");

	updateVector3(_objPosition, obj ? &obj->getPosition() : NULL);
	updateVector3(_objRotation, obj ? &obj->getRotation() : NULL);
	updateVector3(_objScale, obj ? &obj->getScale() : NULL);

	_shaderGUI.update();
}

void WindowGUI::initLightWidgets()
{
	_lightGroup->clear();

	addVector3(725, 50, "Position", "X", "Y", "Z", &_light.position);
}

WindowGUI::Vector3GUI *WindowGUI::addVector3(int x, int y, const char *label, const char *label1, const char *label2, const char *label3, glm::vec3 *vec)
{
	addWidget(new Fl_Box(x + 10, y, 20, 20, label));

	Fl_Input *w1 = addWidget(new Fl_Float_Input(x + 70, y, 50, 20, label1));
	Fl_Input *w2 = addWidget(new Fl_Float_Input(x + 70 + 65, y, 50, 20, label2));
	Fl_Input *w3 = addWidget(new Fl_Float_Input(x + 70 + 130, y, 50, 20, label3));

	Vector3GUI *vec3gui = new Vector3GUI(w1, w2, w3);

	updateVector3(vec3gui, vec);

	return vec3gui;
}

void WindowGUI::updateVector3(Vector3GUI *vec3gui, glm::vec3 *vec)
{
	if (vec)
	{
		vec3gui->a->value(FLOAT_TO_CROP_STRING(vec->x).c_str());
		vec3gui->a->callback(addVectorCallback, &vec->x);

		vec3gui->b->value(FLOAT_TO_CROP_STRING(vec->y).c_str());
		vec3gui->b->callback(addVectorCallback, &vec->y);

		vec3gui->c->value(FLOAT_TO_CROP_STRING(vec->z).c_str());
		vec3gui->c->callback(addVectorCallback, &vec->z);
	}
}

void WindowGUI::addVectorCallback(Fl_Widget *widget, void *param)
{
	Fl_Input *i = (Fl_Input *)widget;
	std::istringstream iss(i->value());
	float val;
	iss >> val;
	*((float *)param) = val;
	WindowGUI::win->redraw();
}

void WindowGUI::addObjectCallback(Fl_Widget *widget, void *param)
{
	Fl_Button *button = (Fl_Button *)widget;
	WindowGUI *gui = (WindowGUI *)param;
	std::string name = button->label();

	if (name == "Load 3D model")
		gui->_fileChooser.show();
	if (name == "Plane")
		gui->_objs.push_back(new Plane());
	if (name == "Cube")
		gui->_objs.push_back(new Cube());
	if (name == "Sphere")
		gui->_objs.push_back(new Sphere());
	if (name == "Torus")
		gui->_objs.push_back(new Torus());
	gui->win->redraw();
	gui->update();
}
