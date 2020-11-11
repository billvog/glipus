#pragma once

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/stdpaths.h>

#include <future>

#include "version.h"

class UpdateDlg : public wxDialog {
public:
	UpdateDlg();
	~UpdateDlg();

private:
	wxStaticText *m_Message;
	wxGauge *m_Gauge;

	std::string LatestVersion;
	std::string SetupFile;

public:
	void StartUpdate();
	void ClearUpdate();

protected:
	void onClose(wxCloseEvent &e);
};