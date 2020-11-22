#include "App.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit() {
	// Initialize random
	srand(time(NULL));

	frame = new Main(this->argc, this->argv.GetArguments());

	return true;
}