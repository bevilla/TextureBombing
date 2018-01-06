#include <functional>
#include <iostream>
#include <thread>

#include <Windows.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MyGlWindow.h"

#include "Plane.h"
#include "Cube.h"
#include "TextureBombingShader.h"

static float DEFAULT_VIEW_POINT[3] = { 5, 5, 5 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

MyGlWindow::MyGlWindow(int x, int y, int w, int h) :
	Fl_Gl_Window(x, y, w, h),
	_isInitialized(false),
	_gui(this, light, _objs)
{
	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);
	float aspect = w / (float)h;

	_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	_gui.init();
}

MyGlWindow::~MyGlWindow()
{
	delete _viewer;
}

void MyGlWindow::draw(void)
{
	glClearColor(0.2f, 0.2f, .2f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	init();

	glViewport(0, 0, w(), h());

	glm::vec3 eye = _viewer->getViewPoint();
	glm::vec3 look = _viewer->getViewCenter();
	glm::vec3 up = _viewer->getUpVector();

	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*w() / h(), 0.1f, 500.0f);

	for (auto &obj : _objs)
	{
		const glm::vec3 &position = obj->getPosition();
		const glm::vec3 &scale = obj->getScale();
		const glm::vec3 &rotation = obj->getRotation();

		_model.glPushMatrix();
		_model.glTranslate(position.x, position.y, position.z);
		_model.glRotate(rotation.x, 1, 0, 0);
		_model.glRotate(rotation.y, 0, 1, 0);
		_model.glRotate(rotation.z, 0, 0, 1);
		_model.glScale(scale.x, scale.y, scale.z);
		obj->draw(_model.getMatrix(), view, projection, light);
		_model.glPopMatrix();
	}
}

bool MyGlWindow::init()
{
	if (!_isInitialized) {
		glewExperimental = TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			//Problem: glewInit failed, something is seriously wrong.
			std::cout << "glewInit failed, aborting." << std::endl;
			return false;
		}
		_isInitialized = true;

		Plane *plane = new Plane();
		plane->setScale(glm::vec3(10, 1, 10));
		_objs.push_back(plane);
		Cube *cube = new Cube();
		cube->move(glm::vec3(0, 1, 0));
		_objs.push_back(cube);

		_mainObject = NULL;
		_gui.update();
	}
	return true;
}

int MyGlWindow::handle(int e)
{
	static int m_pressedMouseButton = 0;
	static int m_lastMouseX = 0;
	static int m_lastMouseY = 0;

	switch (e) {
	case FL_SHOW:		// you must handle this, or not be seen!
		show();
		return 1;
	case FL_PUSH:
	{

		m_pressedMouseButton = Fl::event_button();

		//	  m_viewer->setAspectRatio( static_cast<double>(this->w()) / static_cast<double>(this->h()) );

		m_lastMouseX = Fl::event_x();
		m_lastMouseY = Fl::event_y();
	}
	damage(1);
	return 1;
	case FL_RELEASE:
		m_pressedMouseButton = -1;
		damage(1);
		return 1;
	case FL_DRAG: // if the user drags the mouse
	{

		float fractionChangeX = (Fl::event_x() - m_lastMouseX) / static_cast<float>(this->w());
		float fractionChangeY = (m_lastMouseY - Fl::event_y()) / static_cast<float>(this->h());

		if (m_pressedMouseButton == 1) {
			_viewer->rotate(fractionChangeX, fractionChangeY);
		}
		else if (m_pressedMouseButton == 2) {
			_viewer->zoom(fractionChangeY);
		}
		else if (m_pressedMouseButton == 3) {
			_viewer->translate(-fractionChangeX, -fractionChangeY, (Fl::event_key(FL_Shift_L) == 0) || (Fl::event_key(FL_Shift_R) == 0));
		}
		else {
			std::cout << "Warning: dragging with unknown mouse button!  Nothing will be done" << std::endl;
		}

		m_lastMouseX = Fl::event_x();
		m_lastMouseY = Fl::event_y();
		redraw();
	}

	return 1;

	case FL_KEYBOARD:
		return 0;

	default:
		return 0;
	}
}
