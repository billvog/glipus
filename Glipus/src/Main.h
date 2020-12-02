#pragma once

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/gbsizer.h>
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/aboutdlg.h>
#include <wx/filepicker.h>
#include <wx/richtooltip.h>

#include <wx/msw/registry.h>

#include "Glip.h"
#include "Version.h"

#include "Dialogs/EncryptDlg.h"
#include "Dialogs/UpdateDlg.h"

#include "Utils/WeakPwds.h"

class Main : public wxFrame {
public:
	Main(int argc, wxArrayString argv);
	~Main();
protected:
	void OnIdleEvent(wxIdleEvent &e);
	void OnWindowFocus(wxFocusEvent &e);
	void OnClose(wxCloseEvent &e);
public:
	void GetArguments(int argc, wxArrayString argv);
private:
	// Settings
	bool ShowPwdCb = false;

	bool Encrypt = true, ShowEncryptionDlg = false;

	void CreateMenubar();

	void ValidateFields();

	void SetPwdShow(bool value);

	bool DeleteFileWithEx(const std::string &file);

	void LoadSettings();
	void SaveSettings();
protected:
	wxMenuBar *menuBar;
	wxMenuItem *openFileMenu, *quitFileMenu;
	wxMenuItem *keygenToolsMenu;
	wxMenuItem *checkForUpdateHelpMenu, *aboutHelpMenu;
	wxPanel *Panel;
	wxGridBagSizer *Sizer;
	wxStaticText *m_Label1, *m_Label2;
	wxFilePickerCtrl *m_PathField;
	wxTextCtrl *m_PasswordField;
	wxStaticText *m_Label3;
	wxTextCtrl *m_RePasswordField;
	wxCheckBox *m_ShowPasswordToggle;
	wxButton *m_EncryptButton;

	void OnOpenFileClicked(wxCommandEvent &e);
	void OnQuitFileClicked(wxCommandEvent &e);

	void OnGeneratePasswordToolsClicked(wxCommandEvent &e);

	void OnCheckForUpdateHelpMenuClicked(wxCommandEvent &e);
	void OnAboutHelpMenuClicked(wxCommandEvent &e);

	void OnFileDropped(wxDropFilesEvent &e);
	void OnFileChanged(wxFileDirPickerEvent &e);

	void OnPasswordChanged(wxCommandEvent &e);
	void OnPasswordShowToggled(wxCommandEvent &e);

	void OnEncryptButtonClicked(wxCommandEvent &e);
};