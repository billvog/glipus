#include "EncryptDlg.h"

EncryptDlg::EncryptDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) : wxFrame(parent, id, title, pos, size, style) {
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxIcon("2_ENCRYPTED_ICON"));
	this->SetMinSize(wxSize(330, 255));

	wxBoxSizer *ParentSizer;
	ParentSizer = new wxBoxSizer(wxVERTICAL);

	Panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	
	wxGridBagSizer *Sizer;
	Sizer = new wxGridBagSizer(0, 0);
	Sizer->SetFlexibleDirection(wxBOTH);
	Sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	
	Sizer->AddGrowableCol(0);
	Sizer->AddGrowableRow(0);
	Sizer->AddGrowableRow(1);

	wxStaticBoxSizer *TopSizer;
	TopSizer = new wxStaticBoxSizer(new wxStaticBox(Panel, wxID_ANY, wxEmptyString), wxVERTICAL);

	wxGridBagSizer *TopContentSizer;
	TopContentSizer = new wxGridBagSizer(0, 0);
	TopContentSizer->SetFlexibleDirection(wxBOTH);
	TopContentSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	TopContentSizer->AddGrowableCol(0);
	TopContentSizer->AddGrowableRow(0);
	TopContentSizer->AddGrowableRow(2);
	TopContentSizer->AddGrowableRow(4);
	TopContentSizer->AddGrowableRow(5);

	m_PathLabel = new wxStaticText(TopSizer->GetStaticBox(), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	m_PathLabel->Wrap(-1);
	TopContentSizer->Add(m_PathLabel, wxGBPosition(0, 0), wxGBSpan(1, 2), wxALL | wxEXPAND, 3);

	m_TaskText = new wxStaticText(TopSizer->GetStaticBox(), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	m_TaskText->Wrap(-1);
	TopContentSizer->Add(m_TaskText, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALL, 3);

	m_ProgressText = new wxStaticText(TopSizer->GetStaticBox(), wxID_ANY, wxT("0%"), wxDefaultPosition, wxDefaultSize, 0);
	m_ProgressText->Wrap(-1);
	TopContentSizer->Add(m_ProgressText, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_RIGHT | wxALL, 3);

	m_ProgressBar = new wxGauge(TopSizer->GetStaticBox(), wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_ProgressBar->SetValue(0);
	TopContentSizer->Add(m_ProgressBar, wxGBPosition(2, 0), wxGBSpan(1, 2), wxALL | wxEXPAND, 5);

	m_seperator = new wxStaticLine(TopSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	TopContentSizer->Add(m_seperator, wxGBPosition(3, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 8);

	m_ElapsedTimeLabel = new wxStaticText(TopSizer->GetStaticBox(), wxID_ANY, wxT("Elapsed time"), wxDefaultPosition, wxDefaultSize, 0);
	m_ElapsedTimeLabel->Wrap(-1);
	TopContentSizer->Add(m_ElapsedTimeLabel, wxGBPosition(4, 0), wxGBSpan(1, 1), wxALL, 5);

	m_ElapsedTimeText = new wxStaticText(TopSizer->GetStaticBox(), wxID_ANY, wxT("00:00:00"), wxDefaultPosition, wxDefaultSize, 0);
	m_ElapsedTimeText->Wrap(-1);
	TopContentSizer->Add(m_ElapsedTimeText, wxGBPosition(4, 1), wxGBSpan(1, 1), wxALIGN_RIGHT | wxALL, 5);

	m_RemainingTimeLabel = new wxStaticText(TopSizer->GetStaticBox(), wxID_ANY, wxT("Time left"), wxDefaultPosition, wxDefaultSize, 0);
	m_RemainingTimeLabel->Wrap(-1);
	TopContentSizer->Add(m_RemainingTimeLabel, wxGBPosition(5, 0), wxGBSpan(1, 1), wxALL, 5);

	m_RemainingTimeText = new wxStaticText(TopSizer->GetStaticBox(), wxID_ANY, wxT("00:00:00"), wxDefaultPosition, wxDefaultSize, 0);
	m_RemainingTimeText->Wrap(-1);
	TopContentSizer->Add(m_RemainingTimeText, wxGBPosition(5, 1), wxGBSpan(1, 1), wxALIGN_RIGHT | wxALL, 5);

	TopSizer->Add(TopContentSizer, 1, wxALL | wxEXPAND, 5);

	Sizer->Add(TopSizer, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL | wxEXPAND, 5);

	wxGridSizer *BottomSizer;
	BottomSizer = new wxGridSizer(0, 2, 0, 0);

	m_PauseButton = new wxButton(Panel, wxID_ANY, wxT("Pause"), wxDefaultPosition, wxDefaultSize, 0);
	BottomSizer->Add(m_PauseButton, 0, wxALL | wxEXPAND, 5);

	m_CancelButton = new wxButton(Panel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	BottomSizer->Add(m_CancelButton, 0, wxALL | wxEXPAND, 5);

	Sizer->Add(BottomSizer, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND, 5);

	Panel->SetSizer(Sizer);
	Panel->Layout();
	Sizer->Fit(Panel);

	ParentSizer->Add(Panel, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(ParentSizer);
	this->Layout();

	// Events
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(EncryptDlg::OnWindowResize), NULL, this);
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(EncryptDlg::OnWindowClose), NULL, this);
	
	m_PauseButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EncryptDlg::OnPauseClicked), NULL, this);
	m_CancelButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EncryptDlg::OnCancelClicked), NULL, this);
}

