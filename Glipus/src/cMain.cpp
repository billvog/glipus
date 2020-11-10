#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_MENU(10000, cMain::onOpenFileClicked)
EVT_MENU(10001, cMain::onQuitFileClicked)
EVT_MENU(10002, cMain::onGeneratePasswordToolsClicked)
EVT_MENU(10003, cMain::onAboutHelpMenuClicked)
EVT_CLOSE(cMain::onClose)
wxEND_EVENT_TABLE()

cMain::cMain(int argc, wxArrayString argv) : wxFrame(nullptr, wxID_ANY, wxT("Glipus")) {
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	int HEIGHT = 210;

	ConfigFile = wxStandardPaths::Get().GetUserConfigDir() + "\\BILLVOG\\Glipus\\config.ini";
	std::fstream config(ConfigFile, std::ios::in);
	if (config.is_open()) {
		int position_x = 0, position_y = 0;
		int size_x = 0;
		
		std::string buffer((std::istreambuf_iterator<char>(config)), std::istreambuf_iterator<char>());
		config.close();
		
		int returned = sscanf(buffer.c_str(), "pos: %d x %d\nsize: %d", &position_x, &position_y, &size_x);
		
		this->SetPosition(wxPoint(position_x, position_y));
		this->SetSize(wxSize(size_x, HEIGHT));
	}
	else {
		fs::create_directories(fs::path(ConfigFile).remove_filename());
		this->CenterOnScreen();
		this->SetSize(wxSize(400, HEIGHT));
	}

	this->SetMinSize(wxSize(400, HEIGHT));
	this->SetMaxSize(wxSize(1000, HEIGHT));
	this->SetWindowStyle(wxCAPTION | wxCLOSE_BOX | wxICONIZE | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCLIP_CHILDREN | wxRESIZE_BORDER);
	this->CreateStatusBar(2);

	// Create Menu Bar
	wxSize iconSize(16, 16);
	wxIcon OpenFileIcon = wxArtProvider::GetIcon(wxART_FILE_OPEN, wxART_OTHER, iconSize);
	wxIcon QuitFileIcon = wxArtProvider::GetIcon(wxART_QUIT, wxART_OTHER, iconSize);
	wxIcon PassToolsIcon = wxIcon("KEY_ICON1", wxBITMAP_TYPE_ICO_RESOURCE, 16, 16);
	wxIcon AboutHelpIcon = wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, iconSize);

	menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu();
	wxMenu *toolsMenu = new wxMenu();
	wxMenu *helpMenu = new wxMenu();

	wxMenuItem *openFileMenu = new wxMenuItem(fileMenu, 10000, "&Open...\tCtrl+O", wxEmptyString);
	openFileMenu->SetBitmap(OpenFileIcon);

	wxMenuItem *quitFileMenu = new wxMenuItem(fileMenu, 10001, "&Quit\tAlt+F4", wxEmptyString);
	quitFileMenu->SetBitmap(QuitFileIcon);

	wxMenuItem *gPassToolsMenu = new wxMenuItem(toolsMenu, 10002, "&Generate Password", wxEmptyString);
	gPassToolsMenu->SetBitmap(PassToolsIcon);

	wxMenuItem *aboutHelpMenu = new wxMenuItem(helpMenu, 10003, "&About", wxEmptyString);
	aboutHelpMenu->SetBitmap(AboutHelpIcon);

	fileMenu->Append(openFileMenu);
	fileMenu->AppendSeparator();
	fileMenu->Append(quitFileMenu);
	toolsMenu->Append(gPassToolsMenu);
	helpMenu->Append(aboutHelpMenu);

	menuBar->Append(fileMenu, "&File");
	menuBar->Append(toolsMenu, "&Tools");
	menuBar->Append(helpMenu, "&Help");
	this->SetMenuBar(menuBar);
	
	// Create UI
	wxGridBagSizer *sizer;
	sizer = new wxGridBagSizer(0, 15);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	int borderSize = 8;

	m_Label1 = new wxStaticText(this, wxID_ANY, wxT("Path"), wxDefaultPosition, wxDefaultSize, 0);
	m_Label1->Wrap(-1);
	sizer->Add(m_Label1, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, borderSize);

	m_PathField = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select input file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE | wxFLP_FILE_MUST_EXIST | wxFLP_OPEN);
	sizer->Add(m_PathField, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL | wxEXPAND, borderSize);

	m_Label2 = new wxStaticText(this, wxID_ANY, wxT("Password"), wxDefaultPosition, wxDefaultSize, 0);
	m_Label2->Wrap(-1);
	sizer->Add(m_Label2, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, borderSize);

	m_PasswordField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	m_PasswordField->SetMaxLength(128);
	sizer->Add(m_PasswordField, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALL | wxEXPAND, borderSize);

	m_DoBtn = new wxButton(this, wxID_ANY, wxT("Encrypt"), wxDefaultPosition, wxSize(343, 30), 0);
	m_DoBtn->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_DoBtn->Enable(false);
	sizer->Add(m_DoBtn, wxGBPosition(2, 0), wxGBSpan(1, 2), wxALL | wxEXPAND, borderSize);

	sizer->AddGrowableCol(1);
	this->SetSizer(sizer);
	this->Layout();

	this->DragAcceptFiles(true);

	// Events
	this->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(cMain::onWindowFocus), NULL, this);
	
	this->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(cMain::onFileDropped), NULL, this);
	m_PathField->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(cMain::onFileChanged), NULL, this);
	
	m_DoBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(cMain::doButtonClicked), NULL, this);

	// Get command line args
	if (argc > 1) {
		m_PathField->SetPath(argv[1]);
		CheckPath();
	}

	srand(time(NULL));
}

