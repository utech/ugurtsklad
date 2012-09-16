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

#include "UDialog_zbutFilter.h"

//--------------------utech--------------------utech--------------------utech--------------------
UDialog_zbutFilter::UDialog_zbutFilter(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	QDate date = QDate::currentDate();
	ui.dateEdit_begDate->setDate(QDate(date.year(), date.month(), 1));
	ui.dateEdit_endDate->setDate(QDate(date.year(), date.month(), date.daysInMonth()));
	
	populateTovarGroupFilterComboBox();
	populateTovarCombobox();
	populateKlientComboBox();
	populateNakladnaNumComboBox();
	
	connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(reject()));
	
	connect(ui.dateEdit_begDate, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateKlientComboBox()));
	connect(ui.dateEdit_begDate, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateNakladnaNumComboBox()));
	connect(ui.dateEdit_endDate, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateKlientComboBox()));
	connect(ui.dateEdit_endDate, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateNakladnaNumComboBox()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_zbutFilter::populateTovarGroupFilterComboBox()
{
	disconnect(ui.comboBox_groupFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(populateTovarCombobox()));
	
	int prevId = ui.comboBox_groupFilter->itemData(ui.comboBox_groupFilter->currentIndex(), Qt::UserRole).toInt();
	ui.comboBox_groupFilter->clear();
	ui.comboBox_groupFilter->addItem("Усі", 0);
	
	QSqlQuery query;
	query.exec("SELECT id, Name, Rahunok FROM tovgroups");
	while (query.next()){
		ui.comboBox_groupFilter->addItem(query.value(1).toString()+" /"+query.value(2).toString()+"/", query.value(0));
		if (query.value(0).toInt() == prevId)
			ui.comboBox_groupFilter->setCurrentIndex(ui.comboBox_groupFilter->count()-1);
	}
	
	connect(ui.comboBox_groupFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(populateTovarCombobox()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_zbutFilter::populateTovarCombobox()
{
	int prevId = ui.comboBox_tovarId->itemData(ui.comboBox_groupFilter->currentIndex(), Qt::UserRole).toInt();
	ui.comboBox_tovarId->clear();
	ui.comboBox_tovarId->addItem("Усі", 0);
	
	QSqlQuery query;
	int curGroup = groupId();
	query.exec("SELECT id, Nomer, Name, Odynyci FROM skladtke.tovperelik "+(curGroup>0?QString(" WHERE GroupId=")+QString::number(groupId()):QString("")));
	while (query.next()){
		ui.comboBox_tovarId->addItem(query.value(1).toString()+", "+query.value(2).toString()+" /"+query.value(0).toString()+"/", query.value(0));
		if (query.value(0).toInt() == prevId)
			ui.comboBox_tovarId->setCurrentIndex(ui.comboBox_tovarId->count()-1);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_zbutFilter::populateKlientComboBox()
{
	int prevId = ui.comboBox_klientFilter->itemData(ui.comboBox_klientFilter->currentIndex(), Qt::UserRole).toInt();
	ui.comboBox_klientFilter->clear();
	ui.comboBox_klientFilter->addItem("Усі", 0);
	
	QSqlQuery query;
	query.exec("SELECT dovPartners.id, dovPartners.Name FROM nakladnaVyh \
				LEFT JOIN dovPartners ON dovPartners.id=nakladnaVyh.Partner_id \
				WHERE (nakladnaVyh.CDate BETWEEN date('"+ui.dateEdit_begDate->date().toString("yyyy-MM-dd")+"') \
						AND date('"+ui.dateEdit_endDate->date().toString("yyyy-MM-dd")+"')) \
					and dovPartners.id is not null \
				GROUP BY dovPartners.id \
				ORDER BY dovPartners.Name");
	while (query.next()){
		ui.comboBox_klientFilter->addItem(query.value(1).toString(), query.value(0));
		if (query.value(0).toInt() == prevId)
			ui.comboBox_klientFilter->setCurrentIndex(ui.comboBox_klientFilter->count()-1);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_zbutFilter::populateNakladnaNumComboBox()
{
	int prevId = ui.comboBox_nakladnaNumberFilter->itemData(ui.comboBox_nakladnaNumberFilter->currentIndex(), Qt::UserRole).toInt();
	ui.comboBox_nakladnaNumberFilter->clear();
	ui.comboBox_nakladnaNumberFilter->addItem("Усі", 0);
	
	QSqlQuery query;
	query.exec("SELECT nakladnaVyh.id, nakladnaVyh.Num, dovPartners.Name FROM nakladnaVyh \
				LEFT JOIN dovPartners ON dovPartners.id=nakladnaVyh.Partner_id \
				WHERE nakladnaVyh.CDate BETWEEN date('"+ui.dateEdit_begDate->date().toString("yyyy-MM-dd")+"') \
					AND date('"+ui.dateEdit_endDate->date().toString("yyyy-MM-dd")+"') \
				ORDER BY nakladnaVyh.Num");
	while (query.next()){
		ui.comboBox_nakladnaNumberFilter->addItem(query.value(1).toString()+" - "+query.value(2).toString(), query.value(0));
		if (query.value(0).toInt() == prevId)
			ui.comboBox_nakladnaNumberFilter->setCurrentIndex(ui.comboBox_nakladnaNumberFilter->count()-1);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
QDate UDialog_zbutFilter::begDate()
{
	return ui.dateEdit_begDate->date();
}
//--------------------utech--------------------utech--------------------utech--------------------
QDate UDialog_zbutFilter::endDate()
{
	return ui.dateEdit_endDate->date();
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_zbutFilter::tovarId()
{
	return ui.comboBox_tovarId->itemData(ui.comboBox_tovarId->currentIndex(), Qt::UserRole).toInt();
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_zbutFilter::groupId()
{
	return ui.comboBox_groupFilter->itemData(ui.comboBox_groupFilter->currentIndex(), Qt::UserRole).toInt();
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_zbutFilter::klientId()
{
	return ui.comboBox_klientFilter->itemData(ui.comboBox_klientFilter->currentIndex(), Qt::UserRole).toInt();
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_zbutFilter::nakladnaId()
{
	return ui.comboBox_nakladnaNumberFilter->itemData(ui.comboBox_nakladnaNumberFilter->currentIndex(), Qt::UserRole).toInt();
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialog_zbutFilter::~UDialog_zbutFilter()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
