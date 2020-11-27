#include "UpdateDlg.h"
#include "Updater.h"

namespace Updater {
	int DLL Progress;
	bool DLL isCancelled;

	std::string DLL GetVersion();
	int DLL DownloadUpdate(const std::string &path);
}

UpdateDlg::UpdateDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style): wxDialog(parent, id, title, pos, size, style) {
	LatestVersion = Updater::GetVersion();
	SetupFile = GetSetupFilePath();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer *ParentSizer;
	ParentSizer = new wxBoxSizer(wxVERTICAL);

	Panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	
	wxGridBagSizer *Sizer;
	Sizer = new wxGridBagSizer(0, 0);
	Sizer->SetFlexibleDirection(wxBOTH);
	Sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_Message = new wxStaticText(Panel, wxID_ANY, wxT("Downloading the latest version of Glipus..."), wxDefaultPosition, wxDefaultSize, 0);
	m_Message->Wrap(-1);
	Sizer->Add(m_Message, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL | wxEXPAND, 5);

	m_ProgressText = new wxStaticText(Panel, wxID_ANY, wxT("0%"), wxDefaultPosition, wxDefaultSize, 0);
	m_ProgressText->Wrap(-1);
	Sizer->Add(m_ProgressText, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 7);

	m_ProgressBar = new wxGauge(Panel, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH);
	m_ProgressBar->SetValue(0);
	Sizer->Add(m_ProgressBar, wxGBPosition(1, 0), wxGBSpan(1, 2), wxALL | wxEXPAND, 5);

	Sizer->Add(0, 10, wxGBPosition(2, 0), wxGBSpan(1, 1), wxEXPAND, 5);

	m_CancelButton = new wxButton(Panel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	Sizer->Add(m_CancelButton, wxGBPosition(3, 0), wxGBSpan(1, 2), wxALIGN_RIGHT | wxALL, 5);

	Sizer->AddGrowableCol(0);

	Panel->SetSizer(Sizer);
	Panel->Layout();
	Sizer->Fit(Panel);
	ParentSizer->Add(Panel, 1, wxALL | wxEXPAND, 10);

	this->SetSizer(ParentSizer);
	this->Layout();
	this->Centre(wxBOTH);

	m_CancelButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UpdateDlg::OnCancelButtonClicked), NULL, this);
	this->Connect(this->GetId(), wxEVT_CLOSE_WINDOW, wxCloseEventHandler(UpdateDlg::OnClose));
}

UpdateDlg::~UpdateDlg() {
	m_CancelButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UpdateDlg::OnCancelButtonClicked), NULL, this);
	this->Disconnect(this->GetId(), wxEVT_CLOSE_WINDOW, wxCloseEventHandler(UpdateDlg::OnClose));
}

const std::string UpdateDlg::GetSetupFilePath() {
	return fs::path(wxStandardPaths::Get().GetTempDir().ToStdString() + "/glipus-latest-setup.exe").string();
}

void UpdateDlg::OnCancelButtonClicked(wxCommandEvent &e) {
	this->Close();
}

void UpdateDlg::OnClose(wxCloseEvent &e) {
	int response = wxMessageBox("Are you sure you want to cancel downloading?", "Confirm", wxYES_NO | wxICON_QUESTION);
	if (response == wxYES) {
		Updater::isCancelled = true;

		m_Message->SetLabelText("Cancelling download...");
		m_CancelButton->Enable(false);
	}
	else {
		return;
	}
}

bool UpdateDlg::StartUpdate() {
	if (LatestVersion.length() > 0 && LatestVersion != VER_FILEVERSION_STR) {
		int reponse = wxMessageBox("There is a new version of Glipus available. Install it?\n\nInstalled: " + wxString(VER_FILEVERSION_STR) + "\nLatest: " + LatestVersion, "Updater", wxYES_NO | wxICON_QUESTION);
		if (reponse == wxYES) {
			this->Show();

			this->GetParent()->Enable(false);

			wxBeginBusyCursor();

			int Value = -1;
			try {
				std::future<int> fut = std::async(Updater::DownloadUpdate, SetupFile);
				while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
					wxYield();
					
					if (Updater::Progress > 0) {
						m_ProgressBar->SetValue(Updater::Progress);
						m_ProgressText->SetLabelText(std::to_string(Updater::Progress) + "%");
						this->Layout();
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(5));
				}

				Value = fut.get();
			}
			catch (std::exception &e) {
				Value = -2;
				wxMessageBox(std::string(e.what()), "Updater", wxICON_ERROR);
			}

			this->GetParent()->Enable(true);

			wxEndBusyCursor();

			this->Disconnect(this->GetId(), wxEVT_CLOSE_WINDOW, wxCloseEventHandler(UpdateDlg::OnClose));
			this->Close();

			if (Value == -2 || Updater::isCancelled)
				return false;

			if (Value == 0) {
				wxExecute(SetupFile, wxEXEC_ASYNC);
				exit(0);
			}
			// Cancelled
			else if (Value == 23 && Updater::isCancelled) {
				wxExecute(wxStandardPaths::Get().GetExecutablePath().ToStdString() + " -clear-update", wxEXEC_ASYNC);
			}
			else {
				#ifdef GL_CONFIG_DEBUG
					wxMessageBox("ERROR: " + std::to_string(Value), "Updater", wxICON_ERROR);
				#endif

				wxMessageBox("Could't download update, try again later.", "Updater", wxICON_ERROR);
			}
		}
	}
	else {
		wxMessageBox("You have the latest version of Glipus installed.\n\nInstalled: " + wxString(VER_FILEVERSION_STR), "Updater", wxICON_INFORMATION);
	}
	
	return false;
}

void UpdateDlg::ClearUpdate() {
	fs::remove(GetSetupFilePath());
}
