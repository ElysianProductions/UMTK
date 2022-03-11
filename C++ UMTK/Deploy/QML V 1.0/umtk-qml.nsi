; umtk-qml.nsi
;
; This script is based on example2.nsi,
;
; The User Maintenance tool kit will be installed, an uninstalled will be created,
; start menu entries will also be generated as well as a desktop shortcut.
;--------------------------------
!include "x64.nsh"

 
; The name of the installer
Name "UMTK"

; The file to write
OutFile "..\UMKT-QML Installer.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\UMTK-QML"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\UMTK-QML" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Set License info
LicenseData "C:\BuildDir\umtkqml\QML-UMTK-V1.0\GPLv3.txt"
;--------------------------------

; Pages
Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------


; The stuff to install
Section "UMTK QML(Required)"

  SectionIn RO
  SetOutPath $INSTDIR
  ; Set output path to the installation directory.
  File /r "C:\BuildDir\umtkqml\QML-UMTK-V1.0\*"
  ; Put file there
  ;make menu entrys for main program else workpath is wrong?
  CreateDirectory "$SMPROGRAMS\UMTK-QML"
  CreateShortCut "$SMPROGRAMS\UMTK-QML\UMTK-QML.lnk" "$INSTDIR\UMTK-QML.exe" "" "$INSTDIR\" 0  
  CreateShortCut "$SMPROGRAMS\UMTK-QML\UMTK-QML Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0 
  
  ; Write the installation path into the registry
  WriteRegStr HKLM Software\UMTK-QML "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-QML" "DisplayName" "UMTK QML"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-QML" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-QML" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-QML" "NoRepair" 1
	
  WriteUninstaller "uninstall.exe"
  
  ;make menu entrys
 
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UMTK-QML"
  DeleteRegKey HKLM "SOFTWARE\UMTK-QML"
  DeleteRegKey HKCU "Software\UMTK-QML"

  ; Remove files and uninstaller
  RMDir /r "$INSTDIR"
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\UMTK-QML\*.*"

  ; Remove directories used

SectionEnd