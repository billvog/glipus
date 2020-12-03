#include "Main.h"

Main::Main(int argc, wxArrayString argv) : wxFrame(nullptr, wxID_ANY, "Glipus") {
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	this->SetIcon(wxIcon("1_MAINICON"));
	
	LoadSettings();
	
	this->SetMinSize(wxSize(400, 365));
	this->SetWindowStyle(wxCAPTION | wxCLOSE_BOX | wxICONIZE | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCLIP_CHILDREN | wxRESIZE_BORDER);
	this->DragAcceptFiles(true);

	// Create Menubar
	CreateMenubar();
	
	// Create UI
	wxBoxSizer *ParentSizer;
	ParentSizer = new wxBoxSizer(wxVERTICAL);

	Panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxGridBagSizer *Sizer;
	Sizer = new wxGridBagSizer(0, 25);
	Sizer->SetFlexibleDirection(wxBOTH);
	Sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxStaticBoxSizer *OptionsSizer;
	OptionsSizer = new wxStaticBoxSizer(new wxStaticBox(Panel, wxID_ANY, wxT("Options")), wxVERTICAL);

	wxGridBagSizer *OptContentSizer;
	OptContentSizer = new wxGridBagSizer(0, 0);
	OptContentSizer->SetFlexibleDirection(wxBOTH);
	OptContentSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_Label1 = new wxStaticText(OptionsSizer->GetStaticBox(), wxID_ANY, wxT("Filepath"), wxDefaultPosition, wxDefaultSize, 0);
	m_Label1->Wrap(-1);
	OptContentSizer->Add(m_Label1, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 2);

	m_PathField = new wxFilePickerCtrl(OptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE | wxFLP_FILE_MUST_EXIST | wxFLP_OPEN);
	OptContentSizer->Add(m_PathField, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALL | wxEXPAND, 2);

	OptContentSizer->Add(0, 10, wxGBPosition(2, 0), wxGBSpan(1, 1), wxEXPAND, 5);

	m_Label2 = new wxStaticText(OptionsSizer->GetStaticBox(), wxID_ANY, wxT("Enter password"), wxDefaultPosition, wxDefaultSize, 0);
	m_Label2->Wrap(-1);
	OptContentSizer->Add(m_Label2, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALL, 2);

	m_PasswordField = new wxTextCtrl(OptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	OptContentSizer->Add(m_PasswordField, wxGBPosition(4, 0), wxGBSpan(1, 1), wxALL | wxEXPAND, 2);

	OptContentSizer->Add(0, 10, wxGBPosition(5, 0), wxGBSpan(1, 1), wxEXPAND, 5);

	m_Label3 = new wxStaticText(OptionsSizer->GetStaticBox(), wxID_ANY, wxT("Reenter password for validation"), wxDefaultPosition, wxDefaultSize, 0);
	m_Label3->Wrap(-1);
	OptContentSizer->Add(m_Label3, wxGBPosition(6, 0), wxGBSpan(1, 1), wxALL, 2);

	m_RePasswordField = new wxTextCtrl(OptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	OptContentSizer->Add(m_RePasswordField, wxGBPosition(7, 0), wxGBSpan(1, 1), wxALL | wxEXPAND, 2);

	OptContentSizer->Add(0, 10, wxGBPosition(8, 0), wxGBSpan(1, 1), wxEXPAND, 5);

	m_ShowPasswordToggle = new wxCheckBox(OptionsSizer->GetStaticBox(), wxID_ANY, wxT("Show Password"), wxDefaultPosition, wxDefaultSize, 0);
	m_ShowPasswordToggle->Set3StateValue(ShowPwdCb ? wxCheckBoxState::wxCHK_CHECKED : wxCheckBoxState::wxCHK_UNCHECKED);
	OptContentSizer->Add(m_ShowPasswordToggle, wxGBPosition(9, 0), wxGBSpan(1, 1), wxALL, 2);

	OptContentSizer->AddGrowableCol(0);
	OptContentSizer->AddGrowableRow(2);
	OptContentSizer->AddGrowableRow(5);
	OptContentSizer->AddGrowableRow(8);

	OptionsSizer->Add(OptContentSizer, 1, wxALL | wxEXPAND, 5);

	Sizer->Add(OptionsSizer, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL | wxEXPAND, 5);

	m_EncryptButton = new wxButton(Panel, wxID_ANY, wxT("Encrypt"), wxDefaultPosition, wxDefaultSize, 0);
	m_EncryptButton->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	Sizer->Add(m_EncryptButton, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALL | wxEXPAND, 5);

	Sizer->AddGrowableCol(0);
	Sizer->AddGrowableRow(0);
	Sizer->AddGrowableRow(1);

	Panel->SetSizer(Sizer);
	Panel->Layout();
	Sizer->Fit(Panel);
	ParentSizer->Add(Panel, 1, wxALL | wxEXPAND, 10);

	this->SetSizer(ParentSizer);
	this->Layout();

	GetArguments(argc, argv);

	// Events
	this->Connect(wxEVT_IDLE, wxIdleEventHandler(Main::OnIdleEvent), NULL, this);
	this->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(Main::OnWindowFocus), NULL, this);
	this->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(Main::OnFileDropped), NULL, this);
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(Main::OnClose), NULL, this);

	m_PathField->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(Main::OnFileChanged), NULL, this);
	m_PasswordField->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(Main::OnPasswordChanged), NULL, this);
	m_ShowPasswordToggle->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(Main::OnPasswordShowToggled), NULL, this);
	m_EncryptButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Main::OnEncryptButtonClicked), NULL, this);

	SetPwdShow(ShowPwdCb);
}

