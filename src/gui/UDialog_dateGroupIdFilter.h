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

#ifndef UDIALOG_DATEGROUPIDFILTER_H
#define UDIALOG_DATEGROUPIDFILTER_H

#include <QDialog>
#include <QSqlQuery>

#include "ui_UDialog_dateGroupIdFilter.h"

#include <QtDebug>

class UDialog_dateGroupIdFilter : public QDialog
{
    Q_OBJECT
  public:
	UDialog_dateGroupIdFilter(QWidget *parent = 0)
	{
		ui.setupUi(this);
			
		QSqlQuery query;
		ui.comboBox_groupFilter->addItem("Усі групи /усі рахунки/", 0 );
		query.exec("SELECT id, Name, Rahunok FROM tovgroups");
		while (query.next()){
			ui.comboBox_groupFilter->addItem(query.value(1).toString()+" /"+query.value(2).toString()+"/", query.value(0) );
		}
		
		QDate date = QDate::currentDate();
		ui.dateEdit_begDate->setDate(QDate(date.year(), date.month(), 1));
		ui.dateEdit_endDate->setDate(QDate(date.year(), date.month(), date.daysInMonth()));
		ui.checkBox_printNotNull->setChecked(true);
		
		connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(accept()));
		connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(reject()));
	};
	QDate begDate()
	{
		return ui.dateEdit_begDate->date();
	};
	QDate endDate()
	{
		return ui.dateEdit_endDate->date();
	};
	int groupId()
	{
		return ui.comboBox_groupFilter->itemData(ui.comboBox_groupFilter->currentIndex(),Qt::UserRole).toInt();
	};
	bool notNullOnly()
	{
		return ui.checkBox_printNotNull->isChecked();
	};
	~UDialog_dateGroupIdFilter(){};
  public slots:
		
  signals:
	
  private:
	Ui::UDialog_dateGroupIdFilter ui;
};

#endif
