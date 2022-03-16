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
InstallDirRegKey HKLM "Software\Microsoft\UMTK-Classic" "Installation Directory"



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
  File /r "C:\BuildDir\UMTK-Classic\umtkclassic\*"
  ; Put file there
  ;make menu entrys for main program else workpath is wrong?
  CreateDirectory "$SMPROGRAMS\UMTK-Classic"
  CreateShortCut "$SMPROGRAMS\UMTK-Classic\UMTK-Classic.lnk" "$INSTDIR\UMTK-Classic.exe" "" "$INSTDIR\Icons\A.ico" 0
  CreateShortcut "$DESKTOP\UMTK-Classic.lnk" "$INSTDIR\UMTK-Classic.exe" "" "$INSTDIR\Icons\A.ico" 0
  CreateShortCut "$SMPROGRAMS\UMTK-Classic\UMTK-QML Classic.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0 
  
  ; Write the registry keys (WriteRegStr rootkey subkey entry_name new_value_string)

  
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Installation Directory" "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\PDF Settings" "Creation_Text" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\PDF Settings" "Disable_Text" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\PDF Settings" "Image_Path" ""
  WriteRegDWORD  HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\SamAccount Settings" "Option_one" "0"
  WriteRegDWORD  HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\SamAccount Settings" "Option_two" "0"
  WriteRegDWORD  HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\SamAccount Settings" "Option_three" "0"
  WriteRegDWORD  HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\SamAccount Settings" "Option_four" "0"
  WriteRegDWORD  HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\SamAccount Settings" "Option_five" "0"
  WriteRegDWORD  HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\SamAccount Settings" "Option_six" "0"
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\Ticket Settings" "Ticketing_on" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\Ticket Settings" "Ticketing_off" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\Ticket Settings" "Top_Position" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Generation Settings\Ticket Settings" "Bottom_Position" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Disable Settings\" "Cleanup_on" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Disable Settings\" "Cleanup_off" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Disable Settings\" "Redirection_share" ""
  WriteRegStr HKLM "Software\Microsoft\UMTK-Classic\Disable Settings\" "Storage_location" ""

	
  
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