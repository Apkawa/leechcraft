!include "MUI.nsh"

OutFile ../leechcraft-installer.exe
Name "LeechCraft"
SetCompressor /SOLID lzma
InstallDir "$PROGRAMFILES\Deviant\LeechCraft"
!define MUI_ABORTWARNING
!define MUI_ICON icon64.ico
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall-colorful.ico"
#!define MUI_COMPONENTSPAGE_SMALLDESC

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Deviant\LeechCraft"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "LeechCraft"

!define MUI_WELCOMEFINISHPAGE_BITMAP installscreen.bmp
!define MUI_HEADERIMAGE_BITMAP headerimage.bmp
!insertmacro MUI_PAGE_WELCOME
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
Var STARTMENU_FOLDER
!insertmacro MUI_PAGE_STARTMENU LeechCraft $STARTMENU_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!define MUI_FINISHPAGE_RUN $INSTDIR\leechcraft.exe
!define MUI_FINISHPAGE_LINK "Web site"
!define MUI_FINISHPAGE_LINK_LOCATION http://leechcraft.org
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

InstType "Full"
InstType "Minimal"

SectionGroup "Core"
	Section "LeechCraft" MAINFILES
		SetOutPath $INSTDIR
		File icon64.ico
#		File icon32.ico
#		File icon24.ico
#		File icon16.ico
		File plugininterface.dll
		File xmlsettingsdialog.dll
		File leechcraft.exe
		File /r icons
		File /r leechcraft
		File /r oxygen
		SetOutPath $INSTDIR\settings
		File settings\coresettings.xml

		WriteRegStr HKCU "Software\Deviant\LeechCraft" "" $INSTDIR
		WriteUninstaller "$INSTDIR\Uninstall.exe"
	
		!insertmacro MUI_STARTMENU_WRITE_BEGIN LeechCraft
			CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
			CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Leechcraft.lnk" "$INSTDIR\leechcraft.exe" "" "$INSTDIR\icon64.ico"
			CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
		!insertmacro MUI_STARTMENU_WRITE_END

		SectionIn 1 2 RO
	SectionEnd

	Section "Qt" QT
		SetOutPath $INSTDIR
		File QtCore4.dll
		File QtGui4.dll
		File QtNetwork4.dll
		File QtScript4.dll
		File QtSql4.dll
		File QtSvg4.dll
		File QtWebkit4.dll
		File QtXml4.dll
		File qt.conf
		File phonon4.dll
#		File phonon_ds94.dll
		SetOutPath $INSTDIR\plugins
		File /r plugins\imageformats
		File /r plugins\sqldrivers
		SectionIn 1 2 RO
	SectionEnd

	Section "OpenSSL" OPENSSL
		SetOutPath $INSTDIR
		File libeay32.dll
		File ssleay32.dll
		SectionIn 1 2 RO
	SectionEnd

	Section "MSVC" MSVC
#		SetOutPath $WINDIR
#		File /r WinSxS
#		SetOutPath $INSTDIR
		SetOutPath $TEMP
		File vcredist_x86.exe
		DetailPrint "Installing Visual C++ 2008 Libraries"
		ExecWait '"$TEMP\vcredist_x86.exe" /q:a /c:"msiexec /i vcredist.msi /quiet"'
		SectionIn 1 2 RO
	SectionEnd
SectionGroupEnd

SectionGroup "Plugins"
	Section "Aggregator" AGGREGATORPLUGIN
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_aggregator.dll
		SetOutPath $INSTDIR\settings
		File settings\aggregatorsettings.xml
		SectionIn 1
	SectionEnd
	Section "BitTorrent" TORRENTPLUGIN
		SetOutPath $INSTDIR
		File torrent.dll
		File boost_date_time-vc90-mt-1_40.dll
		File boost_filesystem-vc90-mt-1_40.dll
		File boost_system-vc90-mt-1_40.dll
		File boost_thread-vc90-mt-1_40.dll
		SetOutPath $INSTDIR\settings
		File settings\torrentsettings.xml
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_bittorrent.dll
		SectionIn 1
	SectionEnd
	Section "CSTP" HTTPPLUGIN
		SetOutPath $INSTDIR\settings
		File settings\cstpsettings.xml
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_cstp.dll
		SectionIn 1
	SectionEnd
    Section "Chatter" CHATTERPLUGIN
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_chatter.dll
		SectionIn 1
	SectionEnd
	Section "DeadLyrics" DEADLYRICSPLUGIN
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_deadlyrics.dll
		SectionIn 1
	SectionEnd
	Section "HistoryHolder" HISTORYHOLDERPLUGIN
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_historyholder.dll
		SectionIn 1
	SectionEnd
    Section "LCFTP" LCFTPPLUGIN
        SetOutPath $INSTDIR
		File libcurl.dll
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_lcftp.dll
		SectionIn 1
	SectionEnd
    Section "LMP" LMPPLUGIN
        SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_lmp.dll
        SectionIn 1
    SectionEnd
	Section "NetworkMonitor" NETWORKMONITORPLUGIN
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_networkmonitor.dll
		SectionIn 1
	SectionEnd
    Section "NewLife" NEWLIFEPLUGIN
        SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_newlife.dll
		SectionIn 1
    SectionEnd
	Section "Poshuku" POSHUKUPLUGIN
		SetOutPath $INSTDIR\settings
		File settings\poshukusettings.xml
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_poshuku.dll
		SectionIn 1
	SectionEnd
	Section "Poshuku CleanWeb" POSHUKUCLEANWEBPLUGIN
		SetOutPath $INSTDIR\settings
		File settings\poshukucleanwebsettings.xml
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_poshuku_cleanweb.dll
		SectionIn 1
	SectionEnd
	Section "Poshuku FileScheme" POSHUKUFILESCHEMEPLUGIN
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_poshuku_filescheme.dll
		SectionIn 1
	SectionEnd
	Section "Poshuku FUA" POSHUKUFUAPLUGIN
		SetOutPath $INSTDIR\settings
		File settings\poshukufuasettings.xml
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_poshuku_fua.dll
		SectionIn 1
	SectionEnd
	Section "SeekThru" SEEKTHRUPLUGIN
		SetOutPath $INSTDIR\settings
		File settings\seekthrusettings.xml
		SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_seekthru.dll
		SectionIn 1
	SectionEnd
    Section "VGrabber" VGRABBERPLUGIN
        SetOutPath $INSTDIR\plugins\bin
		File plugins\bin\leechcraft_vgrabber.dll
		SectionIn 1
    SectionEnd
