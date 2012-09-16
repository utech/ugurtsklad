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

#ifndef UFORM_VHIDNINAKLADNI_H
#define UFORM_VHIDNINAKLADNI_H

#include <QDialog>
#include <QSettings>
#include <QSqlQuery>
#include <QListWidgetItem>

#include "ui_UForm_VhidniNakladni.h"
#include "USkladTabWidget.h"

#include <UTableWidgetSqlExt>
#include <UStandardDelegate>

#include <QtDebug>
//--------------------utech--------------------utech--------------------utech--------------------
class UForm_VhidniNakladni : public USkladTabWidget
{
        Q_OBJECT

    public:
        UForm_VhidniNakladni(QWidget *parent = 0);
		~UForm_VhidniNakladni();
		
		void setTableWidgetGroupTovDelegate();
		void populateToolBar(QToolBar * tBar);
    public slots:
		void populateNakladniTable();
		void populateNakladniTableRow(int row, int id);
		void setTableWidgetPartnerDelegate();
		
		void action_openNakladna_activated();
		void action_createNakladna_activated();
		void action_deleteNakladna_activated();
		void action_printNakladna_activated();
		
		void activateTableSorting();
	signals:
		void requestDebugWindow();
		void closeLoginSignal();
		void mainWindowClosed();
		void mainWindowClosedThis();
    private:
		Ui::UForm_VhidniNakladni ui;
		
		UTableWidgetSqlExt * twExt;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
