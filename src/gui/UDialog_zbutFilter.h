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

#ifndef UDIALOG_ZBUTFILTER_H
#define UDIALOG_ZBUTFILTER_H

#include <QDialog>
#include <QSqlQuery>

#include "ui_UDialog_zbutFilter.h"

#include <QtDebug>

class UDialog_zbutFilter : public QDialog
{
    Q_OBJECT
  public:
	UDialog_zbutFilter(QWidget *parent = 0);

	QDate begDate();
	QDate endDate();
	int tovarId();
	int groupId();
	int klientId();
	int nakladnaId();
	~UDialog_zbutFilter();
  public slots:
	void populateTovarCombobox();
	void populateTovarGroupFilterComboBox();
	void populateKlientComboBox();
	void populateNakladnaNumComboBox();
  signals:
	
  private:
	Ui::UDialog_zbutFilter ui;
};

#endif
