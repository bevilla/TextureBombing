#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Value_Slider.H>

#include "WindowGUI.h"
#include "ShaderEditGUI.h"


ShaderEditGUI::ShaderEditGUI(int yoffset) :
	_yoffset(yoffset),
	_obj(NULL),
	_textureBombingFileChooser(".", "*", 0, "Select a file")
{
	_textureBombingFileChooser.callback([](Fl_File_Chooser *fileChooser, void *param) {
		ShaderEditGUI *gui = (ShaderEditGUI *)param;
		Object3D *obj = gui->_obj;

		if (!fileChooser->shown())
		{
			if (gui->_obj->isModel3D())
			{
				for (auto &mesh : ((Model3D *)gui->_obj)->getMeshes())
					((TextureBombingShader *)mesh->getShader())->loadTexture(fileChooser->value());
			}
			else
			{
				if (dynamic_cast<TextureBombingShader *>(gui->_obj->getShader()))
				{
					((TextureBombingShader *)gui->_obj->getShader())->loadTexture(fileChooser->value());
				}
			}
			gui->_textureBombingTextureName->label((new std::string(std::string(fileChooser->value()).substr(std::string(fileChooser->value()).length() - 30)))->c_str());
			gui->update();
			WindowGUI::win->redraw();
		}
	}, this);
}


ShaderEditGUI::~ShaderEditGUI()
{
}


void ShaderEditGUI::setObject(Object3D *obj)
{
	_obj = obj;
	update();
}

void ShaderEditGUI::init()
{
	int y = _yoffset;

	Fl_Box *addObjectFrame = new Fl_Box(710, y, 270, 110);
	addObjectFrame->box(FL_BORDER_FRAME);
	addObjectFrame->color(FL_DARK2);
	addObjectFrame->align(FL_ALIGN_TOP_LEFT);
	addObjectFrame->label("Shader");
	_selectShaderButtons[0] = new Fl_Button(715, y + 5, 260, 30, "Texture bombing");
	_selectShaderButtons[1] = new Fl_Button(715, y + 40, 128, 30, "Blinn Phong");
	_selectShaderButtons[2] = new Fl_Button(715 + 132, y + 40, 128, 30, "Checkerboard");
	_selectShaderButtons[3] = new Fl_Button(715, y + 75, 128, 30, "Cell shading");
	_selectShaderButtons[4] = new Fl_Button(715 + 132, y + 75, 128, 30, "3D Model");
	for (int i = 0; i < 5; i++)
	{
		_selectShaderButtons[i]->callback(&selectShaderCallback, this);
		_shaderGUI[i] = NULL;
	}

	_yoffset = y + 140;

	initTextureBombingGUI();
	initBlinnPhongGUI();
	initCheckerboardGUI();
	initCellShadingGUI();
	initModel3DGUI();
}

void ShaderEditGUI::update()
{
	for (int i = 0; i < 5; i++)
	{
		Fl_Group *selectShaderGroup = _obj ? getEditShaderGroup(_obj) : NULL;

		_selectShaderButtons[i]->color(FL_WHITE);
		_selectShaderButtons[i]->redraw();
		if (_shaderGUI[i] && _shaderGUI[i] != selectShaderGroup)
			_shaderGUI[i]->hide();
	}
	if (_obj)
	{
		Fl_Button *selectShaderButton = getSelectShaderButton(_obj);
		Fl_Group *selectShaderGroup = getEditShaderGroup(_obj);

		if (selectShaderButton)
		{
			selectShaderButton->color(FL_BLACK);
			selectShaderButton->redraw();
		}
		if (selectShaderGroup)
		{
			selectShaderGroup->show();
		}
	}
}

