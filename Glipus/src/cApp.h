#pragma once

#include <wx/wx.h>
#include <wx/event.h>

#include "Updater.h"
#include "cMain.h"

class cApp: public wxApp {
private:
	cMain *frame;
	bool OnInit();

	std::string SetupFile;

public:
	void Update();
	void ClearUpdate();

protected:
	void onCloseUpdateWindow(wxCloseEvent &e);
};