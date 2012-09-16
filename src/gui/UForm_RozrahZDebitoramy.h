//--------------------utech--------------------utech--------------------utech--------------------
//
// Copyright (C) 2005-2007 Utech Company. All rights reserved.
//
// Файл з головною функцією програми обліку товарів на складі
//
// Створений програмістами Компанії "Утех" 15.01.2008р.
// Використані компоненти бібліотеки QT4.3.1
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UFORM_ROZRAHZDEBITORAMY_H
#define UFORM_ROZRAHZDEBITORAMY_H

#include <QDialog>
#include <QSettings>
#include <QSqlQuery>
#include <QListWidgetItem>

#include "ui_UForm_RozrahZDebitoramy.h"
#include "USkladTabWidget.h"

#include <UTableWidgetSqlExt>
#include <UStandardDelegate>

#include <QtDebug>
//--------------------utech--------------------utech--------------------utech--------------------
class UForm_RozrahZDebitoramy : public USkladTabWidget
{
        Q_OBJECT

    public:
        UForm_RozrahZDebitoramy(QWidget *parent = 0);
		~UForm_RozrahZDebitoramy();
		
		void setTableWidgetOplataPartnerDelegate();
		void populateToolBar(QToolBar * tBar);
    public slots:
		
	signals:
		void requestDebugWindow();
		void closeLoginSignal();
		void mainWindowClosed();
		void mainWindowClosedThis();
    private:
		Ui::UForm_RozrahZDebitoramy ui;
		
		UTableWidgetSqlExt * twExt;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
