#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
EVT_MENU(10000, Main::onOpenFileClicked)
EVT_MENU(10001, Main::onQuitFileClicked)
EVT_MENU(10002, Main::onGeneratePasswordToolsClicked)
EVT_MENU(10003, Main::onCheckForUpdateHelpMenuClicked)
EVT_MENU(10004, Main::onAboutHelpMenuClicked)
EVT_CLOSE(Main::OnClose)
wxEND_EVENT_TABLE()

#define WINDOW_HEIGHT 210

Main::Main(int argc, wxArrayString argv) : wxFrame(nullptr, wxID_ANY, "Glipus") {
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	this->SetIcon(wxIcon("IDI_ICON1"));
	
	LoadSettings();

	this->SetMinSize(wxSize(400, WINDOW_HEIGHT));
	this->SetMaxSize(wxSize(1000, WINDOW_HEIGHT));
	this->SetWindowStyle(wxCAPTION | wxCLOSE_BOX | wxICONIZE | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCLIP_CHILDREN | wxRESIZE_BORDER);
	this->CreateStatusBar(2);

	// Create Menu Bar
	wxSize IconSize(16, 16);

	menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu();
	wxMenu *toolsMenu = new wxMenu();
	wxMenu *helpMenu = new wxMenu();

	wxMenuItem *openFileMenu = new wxMenuItem(fileMenu, 10000, "&Open...\tCtrl+O", wxEmptyString);
	openFileMenu->SetBitmap(wxArtProvider::GetIcon(wxART_FILE_OPEN, wxART_OTHER, IconSize));

	wxMenuItem *quitFileMenu = new wxMenuItem(fileMenu, 10001, "&Quit\tCtrl+Q", wxEmptyString);
	quitFileMenu->SetBitmap(wxArtProvider::GetIcon(wxART_QUIT, wxART_OTHER, IconSize));

	wxMenuItem *keygenToolsMenu = new wxMenuItem(toolsMenu, 10002, "&Generate Password", wxEmptyString);
	keygenToolsMenu->SetBitmap(wxIcon("KEY_ICON1", wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));

	wxMenuItem *checkForUpdateHelpMenu = new wxMenuItem(helpMenu, 10003, "&Check for Update", wxEmptyString);

	wxMenuItem *aboutHelpMenu = new wxMenuItem(helpMenu, 10004, "&About", wxEmptyString);
	aboutHelpMenu->SetBitmap(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, IconSize));

	fileMenu->Append(openFileMenu);
	fileMenu->AppendSeparator();
	fileMenu->Append(quitFileMenu);
	toolsMenu->Append(keygenToolsMenu);
	helpMenu->Append(checkForUpdateHelpMenu);
	helpMenu->AppendSeparator();
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

	int BorderSize = 8;

	m_Label1 = new wxStaticText(this, wxID_ANY, wxT("Path"), wxDefaultPosition, wxDefaultSize, 0);
	m_Label1->Wrap(-1);
	sizer->Add(m_Label1, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, BorderSize);

	m_PathField = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select input file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE | wxFLP_FILE_MUST_EXIST | wxFLP_OPEN);
	sizer->Add(m_PathField, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL | wxEXPAND, BorderSize);

	m_Label2 = new wxStaticText(this, wxID_ANY, wxT("Password"), wxDefaultPosition, wxDefaultSize, 0);
	m_Label2->Wrap(-1);
	sizer->Add(m_Label2, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, BorderSize);

	m_PasswordField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	m_PasswordField->SetMaxLength(128);
	sizer->Add(m_PasswordField, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALL | wxEXPAND, BorderSize);

	m_DoBtn = new wxButton(this, wxID_ANY, wxT("Encrypt"), wxDefaultPosition, wxSize(-1, 30), 0);
	m_DoBtn->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_DoBtn->Enable(false);
	sizer->Add(m_DoBtn, wxGBPosition(2, 0), wxGBSpan(1, 2), wxALL | wxEXPAND, BorderSize);

	sizer->AddGrowableCol(1);

	this->SetSizer(sizer);
	this->Layout();

	this->DragAcceptFiles(true);

	GetArguments(argc, argv);

	// Events
	this->Connect(wxEVT_IDLE, wxIdleEventHandler(Main::OnIdleEvent), NULL, this);
	this->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(Main::OnWindowFocus), NULL, this);
	this->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(Main::onFileDropped), NULL, this);
	
	m_PathField->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(Main::onFileChanged), NULL, this);
	m_DoBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Main::DoButtonClicked), NULL, this);
}

