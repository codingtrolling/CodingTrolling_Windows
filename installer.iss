; --- CODINGTROLLING SUITE INSTALLER SCRIPT ---

#define MyAppName "CodingTrolling Hub"
#define MyAppVersion "1.0"
#define MyAppPublisher "CodingTrolling"
#define MyAppExeName "CodingTrollingHub.exe"

[Setup]
; Unique ID for the app
AppId={{C0D1NG-TR0LL-1NG-HUB-0001}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; The name of the final installer file
OutputBaseFilename=CodingTrollingSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
; Create the hidden internal folder
Name: "{app}\_internal"; Attribs: hidden

[Files]
; 1. The main visible Hub
Source: "C:\Path\To\Your\Project\_DISTRIBUTION_READY\CodingTrollingHub.exe"; DestDir: "{app}"; Flags: ignoreversion

; 2. The Tools (placed in hidden folder)
Source: "C:\Path\To\Your\Project\_DISTRIBUTION_READY\_internal\bin\*"; DestDir: "{app}\_internal\bin"; Flags: ignoreversion recursesubdirs createallsubdirs; Attribs: hidden

; 3. The Dependencies (ADB, QEMU, DLLs)
Source: "C:\Path\To\Your\Project\_DISTRIBUTION_READY\_internal\deps\*"; DestDir: "{app}\_internal\deps"; Flags: ignoreversion recursesubdirs createallsubdirs; Attribs: hidden

; 4. Data folder for config files
Source: "C:\Path\To\Your\Project\_DISTRIBUTION_READY\_internal\data\*"; DestDir: "{app}\_internal\data"; Flags: ignoreversion recursesubdirs createallsubdirs; Attribs: hidden

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
