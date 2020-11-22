#pragma once

#include <wx/wx.h>

#include "Main.h"

class cApp: public wxApp {
private:
	Main *frame;

	bool OnInit();
};