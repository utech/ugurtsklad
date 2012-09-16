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

#ifndef UDIALOGCREATEPRICE_H
#define UDIALOGCREATEPRICE_H

#include <QDialog>
#include <QSqlQuery>

#include "ui_UDialogCreatePrice.h"

#include <QtDebug>

class UDialogCreatePrice : public QDialog
{
    Q_OBJECT
  public:
	UDialogCreatePrice(QWidget *parent = 0);
	
	QDate begDate();
	QDate endDate();
	double minZamovl();
	int priceType();
	int dbVersion();
	
	~UDialogCreatePrice();
  public slots:
	
  signals:
	
  private:
	Ui::UDialogCreatePrice ui;
};

#endif