void ShaderEditGUI::initTextureBombingGUI()
{
	int y = _yoffset;
	Fl_Group *group = new Fl_Group(710, y, 270, 650);

	_shaderGUI[0] = group;
	group->box(FL_BORDER_FRAME);
	group->color(FL_DARK2);
	group->align(FL_ALIGN_TOP_LEFT);
	group->label("Texture bombing");

	(new Fl_Button(720, y += 10, 250, 30, "Generate Random"))->callback([](Fl_Widget *widget, void *param) {
		((TextureBombingShader *)((ShaderEditGUI *)param)->_obj->getShader())->randomGenerate();
		WindowGUI::win->redraw();

		ShaderEditGUI *gui = (ShaderEditGUI *)param;
		if (gui->_obj->isModel3D())
		{
			for (auto &mesh : ((Model3D *)gui->_obj)->getMeshes())
				((TextureBombingShader *)mesh->getShader())->randomGenerate();
		}
		else
		{
			if (dynamic_cast<TextureBombingShader *>(gui->_obj->getShader()))
			{
				((TextureBombingShader *)gui->_obj->getShader())->randomGenerate();
			}
		}
		gui->update();
		WindowGUI::win->redraw();
	}, this);
	(new Fl_Color_Chooser(720, y += 55, 250, 100, "Background"))->callback(EDIT_COLOR_CALLBACK(TextureBombingShader, kd), this);
	(new Fl_Color_Chooser(720, y += 125, 250, 100, "Specular"))->callback(EDIT_COLOR_CALLBACK(TextureBombingShader, ks), this);
	CREATE_SLIDER(TextureBombingShader, kaRatio, 720, y += 125, 250, 20, 0, 1, 2, "Ambient ratio");
	CREATE_SLIDER(TextureBombingShader, shininess, 720, y += 40, 250, 20, 0, 200, 2, "Shininess");
	_textureBombingRoundButtons[0] = new Fl_Round_Button(720, y += 30, 250, 20, "PNG File");
	_textureBombingRoundButtons[0]->value(1);
	_textureBombingRoundButtons[1] = new Fl_Round_Button(720, y += 20, 250, 20, "Circles");
	_textureBombingRoundButtons[2] = new Fl_Round_Button(720, y += 20, 250, 20, "Veronoi");
	for (int i = 0; i < 3; i++)
	{
		_textureBombingRoundButtons[i]->callback([](Fl_Widget *widget, void *param) {
			Fl_Round_Button *button = (Fl_Round_Button *)widget;
			ShaderEditGUI *gui = (ShaderEditGUI *)param;
			TextureBombingShader::TextureType type = TextureBombingShader::PNG;

			if (button->label() == "PNG File")
				type = TextureBombingShader::PNG;
			if (button->label() == "Circles")
				type = TextureBombingShader::CIRCLE;
			if (button->label() == "Veronoi")
				type = TextureBombingShader::VERONOI;

			EDIT_EVERY_SHADER_PARAM(TextureBombingShader, textureType, type);

			for (int i = 0; i < 3; i++)
				gui->_textureBombingRoundButtons[i]->value(0);
			button->value(1);
			gui->update();
			WindowGUI::win->redraw();
		}, this);
	}
	(new Fl_Button(720, y += 30, 250, 30, "Load texture..."))->callback([](Fl_Widget *widget, void *param) {
		((ShaderEditGUI *)param)->_textureBombingFileChooser.show();
	}, this);
	_textureBombingTextureName = new Fl_Box(720, y += 30, 250, 18, "./textures/multi-tex.png");
	CREATE_SLIDER(TextureBombingShader, numberOfTexture, 720, y += 40, 250, 20, 0, 16, 0, "Number of textures");
	CREATE_SLIDER(TextureBombingShader, scale, 720, y += 40, 250, 20, 0, 100, 2, "Scale");
	CREATE_SLIDER(TextureBombingShader, density, 720, y += 40, 250, 20, 0, 1, 2, "Density");

	group->end();
}

