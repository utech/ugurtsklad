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

#include <QMessageBox>
#include <QSqlError>
#include <USqlAccessible>
#include "UDialog_fastTovarInsert.h"

//--------------------utech--------------------utech--------------------utech--------------------
UDialog_fastTovarInsert::UDialog_fastTovarInsert(int nakladnaId, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	populateTovGroupsComboBox();
	
	nId = nakladnaId;
	
	connect(ui.pushButton_accept, SIGNAL(clicked(bool)), this, SLOT(pushButton_accept_clicked()));
	connect(ui.pushButton_reject, SIGNAL(clicked(bool)), this, SLOT(reject()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_fastTovarInsert::populateTovGroupsComboBox()
{
	QSqlQuery query;
	ui.comboBox_tovarGroup->clear();
	query.exec("SELECT id, Name, Rahunok FROM tovgroups ORDER BY Name");
	while (query.next()){
		ui.comboBox_tovarGroup->addItem(query.value(1).toString()+" /"+query.value(2).toString()+"/", query.value(0));
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_fastTovarInsert::pushButton_accept_clicked()
{
	if (!fillTest())
		return;
	
	QSqlQuery query;
	if (!query.exec("INSERT INTO tovperelik (GroupId, Nomer, Name, Odynyci, Cina, Prymitka) \
				VALUES ("+sqlStr(ui.comboBox_tovarGroup->itemData(ui.comboBox_tovarGroup->currentIndex(),Qt::UserRole).toInt())+", \
						"+sqlStr(ui.lineEdit_invNomer->text())+", \
						"+sqlStr(ui.lineEdit_nazvaTovaru->text())+", \
						"+sqlStr(ui.lineEdit_odynyci->text())+", \
						"+sqlStr(ui.doubleSpinBox__cinaProdaju->text().replace(',','.'))+", \
						"+sqlStr(ui.lineEdit_prymitka->text())+")")){
		QMessageBox::critical(0,"Помилка внесення товару до довідника", "Програма не змогла внести стрічку товару\nПричина: "+query.lastError().text());
		return;
	}
	query.exec("SELECT last_insert_id()");
	query.next();
	int tovId = query.value(0).toInt();
	
	if (!query.exec("INSERT INTO skladpakety (NakladnaVh_id, TovOrder, Tovar_id, Kilkist, Cina, Closed) \
					VALUES ("+sqlStr(nId)+", \
							0, \
							"+sqlStr(tovId)+", \
							"+sqlStr(ui.doubleSpinBox_kilkist->text().replace(',','.'))+", \
							"+sqlStr(ui.doubleSpinBox_cinaZakup->text().replace(',','.'))+", \
							false)"))
		QMessageBox::information(0,"Помилка внесення товару до накладної", "Програма не змогла внести товар до накладної,\nале його було успішно внесено до довідника товарів.\n!!!Внесіть товар до накладної вручну!!!");
	qDebug() << query.lastQuery();
	
	query.exec("SELECT last_insert_id()");
	query.next();
	pId = query.value(0).toInt();
	
	accept();
}
//--------------------utech--------------------utech--------------------utech--------------------
bool UDialog_fastTovarInsert::fillTest()
{
	if (ui.comboBox_tovarGroup->itemData(ui.comboBox_tovarGroup->currentIndex(), Qt::UserRole).toInt() == 0){
		QMessageBox::critical(0,"Помилка заповнення форми", "Необхідно обрати групу товару");
		return false;
	}
	
	if (ui.lineEdit_nazvaTovaru->text().size() < 3){
		QMessageBox::critical(0,"Помилка заповнення форми", "Назва товару повинна містити більше двох символів");
		return false;
	}
	
	if (ui.lineEdit_odynyci->text().size() < 1){
		QMessageBox::critical(0,"Помилка заповнення форми", "Одиниці товару повинні містити хоча б один символ");
		return false;
	}
	
	if (ui.doubleSpinBox_kilkist->value() == 0){
		QMessageBox::critical(0,"Помилка заповнення форми", "Внесіть ненульову кількість товару");
		return false;
	}
	
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_fastTovarInsert::newPacketId()
{
	return pId;
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialog_fastTovarInsert::~UDialog_fastTovarInsert()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
