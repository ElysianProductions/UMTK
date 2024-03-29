; umtk-qml.nsi
;
; This script is based on example2.nsi,
;
; The User Maintenance tool kit will be installed, an uninstalled will be created,
; start menu entries will also be generated as well as a desktop shortcut.
;--------------------------------
!include "x64.nsh"


 
; The name of the installer
Name "UMTK Classic"

; The file to write
OutFile "UMKT-Classic Installer.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\UMTK-Classic"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Elysian Productions\UMTK-Classic" "Installation Directory"



; Request application privileges (Needs to be admin to write reg keys)
RequestExecutionLevel admin

; Set License info
LicenseData "C:\BuildDir\UMTK-Classic\umtkclassic\License\GPLv3.txt"
;--------------------------------

; Pages
Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------



Section "UMTK Classic(Required)"

  SectionIn RO
  SetOutPath $INSTDIR
  ; Set output path to the installation directory.
  File /r "C:\BuildDir\UMTK-Classic\UMTK-Classic-V2.0\*"
  ; Put file there
  ;make menu entrys for main program 
  CreateDirectory "$SMPROGRAMS\UMTK-Classic"
  CreateShortCut "$SMPROGRAMS\UMTK-Classic\UMTK-Classic.lnk" "$INSTDIR\UMTK-Classic.exe" "" "$INSTDIR\Icons\A.ico" 0
  CreateShortcut "$DESKTOP\UMTK-Classic.lnk" "$INSTDIR\UMTK-Classic.exe" "" "$INSTDIR\Icons\A.ico" 0
  CreateShortCut "$SMPROGRAMS\UMTK-Classic\UMTK-QML Classic.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0 
  
  ; Write the registry keys (WriteRegStr rootkey subkey entry_name new_value_string)

  
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\Installation Directory" "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "UserCreationText" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "UserDisableText" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "CompanyLogoPath" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\PDF Settings" "LogoPosition" ""
  WriteRegDWORD  HKLM "Software\Elysian Productions\UMTK-Classic\Generation Settings\SamAccount Settings" "SamStyle" "3"
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\Generation Settings\Ticket Settings" "Ticketing_on" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\Generation Settings\Ticket Settings" "Ticketing_off" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\Generation Settings\Ticket Settings" "Top_Position" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\Generation Settings\Ticket Settings" "Bottom_Position" ""
  WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Disable Settings\" "CleanupOn" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\Disable Settings\" "RedirectionShare" ""
  WriteRegStr HKLM "Software\Elysian Productions\UMTK-Classic\Disable Settings\" "Storagelocation" ""
  WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Company Settings\" "MultiCompanyEnabled" "0"
  WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Advanced Sort and Display\" "OU Prefix" "0"
  WriteRegDWORD HKLM "Software\Elysian Productions\UMTK-Classic\Advanced Sort and Display\" "Display ACL" "0"

	
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-Classic" "DisplayName" "UMTK Classic"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-Classic" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-Classic" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-Classic" "NoRepair" 1
	
  WriteUninstaller "uninstall.exe"
  

  
  ;make menu entrys
 
SectionEnd
;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-Classic"
  DeleteRegKey HKLM "SOFTWARE\Microsoft\UMTK-Classic"
  DeleteRegKey HKCU "Software\Microsoft\UMTK-Classic"

  ; Remove files and uninstaller
  RMDir /r "$INSTDIR"
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\UMTK-Classic\*.*"
  Delete "$DESKTOP\UMTK-Classic.lnk"

  ; Remove directories used

SectionEnd