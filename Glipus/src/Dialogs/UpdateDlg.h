#pragma once

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/gbsizer.h>

#include <future>

#include "../version.h"

class UpdateDlg : public wxDialog {
public:
	UpdateDlg(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxT("Updater"), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(350, 155), long style = wxDEFAULT_DIALOG_STYLE);
	~UpdateDlg();
	
	static const std::string GetSetupFilePath();
private:
	wxPanel *Panel;
	wxStaticText *m_Message;
	wxStaticText *m_ProgressText;
	wxGauge *m_ProgressBar;
	wxButton *m_CancelButton;

	std::string SetupFile;
	std::string LatestVersion;
public:
	bool StartUpdate();
	static void ClearUpdate();

protected:
	void OnCancelButtonClicked(wxCommandEvent &e);
	void OnClose(wxCloseEvent &e);
};