cMain::~cMain() {
	this->Disconnect(wxEVT_SET_FOCUS, wxFocusEventHandler(cMain::onWindowFocus), NULL, this);
	
	m_PathField->Disconnect(wxEVT_DROP_FILES, wxDropFilesEventHandler(cMain::onFileDropped), NULL, this);
	m_PathField->Disconnect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(cMain::onFileChanged), NULL, this);
	
	m_DoBtn->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(cMain::doButtonClicked), NULL, this);
}

std::string cMain::FormatTime(int s) {
	int seconds, minutes;
	seconds = s;
	minutes = seconds / 60;
	seconds = seconds - (minutes * 60);

	std::string sMinutes = std::to_string(minutes), sSeconds = std::to_string(seconds);
	if (minutes < 10)
		sMinutes = "0" + std::to_string(minutes);

	if (seconds < 10)
		sSeconds = "0" + std::to_string(seconds);

	return std::string(sMinutes + ":" + sSeconds);
}

void cMain::CheckPath() {
	try {
		fs::path fPath = fs::path(m_PathField->GetPath().ToStdString());
		if (fs::exists(fPath) && fs::is_regular_file(fPath)) {
			if (fPath.extension() == ".xor") {
				m_DoBtn->SetLabel("Decrypt");
				bEncrypt = false;
			}
			else {
				m_DoBtn->SetLabel("Encrypt");
				bEncrypt = true;
			}

			menuBar->Enable(10002, bEncrypt);
			m_DoBtn->Enable(true);
		}
		else {
			m_DoBtn->Enable(false);
		}
	}
	catch (std::exception &e) {
		this->SetStatusText("Unexpected error", 0);
		wxMessageBox(e.what(), "Unexpected error", wxICON_ERROR);
	}
}

void cMain::StartTimer() {
	ElapsedTime = 0;
	timer = new wxTimer(this, 13001);
	timer->Start(1000, false);
	this->Connect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(cMain::onTimerUpdate), NULL, this);
	
	this->SetStatusText(std::string(), 0);
	this->SetStatusText(std::string(), 1);

	isWorking = true;
	UpdateControls();
}

void cMain::StopTimer() {
	if (timer == nullptr)
		return;

	timer->Stop();
	this->Disconnect(timer->GetId(), wxEVT_TIMER);

	isWorking = false;
	UpdateControls();
}