SectionGroupEnd

SectionGroup "Translations"
	Section "Arabic"
		SetOutPath $INSTDIR
		File /r *_ar.qm
		SectionIn 1
	SectionEnd
    Section "Deutch"
		SetOutPath $INSTDIR
		File /r *_de_DE.qm
		SectionIn 1
	SectionEnd
    Section "Esperanto"
		SetOutPath $INSTDIR
		File /r *_eo.qm
		SectionIn 1
	SectionEnd
	Section "Italian"
		SetOutPath $INSTDIR
		File /r *_it.qm
		SectionIn 1
	SectionEnd
	Section "Russian"
		SetOutPath $INSTDIR
		File /r *_ru_RU.qm
		SectionIn 1
	SectionEnd
	Section "Ukrainian"
		SetOutPath $INSTDIR
		File /r *_uk_UA.qm
		SectionIn 1
	SectionEnd
SectionGroupEnd

Var MUI_TEMP

Section "Uninstall"
	RMDir /r "$INSTDIR"
		
	!insertmacro MUI_STARTMENU_GETFOLDER LeechCraft $MUI_TEMP
	Delete "$SMPROGRAMS\$MUI_TEMP\Leechcraft.lnk"
	Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
	
	StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
	startMenuDeleteLoop:
		ClearErrors
		RMDir $MUI_TEMP
		GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
		IfErrors startMenuDeleteLoopDone
  
		StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
	startMenuDeleteLoopDone:
	DeleteRegKey /ifempty HKCU "Software\Deviant\LeechCraft"
SectionEnd

LangString DESC_MAINFILES ${LANG_ENGLISH} "LeechCraft and its support libraries."
LangString DESC_QT ${LANG_ENGLISH} "Qt libraries."
LangString DESC_MSVC ${LANG_ENGLISH} "Microsoft Visual Studio libraries."
LangString DESC_OPENSSL ${LANG_ENGLISH} "OpenSSL support."
LangString DESC_HTTPPLUGIN ${LANG_ENGLISH} "Support for the HTTP protocol."
LangString DESC_CHATTERPLUGIN ${LANG_ENGLISH} "IRC chat."
LangString DESC_TORRENTPLUGIN ${LANG_ENGLISH} "Feature-rich BitTorrent client."
LangString DESC_AGGREGATORPLUGIN ${LANG_ENGLISH} "Sophisticated RSS/Atom news feed reader."
LangString DESC_HISTORYHOLDERPLUGIN ${LANG_ENGLISH} "Keeps history of downloaded files."
LangString DESC_LCFTPPLUGIN ${LANG_ENGLISH} "FTP client."
LangString DESC_LMPPLUGIN ${LANG_ENGLISH} "LeechCraft Media Player."
LangString DESC_NETWORKMONITORPLUGIN ${LANG_ENGLISH} "Monitors HTTP network requests."
LangString DESC_NEWLIFEPLUGIN ${LANG_ENGLISH} "Imports settings from other programs."
LangString DESC_POSHUKUPLUGIN ${LANG_ENGLISH} "Full-featured web browser."
LangString DESC_POSHUKUCLEANWEBPLUGIN ${LANG_ENGLISH} "Ad blocker for the Poshuku compatible with Firefox's AdBlock lists."
LangString DESC_POSHUKUFILESCHEMEPLUGIN ${LANG_ENGLISH} "Support of the file:// scheme for the Poshuku."
LangString DESC_POSHUKUFUAPLUGIN ${LANG_ENGLISH} "Fake User Agent plugin for the Poshuku."
LangString DESC_DEADLYRICSPLUGIN ${LANG_ENGLISH} "Fetches lyrics from LyricWiki.org."
LangString DESC_SEEKTHRUPLUGIN ${LANG_ENGLISH} "Client for OpenSearch-aware search engines."
LangString DESC_VGRABBERPLUGIN ${LANG_ENGLISH} "VKontakte.ru stuff grabber."