Main::~Main() {
	this->Disconnect(wxEVT_SET_FOCUS, wxFocusEventHandler(Main::OnWindowFocus), NULL, this);
	this->Disconnect(wxEVT_DROP_FILES, wxDropFilesEventHandler(Main::OnFileDropped), NULL, this);
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(Main::OnClose), NULL, this);

	m_PathField->Disconnect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(Main::OnFileChanged), NULL, this);
	m_PasswordField->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(Main::OnPasswordChanged), NULL, this);
	m_EncryptButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Main::OnEncryptButtonClicked), NULL, this);
}

void Main::OnIdleEvent(wxIdleEvent &e) {
	this->Disconnect(wxEVT_IDLE, wxIdleEventHandler(Main::OnIdleEvent), NULL, this);

	if (!ShowEncryptionDlg) {
		this->Show();
		e.Skip();

		UpdateDlg::ClearUpdate();

		return;
	}

	EncryptDlg *dialog = new EncryptDlg(nullptr);
	dialog->SetInfo(m_PathField->GetPath().ToStdString(), m_PasswordField->GetLabelText().ToStdString());
	dialog->Start();

	e.Skip();
	exit(0);
}

void Main::OnWindowFocus(wxFocusEvent &e) {
	ValidateFields();
	e.Skip();
}

void Main::OnClose(wxCloseEvent &e) {
	SaveSettings();
	exit(0);
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

void Main::CreateMenubar() {
	wxSize IconSize(16, 16);

	menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu();
	wxMenu *toolsMenu = new wxMenu();
	wxMenu *helpMenu = new wxMenu();

	openFileMenu = new wxMenuItem(fileMenu, wxID_ANY, "&Open...\tCtrl+O", wxEmptyString);
	quitFileMenu = new wxMenuItem(fileMenu, wxID_ANY, "&Quit\tCtrl+Q", wxEmptyString);
	keygenToolsMenu = new wxMenuItem(toolsMenu, wxID_ANY, "&Password Generator...", wxEmptyString);
	checkForUpdateHelpMenu = new wxMenuItem(helpMenu, wxID_ANY, "&Check for Updates", wxEmptyString);
	aboutHelpMenu = new wxMenuItem(helpMenu, wxID_ANY, "&About", wxEmptyString);

	openFileMenu->SetBitmap(wxArtProvider::GetIcon(wxART_FILE_OPEN, wxART_OTHER, IconSize));
	quitFileMenu->SetBitmap(wxArtProvider::GetIcon(wxART_QUIT, wxART_OTHER, IconSize));
	keygenToolsMenu->SetBitmap(wxIcon("KEY_ICON", wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	checkForUpdateHelpMenu->SetBitmap(wxIcon("UPDATE_ICON", wxBITMAP_TYPE_ICO_RESOURCE, 14, 14));
	aboutHelpMenu->SetBitmap(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, IconSize));

	menuBar->Connect(openFileMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Main::OnOpenFileClicked), NULL, this);
	menuBar->Connect(quitFileMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Main::OnQuitFileClicked), NULL, this);
	menuBar->Connect(keygenToolsMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Main::OnGeneratePasswordToolsClicked), NULL, this);
	menuBar->Connect(checkForUpdateHelpMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Main::OnCheckForUpdateHelpMenuClicked), NULL, this);
	menuBar->Connect(aboutHelpMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Main::OnAboutHelpMenuClicked), NULL, this);

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
}

void Main::ValidateFields() {
	try {
		fs::path fPath = fs::path(m_PathField->GetPath().ToStdString());
		if (fs::exists(fPath) && fs::is_regular_file(fPath)) {
			if (wxString(fPath.extension().string()).MakeLower() == ".glp") {
				m_EncryptButton->SetLabel("Decrypt");
				Encrypt = false;
			}
			else {
				m_EncryptButton->SetLabel("Encrypt");
				Encrypt = true;
			}

			if (!ShowPwdCb && Encrypt) {
				m_RePasswordField->Enable(true);
			}
			else {
				m_RePasswordField->Clear();
				m_RePasswordField->Enable(false);
			}

			keygenToolsMenu->Enable(Encrypt);
			m_EncryptButton->Enable(m_PasswordField->GetValue().length() > 0);
		}
		else {
			m_EncryptButton->Enable(false);
		}
	}
	catch (std::exception &e) {
		this->SetStatusText("Unexpected error", 0);
		wxMessageBox(e.what(), "Unexpected error", wxICON_ERROR);
	}
}

void Main::SetPwdShow(bool value) {
	HWND hwnd = (HWND) m_PasswordField->GetHandle();
	SendMessage(hwnd, EM_SETPASSWORDCHAR, !value ? 0x25cf : 0, 0);
	m_PasswordField->Refresh();

	m_RePasswordField->Clear();
	m_RePasswordField->Enable(!value && Encrypt);
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
	wxRegKey RegKey(wxRegKey::HKCU, "Software\\BILLVOG\\Glipus\\Window");
	if (RegKey.Exists()) {
		long position_x = 0, position_y = 0;
		long size_x = 0, size_y = 0;
		long ShowPasswordCheckbox = 1;

		RegKey.QueryValue("PositionX", &position_x);
		RegKey.QueryValue("PositionY", &position_y);
		RegKey.QueryValue("SizeX", &size_x);
		RegKey.QueryValue("SizeY", &size_y);
		RegKey.QueryValue("ShowPwdCb", &ShowPasswordCheckbox);

		this->SetPosition(wxPoint(position_x, position_y));
		this->SetSize(wxSize(size_x, size_y));
		ShowPwdCb = (ShowPasswordCheckbox == 0);
	}
	else {
		this->SetSize(wxSize(400, 350));
		this->CenterOnScreen();
		ShowPwdCb = false;
	}
}

void Main::SaveSettings() {
	wxRegKey MainKey(wxRegKey::HKCU, "Software\\BILLVOG\\Glipus");
	MainKey.Create();

	int position_x = 0, position_y = 0;
	this->GetScreenPosition(&position_x, &position_y);
	int size_x = this->GetSize().x, size_y = this->GetSize().y;

	wxRegKey WindowKey(wxRegKey::HKCU, "Software\\BILLVOG\\Glipus\\Window");
	WindowKey.Create();
	WindowKey.SetValue("PositionX", position_x);
	WindowKey.SetValue("PositionY", position_y);
	WindowKey.SetValue("SizeX", size_x);
	WindowKey.SetValue("SizeY", size_y);
	WindowKey.SetValue("ShowPwdCb", ShowPwdCb ? 0 : 1);
}

void Main::OnOpenFileClicked(wxCommandEvent &e) {
	wxString DefaultDir = fs::path(m_PathField->GetPath().ToStdString()).parent_path();
	wxString DefaultFile = fs::path(m_PathField->GetPath().ToStdString()).filename();

	wxFileDialog *dialog = new wxFileDialog(this, "Select a file", DefaultDir, DefaultFile, "*.*", wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST | wxFD_OPEN);
	if (dialog->ShowModal() == wxID_OK) {
		m_PathField->SetPath(dialog->GetPath());
		ValidateFields();
	}

	e.Skip();
}

void Main::OnQuitFileClicked(wxCommandEvent &e) {
	this->Close();
	e.Skip();
}

void Main::OnGeneratePasswordToolsClicked(wxCommandEvent &e) {
	wxDialog *dialog = new wxDialog(this, wxID_ANY, wxT("Keygen"), wxDefaultPosition, wxSize(480, 130), wxDEFAULT_DIALOG_STYLE);
	dialog->SetIcon(wxIcon("KEY_ICON"));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel *panel = new wxPanel(dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxGridBagSizer *sizer;
	sizer = new wxGridBagSizer(0, 10);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxStaticText *m_Label01 = new wxStaticText(panel, wxID_ANY, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0);
	sizer->Add(m_Label01, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER | wxALL | wxEXPAND, 3);

	wxSpinCtrl *m_SpinLength = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2048, 16);
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

	sizer->Add(0, 10, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND, 5);

	wxButton *m_GenButton = new wxButton(panel, wxID_OK, wxT("Generate"), wxDefaultPosition, wxSize(-1, 25));
	sizer->Add(m_GenButton, wxGBPosition(2, 0), wxGBSpan(1, 4), wxALL | wxEXPAND, 3);

	wxButton *m_CancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize(-1, 25));
	sizer->Add(m_CancelButton, wxGBPosition(2, 4), wxGBSpan(1, 4), wxALL | wxEXPAND, 3);

	sizer->AddGrowableCol(1);

	panel->SetSizer(sizer);
	panel->Layout();

	sizer->Fit(panel);

	panelSizer->Add(panel, 1, wxEXPAND | wxALL, 10);

	dialog->SetSizer(panelSizer);
	dialog->Layout();
	dialog->Centre(wxBOTH);

	bool isOk = false;

	while (!isOk) {
		if (dialog->ShowModal() == wxID_OK) {
			bool ContainLetters = m_ContainLetters->GetValue();
			bool ContainNumbers = m_ContainNumbers->GetValue();
			bool ContainSpecialChar = m_ContainSpecialChar->GetValue();

			if (!ContainLetters && !ContainNumbers && !ContainSpecialChar) {
				wxMessageBox("Please check at least on option", "Error", wxICON_ERROR, this);
				continue;
			}

			std::string password = Glip::RandPassword(m_SpinLength->GetValue(), ContainLetters, ContainNumbers, ContainSpecialChar);
			wxClipboard *clip = new wxClipboard();
			if (clip->Open()) {
				clip->Clear();
				clip->SetData(new wxTextDataObject(password));
				clip->Flush();
				clip->Close();

				m_PasswordField->SetLabelText(password);

				wxRichToolTip *tooltip = new wxRichToolTip("Password Generator", "Generated password is copied to your clipboard.");
				tooltip->SetIcon(wxICON_INFORMATION);
				tooltip->ShowFor(m_PasswordField);
			}
			else {
				wxMessageBox("An error occurred while trying to copy the generated password to your clipboard. Please try again later.", "Error", wxICON_ERROR);
			}

			delete clip;
		}

		break;
	}

	e.Skip();
}

void Main::OnCheckForUpdateHelpMenuClicked(wxCommandEvent &e) {
	UpdateDlg *updater = new UpdateDlg(this);
	if (!updater->StartUpdate()) {
		updater->ClearUpdate();
	}
}

void Main::OnAboutHelpMenuClicked(wxCommandEvent &e) {
#ifdef GL_ARCH_64
	int Architecture = 64;
#else
	int Architecture = 86;
#endif

	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("Glipus (x" + std::to_string(Architecture) + ")");
	aboutInfo.SetVersion(VER_FILEVERSION_STR, "Version " + wxString(VER_FILEVERSION_STR));
	aboutInfo.SetDescription("Glipus is a tool to help you keep your files secure with strong AES 256-bit encryption.\nIt's free, open-source software, licensed under GPLv3.");
	aboutInfo.SetCopyright(VER_LEGALCOPYRIGHT_STR);
	aboutInfo.SetWebSite(VER_COMPANYDOMAIN_STR, "Find Glipus at Github");
	aboutInfo.SetLicence("Copyright © 2020 BILLVOG\n\nThis program is free software: you can redistribute it and /or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see <https://www.gnu.org/licenses/>.");
	aboutInfo.AddDeveloper(VER_COMPANYNAME_STR);

	wxAboutBox(aboutInfo, this);
}

void Main::OnFileDropped(wxDropFilesEvent &e) {
	if (e.GetNumberOfFiles() > 0) {
		m_PathField->SetPath(e.GetFiles()[0]);
		ValidateFields();
	}
	
	e.Skip();
}

void Main::OnFileChanged(wxFileDirPickerEvent &e) {
	ValidateFields();
	e.Skip();
}

void Main::OnPasswordChanged(wxCommandEvent &e) {
	// Check if password is weak
	if (IsPasswordWeak(m_PasswordField->GetValue().ToStdString())) {
		wxRichToolTip *tooltip = new wxRichToolTip("Weak Password", "This is a commonly used password, you should better not use it.");
		tooltip->SetIcon(wxICON_WARNING);
		tooltip->ShowFor(m_PasswordField);
	}

	ValidateFields();
	e.Skip();
}

void Main::OnPasswordShowToggled(wxCommandEvent &e) {
	ShowPwdCb = !ShowPwdCb;
	SetPwdShow(ShowPwdCb);
	e.Skip();
}

void Main::OnEncryptButtonClicked(wxCommandEvent &e) {
	std::string path = m_PathField->GetPath().ToStdString();
	std::string password = m_PasswordField->GetValue().ToStdString(),
		repassword = m_RePasswordField->GetValue().ToStdString();

	if (Encrypt && !ShowPwdCb && (password != repassword)) {
		wxMessageBox("Passwords don't match", "Warning", wxICON_WARNING);
		e.Skip();
		return;
	}

	std::stringstream cmd;
	cmd << '\"' << wxStandardPaths::Get().GetExecutablePath().ToStdString() << '\"';
	cmd << ' ' << "-i " << '\"' << path << '\"';
	cmd << ' ' << "-p " << '\"' << password << '\"';

	wxExecute(cmd.str(), wxEXEC_ASYNC);

	e.Skip();
}