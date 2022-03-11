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
OutFile "..\UMKT-Classic Installer.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\UMTK-Classic"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\UMTK-Classic" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Set License info
LicenseData "C:\BuildDir\UMTK-Classic\umtkclassic\GPLv3.txt"
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
Section "UMTK Classic(Required)"

  SectionIn RO
  SetOutPath $INSTDIR
  ; Set output path to the installation directory.
  File /r "C:\BuildDir\UMTK-Classic\umtkclassic\*"
  ; Put file there
  ;make menu entrys for main program else workpath is wrong?
  CreateDirectory "$SMPROGRAMS\UMTK-Classic"
  CreateShortCut "$SMPROGRAMS\UMTK-Classic\UMTK-Classic.lnk" "$INSTDIR\UMTK-Classic.exe" "" "$INSTDIR\A.ico" 0
  CreateShortcut "$DESKTOP\UMTK-Classic.lnk" "$INSTDIR\UMTK-Classic.exe" "" "$INSTDIR\A.ico" 0
  CreateShortCut "$SMPROGRAMS\UMTK-Classic\UMTK-QML Classic.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0 
  
  ; Write the installation path into the registry
  WriteRegStr HKLM Software\UMTK-Classic "Install_Dir" "$INSTDIR"
  
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
  DeleteRegKey HKLM "SOFTWARE\UMTK-Classic"
  DeleteRegKey HKCU "Software\UMTK-Classic"

  ; Remove files and uninstaller
  RMDir /r "$INSTDIR"
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\UMTK-Classic\*.*"
  Delete "$DESKTOP\UMTK-Classic.lnk"

  ; Remove directories used

SectionEnd