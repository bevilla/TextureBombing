#include <FL/Fl_Double_Window.H>

#include "MyGlWindow.h"

int main()
{
	Fl::scheme("plastic");// plastic
	int width = 1020;
	int height = 700;
	Fl_Double_Window* wind = new Fl_Double_Window(100, 100, width, height, "Texture Bombing Project");

	wind->begin();		// put widgets inside of the window
	MyGlWindow* gl = new MyGlWindow(10, 10, width - 340, height - 20);

	wind->resizable(gl);
	wind->show();	// this actually opens the window


	Fl::run();
	delete wind;

	return 0;
}