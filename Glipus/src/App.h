#pragma once

#include <wx/wx.h>

#include "UpdateDlg.h"
#include "Main.h"

class cApp: public wxApp {
private:
	Main *frame;
	bool OnInit();
};