void ShaderEditGUI::initBlinnPhongGUI()
{
	int y = _yoffset;
	Fl_Group *group = new Fl_Group(710, y, 270, 440);

	_shaderGUI[1] = group;
	group->box(FL_BORDER_FRAME);
	group->color(FL_DARK2);
	group->align(FL_ALIGN_TOP_LEFT);
	group->label("Blinn Phong");

	(new Fl_Color_Chooser(720, y += 20, 250, 100, "Diffuse"))->callback(EDIT_COLOR_CALLBACK(BlinnPhongShader, kd), this);
	(new Fl_Color_Chooser(720, y += 130, 250, 100, "Ambient"))->callback(EDIT_COLOR_CALLBACK(BlinnPhongShader, ka), this);
	(new Fl_Color_Chooser(720, y += 130, 250, 100, "Specular"))->callback(EDIT_COLOR_CALLBACK(BlinnPhongShader, ks), this);
	CREATE_SLIDER(BlinnPhongShader, shininess, 720, y += 125, 250, 20, 0, 200, 2, "Shininess");

	group->end();
}

void ShaderEditGUI::initCheckerboardGUI()
{
	int y = _yoffset;
	Fl_Group *group = new Fl_Group(710, y, 270, 530);

	_shaderGUI[2] = group;
	group->box(FL_BORDER_FRAME);
	group->color(FL_DARK2);
	group->align(FL_ALIGN_TOP_LEFT);
	group->label("Checkerboard");

	CREATE_SLIDER(CheckerboardShader, scale, 720, y += 25, 250, 20, 0, 50, 0, "Scale");

	(new Fl_Color_Chooser(720, y += 45, 250, 100, "Color 1"))->callback(EDIT_COLOR_CALLBACK(CheckerboardShader, color1), this);
	(new Fl_Color_Chooser(720, y += 130, 250, 100, "Color 2"))->callback(EDIT_COLOR_CALLBACK(CheckerboardShader, color2), this);
	(new Fl_Color_Chooser(720, y += 130, 250, 100, "Specular"))->callback(EDIT_COLOR_CALLBACK(CheckerboardShader, ks), this);
	CREATE_SLIDER(CheckerboardShader, kaRatio, 720, y += 125, 250, 20, 0, 1, 2, "Ambient ratio");
	CREATE_SLIDER(CheckerboardShader, shininess, 720, y += 45, 250, 20, 0, 200, 2, "Shininess");

	group->end();
}

void ShaderEditGUI::initCellShadingGUI()
{
	int y = _yoffset;
	Fl_Group *group = new Fl_Group(710, y, 270, 480);

	_shaderGUI[3] = group;
	group->box(FL_BORDER_FRAME);
	group->color(FL_DARK2);
	group->align(FL_ALIGN_TOP_LEFT);
	group->label("Cell shading");

	(new Fl_Color_Chooser(720, y += 20, 250, 100, "Diffuse"))->callback(EDIT_COLOR_CALLBACK(CellShadingShader, kd), this);
	(new Fl_Color_Chooser(720, y += 130, 250, 100, "Ambient"))->callback(EDIT_COLOR_CALLBACK(CellShadingShader, ka), this);
	CREATE_SLIDER(CellShadingShader, level, 720, y += 120, 250, 20, 0, 10, 0, "Level");
	(new Fl_Color_Chooser(720, y += 50, 250, 100, "Silouhette"))->callback(EDIT_COLOR_CALLBACK(CellShadingShader, silouhetteColor), this);
	CREATE_SLIDER(CellShadingShader, silouhetteSize, 720, y += 120, 250, 20, 0, 1, 3, "Silouhette size");

	group->end();
}

