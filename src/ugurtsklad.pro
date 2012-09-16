CONFIG		+= release
CONFIG		-= debug debug_and_release debug_and_release_target

TEMPLATE = app
TARGET = 
DEPENDPATH += $$PWD/. $$PWD/.. $$PWD/debug_form $$PWD/gui $$PWD/images $$PWD/main
INCLUDEPATH += $$PWD/. $$PWD/.. $$PWD/debug_form $$PWD/gui $$PWD/images $$PWD/main

QT +=   sql network

include(../../UtechLib/UtechLib.pri)

# Input
HEADERS += UDialog_editNakladnaSklad.h \
           UMainWindow_gurt_sklad.h \
		   UDialog_katalog_tovaru_sklad.h \
		   UForm_VhidniNakladni.h \
		   UForm_VyhidniNakladni.h \
		   UForm_Partnery_sklad.h \
		   UPrintDocs_Sklad.h \
		   UGlobal_Sklad.h \
		   UDialog_editVyhNakladnaSklad.h \
		   UDialog_VnesTovaruVyhNakl_sklad.h \
		   USkladTabWidget.h \
		   USkladProgOptions.h \
		   UDialog_dateGroupIdFilter.h \
		   UDialog_zbutFilter.h \
		   UDialogFirmInfo_sklad.h \
		   UDialog_fastTovarInsert.h \
		   UForm_RozrahZDebitoramy.h \
		   USklad.h \
		   UDialogCreatePrice.h
FORMS += UDialog_editNakladnaSklad.ui \
         UMainWindow_gurt_sklad.ui \
		 UDialog_katalog_tovaru_sklad.ui \
		 UForm_VhidniNakladni.ui \
		 UForm_VyhidniNakladni.ui \
		 UForm_Partnery_sklad.ui \
		 UDialog_editVyhNakladnaSklad.ui \
		 UDialog_VnesTovaruVyhNakl_sklad.ui \
		 UDialog_dateGroupIdFilter.ui \
		 UDialog_zbutFilter.ui \
		 UDialogFirmInfo_sklad.ui \
		 UDialog_fastTovarInsert.ui \
		 UForm_RozrahZDebitoramy.ui \
		 UDialogCreatePrice.ui
SOURCES += main_sklad.cpp \
           UDialog_editNakladnaSklad.cpp \
           UMainWindow_gurt_sklad.cpp \
		   UDialog_katalog_tovaru_sklad.cpp \
		   UForm_VhidniNakladni.cpp \
		   UForm_VyhidniNakladni.cpp \
		   UForm_Partnery_sklad.cpp \
		   UPrintDocs_Sklad.cpp \
		   UDialog_editVyhNakladnaSklad.cpp \
		   UDialog_VnesTovaruVyhNakl_sklad.cpp \
		   USkladProgOptions.cpp \
		   UDialog_zbutFilter.cpp \
		   UDialogFirmInfo_sklad.cpp \
		   UDialog_fastTovarInsert.cpp \
		   UForm_RozrahZDebitoramy.cpp \
		   USklad.cpp \
		   UDialogCreatePrice.cpp
RESOURCES += images_sklad.qrc #../../images/UAutoParkImg.qrc

#CONFIG +=console

RC_FILE +=   prog_icon.rc
DESTDIR +=   ../bin

OBJECTS_DIR = ../tmp
MOC_DIR = ../tmp
UI_HEADERS_DIR = ../tmp
UI_SOURCES_DIR = ../tmp
RCC_DIR = ../tmp