Main::~Main() {
	this->Disconnect(wxEVT_SET_FOCUS, wxFocusEventHandler(Main::OnWindowFocus), NULL, this);
	
	m_PathField->Disconnect(wxEVT_DROP_FILES, wxDropFilesEventHandler(Main::onFileDropped), NULL, this);
	m_PathField->Disconnect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(Main::onFileChanged), NULL, this);
	
	m_DoBtn->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Main::DoButtonClicked), NULL, this);
}

void Main::GetArguments(int argc, wxArrayString argv) {
	if (argc > 1) {
		if (argc == 2) {
			if (argv[1] == "-clear-update") {
				UpdateDlg::ClearUpdate();
				exit(0);
				return;
			}
			else {
				m_PathField->SetPath(argv[1]);
				CheckPath();
				return;
			}
		}

		ShowEncryptionDlg = true;

		for (size_t i = 1; i < argc; i++) {
			if (argv[i] == "-i")
				m_PathField->SetPath(argv[++i]);

			if (argv[i] == "-p")
				m_PasswordField->SetLabelText(argv[++i]);
		}
	}
}

void Main::CheckPath() {
	try {
		fs::path fPath = fs::path(m_PathField->GetPath().ToStdString());
		if (fs::exists(fPath) && fs::is_regular_file(fPath)) {
			if (fPath.extension() == ".glp") {
				m_DoBtn->SetLabel("Decrypt");
				Encrypt = false;
			}
			else {
				m_DoBtn->SetLabel("Encrypt");
				Encrypt = true;
			}

			menuBar->Enable(10002, Encrypt);
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

bool Main::DeleteFileWithEx(const std::string &file) {
	try {
		if (fs::exists(fs::path(file))) {
			bool canDelete = fs::remove(fs::path(file));
			if (!canDelete) throw;
		}
	}
	catch (const std::exception &) {
		wxMessageBox("An error occurred while trying to delete file \"" + file + "\". Please try again later.", "Error", wxICON_ERROR);
		return false;
	}

	return true;
}

void Main::LoadSettings() {
	ConfigFile = wxStandardPaths::Get().GetUserConfigDir() + "\\BILLVOG\\Glipus\\config.ini";
	std::fstream config(ConfigFile, std::ios::in);
	if (config.is_open()) {
		int position_x = 0, position_y = 0;
		int size_x = 0;

		std::string buffer((std::istreambuf_iterator<char>(config)), std::istreambuf_iterator<char>());
		config.close();

		int returned = sscanf(buffer.c_str(), "pos: %d x %d\nsize: %d", &position_x, &position_y, &size_x);

		this->SetPosition(wxPoint(position_x, position_y));
		this->SetSize(wxSize(size_x, WINDOW_HEIGHT));
	}
	else {
		fs::create_directories(fs::path(ConfigFile).remove_filename());
		this->CenterOnScreen();
		this->SetSize(wxSize(400, WINDOW_HEIGHT));
	}
}

void Main::SaveSettings() {
	std::fstream config(ConfigFile, std::ios::out);
	int position_x = 0, position_y = 0;
	this->GetScreenPosition(&position_x, &position_y);
	int size_x = this->GetSize().x;

	config << "pos:" << position_x << "x" << position_y << std::endl;
	config << "size:" << size_x << std::endl;
	config.close();
}

void Main::OnIdleEvent(wxIdleEvent &e) {
	this->Disconnect(wxEVT_IDLE, wxIdleEventHandler(Main::OnIdleEvent), NULL, this);

	if (!ShowEncryptionDlg) {
		this->Show();
		e.Skip();

		UpdateDlg::ClearUpdate();

		return;
	}

	EncryptDlg *dialog = new EncryptDlg(nullptr, wxID_ANY);
	dialog->SetInfo(m_PathField->GetPath().ToStdString(), m_PasswordField->GetLabelText().ToStdString());
	dialog->Show();
	dialog->Centre(wxBOTH);
	dialog->Start();
	
	e.Skip();
	exit(0);
}

void Main::OnWindowFocus(wxFocusEvent &e) {
	CheckPath();
	e.Skip();
}

void Main::OnClose(wxCloseEvent &e) {
	SaveSettings();
	exit(0);
}

void Main::DoButtonClicked(wxCommandEvent &e) {
	std::string path = m_PathField->GetPath().ToStdString(), password = m_PasswordField->GetValue().ToStdString();

	std::stringstream cmd;

	cmd << '\"' << wxStandardPaths::Get().GetExecutablePath().ToStdString() << '\"';
	cmd << ' ' << "-i " << '\"' << path << '\"';
	cmd << ' ' << "-p " << '\"' << password << '\"';

	wxExecute(cmd.str(), wxEXEC_ASYNC);

	e.Skip();
}

void Main::onFileDropped(wxDropFilesEvent &e) {
	if (e.GetNumberOfFiles() > 0) {
		m_PathField->SetPath(e.GetFiles()[0]);
		CheckPath();
	}
	
	e.Skip();
}

void Main::onFileChanged(wxFileDirPickerEvent &e) {
	CheckPath();
	e.Skip();
}

void Main::OpenKeygenDialog() {
	if (Encrypt) {
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

		wxSpinCtrl *m_SpinLength = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 4, 128, 14);
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

void Main::onGeneratePasswordToolsClicked(wxCommandEvent &e) {
	OpenKeygenDialog();
	e.Skip();
}

void Main::onOpenFileClicked(wxCommandEvent &e) {
	wxString DefaultDir = fs::path(m_PathField->GetPath().ToStdString()).parent_path();
	wxString DefaultFile = fs::path(m_PathField->GetPath().ToStdString()).filename();

	wxFileDialog *dialog = new wxFileDialog(this, "Select a file", DefaultDir, DefaultFile, "*.*", wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST | wxFD_OPEN);
	if (dialog->ShowModal() == wxID_OK) {
		m_PathField->SetPath(dialog->GetPath());
		CheckPath();
	}

	e.Skip();
}

void Main::onQuitFileClicked(wxCommandEvent &e) {
	this->Close();
	e.Skip();
}

void Main::onCheckForUpdateHelpMenuClicked(wxCommandEvent &e) {
	UpdateDlg *updater = new UpdateDlg(this);
	if (!updater->StartUpdate()) {
		updater->ClearUpdate();
	}
}

void Main::onAboutHelpMenuClicked(wxCommandEvent &e) {
#ifdef GL_ARCH_64
	int Architecture = 64;
#else
	int Architecture = 86;
#endif

	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("Glipus (x" + std::to_string(Architecture) + ")");
	aboutInfo.SetVersion(VER_FILEVERSION_STR);
	aboutInfo.SetDescription("Glipus is an open-source encryption software that uses AES 256-bit encryption.\nIt helps you keep your files safe by simply clicking two buttons.");
	aboutInfo.SetCopyright(VER_LEGALCOPYRIGHT_STR);
	aboutInfo.SetWebSite(VER_COMPANYDOMAIN_STR);
	aboutInfo.AddDeveloper(VER_COMPANYNAME_STR);
	aboutInfo.SetLicence("Copyright (C) 2020 BILLVOG\n\nThis program is free software: you can redistribute it and /or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see <https://www.gnu.org/licenses/>.");
	
	wxAboutBox(aboutInfo, this);
}