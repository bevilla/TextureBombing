#pragma once

#include <FL/Fl_Round_Button.H>

#include "Object3D.h"

#include "BlinnPhongShader.h"
#include "CheckerboardShader.h"
#include "TextureBombingShader.h"
#include "CellShadingShader.h"
#include "Model3DShader.h"

#define EDIT_EVERY_SHADER_PARAM(classType, varName, value) \
if (gui->_obj->isModel3D()) \
{ \
	for (auto &mesh : ((Model3D *)gui->_obj)->getMeshes()) \
		((classType *)mesh->getShader())->varName = value; \
} \
else \
{ \
	if (dynamic_cast<classType *>(gui->_obj->getShader())) \
	{ \
		((classType *)gui->_obj->getShader())->varName = value; \
	} \
}
#define EDIT_COLOR_CALLBACK(classType, varName) \
[](Fl_Widget *widget, void *param) { \
	Fl_Color_Chooser *color = (Fl_Color_Chooser *)widget; \
	ShaderEditGUI *gui = (ShaderEditGUI *)param; \
	if (gui->_obj == NULL) \
		return ; \
	EDIT_EVERY_SHADER_PARAM(classType, varName, glm::vec3(color->r(), color->g(), color->b())); \
	gui->update(); \
	WindowGUI::win->redraw(); \
}
#define CREATE_SLIDER(classType, varName, _x, _y, _w, _h, _min, _max, _precision, _name) \
{ \
	Fl_Value_Slider *slider = new Fl_Value_Slider(_x, _y, _w, _h); \
	slider->type(FL_HORIZONTAL); \
	slider->minimum(_min); \
	slider->maximum(_max); \
	slider->precision(_precision); \
	slider->align(FL_ALIGN_TOP); \
	slider->label(_name); \
	slider->callback([](Fl_Widget *widget, void *param) { \
		Fl_Value_Slider *slider = (Fl_Value_Slider *)widget; \
		ShaderEditGUI *gui = (ShaderEditGUI *)param; \
		if (gui->_obj == NULL) \
			return; \
		EDIT_EVERY_SHADER_PARAM(classType, varName, (float)slider->value()); \
		gui->update(); \
		WindowGUI::win->redraw(); \
	}, this); \
}

class ShaderEditGUI
{
private:
	int			_yoffset;
	Object3D	*_obj;

	Fl_Button	*_selectShaderButtons[5];
	Fl_Group	*_shaderGUI[5];

	Fl_Round_Button *_textureBombingRoundButtons[3];
	Fl_File_Chooser	_textureBombingFileChooser;
	Fl_Box			*_textureBombingTextureName;

public:
	ShaderEditGUI(int yoffset);
	~ShaderEditGUI();

	void setObject(Object3D *obj);

	void init();
	void update();

private:
	void initTextureBombingGUI();
	void initBlinnPhongGUI();
	void initCheckerboardGUI();
	void initCellShadingGUI();
	void initModel3DGUI();

	Fl_Button *getSelectShaderButton(Object3D *obj);
	Fl_Button *getSelectShaderButton(Shader *shader);
	Fl_Group *getEditShaderGroup(Object3D *obj);

	Shader *createShader(const std::string &name);

	static void selectShaderCallback(Fl_Widget *widget, void *param);
};