void ShaderEditGUI::initModel3DGUI()
{
	int y = _yoffset;
	Fl_Group *group = new Fl_Group(710, y, 270, 50);

	_shaderGUI[4] = group;
	group->box(FL_BORDER_FRAME);
	group->color(FL_DARK2);
	group->align(FL_ALIGN_TOP_LEFT);
	group->label("3D Model");

	Fl_Check_Button *textureButton = new Fl_Check_Button(720, y += 5, 250, 20, "Enable texture map");
	textureButton->value(1);
	textureButton->callback([](Fl_Widget *widget, void *param) {
		Fl_Check_Button *button = (Fl_Check_Button *)widget;
		ShaderEditGUI *gui = (ShaderEditGUI *)param;
		if (gui->_obj == NULL)
			return;
		if (gui->_obj->isModel3DShader() && gui->_obj->isModel3D())
			((Model3D *)gui->_obj)->enableTexture(button->value() == 1);
		gui->update();
		WindowGUI::win->redraw();
	}, this);
	Fl_Check_Button *specularButton = new Fl_Check_Button(720, y += 20, 250, 20, "Enable specular map");
	specularButton->value(1);
	specularButton->callback([](Fl_Widget *widget, void *param) {
		Fl_Check_Button *button = (Fl_Check_Button *)widget;
		ShaderEditGUI *gui = (ShaderEditGUI *)param;
		if (gui->_obj == NULL)
			return;
		if (gui->_obj->isModel3DShader() && gui->_obj->isModel3D())
			((Model3D *)gui->_obj)->enableSpecular(button->value() == 1);
		gui->update();
		WindowGUI::win->redraw();
	}, this);

	group->end();
}

Fl_Button *ShaderEditGUI::getSelectShaderButton(Shader *shader)
{
	if (dynamic_cast<TextureBombingShader *>(shader))
		return _selectShaderButtons[0];
	if (dynamic_cast<BlinnPhongShader *>(shader))
		return _selectShaderButtons[1];
	if (dynamic_cast<CheckerboardShader *>(shader))
		return _selectShaderButtons[2];
	if (dynamic_cast<CellShadingShader *>(shader))
		return _selectShaderButtons[3];
	if (dynamic_cast<Model3DShader *>(shader))
		return _selectShaderButtons[4];
	return NULL;
}

Fl_Button *ShaderEditGUI::getSelectShaderButton(Object3D *obj)
{
	return getSelectShaderButton(_obj->getShader());
}

Fl_Group *ShaderEditGUI::getEditShaderGroup(Object3D *obj)
{
	if (obj)
	{
		Shader *shader = obj->getShader();

		if (shader)
		{
			if (dynamic_cast<TextureBombingShader *>(shader))
				return _shaderGUI[0];
			if (dynamic_cast<BlinnPhongShader *>(shader))
				return _shaderGUI[1];
			if (dynamic_cast<CheckerboardShader *>(shader))
				return _shaderGUI[2];
			if (dynamic_cast<CellShadingShader *>(shader))
				return _shaderGUI[3];
			if (dynamic_cast<Model3DShader *>(shader))
				return _shaderGUI[4];
		}
	}
	return NULL;
}

Shader *ShaderEditGUI::createShader(const std::string &name)
{
	if (name == "Texture bombing")
	{
		TextureBombingShader *shader = new TextureBombingShader();
		shader->loadTexture("./textures/multi-tex.png");
		shader->numberOfTexture = 4;
		return shader;
	}
	if (name == "Blinn Phong")
		return new BlinnPhongShader();
	if (name == "Checkerboard")
		return new CheckerboardShader();
	if (name == "Cell shading")
		return new CellShadingShader();
	if (name == "3D Model")
		return new Model3DShader();
	return NULL;
}

void ShaderEditGUI::selectShaderCallback(Fl_Widget *widget, void *param)
{
	Fl_Button *button = (Fl_Button *)widget;
	ShaderEditGUI *gui = (ShaderEditGUI *)param;
	Object3D *obj = gui->_obj;

	if (obj)
	{
		Shader *shader = gui->createShader(button->label());
		if (shader)
		{
			obj->setShader(shader);
		}
	}
	gui->update();
	WindowGUI::win->redraw();
}