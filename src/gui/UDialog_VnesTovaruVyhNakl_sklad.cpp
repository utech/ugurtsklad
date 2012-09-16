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

#include <QToolButton>
#include <QMessageBox>

#include "UDialog_VnesTovaruVyhNakl_sklad.h"
#include "UPrintDocs_Sklad.h"
#include <UStandardDelegate.h>
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UDialog_VnesTovaruVyhNakl_sklad::UDialog_VnesTovaruVyhNakl_sklad(int nakladnaId, QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	nId = nakladnaId;
	ui.tableWidget_zalyshky->setColumnHidden(0,true);
	
	ui.tableWidget_zalyshky->setColumnWidth(1,80);
	ui.tableWidget_zalyshky->setColumnWidth(2,80);
	ui.tableWidget_zalyshky->setColumnWidth(3,70);
	ui.tableWidget_zalyshky->setColumnWidth(4,70);
	
	//Ініціалізація вибору цінитовару
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	ui.checkBox_zakupCina->setChecked(settings->value("Zbut/PoZakupCini").toBool());
	delete settings;
	
	dKatalogTovaru = new UDialog_katalog_tovaru(this);
	connect(dKatalogTovaru, SIGNAL(curTovarChanged(int, double)), this, SLOT(dKatalogTovaru_curTovarChanged(int, double)));
	dKatalogTovaru_curTovarChanged(dKatalogTovaru->curTovarId(), dKatalogTovaru->curTovarCina());
	ui.verticalLayout_tovPerelikConteiner->addWidget(dKatalogTovaru);
	
	activateCinaSpinbox();
	
	connect(ui.pushButton_accept, SIGNAL(clicked(bool)), this, SLOT(pushButton_accept_clicked()));
	connect(ui.pushButton_reject, SIGNAL(clicked(bool)), this, SLOT(pushButton_reject_clicked()));
	
	connect(ui.doubleSpinBox_kilkist, SIGNAL(valueChanged(double)), this, SLOT(calcTovVartist()));
	connect(ui.doubleSpinBox_cina, SIGNAL(valueChanged(double)), this, SLOT(calcTovVartist()));
	
	connect(ui.checkBox_zakupCina, SIGNAL(stateChanged(int)), this, SLOT(activateCinaSpinbox()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_VnesTovaruVyhNakl_sklad::pushButton_accept_clicked()
{
	if (ui.doubleSpinBox_kilkist->value()==0 && ui.doubleSpinBox_cina->value()==0)
		reject();
	USkladskyiRuhTovaru::vydatyTovar(nId, dKatalogTovaru->curTovarId(), ui.doubleSpinBox_kilkist->value(), ui.doubleSpinBox_cina->value(), ui.checkBox_zakupCina->isChecked());
	accept();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_VnesTovaruVyhNakl_sklad::pushButton_reject_clicked()
{
	reject();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_VnesTovaruVyhNakl_sklad::calcTovVartist()
{
	double sum = uDToM(ui.doubleSpinBox_kilkist->value() * ui.doubleSpinBox_cina->value());
	ui.doubleSpinBox_suma->setValue(sum);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_VnesTovaruVyhNakl_sklad::dKatalogTovaru_curTovarChanged(int tovId, double cina)
{
	curTovarId = tovId;
	activateNakladnaControls(tovId);
	populateZalyshkyTable(tovId);
	
	if (tovId == 0){
		ui.doubleSpinBox_zalyshok->setValue(0);
		ui.doubleSpinBox_cina->setValue(0);
	}
	else{
		ui.doubleSpinBox_zalyshok->setValue(USkladskyiRuhTovaru::calcTovZalyshky(tovId));
		ui.doubleSpinBox_cina->setValue(cina);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_VnesTovaruVyhNakl_sklad::activateNakladnaControls(int tovId)
{
	bool active;
	if (tovId == 0)
		active = false;
	else
		active = true;
	
	ui.doubleSpinBox_zalyshok->setEnabled(active);
	ui.doubleSpinBox_kilkist->setEnabled(active);
	ui.doubleSpinBox_cina->setEnabled(active & (!ui.checkBox_zakupCina->isChecked()));
	ui.doubleSpinBox_suma->setEnabled(active);
	ui.pushButton_accept->setEnabled(active);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_VnesTovaruVyhNakl_sklad::activateCinaSpinbox()
{
	ui.doubleSpinBox_cina->setEnabled(!ui.checkBox_zakupCina->isChecked() && (curTovarId!=0?true:false));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_VnesTovaruVyhNakl_sklad::populateZalyshkyTable(int tovarId)
{
	//Нульовий товар ІД
	if (tovarId == 0){
		ui.tableWidget_zalyshky->setRowCount(0);
		return;
	}
	
	QSqlQuery query;
	query.exec("SELECT s.id, s.Kilkist, s.Cina, nakladnaVh.CDate, nakladnaVh.Num, v.Kilk \
				FROM ((SELECT * FROM skladPakety WHERE Tovar_id="+sqlStr(tovarId)+" and Closed<>true) AS s \
				LEFT JOIN (SELECT sum(Kilkist) AS Kilk, Paket_id FROM vidpuskTovariv GROUP BY Paket_id) AS v \
					ON v.Paket_id=s.id) \
				LEFT JOIN nakladnaVh \
					ON nakladnaVh.id=s.NakladnaVh_id \
				WHERE (s.Kilkist-if(v.Kilk is null,0,v.Kilk))>0 \
				ORDER BY nakladnaVh.CDate, nakladnaVh.id");
	int rowCount = query.size();
	ui.tableWidget_zalyshky->setRowCount(rowCount);
	QTableWidgetItem * item;
	for (int row=0; row<rowCount && query.next(); row++){
		//id
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(0));
		ui.tableWidget_zalyshky->setItem(row, 0, item);
		//Кількість
		item = new QTableWidgetItem(QString::number(query.value(1).toDouble()-query.value(5).toDouble()).replace('.',','));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_zalyshky->setItem(row, 1, item);
		//Ціна
		item = new QTableWidgetItem(query.value(2).toString().replace('.',','));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_zalyshky->setItem(row, 2, item);
		//Дата
		item = new QTableWidgetItem(query.value(3).toDate().toString("dd.MM.yyyy"));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_zalyshky->setItem(row, 3, item);
		//Накладна
		item = new QTableWidgetItem(query.value(4).toString());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget_zalyshky->setItem(row, 4, item);
	}
	ui.tableWidget_zalyshky->resizeRowsToContents();
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialog_VnesTovaruVyhNakl_sklad::~UDialog_VnesTovaruVyhNakl_sklad()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	settings->setValue("Zbut/PoZakupCini", ui.checkBox_zakupCina->isChecked());
	delete settings;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USkladskyiRuhTovaru::vydatyTovar(const int nId, const int tovId, const double kilkist, const double cina, bool zakupCina)
{
	double xKilkist = kilkist;
	QSqlQuery query, insQuery;
	query.exec("SELECT s.id, s.Kilkist, v.Kilk, s.Cina \
				FROM ((SELECT * FROM skladPakety WHERE Tovar_id="+QString::number(tovId)+" and Closed<>true) AS s \
				LEFT JOIN (SELECT sum(Kilkist) AS Kilk, Paket_id FROM vidpuskTovariv GROUP BY Paket_id) AS v \
					ON v.Paket_id=s.id) \
				LEFT JOIN nakladnaVh \
					ON nakladnaVh.id=s.NakladnaVh_id \
				WHERE (s.Kilkist-if(v.Kilk is null,0,v.Kilk))>0 \
				ORDER BY nakladnaVh.CDate, nakladnaVh.id");
	while (query.next()){
		double curKilkist = query.value(1).toDouble() - query.value(2).toDouble();
		if (xKilkist < curKilkist)
			curKilkist = xKilkist;
		xKilkist -= curKilkist;
		double curCina = query.value(3).toDouble();
		
		insQuery.exec("INSERT INTO vidpuskTovariv (NakladnaVyh_id, Paket_id, Kilkist, Cina, Closed) VALUES \
						("+QString::number(nId)+", \
						"+query.value(0).toString()+", \
						"+QString::number(curKilkist)+", \
						"+QString::number((zakupCina?curCina:cina))+", \
						"+(zakupCina?QString("true"):QString("false"))+")");
		
		if (xKilkist<=0)
			break;
	}
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USkladskyiRuhTovaru::unVydatyTovar(const int nId, const int tovId, const double kilkist, const double cina)
{
	double xKilkist = kilkist;
	QSqlQuery query, delQuery;
	query.exec("SELECT v.id, v.Kilkist FROM \
					((SELECT * FROM vidpuskTovariv \
						WHERE NakladnaVyh_id="+QString::number(nId)+" and Cina="+QString::number(cina)+") AS v \
				LEFT JOIN \
					(SELECT * FROM skladPakety \
						WHERE Tovar_id="+QString::number(tovId)+") AS s ON v.Paket_id=s.id) \
				LEFT JOIN nakladnaVyh ON nakladnaVyh.id=v.NakladnaVyh_id \
				ORDER BY nakladnaVyh.CDate DESC, v.id ASC");
	while (query.next()){
		double curKilkist = query.value(1).toDouble();
		if (xKilkist < curKilkist){ //Need update
			delQuery.exec("UPDATE vidpuskTovariv SET Kilkist="+QString::number(curKilkist-xKilkist)+" \
						WHERE id="+query.value(0).toString());
			xKilkist = 0;
		}
		else{ //Need delete
			delQuery.exec("DELETE FROM vidpuskTovariv WHERE id="+query.value(0).toString());
			xKilkist -= curKilkist;
		}
		
		if (xKilkist <= 0)
			break;
	}
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USkladskyiRuhTovaru::changeTovarCina(const int nId, const int tovId, const double prevCina, const double curCina)
{
	QSqlQuery query, updQuery;
	query.exec("SELECT vidpuskTovariv.id FROM vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id \
				WHERE vidpuskTovariv.NakladnaVyh_id="+QString::number(nId)+" \
					and vidpuskTovariv.Cina="+QString::number(prevCina)+" \
					and skladPakety.Tovar_id="+QString::number(tovId));
	while (query.next()){
		updQuery.exec("UPDATE vidpuskTovariv SET Cina="+QString::number(curCina)+" \
						WHERE id="+query.value(0).toString());
	}
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USkladskyiRuhTovaru::changeTovarKilkist(const int nId, const int tovId, const double prevKilkist, const double curKilkist, const double cina)
{
	if (prevKilkist == curKilkist)
		return true;
	else if (prevKilkist < curKilkist)
		USkladskyiRuhTovaru::vydatyTovar(nId, tovId, curKilkist-prevKilkist, cina);
	else
		USkladskyiRuhTovaru::unVydatyTovar(nId, tovId, prevKilkist-curKilkist, cina);
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladskyiRuhTovaru::calcTovZalyshky(int tovId)
{
	QSqlQuery query;
	query.exec("SELECT sum(s.Kilkist) - sum(if(v.Kilk is null,0,v.Kilk)) \
				FROM (SELECT * FROM skladPakety WHERE Tovar_id="+sqlStr(tovId)+" and Closed<>true) AS s \
				LEFT JOIN (SELECT sum(Kilkist) AS Kilk, Paket_id FROM vidpuskTovariv GROUP BY Paket_id) AS v \
					ON v.Paket_id=s.id \
				WHERE (s.Kilkist-if(v.Kilk is null,0,v.Kilk))>0");
	query.next();
	double zalyshok = query.value(0).toDouble();
	if (zalyshok <0)
		return 0;
	return zalyshok;
}
//--------------------utech--------------------utech--------------------utech--------------------