void cMain::onTimerUpdate(wxTimerEvent &e) {
	ElapsedTime++;

	this->SetStatusText(*Progress, 0);
	this->SetStatusText(FormatTime(ElapsedTime) + " Elapsed", 1);

	e.Skip();
}

void cMain::SaveSettings() {
	std::fstream config(ConfigFile, std::ios::out);
	int position_x = 0, position_y = 0;
	this->GetScreenPosition(&position_x, &position_y);
	int size_x = this->GetSize().x;

	config << "pos:" << position_x << "x" << position_y << std::endl;
	config << "size:" << size_x << std::endl;
	config.close();
}

bool cMain::DeleteFileWithEx(const std::string &file) {
	try {
		if (fs::exists(fs::path(file))) {
			bool canDelete = fs::remove(fs::path(file));
			if (!canDelete) throw;
		}
	}
	catch (const std::exception &) {
		this->SetStatusText("Unexpected error", 0);
		wxMessageBox("An error occurred while trying to delete file \"" + file + "\". Please try again later.", "Error", wxICON_ERROR);
		return false;
	}

	return true;
}

void cMain::UpdateControls() {
	m_PathField->Enable(!isWorking);
	m_PasswordField->Enable(!isWorking);

	if (isWorking) {
		m_DoBtn->SetLabelText("Cancel");
	}
	else {
		if (bEncrypt) {
			m_DoBtn->SetLabelText("Encrypt");
		}
		else {
			m_DoBtn->SetLabelText("Decrypt");
		}
	}
}

void cMain::onWindowFocus(wxFocusEvent &e) {
	if (!isWorking)
		CheckPath();
	
	e.Skip();
}

