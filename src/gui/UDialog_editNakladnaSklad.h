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

#ifndef UDialog_editNakladnaSklad_H
#define UDialog_editNakladnaSklad_H

#include <QDialog>
#include <QSettings>
#include <QSqlQuery>

#include "ui_UDialog_editNakladnaSklad.h"

#include <QtDebug>

class UDialog_editNakladnaSklad : public QDialog
{
        Q_OBJECT
    public:
        UDialog_editNakladnaSklad(int nakladnaId, QWidget *parent = 0);
		~UDialog_editNakladnaSklad();
		void setNakladnaNumText(QString text);
		QString nakladnaNumText();
		void setPostachalnykID(int id);
		int postachalnykID();
		void setNakladnaCreateDate(QDate date);
		QDate nakladnaCreateDate();
		void connectHeaderEditors();
		void disconnectHeaderEditors();
		void activationOfControlButtons();
    public slots:
		void populateNakladnaHeader();
		void populateTableWidget();
		void populateRow(int row, QSqlRecord & rec, bool disconcon=false);
		void pushButton_accept_clicked();
		void pushButton_reject_clicked();
		void pushButton_add_tovar_str_clicked();
		void pushButton_addTovarDovNakl_clicked();
		void pushButton_del_tovar_str_clicked();
		void pushButton_printNakladna_clicked();
		void showNakladnaSum();
		void tableWidget_cellChanged(int row, int col);
		void setTableWidgetTovarDelegate();
		void populatePartnersComboBox();
		void headerEdited();
		void activateTableSorting();
	signals:
	
    private:
		Ui::UDialog_editNakladnaSklad ui;
		int nId;
		QStringList paketsTableColNamesList;
		QHash<int,int> postachHash;
};

#endif
