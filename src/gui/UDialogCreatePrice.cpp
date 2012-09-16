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

#include <QSettings>
#include <QDir>

#include "UDialogCreatePrice.h"

//--------------------utech--------------------utech--------------------utech--------------------
UDialogCreatePrice::UDialogCreatePrice(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	QDate dt = QDate::currentDate();
	ui.dateEdit_begPriceDate->setDate(QDate(dt.year(), dt.month(), 1));
	ui.dateEdit_endPriceDate->setDate(QDate(dt.year(), dt.month(), dt.daysInMonth()));
	
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	ui.doubleSpinBox_minZamovlSum->setValue(settings->value("Price/DefaultMinZamovlSuma").toDouble());
	ui.spinBox_priceType->setValue(settings->value("Price/DefaultPriceType").toInt());
	ui.spinBox_dbVersion->setValue(settings->value("Price/DefaultDbVersion").toInt());
	delete settings;
	
	connect(ui.pushButton_createPrice, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(reject()));
}
//--------------------utech--------------------utech--------------------utech--------------------
QDate UDialogCreatePrice::begDate()
{
	return ui.dateEdit_begPriceDate->date();
}
//--------------------utech--------------------utech--------------------utech--------------------
QDate UDialogCreatePrice::endDate()
{
	return ui.dateEdit_endPriceDate->date();
}
//--------------------utech--------------------utech--------------------utech--------------------
double UDialogCreatePrice::minZamovl()
{
	return ui.doubleSpinBox_minZamovlSum->value();
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialogCreatePrice::priceType()
{
	return ui.spinBox_priceType->value();
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialogCreatePrice::dbVersion()
{
	return ui.spinBox_dbVersion->value();
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogCreatePrice::~UDialogCreatePrice()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	settings->beginGroup("Price");
	settings->setValue("DefaultMinZamovlSuma",ui.doubleSpinBox_minZamovlSum->value());
	settings->setValue("DefaultPriceType",ui.spinBox_priceType->value());
	settings->setValue("DefaultDbVersion",ui.spinBox_dbVersion->value());
	settings->endGroup();
	delete settings;
}
//--------------------utech--------------------utech--------------------utech--------------------