void cMain::doButtonClicked(wxCommandEvent &e) {
	if (isWorking) {
		isCancelled = true;
		return;
	}

	isCancelled = false;

	std::string path = m_PathField->GetPath().ToStdString(),
		password = m_PasswordField->GetValue().ToStdString(), output;

	if (path.empty() || !fs::exists(fs::path(path))) {
		this->SetStatusText("Invalid path", 0);
		wxMessageBox("Please fill a valid file path", "Invalid path", wxICON_ERROR);
		e.Skip();
		return;
	}

	if (password.empty() || password.length() < 8 || password.length() > 128) {
		this->SetStatusText("Invalid password", 0);
		wxMessageBox("Password is important, please fill a valid one (min. 8, max. 128)", "Invalid password", wxICON_ERROR);
		e.Skip();
		return;
	}

	if (bEncrypt) {
		output = path + ".xor";
		if (fs::exists(fs::path(output))) {
			int response = wxMessageBox("File \"" + output + "\" already exists. Overwrite?", "Overwrite?", wxYES | wxNO | wxCANCEL | wxICON_WARNING);
			if (response == wxYES) {
				if (!DeleteFileWithEx(output)) {
					e.Skip();
					return;
				}

			}
			else if (response == wxNO) {
				int count = 1;
				std::string tempPath = fs::path(path).remove_filename().string();
				std::string tempFilename = fs::path(path).filename().replace_extension("").string();
				std::string tempExt = fs::path(path).extension().string();
				
				do {
					output = tempPath + "(" + std::to_string(count++) + ") " + tempFilename + tempExt + ".xor";
				}
				while (fs::exists(fs::path(output)));
			}
			else {
				e.Skip();
				return;
			}
		}

		wxBeginBusyCursor();
		StartTimer();

		std::string *log = new std::string("Please wait...");
		bool bException = false;
		int value = 0;

		Progress = log;

		try {
			std::future<int> fut = std::async(Glip::Encrypt, path, password, output, log);
			while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
				wxYield();

				if (isCancelled) {
					*log = "c-1";
					this->SetStatusText("Cancelling...", 0);
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}

			value = fut.get();
		}
		catch (const std::bad_alloc &) {
			this->SetStatusText("Unexpected error", 0);
			
			wxMessageBox("Bad allocation exception. File is too big.", "Error", wxICON_ERROR);
			bException = true;
		}
		catch (const std::exception &exc) {
			this->SetStatusText("Unexpected error", 0);

			wxMessageBox(exc.what(), "Unexpected error", wxICON_ERROR);
			bException = true;
		}

		this->SetStatusText("", 1);

		wxEndBusyCursor();
		StopTimer();

		if (bException)
			return;

		if (value == 0) {
			this->SetStatusText(*log, 0);
		}
		else if (value == 1) {
			this->SetStatusText("Cancelled", 0);

		}
		else {
			this->SetStatusText("Finished with error", 0);
		}

		switch (value) {
			case 1: break;
			case 0:
				wxMessageBox("File encrypted at \"" + output + "\" in " + FormatTime(ElapsedTime) + " seconds", "Success", wxICON_INFORMATION);
				break;
			case -1:
				wxMessageBox(*log, "Error", wxICON_ERROR);
				break;
			case -2:
				wxMessageBox(*log, "Error", wxICON_ERROR);
				break;
			default:
				wxMessageBox("An unexpected error occured. Try again later.", "Error", wxICON_ERROR);
				break;
		}
	}
	else {
		try {
			if (!Glip::CheckPassword(path, password)) {
				this->SetStatusText("Incorrect password", 0);
				wxMessageBox("The password you entered is incorrect", "Wrong password", wxICON_ERROR);
				e.Skip();
				return;
			}
		}
		catch (const std::exception &ex) {
			this->SetStatusText("Unexpected error", 0);
			wxMessageBox(ex.what(), "Unexpected error", wxICON_ERROR);
			e.Skip();
			return;
		}

		output = path.substr(0, path.length() - 4);
		if (fs::path(output).extension() == ".xor") {
			this->SetStatusText("Invalid output", 0);
			wxMessageBox("Please fill a valid output path", "Invalid output", wxICON_ERROR);
			e.Skip();
			return;
		}

		if (fs::exists(fs::path(output))) {
			int response = wxMessageBox("File \"" + output + "\" already exists. Overwrite?", "Overwrite?", wxYES | wxNO | wxCANCEL | wxICON_WARNING);
			if (response == wxYES) {
				if (!DeleteFileWithEx(output)) {
					e.Skip();
					return;
				}
			}
			else if (response == wxNO) {
				int count = 1;
				std::string tempPath = fs::path(path).remove_filename().string();
				std::string tempFilename = fs::path(path).filename().replace_extension("").string();
				do {
					output = tempPath + "(" + std::to_string(count++) + ") " + tempFilename;
				}
				while (fs::exists(fs::path(output)));
			}
			else {
				e.Skip();
				return;
			}
		}

		wxBeginBusyCursor();
		StartTimer();

		std::string *log = new std::string("Please wait...");
		bool bException = false;
		int value = 0;

		Progress = log;

		try {
			std::future<int> fut = std::async(Glip::Decrypt, path, password, output, log);
			do {
				wxYield();

				if (isCancelled) {
					*log = "c-1";
					this->SetStatusText("Cancelling...", 0);
				}
				
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready);
			
			value = fut.get();
		}
		catch (const std::bad_alloc &) {
			this->SetStatusText("Unexpected error", 0);

			wxMessageBox("Bad allocation exception. File is too big.", "Error", wxICON_ERROR);
			bException = true;
		}
		catch (const std::exception &exc) {
			this->SetStatusText("Unexpected error", 0);

			wxMessageBox(exc.what(), "Unexpected error", wxICON_ERROR);
			bException = true;
		}

		this->SetStatusText("", 1);
		
		wxEndBusyCursor();
		StopTimer();

		if (bException)
			return;

		if (value == 0) {
			this->SetStatusText(*log, 0);
		}
		else if (value == 1) {
			this->SetStatusText("Cancelled", 0);
		}
		else {
			this->SetStatusText("Finished with error", 0);
		}

		switch (value) {
			case 1: break;
			case 0:
				wxMessageBox("File decrypted at \"" + output + "\" in " + FormatTime(ElapsedTime) + " seconds", "Success", wxICON_INFORMATION);
				break;
			case -1:
				wxMessageBox(*log, "Error", wxICON_ERROR);
				break;
			case -2:
				wxMessageBox(*log, "Error", wxICON_ERROR);
				break;
			default:
				wxMessageBox("An unexpected error occured. Try again later.", "Error", wxICON_ERROR);
				break;
		}
	}

	if (isCancelled) {
		DeleteFileWithEx(output);
	}

	e.Skip();
}

