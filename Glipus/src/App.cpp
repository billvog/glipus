#include "App.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit() {
	UpdateDlg *updater = new UpdateDlg();

	if (this->argv.GetArguments().Index("-clear-update") != wxNOT_FOUND) {
		updater->ClearUpdate();
		exit(0);
	}

	updater->StartUpdate();

    frame = new Main(this->argc, this->argv.GetArguments());
	frame->SetIcon(wxIcon("IDI_ICON1"));
	frame->Show();

	updater->ClearUpdate();

	return true;
}