EncryptDlg::~EncryptDlg() {
	this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(EncryptDlg::OnWindowResize), NULL, this);
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(EncryptDlg::OnWindowClose), NULL, this);

	m_PauseButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EncryptDlg::OnPauseClicked), NULL, this);
	m_CancelButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EncryptDlg::OnCancelClicked), NULL, this);
}

void EncryptDlg::SetInfo(const std::string &path, const std::string &password) {
	FilePath = path;
	Password = password;

	Encrypt = (wxString(fs::path(FilePath).extension().string()).MakeLower() != ".glp");

	if (Encrypt) {
		WindowTitle = "Encrypting " + fs::path(FilePath).filename().string();
		m_TaskText->SetLabelText("Encrypting...");
	}
	else {
		WindowTitle = "Decrypting " + fs::path(FilePath).filename().string();
		m_TaskText->SetLabelText("Decrypting...");
	}

	this->SetTitle(WindowTitle);
}

void EncryptDlg::Start() {
	if (FilePath.empty() || !fs::exists(fs::path(FilePath))) {
		wxMessageBox("Please fill a valid file path", "Error", wxICON_ERROR);
		this->Close();
	}

	if (Password.empty()) {
		wxMessageBox("Password is important, please fill a valid one.", "Error", wxICON_ERROR);
		this->Close();
	}

	std::string *Log = new std::string("");
	Progress = new int(0);
	ProcessedBytes = new double(0);

	bool ExceptionOccurred = false;
	int Value = 0;

	if (Encrypt) {
		OutputPath = FilePath + ".glp";
		if (fs::exists(fs::path(OutputPath))) {
			int response = wxMessageBox("File \"" + OutputPath + "\" already exists. Overwrite?", "Overwrite?", wxYES | wxNO | wxCANCEL | wxICON_WARNING);
			if (response == wxYES) {
				if (!DeleteFileWithEx(OutputPath)) {
					wxMessageBox("Couldn't delete file " + OutputPath, "Error", wxICON_ERROR);
					this->Close();
				}

			}
			else if (response == wxNO) {
				int count = 1;
				std::string tempPath = fs::path(FilePath).remove_filename().string();
				std::string tempFilename = fs::path(FilePath).filename().replace_extension("").string();
				std::string tempExt = fs::path(FilePath).extension().string();

				do {
					OutputPath = tempPath + "(" + std::to_string(count++) + ") " + tempFilename + tempExt + ".glp";
				}
				while (fs::exists(fs::path(OutputPath)));
			}
			else {
				this->Close();
			}
		}

		wxWindowDC dc(this);
		m_PathLabel->SetLabelText(wxControl::Ellipsize(OutputPath, dc, wxELLIPSIZE_MIDDLE, this->GetSize().x - 80));
		m_PathLabel->SetToolTip(OutputPath);

		this->Show();
		this->CenterOnScreen();

		StartTimer();

		try {
			std::future<int> fut = std::async(Glip::Encrypt, FilePath, Password, OutputPath, Log, Progress, ProcessedBytes);
			while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
				wxYield();

				if (isCancelled) {
					*Log = "cmd-cancel";
				}
				else if (isPaused) {
					*Log = "cmd-pause";
				}
				else {
					*Log = "";
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}

			Value = fut.get();
		}
		catch (const std::bad_alloc &) {
			wxMessageBox("Bad allocation exception: File is too big", "Error", wxICON_ERROR);
			ExceptionOccurred = true;
		}
		catch (const std::exception &exc) {
			wxMessageBox(exc.what(), "Unexpected error", wxICON_ERROR);
			ExceptionOccurred = true;
		}

		StopTimer();

		if (ExceptionOccurred)
			this->Close();

		switch (Value) {
		case 1: break;
		case 0: break;
		case -1:
			wxMessageBox(*Log, "Error", wxICON_ERROR);
			break;
		case -2:
			wxMessageBox(*Log, "Error", wxICON_ERROR);
			break;
		default:
			wxMessageBox("An unexpected error occured", "Error", wxICON_ERROR);
			break;
		}
	}
	else {
		try {
			if (!Glip::CheckPassword(FilePath, Password)) {
				wxMessageBox("The entered password was incorrect", "Error", wxICON_ERROR);
				this->Close();
			}
		}
		catch (const std::exception &ex) {
			wxMessageBox(ex.what(), "Unexpected error", wxICON_ERROR);
			this->Close();
		}

		OutputPath = FilePath.substr(0, FilePath.length() - 4);
		if (fs::exists(fs::path(OutputPath))) {
			int response = wxMessageBox("File \"" + OutputPath + "\" already exists. Overwrite?", "Overwrite?", wxYES | wxNO | wxCANCEL | wxICON_WARNING);
			if (response == wxYES) {
				if (!DeleteFileWithEx(OutputPath)) {
					wxMessageBox("Couldn't delete file " + OutputPath, "Error", wxICON_ERROR);
					this->Close();
				}
			}
			else if (response == wxNO) {
				int count = 1;
				std::string tempPath = fs::path(FilePath).remove_filename().string();
				std::string tempFilename = fs::path(FilePath).filename().replace_extension("").string();
				do {
					OutputPath = tempPath + "(" + std::to_string(count++) + ") " + tempFilename;
				}
				while (fs::exists(fs::path(OutputPath)));
			}
			else {
				this->Close();
			}
		}

		wxWindowDC dc(this);
		m_PathLabel->SetLabelText(wxControl::Ellipsize(OutputPath, dc, wxELLIPSIZE_MIDDLE, this->GetSize().x - 80));
		m_PathLabel->SetToolTip(OutputPath);

		this->Show();
		this->CenterOnScreen();

		StartTimer();

		try {
			std::future<int> fut = std::async(Glip::Decrypt, FilePath, Password, OutputPath, Log, Progress, ProcessedBytes);
			do {
				wxYield();

				if (isCancelled) {
					*Log = "cmd-cancel";
				}
				else if (isPaused) {
					*Log = "cmd-pause";
				}
				else {
					*Log = "";
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready);

			Value = fut.get();
		}
		catch (const std::bad_alloc &) {
			wxMessageBox("Bad allocation exception: File is too big.", "Error", wxICON_ERROR);
			ExceptionOccurred = true;
		}
		catch (const std::exception &exc) {
			wxMessageBox(exc.what(), "Unexpected error", wxICON_ERROR);
			ExceptionOccurred = true;
		}

		StopTimer();

		if (ExceptionOccurred)
			this->Close();

		switch (Value) {
		case 1: break;
		case 0: break;
		case -1:
			wxMessageBox(*Log, "Error", wxICON_ERROR);
			break;
		case -2:
			wxMessageBox(*Log, "Error", wxICON_ERROR);
			break;
		default:
			wxMessageBox("An unexpected error occured", "Error", wxICON_ERROR);
			break;
		}
	}

	if (isCancelled) {
		DeleteFileWithEx(OutputPath);
	}

	this->Close();
}

std::string EncryptDlg::FormatTime(int s) {
	if (s < 0)
		return "00:00:00";

	int hours = 0, minutes = 0, seconds = s;

	if (seconds >= 3600) {
		hours = seconds / 3600;
		seconds -= hours * 3600;
	}

	minutes = seconds / 60;
	seconds -= minutes * 60;

	std::string sHours = std::to_string(hours), sMinutes = std::to_string(minutes), sSeconds = std::to_string(seconds);

	if (hours < 10)
		sHours = "0" + std::to_string(hours);

	if (minutes < 10)
		sMinutes = "0" + std::to_string(minutes);

	if (seconds < 10)
		sSeconds = "0" + std::to_string(seconds);

	return std::string(sHours + ":" + sMinutes + ":" + sSeconds);
}

void EncryptDlg::StartTimer() {
	ElapsedTime = 0;
	RemainingTime = 0;
	PrevRemainingTime = 0;
	RemainingBytes = fs::file_size(FilePath);

	timer = new wxTimer(this, 13001);
	timer->Start(100, false);
	this->Connect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(EncryptDlg::OnTimerUpdate), NULL, this);

	ProgressIndicator = new wxAppProgressIndicator(this, 100);
	isWorking = true;
}

