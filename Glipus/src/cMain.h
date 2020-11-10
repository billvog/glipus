#pragma once

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/gbsizer.h>
#include <wx/stdpaths.h>
#include <wx/aboutdlg.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/appprogress.h>

#include <future>

#include "Glip.h"
#include "version.h"

class cMain : public wxFrame {
public:
	cMain(int argc, wxArrayString argv);
	~cMain();

private:
	wxMenuBar *menuBar;
	wxStaticText *m_Label1, *m_Label2;
	wxFilePickerCtrl *m_PathField;
	wxTextCtrl *m_PasswordField;
	wxButton *m_DoBtn;
	wxTimer *timer;

	std::string ConfigFile;
	std::string *Progress;
	int ElapsedTime = 0;
	bool bEncrypt = true, isWorking = false, isCancelled = false;

	void CheckPath();

	std::string FormatTime(int s);

	void StartTimer();
	void StopTimer();
	void onTimerUpdate(wxTimerEvent &e);

	void SaveSettings();
	bool DeleteFileWithEx(const std::string &file);

	void OpenKeygenDialog();

	void UpdateControls();

protected:
	wxDECLARE_EVENT_TABLE();

	void onWindowFocus(wxFocusEvent &e);

	void doButtonClicked(wxCommandEvent &e);

	void onGeneratePasswordToolsClicked(wxCommandEvent &e);
	void onOpenFileClicked(wxCommandEvent &e);

	void onFileDropped(wxDropFilesEvent &e);
	void onFileChanged(wxFileDirPickerEvent &e);

	void onQuitFileClicked(wxCommandEvent &e);

	void onAboutHelpMenuClicked(wxCommandEvent &e);

	void onClose(wxCloseEvent &e);
};