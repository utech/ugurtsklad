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

#ifndef UDIALOG_KATALOG_TOVARU_H
#define UDIALOG_KATALOG_TOVARU_H

#include <QDialog>
#include <QSettings>
#include <QSqlQuery>
#include <QTableWidgetItem>

#include "ui_UDialog_katalog_tovaru_sklad.h"

#include "USkladTabWidget.h"

#include <UTableWidgetSqlExt>
#include <UStandardDelegate>

#include <QtDebug>
//--------------------utech--------------------utech--------------------utech--------------------
class UDialog_katalog_tovaru : public USkladTabWidget
{
        Q_OBJECT

    public:
        UDialog_katalog_tovaru(QWidget *parent = 0);
		
		void setTableWidgetGroupTovDelegate();
		int curTovarId();
		double curTovarCina();
		void populateToolBar(QToolBar * tBar);
		double cinaTovaru(QString str);
		void createPriceFile(QDate begDate,QDate endDate,double minZamovl,int priceType,int dbVersion);
    public slots:
		void populateTovGroupsTableWidget();
		void pushButton_addGroupTov_clicked();
		void pushButton_delGroupTov_clicked();
		void tableWidget_tovGroups_itemChanged(QTableWidgetItem *item);
		void tableWidget_tovPerelik_currentItemChanged();
		void pushButton_addTov_clicked();
		void pushButton_delTov_clicked();
		void setTovGroupFilter();
		
		void action_pasteTovStrings_activated();
		void action_createPrice_activated();
	signals:
		void requestDebugWindow();
		void closeLoginSignal();
		void mainWindowClosed();
		void curTovarChanged(int tovId, double cina);
	
    private:
		Ui::UDialog_katalog_tovaru ui;
		
		int __curTovarId;
		double __curTovarCina;
		UTableWidgetSqlExt * twExt;
		QStringList tovGroupsColNames;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
