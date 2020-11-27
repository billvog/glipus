#pragma once

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/gbsizer.h>
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/aboutdlg.h>
#include <wx/filepicker.h>

#include <wx/msw/registry.h>

#include "Glip.h"
#include "version.h"

#include "Dialogs/UpdateDlg.h"
#include "Dialogs/EncryptDlg.h"

class Main : public wxFrame {
public:
	Main(int argc, wxArrayString argv);
	~Main();

	void GetArguments(int argc, wxArrayString argv);

private:
	wxMenuBar *menuBar;
	wxStaticText *m_Label1, *m_Label2;
	wxFilePickerCtrl *m_PathField;
	wxTextCtrl *m_PasswordField;
	wxButton *m_DoBtn;

	bool Encrypt = true, ShowEncryptionDlg = false;

	void CheckPath();
	bool DeleteFileWithEx(const std::string &file);

	void LoadSettings();
	void SaveSettings();

	void OpenKeygenDialog();

protected:
	wxDECLARE_EVENT_TABLE();

	void OnIdleEvent(wxIdleEvent &e);
	void OnWindowFocus(wxFocusEvent &e);
	void OnClose(wxCloseEvent &e);

	void DoButtonClicked(wxCommandEvent &e);

	void OnOpenFileClicked(wxCommandEvent &e);
	void OnQuitFileClicked(wxCommandEvent &e);

	void OnGeneratePasswordToolsClicked(wxCommandEvent &e);

	void OnFileDropped(wxDropFilesEvent &e);
	void OnFileChanged(wxFileDirPickerEvent &e);

	void OnCheckForUpdateHelpMenuClicked(wxCommandEvent &e);
	void OnAboutHelpMenuClicked(wxCommandEvent &e);
};