//--------------------utech--------------------utech--------------------utech--------------------
//
// Copyright (C) 2005-2007 Utech Company. All rights reserved.
//
// Файл з головною функцією програми обліку товарів на складі
//
// Створений програмістами Компанії "Утех" 15.01.2008р.
// Використані компоненти бібліотеки QT4.3.1
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UForm_VyhidniNakladni.h"

#include "UDialog_editVyhNakladnaSklad.h"
#include "UPrintDocs_Sklad.h"

//--------------------utech--------------------utech--------------------utech--------------------
UForm_VyhidniNakladni::UForm_VyhidniNakladni(QWidget *parent)
        : USkladTabWidget(parent)
{
	ui.setupUi(this);
	
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	//фільтри по датах
	ui.dateEdit_vydatkovi_nakladni_filtr_from_date->setDate(settings->value("Filters/Vydatk_nakl_poch_date").toDate());
	ui.dateEdit_vydatkovi_nakladni_filtr_to_date->setDate(settings->value("Filters/Vydatk_nakl_kinc_date").toDate());
	delete settings;
	
	ui.tableWidget_vydatkovi_nakladni->addAction(ui.action_openNakladna);
	ui.tableWidget_vydatkovi_nakladni->addAction(ui.action_createNakladna);
	ui.tableWidget_vydatkovi_nakladni->addAction(ui.action_deleteNakladna);
	ui.tableWidget_vydatkovi_nakladni->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	setTableWidgetPartnerDelegate();
	ui.tableWidget_vydatkovi_nakladni->setColumnHidden(0, true);
	ui.tableWidget_vydatkovi_nakladni->setColumnWidth(1, 110);
	ui.tableWidget_vydatkovi_nakladni->setColumnWidth(2, 90);
	ui.tableWidget_vydatkovi_nakladni->setColumnWidth(3, 300);
	ui.tableWidget_vydatkovi_nakladni->setColumnWidth(4, 70);
	ui.tableWidget_vydatkovi_nakladni->setColumnWidth(5, 200);
	
	populateNakladniTable();
	
	connect(ui.dateEdit_vydatkovi_nakladni_filtr_from_date, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateNakladniTable()));
	connect(ui.dateEdit_vydatkovi_nakladni_filtr_to_date, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateNakladniTable()));
	
	connect(ui.action_openNakladna, SIGNAL(activated()), this, SLOT(action_openNakladna_activated()));
	connect(ui.action_createNakladna, SIGNAL(activated()), this, SLOT(action_createNakladna_activated()));
	connect(ui.action_deleteNakladna, SIGNAL(activated()), this, SLOT(action_deleteNakladna_activated()));
	connect(ui.action_printNakladna, SIGNAL(activated()), this, SLOT(action_printNakladna_activated()));
	connect(ui.action_printNakladnaPidpysyDyrBuh, SIGNAL(activated()), this, SLOT(action_printNakladnaPidpysyDyrBuh_activated()));
	connect(ui.action_printRahunok, SIGNAL(activated()), this, SLOT(action_printRahunok_activated()));
	connect(ui.action_printPodatkovaNakladna, SIGNAL(activated()), this, SLOT(action_printPodatkovaNakladna_activated()));
	
	connect(ui.tableWidget_vydatkovi_nakladni, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(action_openNakladna_activated()));
	connect(ui.pushButton_vydatkova_nakladna_stvoryty, SIGNAL(clicked(bool)), this, SLOT(action_createNakladna_activated()));
	connect(ui.pushButton_vydatkova_nakladna_vydalyty, SIGNAL(clicked(bool)), this, SLOT(action_deleteNakladna_activated()));
	connect(ui.pushButton_vydatkova_nakladna_drukuvaty, SIGNAL(clicked(bool)), this, SLOT(action_printNakladna_activated()));
	connect(ui.pushButton_closeProgram, SIGNAL(clicked(bool)), this, SIGNAL(mainWindowClosedThis()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::populateNakladniTable()
{
	ui.tableWidget_vydatkovi_nakladni->setSortingEnabled(false);
	
	int prevId = 0;
	int prevCol = 1;
	bool needSelectFirstRow = true;
	if (ui.tableWidget_vydatkovi_nakladni->rowCount() > 0){
		prevId = ui.tableWidget_vydatkovi_nakladni->item(ui.tableWidget_vydatkovi_nakladni->currentRow(), 0)->data(Qt::EditRole).toInt();
		prevCol = ui.tableWidget_vydatkovi_nakladni->currentColumn();
	}
	QSqlQuery query;
	query.exec("SELECT count(*) FROM nakladnaVyh \
				WHERE CDate BETWEEN '"+ui.dateEdit_vydatkovi_nakladni_filtr_from_date->date().toString("yyyy-MM-dd")+"' \
					AND '"+ui.dateEdit_vydatkovi_nakladni_filtr_to_date->date().toString("yyyy-MM-dd")+"'");
	query.next();
	int rowCount = query.value(0).toInt();
	ui.tableWidget_vydatkovi_nakladni->setRowCount(rowCount);
	
	query.exec("SELECT nakladnaVyh.id, nakladnaVyh.Num, nakladnaVyh.CDate, nakladnaVyh.Partner_id, \
						vyhNaklSum.Suma, nakladnaVyh.Prymitka \
				FROM nakladnaVyh \
				LEFT JOIN \
					(SELECT vidpusk.nakladnaVyh_id, sum(ROUND(vidpusk.Kilk*vidpusk.Cina,2)) AS Suma, vidpusk.Tovar_id \
					FROM \
						(SELECT vidpuskTovariv.nakladnaVyh_id, sum(vidpuskTovariv.Kilkist) AS Kilk, \
							vidpuskTovariv.Cina, skladPakety.Tovar_id \
						FROM vidpuskTovariv \
						LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id \
						GROUP BY vidpuskTovariv.nakladnaVyh_id, skladPakety.Tovar_id, vidpuskTovariv.Cina) \
						AS vidpusk \
					GROUP BY vidpusk.nakladnaVyh_id) \
					AS vyhNaklSum \
				ON vyhNaklSum.nakladnaVyh_id=nakladnaVyh.id \
				WHERE CDate BETWEEN date('"+ui.dateEdit_vydatkovi_nakladni_filtr_from_date->date().toString("yyyy-MM-dd")+"') \
					AND date('"+ui.dateEdit_vydatkovi_nakladni_filtr_to_date->date().toString("yyyy-MM-dd")+"')");
	QTableWidgetItem *item;
	for (int row=0; row<rowCount && query.next(); row++){
		//id
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(0));
		//item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget_vydatkovi_nakladni->setItem(row, 0, item);
		//Номер накладної
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(1));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_vydatkovi_nakladni->setItem(row, 1, item);
		//Дата накладної
		item = new QTableWidgetItem(query.value(2).toDate().toString("dd.MM.yyyy"));
		//item->setData(Qt::EditRole, query.value(2));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_vydatkovi_nakladni->setItem(row, 2, item);
		//Партнер
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(3));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget_vydatkovi_nakladni->setItem(row, 3, item);
		//Сума
		item = new QTableWidgetItem(QString::number(query.value(4).toDouble(),'f',2).replace('.',','));
		//item->setData(Qt::EditRole, query.value(4));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_vydatkovi_nakladni->setItem(row, 4, item);
		//Примітка
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(5).toString());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget_vydatkovi_nakladni->setItem(row, 5, item);
		
		if (needSelectFirstRow && (prevId == query.value(0).toInt())){
			item = ui.tableWidget_vydatkovi_nakladni->item(row, prevCol);
			ui.tableWidget_vydatkovi_nakladni->setCurrentItem(item);
			ui.tableWidget_vydatkovi_nakladni->scrollToItem(item);
			needSelectFirstRow = false;
		}
	}
	if (needSelectFirstRow){
		item = ui.tableWidget_vydatkovi_nakladni->item(0, prevCol);
		ui.tableWidget_vydatkovi_nakladni->setCurrentItem(item);
		ui.tableWidget_vydatkovi_nakladni->scrollToItem(item);
	}
		
	ui.tableWidget_vydatkovi_nakladni->resizeRowsToContents();
	//ui.tableWidget_vydatkovi_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::populateNakladniTableRow(int row, int id)
{
	ui.tableWidget_vydatkovi_nakladni->setSortingEnabled(false);
	
	QSqlQuery query;
	query.exec("SELECT nakladnaVyh.id, nakladnaVyh.Num, nakladnaVyh.CDate, nakladnaVyh.Partner_id, \
						vyhNaklSum.Suma, nakladnaVyh.Prymitka \
				FROM nakladnaVyh \
				LEFT JOIN \
					(SELECT vidpusk.nakladnaVyh_id, sum(ROUND(vidpusk.Kilk*vidpusk.Cina,2)) AS Suma, vidpusk.Tovar_id \
					FROM \
						(SELECT vidpuskTovariv.nakladnaVyh_id, sum(vidpuskTovariv.Kilkist) AS Kilk, \
							vidpuskTovariv.Cina, skladPakety.Tovar_id \
						FROM vidpuskTovariv \
						LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id \
						GROUP BY vidpuskTovariv.nakladnaVyh_id, skladPakety.Tovar_id, vidpuskTovariv.Cina) \
						AS vidpusk \
					GROUP BY vidpusk.nakladnaVyh_id) \
					AS vyhNaklSum \
				ON vyhNaklSum.nakladnaVyh_id=nakladnaVyh.id \
				WHERE id="+QString::number(id));
	query.next();
	QTableWidgetItem * item;
	//id
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(0));
	//item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget_vydatkovi_nakladni->setItem(row, 0, item);
	//Номер накладної
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(1));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget_vydatkovi_nakladni->setItem(row, 1, item);
	//Дата накладної
	item = new QTableWidgetItem(query.value(2).toDate().toString("dd.MM.yyyy"));
	//item->setData(Qt::EditRole, query.value(2));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget_vydatkovi_nakladni->setItem(row, 2, item);
	//Партнер
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(3));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget_vydatkovi_nakladni->setItem(row, 3, item);
	//Сума
	item = new QTableWidgetItem(QString::number(query.value(4).toDouble(),'f',2).replace('.',','));
	//item->setData(Qt::EditRole, query.value(4));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget_vydatkovi_nakladni->setItem(row, 4, item);
	//Примітка
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(5));
	item->setTextAlignment(Qt::AlignVCenter |Qt::AlignLeft);
	ui.tableWidget_vydatkovi_nakladni->setItem(row, 5, item);
	
	//ui.tableWidget_vydatkovi_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::setTableWidgetPartnerDelegate()
{
	QHash<int, QString> hash;
	QSqlQuery query;
	query.exec("SELECT id, Name FROM dovpartners ORDER BY Name");
	while (query.next())
		hash.insert(query.value(0).toInt(), query.value(1).toString());
	ui.tableWidget_vydatkovi_nakladni->setItemDelegateForColumn(3, new UHashComboDelegate(ui.tableWidget_vydatkovi_nakladni, hash));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::action_openNakladna_activated()
{
	if (ui.tableWidget_vydatkovi_nakladni->rowCount() == 0)
		return;
	int id = ui.tableWidget_vydatkovi_nakladni->item(ui.tableWidget_vydatkovi_nakladni->currentRow(), 0)->data(Qt::EditRole).toInt();
	UDialog_editVyhNakladnaSklad * d = new UDialog_editVyhNakladnaSklad(id, this);
	d->exec();
	populateNakladniTableRow(ui.tableWidget_vydatkovi_nakladni->currentRow(), id);
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::action_createNakladna_activated()
{
	QSqlQuery query;
	if (!query.exec("INSERT INTO nakladnaVyh (Num, CDate, Partner_id) VALUES ('0000', CURDATE(), 0)")){
		QMessageBox::critical(0,"Помилка", "Помилка створення стрічки");
		return;
	}
	
	ui.tableWidget_vydatkovi_nakladni->setSortingEnabled(false);
	
	query.exec("SELECT last_insert_id()");
	query.next();
	int id = query.value(0).toInt();
	int row = ui.tableWidget_vydatkovi_nakladni->rowCount();
	ui.tableWidget_vydatkovi_nakladni->insertRow(row);
	populateNakladniTableRow(row, id);
	
	ui.tableWidget_vydatkovi_nakladni->resizeRowToContents(row);
	QTableWidgetItem * item;
	item = ui.tableWidget_vydatkovi_nakladni->item(row,1);
	ui.tableWidget_vydatkovi_nakladni->setCurrentItem(item);
	ui.tableWidget_vydatkovi_nakladni->scrollToItem(item);
	action_openNakladna_activated();
	
	ui.tableWidget_vydatkovi_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::action_deleteNakladna_activated()
{
	if	( QMessageBox::question(
                this,
                "!!! Видалення накладної !!!",
                "Ви дійсно бажаєте видалити накладну?",
                "Так",
				"Ні-ні-ні",
                QString(), 0, 1)
		)
		return;
	
	QSqlQuery query;
	int row = ui.tableWidget_vydatkovi_nakladni->currentRow();
	int id = ui.tableWidget_vydatkovi_nakladni->item(row,0)->data(Qt::EditRole).toInt();
	bool doneOk=true;
	doneOk &= query.exec("DELETE FROM vidpuskTovariv WHERE NakladnaVyh_id="+QString::number(id));
	doneOk &= query.exec("DELETE FROM nakladnaVyh WHERE id="+QString::number(id));
	
	if (doneOk){
		ui.tableWidget_vydatkovi_nakladni->removeRow(row);
		if (row >= ui.tableWidget_vydatkovi_nakladni->rowCount())
			row = ui.tableWidget_vydatkovi_nakladni->rowCount()-1;
		QTableWidgetItem *item = ui.tableWidget_vydatkovi_nakladni->item(row,1);
		ui.tableWidget_vydatkovi_nakladni->setCurrentItem(item);
		ui.tableWidget_vydatkovi_nakladni->scrollToItem(item);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::action_printNakladna_activated()
{
	UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
	printDocs->print_vyhidnaNakladna(ui.tableWidget_vydatkovi_nakladni->item(ui.tableWidget_vydatkovi_nakladni->currentRow(),0)->data(Qt::EditRole).toInt());
	connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::action_printNakladnaPidpysyDyrBuh_activated()
{
	UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
	printDocs->print_vyhidnaNakladna(ui.tableWidget_vydatkovi_nakladni->item(ui.tableWidget_vydatkovi_nakladni->currentRow(),0)->data(Qt::EditRole).toInt(), true);
	connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::action_printRahunok_activated()
{
	UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
	printDocs->print_rahunok(ui.tableWidget_vydatkovi_nakladni->item(ui.tableWidget_vydatkovi_nakladni->currentRow(),0)->data(Qt::EditRole).toInt());
	connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::action_printPodatkovaNakladna_activated()
{
	UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
	printDocs->print_podatkovaNakladna(ui.tableWidget_vydatkovi_nakladni->item(ui.tableWidget_vydatkovi_nakladni->currentRow(),0)->data(Qt::EditRole).toInt());
	connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::activateTableSorting()
{
	ui.tableWidget_vydatkovi_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VyhidniNakladni::populateToolBar(QToolBar * tBar)
{
	tBar->addAction(ui.action_printNakladna);
	tBar->addAction(ui.action_printNakladnaPidpysyDyrBuh);
	tBar->addAction(ui.action_printRahunok);
	tBar->addAction(ui.action_printPodatkovaNakladna);
}
//--------------------utech--------------------utech--------------------utech--------------------
UForm_VyhidniNakladni::~UForm_VyhidniNakladni()
{
	//Запис значень віджетів і розмірів головного вікна до ініфайлу
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	//фільтри по датах
	settings->setValue("Filters/Vydatk_nakl_poch_date", ui.dateEdit_vydatkovi_nakladni_filtr_from_date->date());
	settings->setValue("Filters/Vydatk_nakl_kinc_date", ui.dateEdit_vydatkovi_nakladni_filtr_to_date->date());

	delete settings;
}
//--------------------utech--------------------utech--------------------utech--------------------
