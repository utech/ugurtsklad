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

#ifndef UDIALOG_EDITVYHNAKLADNASKLAD_H
#define UDIALOG_EDITVYHNAKLADNASKLAD_H

#include <QDialog>
#include <QSettings>
#include <QSqlQuery>

#include "ui_UDialog_editVyhNakladnaSklad.h"

#include <QtDebug>

class UDialog_editVyhNakladnaSklad : public QDialog
{
        Q_OBJECT
    public:
        UDialog_editVyhNakladnaSklad(int nakladnaId, QWidget *parent = 0);
		~UDialog_editVyhNakladnaSklad();
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
		void pushButton_accept_clicked();
		void pushButton_reject_clicked();
		void pushButton_add_tovar_str_clicked();
		void pushButton_del_tovar_str_clicked();
		void pushButton_printNakladna_clicked();
		void showNakladnaSum();
		void tableWidget_cellChanged(int row, int col);
		void tableWidget_currentRowChanged();
		void setTableWidgetTovarDelegate();
		void populatePartnersComboBox();
		void headerEdited();
		void activateTableSorting();
	signals:
	
    private:
		Ui::UDialog_editVyhNakladnaSklad ui;
		int nId;
		QHash<int,int> postachHash;
		double __curTovarCina;
		double __curTovarKilkist;
};

#endif