void cMain::onFileDropped(wxDropFilesEvent &e) {
	if (isWorking)
		return;

	if (e.GetNumberOfFiles() > 0) {
		m_PathField->SetPath(e.GetFiles()[0]);
		CheckPath();
	}
	
	e.Skip();
}

void cMain::onFileChanged(wxFileDirPickerEvent &e) {
	CheckPath();
	e.Skip();
}

void cMain::OpenKeygenDialog() {
	if (isWorking)
		return;

	if (bEncrypt) {
		wxDialog *dialog = new wxDialog(this, wxID_ANY, wxT("Keygen"), wxDefaultPosition, wxSize(480, 115), wxDEFAULT_DIALOG_STYLE);
		dialog->SetIcon(wxIcon("KEY_ICON1"));

		wxBoxSizer *panelSizer = new wxBoxSizer(wxHORIZONTAL);
		wxPanel *panel = new wxPanel(dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

		wxGridBagSizer *sizer;
		sizer = new wxGridBagSizer(0, 10);
		sizer->SetFlexibleDirection(wxBOTH);
		sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

		wxStaticText *m_Label01 = new wxStaticText(panel, wxID_ANY, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_Label01, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER | wxALL | wxEXPAND, 3);

		wxSpinCtrl *m_SpinLength = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 8, 128, 16);
		sizer->Add(m_SpinLength, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL | wxEXPAND, 3);

		wxStaticText *m_Label02 = new wxStaticText(panel, wxID_ANY, wxT("Letters"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_Label02, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALIGN_CENTER | wxALL | wxEXPAND, 3);

		wxCheckBox *m_ContainLetters = new wxCheckBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		m_ContainLetters->Set3StateValue(wxCheckBoxState::wxCHK_CHECKED);
		sizer->Add(m_ContainLetters, wxGBPosition(0, 3), wxGBSpan(1, 1), wxALL | wxEXPAND, 3);

		wxStaticText *m_Label03 = new wxStaticText(panel, wxID_ANY, wxT("Numbers"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_Label03, wxGBPosition(0, 4), wxGBSpan(1, 1), wxALIGN_CENTER | wxALL | wxEXPAND, 3);

		wxCheckBox *m_ContainNumbers = new wxCheckBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		m_ContainNumbers->Set3StateValue(wxCheckBoxState::wxCHK_CHECKED);
		sizer->Add(m_ContainNumbers, wxGBPosition(0, 5), wxGBSpan(1, 1), wxALL | wxEXPAND, 3);

		wxStaticText *m_Label04 = new wxStaticText(panel, wxID_ANY, wxT("Special Char."), wxDefaultPosition, wxDefaultSize, 0);
		m_Label04->SetToolTip("Special Characters");
		sizer->Add(m_Label04, wxGBPosition(0, 6), wxGBSpan(1, 1), wxALIGN_CENTER | wxALL | wxEXPAND, 3);

		wxCheckBox *m_ContainSpecialChar = new wxCheckBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		m_ContainSpecialChar->Set3StateValue(wxCheckBoxState::wxCHK_CHECKED);
		sizer->Add(m_ContainSpecialChar, wxGBPosition(0, 7), wxGBSpan(1, 1), wxALL | wxEXPAND, 3);

		wxButton *m_GenButton = new wxButton(panel, wxID_OK, wxT("Generate"), wxDefaultPosition);
		sizer->Add(m_GenButton, wxGBPosition(1, 0), wxGBSpan(1, 4), wxALL | wxEXPAND, 3);

		wxButton *m_CancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition);
		sizer->Add(m_CancelButton, wxGBPosition(1, 4), wxGBSpan(1, 4), wxALL | wxEXPAND, 3);

		sizer->AddGrowableCol(1);

		panel->SetSizer(sizer);
		panel->Layout();

		sizer->Fit(panel);

		panelSizer->Add(panel, 1, wxEXPAND | wxALL, 10);

		dialog->SetSizer(panelSizer);
		dialog->Layout();
		dialog->Centre(wxBOTH);

		if (dialog->ShowModal() == wxID_OK) {
			bool ContainLetters = m_ContainLetters->GetValue();
			bool ContainNumbers = m_ContainNumbers->GetValue();
			bool ContainSpecialChar = m_ContainSpecialChar->GetValue();

			if (!ContainLetters && !ContainNumbers && !ContainSpecialChar) {
				wxMessageBox("Please check at least on option", "Error", wxICON_ERROR, this);
				OpenKeygenDialog();
				return;
			}

			std::string password = Glip::RandPassword(m_SpinLength->GetValue(), ContainLetters, ContainNumbers, ContainSpecialChar);
			wxClipboard *clip = new wxClipboard();
			if (clip->Open()) {
				clip->Clear();
				clip->SetData(new wxTextDataObject(password));
				clip->Flush();
				clip->Close();

				m_PasswordField->SetLabel(password);
				this->SetStatusText("Password copied to your clipboard", 0);
			}
			else {
				wxMessageBox("An error occurred while trying to copy the generated password to your clipboard. Please try again later.", "Error", wxICON_ERROR);
			}

			delete clip;
		}
	}
}

void cMain::onGeneratePasswordToolsClicked(wxCommandEvent &e) {
	OpenKeygenDialog();

	e.Skip();
}

void cMain::onOpenFileClicked(wxCommandEvent &e) {
	if (isWorking)
		return;

	wxFileDialog *dialog = new wxFileDialog(this, "Select a file", wxEmptyString, wxEmptyString, "*.*", wxFLP_DEFAULT_STYLE | wxFLP_FILE_MUST_EXIST | wxFLP_OPEN);
	if (dialog->ShowModal() == wxID_OK) {
		m_PathField->SetPath(dialog->GetPath());
		CheckPath();
	}

	e.Skip();
}

void cMain::onQuitFileClicked(wxCommandEvent &e) {
	if (isWorking)
		return;

	this->Close();
	e.Skip();
}

void cMain::onAboutHelpMenuClicked(wxCommandEvent &e) {
	if (isWorking)
		return;

	int arch = 86;
	if (sizeof(void *) == 8) { arch = 64; }

	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("Glipus (x" + std::to_string(arch) + ")");
	aboutInfo.SetVersion(VER_FILEVERSION_STR);
	aboutInfo.SetDescription("Glipus is a simple program to enc/decrypt your files, using XOR cipher, fast. It uses the wxWidgets framework with C++.\n\nYou can find the source-code at:");
	aboutInfo.SetCopyright(VER_LEGALCOPYRIGHT_STR);
	aboutInfo.SetWebSite(VER_COMPANYDOMAIN_STR);
	aboutInfo.AddDeveloper(VER_COMPANYNAME_STR);
	aboutInfo.SetLicence("Copyright 2020 BILLVOG\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software andassociated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, andto permit persons to whom the Software is furnished to do so, subject to the following conditions :\n\nThe above copyright notice andthis permission notice shall be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	wxAboutBox(aboutInfo, this);
}

void cMain::onClose(wxCloseEvent &e) {
	if (isWorking) {
		int ans = wxMessageBox("Glip is working, are you sure you want to quit now?", "Confirm", wxYES | wxNO_DEFAULT);
		if (ans == wxNO) {
			return;
		}
	}

	SaveSettings();

	exit(0);
}