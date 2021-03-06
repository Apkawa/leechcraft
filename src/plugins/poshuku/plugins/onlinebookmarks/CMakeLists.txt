SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
FIND_PACKAGE (QJSON REQUIRED)

INCLUDE_DIRECTORIES (${POSHUKU_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${QJSON_INCLUDE_DIR}
	)

SET (ONLINEBOOKMARKS_SRCS
	onlinebookmarks.cpp
	xmlsettingsmanager.cpp
	settings.cpp
	syncbookmarks.cpp
	core.cpp
	bookmarksdialog.cpp
	readitlater/readitlaterbookmarksservice.cpp
	)
SET (ONLINEBOOKMARKS_HEADERS
	onlinebookmarks.h
	xmlsettingsmanager.h
	abstractbookmarksservice.h
	settings.h
	syncbookmarks.h
	bookmarksdialog.h
	core.h
	readitlater/readitlaterbookmarksservice.h
	)
SET (ONLINEBOOKMARKS_FORMS
	accounts.ui
	bookmarksdialog.ui
	)
SET (ONLINEBOOKMARKS_RESOURCES poshukuonlinebookmarksresources.qrc
	readitlater/onlinebookmarksreaditlater.qrc
	)
SET (ONLINEBOOKMARKS_TRANSLATIONS
	leechcraft_poshuku_onlinebookmarks_ru_RU.ts
	leechcraft_poshuku_onlinebookmarks_en.ts
	)
SET (ONLINEBOOKMARKS_COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_onlinebookmarks_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_onlinebookmarks_en.qm
	)

QT4_WRAP_CPP (ONLINEBOOKMARKS_MOC_SRCS ${ONLINEBOOKMARKS_HEADERS})
QT4_WRAP_UI (ONLINEBOOKMARKS_UIS_H ${ONLINEBOOKMARKS_FORMS})
QT4_ADD_RESOURCES (ONLINEBOOKMARKS_RCCS ${ONLINEBOOKMARKS_RESOURCES})

ADD_CUSTOM_COMMAND (OUTPUT
	${ONLINEBOOKMARKS_COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${ONLINEBOOKMARKS_TRANSLATIONS}
	DEPENDS ${ONLINEBOOKMARKS_TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_poshuku_onlinebookmarks SHARED
	${ONLINEBOOKMARKS_COMPILED_TRANSLATIONS}
	${ONLINEBOOKMARKS_SRCS}
	${ONLINEBOOKMARKS_MOC_SRCS}
	${ONLINEBOOKMARKS_UIS_H}
	${ONLINEBOOKMARKS_RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_poshuku_onlinebookmarks
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	qjson
	)
INSTALL (TARGETS leechcraft_poshuku_onlinebookmarks DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${ONLINEBOOKMARKS_COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
INSTALL (FILES poshukuonlinebookmarkssettings.xml DESTINATION ${LC_SETTINGS_DEST})