void EncryptDlg::StopTimer() {
	if (timer == nullptr)
		return;

	timer->Stop();
	this->Disconnect(timer->GetId(), wxEVT_TIMER);

	ProgressIndicator->~wxAppProgressIndicator();

	isWorking = false;
}

void EncryptDlg::OnTimerUpdate(wxTimerEvent &e) {
	if (isPaused)
		return;

	ElapsedTime++;

	if (ElapsedTime % 10 == 0) {
		m_ElapsedTimeText->SetLabelText(FormatTime(ElapsedTime / 10));		

		bool InitRemTime = (RemainingTime == 0);
		RemainingTime = (int) (RemainingBytes / *ProcessedBytes);

		if (InitRemTime || (RemainingTime - PrevRemainingTime) < 30) {
			PrevRemainingTime = RemainingTime;
			m_RemainingTimeText->SetLabelText(FormatTime(RemainingTime));
		}

		RemainingBytes -= *ProcessedBytes;
		*ProcessedBytes = 0;
	}
	
	m_ProgressText->SetLabelText(std::to_string(*Progress) + "%");
	m_ProgressBar->SetValue(*Progress);

	ProgressIndicator->SetValue(*Progress);

	this->Layout();

	e.Skip();
}

bool EncryptDlg::DeleteFileWithEx(const std::string &file) {
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

void EncryptDlg::OnPauseClicked(wxCommandEvent &e) {
	isPaused = !isPaused;

	if (isPaused) {
		this->SetTitle("Paused. Press \"Resume\" to continue.");
		m_PauseButton->SetLabel("Resume");

		m_TaskText->SetLabelText("Paused");

		wxRichToolTip *tooltip = new wxRichToolTip("Paused", "Glipus is paused. Press \"Resume\" to continue.");
		tooltip->SetIcon(wxICON_INFORMATION);
		tooltip->ShowFor(m_PauseButton);
	}
	else {
		this->SetTitle(WindowTitle);
		m_PauseButton->SetLabel("Pause");

		m_TaskText->SetLabelText(Encrypt ? "Encrypting..." : "Decrypting...");
	}
}

void EncryptDlg::OnCancelClicked(wxCommandEvent &e) {
	if (!isCancelled) {
		this->SetTitle("Cancelling...");
		m_TaskText->SetLabelText("Cancelling...");

		isCancelled = true;
	}
}

void EncryptDlg::OnWindowResize(wxSizeEvent &e) {
	wxWindowDC dc(this);
	m_PathLabel->SetLabelText(wxControl::Ellipsize(OutputPath, dc, wxELLIPSIZE_MIDDLE, e.GetSize().x - 80));
	e.Skip();
}

void EncryptDlg::OnWindowClose(wxCloseEvent &e) {
	if (isWorking) {
		if (isCancelled)
			return;

		isCancelled = true;
		return;
	}

	e.Skip();
	exit(0);
}
