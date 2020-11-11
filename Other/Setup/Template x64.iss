; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Glipus"
#define MyAppVersion "1.0.0.0"
#define MyAppPublisher "BILLVOG"
#define MyAppURL "https://github.com/billvog/glipus"
#define MyAppExeName "Glipus.exe"
#define BinDir "E:\Code Projects\C++ Projects\Glipus\bin\Release64-windows-x86_64\Glipus"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{C0946576-0D2B-4858-BC02-CD56EF4D1B38}
VersionInfoVersion={#MyAppVersion}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppCopyright=Copyright (c) 2020 BILLVOG
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
ArchitecturesInstallIn64BitMode=x64
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\unins000.exe
; The [Icons] "quicklaunchicon" entry uses {userappdata} but its [Tasks] entry has a proper IsAdminInstallMode Check.
UsedUserAreasWarning=no
LicenseFile=C:\Users\Voyager\Documents\LICENSES\GPLv3.rtf
InfoAfterFile=E:\Code Projects\C++ Projects\Glipus\Other\Setup\Update Log.rtf
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputDir=E:\Code Projects\C++ Projects\Glipus\Other\Setup
OutputBaseFilename=Glipus_x64
SetupIconFile=E:\Code Projects\C++ Projects\Glipus\Glipus\Assets\setup.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 6.1; Check: not IsAdminInstallMode

[Files]
Source: "{#BinDir}\Glipus.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BinDir}\*"; DestDir: "{app}";
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

