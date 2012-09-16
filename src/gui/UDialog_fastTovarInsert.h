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

#ifndef UDIALOG_FASTTOVARINSERT_H
#define UDIALOG_FASTTOVARINSERT_H

#include <QDialog>
#include <QSqlQuery>

#include "ui_UDialog_fastTovarInsert.h"

#include <QtDebug>

class UDialog_fastTovarInsert : public QDialog
{
    Q_OBJECT
  public:
	UDialog_fastTovarInsert(int nakladnaId, QWidget *parent = 0);
	bool fillTest();
	int newPacketId();
	~UDialog_fastTovarInsert();
  public slots:
	void populateTovGroupsComboBox();
	void pushButton_accept_clicked();
  signals:
	
  private:
	Ui::UDialog_fastTovarInsert ui;
	int nId;
	int pId;
};

#endif
