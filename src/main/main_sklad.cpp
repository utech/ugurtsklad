//--------------------------------------------------------------------------------------------------------
//
// Copyright (C) 2005-2007 Utech Company. All rights reserved.
//
// Файл з головною функцією програми обліку товарів на складі
//
// Створений програмістами Компанії "Утех" 15.01.2008р.
// Використані компоненти бібліотеки QT4.3.1
//
//--------------------------------------------------------------------------------------------------------
#include <QApplication>
#include <QSplashScreen>
#include <QTextCodec>


#include "USklad.h"
#include "UMainWindow_gurt_sklad.h"
#include <UDebugForm>

//--------------------utech--------------------utech--------------------utech--------------------
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
	
	UDebug::createDebugForm();
	if (USklad::connectToDbSklad()){
		UMainWindow_gurt_sklad *mainWin = new UMainWindow_gurt_sklad();
		mainWin->show();
		QApplication::connect(mainWin, SIGNAL(requestDebugWindow()), UDebug::debugFormPointer, SLOT(showDebug()));
		QApplication::connect(mainWin, SIGNAL(mainWindowClosed()), &app, SLOT(quit()));
		return app.exec();
	}
	else
		return 0;
}
//--------------------utech--------------------utech--------------------utech--------------------