LangString DESC_MAINFILES ${LANG_RUSSIAN} "��� LeechCraft � ��� ��������������� ����������."
LangString DESC_QT ${LANG_RUSSIAN} "���������� Qt."
LangString DESC_MSVC ${LANG_RUSSIAN} "���������� Microsoft Visual Studio."
LangString DESC_OPENSSL ${LANG_RUSSIAN} "���������� OpenSSL."
LangString DESC_HTTPPLUGIN ${LANG_RUSSIAN} "��������� HTTP."
LangString DESC_CHATTERPLUGIN ${LANG_RUSSIAN} "���� IRC."
LangString DESC_TORRENTPLUGIN ${LANG_RUSSIAN} "������������������� Torrent-������."
LangString DESC_AGGREGATORPLUGIN ${LANG_RUSSIAN} "��������� RSS/Atom-����."
LangString DESC_HISTORYHOLDERPLUGIN ${LANG_RUSSIAN} "������ ������� �������."
LangString DESC_LCFTPPLUGIN ${LANG_RUSSIAN} "������ ��������� FTP."
LangString DESC_LMPPLUGIN ${LANG_RUSSIAN} "LeechCraft Media Player - ������������������� �������������."
LangString DESC_NETWORKMONITORPLUGIN ${LANG_RUSSIAN} "������ �� HTTP-���������."
LangString DESC_NEWLIFEPLUGIN ${LANG_RUSSIAN} "������ �������� �� ������ ��������."
LangString DESC_POSHUKUPLUGIN ${LANG_RUSSIAN} "������������������� ���-�������."
LangString DESC_POSHUKUCLEANWEBPLUGIN ${LANG_RUSSIAN} "����������� ������� ��� Poshuku, ����������� � Firefox AdBlock."
LangString DESC_POSHUKUFILESCHEMEPLUGIN ${LANG_RUSSIAN} "��������� file://-����� ��� Poshuku."
LangString DESC_POSHUKUFUAPLUGIN ${LANG_RUSSIAN} "������ ��� Poshuku, ������������� ������������� ��������."
LangString DESC_DEADLYRICSPLUGIN ${LANG_RUSSIAN} "����� ����� �� LyricWiki.org."
LangString DESC_SEEKTHRUPLUGIN ${LANG_RUSSIAN} "������ ��� �����������, �������������� OpenSearch."
LangString DESC_VGRABBERPLUGIN ${LANG_RUSSIAN} "������ ��� ���������� � ������������ ������ � ����� �� ���������� ���� � ��������."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${MAINFILES} $(DESC_MAINFILES)
	!insertmacro MUI_DESCRIPTION_TEXT ${QT} $(DESC_QT)
	!insertmacro MUI_DESCRIPTION_TEXT ${MSVC} $(DESC_MSVC)
	!insertmacro MUI_DESCRIPTION_TEXT ${OPENSSL} $(DESC_OPENSSL)
	!insertmacro MUI_DESCRIPTION_TEXT ${HTTPPLUGIN} $(DESC_HTTPPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${AGGREGATORPLUGIN} $(DESC_AGGREGATORPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${TORRENTPLUGIN} $(DESC_TORRENTPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${LMPPLUGIN} $(DESC_LMPPLUGIN)
    !insertmacro MUI_DESCRIPTION_TEXT ${NEWLIFEPLUGIN} $(DESC_NEWLIFEPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${POSHUKUPLUGIN} $(DESC_POSHUKUPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${POSHUKUFILESCHEMEPLUGIN} $(DESC_POSHUKUFILESCHEMEPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${POSHUKUFUAPLUGIN} $(DESC_POSHUKUFUAPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${POSHUKUCLEANWEBPLUGIN} $(DESC_POSHUKUCLEANWEBPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${DEADLYRICSPLUGIN} $(DESC_DEADLYRICSPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEEKTHRUPLUGIN} $(DESC_SEEKTHRUPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${HISTORYHOLDERPLUGIN} $(DESC_HISTORYHOLDERPLUGIN)
	!insertmacro MUI_DESCRIPTION_TEXT ${NETWORKMONITORPLUGIN} $(DESC_NETWORKMONITORPLUGIN)
    !insertmacro MUI_DESCRIPTION_TEXT ${CHATTERPLUGIN} $(DESC_CHATTERPLUGIN)
    !insertmacro MUI_DESCRIPTION_TEXT ${LCFTPPLUGIN} $(DESC_LCFTPPLUGIN)
    !insertmacro MUI_DESCRIPTION_TEXT ${VGRABBERPLUGIN} $(DESC_VGRABBERPLUGIN)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd