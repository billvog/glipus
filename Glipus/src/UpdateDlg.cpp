#include "UpdateDlg.h"
#include "Updater.h"

namespace Updater {
	std::string DLL GetVersion();
	int DLL DownloadUpdate(const std::string &path);
}

UpdateDlg::UpdateDlg() : wxDialog(nullptr, 10001, "Updater", wxDefaultPosition, wxSize(350, 100), wxDEFAULT_DIALOG_STYLE) {
	LatestVersion = Updater::GetVersion();
	SetupFile = fs::path(wxStandardPaths::Get().GetExecutablePath().ToStdString()).parent_path().string() + "/Setup/Glipus_x64.exe";

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	m_Message = new wxStaticText(this, wxID_ANY, "Downloading the latest version of Glipus... (" + LatestVersion + ")");
	m_Message->Wrap(-1);
	sizer->Add(m_Message, 0, wxALL | wxEXPAND, 10);

	m_Gauge = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_Gauge->Pulse();
	sizer->Add(m_Gauge, 0, wxALL | wxEXPAND, 10);

	this->Connect(10001, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(UpdateDlg::onClose));

	this->SetSizer(sizer);
	this->Layout();
	this->Centre(wxBOTH);
}

UpdateDlg::~UpdateDlg() {}

void UpdateDlg::onClose(wxCloseEvent &e) {
	int response = wxMessageBox("Are you sure you want to cancel downloading?", "Confirm", wxYES_NO | wxICON_QUESTION);
	if (response == wxYES) {
		// Cleans the update files that user stopped from downloading...
		wxExecute(wxStandardPaths::Get().GetExecutablePath().ToStdString() + " -clear-update", wxEXEC_ASYNC);
		exit(0);
	}
	else {
		return;
	}
}

void UpdateDlg::StartUpdate() {
	if (LatestVersion != VER_FILEVERSION_STR) {
		int reponse = wxMessageBox("There is a new version of Glipus available. Install it?", "Updater", wxYES_NO | wxICON_QUESTION);
		if (reponse == wxYES) {
			this->Show();

			wxBeginBusyCursor();

			int s = -1;
			try {
				std::future<int> fut = std::async(Updater::DownloadUpdate, SetupFile);
				while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
					wxYield();

					this->Refresh();
					this->Update();

					std::this_thread::sleep_for(std::chrono::milliseconds(25));
				}

				s = fut.get();
			}
			catch (std::exception &e) {
				s = -2;

				wxMessageBox("ERROR: " + std::string(e.what()), "Updater", wxICON_ERROR);
			}

			this->Disconnect(10001, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(UpdateDlg::onClose));
			this->Close();

			if (s == -2)
				return;

			if (s == 0) {
				wxExecute(SetupFile, wxEXEC_ASYNC);
				exit(0);
			}
			else {
				#ifdef GL_CONFIG_DEBUG
					wxMessageBox("ERROR: " + std::to_string(s), "Updater", wxICON_ERROR);
				#endif
				wxMessageBox("Could't download update, try again later.", "Updater", wxICON_ERROR);
			}
		}
	}
}

void UpdateDlg::ClearUpdate() {
	fs::remove_all(fs::path(SetupFile).parent_path());
}
