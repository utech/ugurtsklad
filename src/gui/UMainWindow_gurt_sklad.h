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

#ifndef UMAINWINDOW_GURT_SKLAD_H
#define UMAINWINDOW_GURT_SKLAD_H

#include <QMainWindow>
#include <QSettings>
#include <QDate>

#include "ui_UMainWindow_gurt_sklad.h"

#include <QtDebug>

class UMainWindow_gurt_sklad : public QMainWindow
{
        Q_OBJECT

    public:
        UMainWindow_gurt_sklad(QWidget *parent = 0);
		 
		~UMainWindow_gurt_sklad();
    public slots:
		void request_debuger_window();
		void tabWidget_sklad_currentChanged();
		void action_printObigIZalyshky_activated();
		void action_printVidomistZbutuExt_activated();
		void action_options_activated();
		void action_about_programm_activated();
	signals:
		void requestDebugWindow();
		void mainWindowClosed();
	
    private:
		Ui::UMainWindow_gurt_sklad ui;
		QDate date;
};
#endif
