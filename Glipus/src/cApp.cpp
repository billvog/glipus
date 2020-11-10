#include "cApp.h"

wxIMPLEMENT_APP(cApp);

namespace Updater {
	std::string DLL GetVersion();
	int DLL DownloadUpdate(const std::string &path);
}

bool cApp::OnInit() {
	SetupFile = fs::path(wxStandardPaths::Get().GetExecutablePath().ToStdString()).parent_path().string() + "/Setup/Glipus_x64.exe";

	if (this->argv.GetArguments().Index("-clear-update") != wxNOT_FOUND) {
		ClearUpdate();
		exit(0);
	}
	
	Update();

    frame = new cMain(this->argc, this->argv.GetArguments());
	frame->SetIcon(wxIcon("IDI_ICON1"));
	frame->Show();

	ClearUpdate();

	return true;
}

void cApp::onCloseUpdateWindow(wxCloseEvent &e) {
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

void cApp::Update() {
	#define UpdaterTitle "Updater"

	std::string LatestVersion = Updater::GetVersion();

	if (LatestVersion != VER_FILEVERSION_STR) {
		int reponse = wxMessageBox("There is a new version of Glipus available. Install it?", UpdaterTitle, wxYES_NO | wxICON_QUESTION);
		if (reponse == wxYES) {
			wxDialog *dialog = new wxDialog(nullptr, 10001, UpdaterTitle, wxDefaultPosition, wxSize(350, 100), wxDEFAULT_DIALOG_STYLE);
			
			wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

			wxStaticText *m_Message = new wxStaticText(dialog, wxID_ANY, "Downloading the latest version of Glipus... (" + LatestVersion + ")");
			m_Message->Wrap(-1);
			sizer->Add(m_Message, 0, wxALL | wxEXPAND, 10);

			wxGauge *m_Gauge = new wxGauge(dialog, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
			m_Gauge->Pulse();
			sizer->Add(m_Gauge, 0, wxALL | wxEXPAND, 10);

			dialog->Connect(10001, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(cApp::onCloseUpdateWindow));

			dialog->SetSizer(sizer);
			dialog->Layout();
			dialog->Centre(wxBOTH);
			dialog->Show();

			wxBeginBusyCursor();
			
			int s = -1;
			try {
				std::future<int> fut = std::async(Updater::DownloadUpdate, SetupFile);
				while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
					wxYield();

					dialog->Refresh();
					dialog->Update();

					std::this_thread::sleep_for(std::chrono::milliseconds(5));
				}

				s = fut.get();
			}
			catch (std::exception &e) {
				s = -2;

				wxMessageBox("ERROR: " + std::string(e.what()), "Updater", wxICON_ERROR);
			}

			dialog->Disconnect(10001, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(cApp::onCloseUpdateWindow));
			dialog->Close();
			
			delete dialog;

			if (s == -2)
				return;

			if (s == 0) {
				wxExecute(SetupFile, wxEXEC_ASYNC);
				exit(0);
			}
			else {
				wxMessageBox("ERROR: " + std::to_string(s), "Updater", wxICON_ERROR);
				wxMessageBox("Could't download update, try again later.", UpdaterTitle, wxICON_ERROR);
			}
		}
	}
}

void cApp::ClearUpdate() {
	fs::remove_all(fs::path(SetupFile).parent_path());
}