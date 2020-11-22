#pragma once

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/gbsizer.h>
#include <wx/appprogress.h>

#include <future>

#include "../Glip.h"

class EncryptDlg : public wxFrame {
public:
	EncryptDlg(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxT("Please wait..."), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(500, 240), long style = wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxICONIZE | wxRESIZE_BORDER);
	~EncryptDlg();

	void SetInfo(const std::string &path, const std::string &password);
	void Start();

private:
	std::string FilePath, Password, OutputPath;
	bool Encrypt = true;

	std::string WindowTitle;

	std::string *Log = nullptr;
	int *Progress = nullptr;
	int ElapsedTime = 0;

	bool isWorking = false, isCancelled = false, isPaused = false;

	std::string FormatTime(int s);

	void StartTimer();
	void StopTimer();
	void onTimerUpdate(wxTimerEvent &e);

	bool DeleteFileWithEx(const std::string &file);

protected:
	wxPanel *Panel;
	wxStaticText *m_PathLabel;
	wxStaticText *m_TaskText;
	wxStaticText *m_ProgressText;
	wxGauge *m_ProgressBar;
	wxStaticLine *m_seperator;
	wxStaticText *m_TimeLabel;
	wxStaticText *m_ElapseTimeText;
	wxButton *m_PauseButton;
	wxButton *m_CancelButton;

	wxTimer *timer = nullptr;
	wxAppProgressIndicator *progressIndicator;

	void OnPauseClicked(wxCommandEvent &e);
	void OnCancelClicked(wxCommandEvent &e);
	void OnWindowResize(wxSizeEvent &e);
	void OnWindowClose(wxCloseEvent &e);
};