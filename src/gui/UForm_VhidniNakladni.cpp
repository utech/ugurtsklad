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

#include "UForm_VhidniNakladni.h"

#include "UDialog_editNakladnaSklad.h"
#include "UPrintDocs_Sklad.h"

//--------------------utech--------------------utech--------------------utech--------------------
UForm_VhidniNakladni::UForm_VhidniNakladni(QWidget *parent)
        : USkladTabWidget(parent)
{
	ui.setupUi(this);
	
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	//фільтри по датах
	ui.dateEdit_vhidni_nakladni_filtr_from_date->setDate(settings->value("Filters/Vhidn_nakl_poch_date").toDate());
	ui.dateEdit_vhidni_nakladni_filtr_to_date->setDate(settings->value("Filters/Vhidn_nakl_kinc_date").toDate());
	delete settings;
	
	ui.tableWidget_vhidni_nakladni->addAction(ui.action_openNakladna);
	ui.tableWidget_vhidni_nakladni->addAction(ui.action_createNakladna);
	ui.tableWidget_vhidni_nakladni->addAction(ui.action_deleteNakladna);
	ui.tableWidget_vhidni_nakladni->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	setTableWidgetPartnerDelegate();
	ui.tableWidget_vhidni_nakladni->setColumnHidden(0, true);
	ui.tableWidget_vhidni_nakladni->setColumnWidth(1, 110);
	ui.tableWidget_vhidni_nakladni->setColumnWidth(2, 90);
	ui.tableWidget_vhidni_nakladni->setColumnWidth(3, 300);
	ui.tableWidget_vhidni_nakladni->setColumnWidth(4, 70);
	ui.tableWidget_vhidni_nakladni->setColumnWidth(5, 200);
	
	populateNakladniTable();
	
	connect(ui.dateEdit_vhidni_nakladni_filtr_from_date, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateNakladniTable()));
	connect(ui.dateEdit_vhidni_nakladni_filtr_to_date, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateNakladniTable()));
	
	connect(ui.action_openNakladna, SIGNAL(activated()), this, SLOT(action_openNakladna_activated()));
	connect(ui.action_createNakladna, SIGNAL(activated()), this, SLOT(action_createNakladna_activated()));
	connect(ui.action_deleteNakladna, SIGNAL(activated()), this, SLOT(action_deleteNakladna_activated()));
	connect(ui.action_printNakladna, SIGNAL(activated()), this, SLOT(action_printNakladna_activated()));
	
	connect(ui.tableWidget_vhidni_nakladni, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(action_openNakladna_activated()));
	connect(ui.pushButton_vhidna_nakladna_stvoryty, SIGNAL(clicked(bool)), this, SLOT(action_createNakladna_activated()));
	connect(ui.pushButton_vhidna_nakladna_vydalyty, SIGNAL(clicked(bool)), this, SLOT(action_deleteNakladna_activated()));
	connect(ui.pushButton_vhidna_nakladna_drukuvaty, SIGNAL(clicked(bool)), this, SLOT(action_printNakladna_activated()));
	connect(ui.pushButton_closeProgram, SIGNAL(clicked(bool)), this, SIGNAL(mainWindowClosedThis()));
	
	connect(ui.tableWidget_vhidni_nakladni->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(activateTableSorting()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::populateNakladniTable()
{
	ui.tableWidget_vhidni_nakladni->setSortingEnabled(false);
	
	int prevId = 0;
	int prevCol = 1;
	bool needSelectFirstRow = true;
	if (ui.tableWidget_vhidni_nakladni->rowCount() > 0){
		prevId = ui.tableWidget_vhidni_nakladni->item(ui.tableWidget_vhidni_nakladni->currentRow(), 0)->data(Qt::EditRole).toInt();
		prevCol = ui.tableWidget_vhidni_nakladni->currentColumn();
	}
	QSqlQuery query;
	query.exec("SELECT count(*) FROM nakladnaVh \
				WHERE CDate BETWEEN '"+ui.dateEdit_vhidni_nakladni_filtr_from_date->date().toString("yyyy-MM-dd")+"' \
					AND '"+ui.dateEdit_vhidni_nakladni_filtr_to_date->date().toString("yyyy-MM-dd")+"'");
	query.next();
	int rowCount = query.value(0).toInt();
	ui.tableWidget_vhidni_nakladni->setRowCount(rowCount);
	
	query.exec("SELECT id, Num, CDate, Partner_id, \
					(SELECT sum(ROUND(skladPakety.Kilkist*skladPakety.Cina,2)) FROM skladPakety \
						WHERE skladPakety.NakladnaVh_id=nakladnaVh.id) AS suma, Prymitka \
				FROM nakladnaVh \
				WHERE CDate BETWEEN '"+ui.dateEdit_vhidni_nakladni_filtr_from_date->date().toString("yyyy-MM-dd")+"' \
					AND '"+ui.dateEdit_vhidni_nakladni_filtr_to_date->date().toString("yyyy-MM-dd")+"'");
	QTableWidgetItem *item;
	for (int row=0; row<rowCount && query.next(); row++){
		//id
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(0));
		//item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget_vhidni_nakladni->setItem(row, 0, item);
		//Номер накладної
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(1));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_vhidni_nakladni->setItem(row, 1, item);
		//Дата накладної
		item = new QTableWidgetItem(query.value(2).toDate().toString("dd.MM.yyyy"));
		//item->setData(Qt::EditRole, query.value(2));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_vhidni_nakladni->setItem(row, 2, item);
		//Партнер
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(3));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget_vhidni_nakladni->setItem(row, 3, item);
		//Сума
		item = new QTableWidgetItem(QString::number(query.value(4).toDouble(),'f',2).replace('.',','));
		//item->setData(Qt::EditRole, query.value(4));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget_vhidni_nakladni->setItem(row, 4, item);
		//Примітка
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(5).toString());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget_vhidni_nakladni->setItem(row, 5, item);
		
		if (needSelectFirstRow && (prevId == query.value(0).toInt())){
			item = ui.tableWidget_vhidni_nakladni->item(row, prevCol);
			ui.tableWidget_vhidni_nakladni->setCurrentItem(item);
			ui.tableWidget_vhidni_nakladni->scrollToItem(item);
			needSelectFirstRow = false;
		}
	}
	if (needSelectFirstRow){
		item = ui.tableWidget_vhidni_nakladni->item(0, prevCol);
		ui.tableWidget_vhidni_nakladni->setCurrentItem(item);
		ui.tableWidget_vhidni_nakladni->scrollToItem(item);
	}
		
	ui.tableWidget_vhidni_nakladni->resizeRowsToContents();
	//ui.tableWidget_vhidni_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::populateNakladniTableRow(int row, int id)
{
	ui.tableWidget_vhidni_nakladni->setSortingEnabled(false);
	
	QSqlQuery query;
	query.exec("SELECT id, Num, CDate, Partner_id, \
					(SELECT sum(ROUND(skladPakety.Kilkist*skladPakety.Cina,2)) FROM skladPakety \
						WHERE skladPakety.NakladnaVh_id=nakladnaVh.id) AS suma, Prymitka \
				FROM nakladnaVh WHERE id="+QString::number(id));
	query.next();
	QTableWidgetItem * item;
	//id
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(0));
	//item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget_vhidni_nakladni->setItem(row, 0, item);
	//Номер накладної
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(1));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget_vhidni_nakladni->setItem(row, 1, item);
	//Дата накладної
	item = new QTableWidgetItem(query.value(2).toDate().toString("dd.MM.yyyy"));
	//item->setData(Qt::EditRole, query.value(2));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget_vhidni_nakladni->setItem(row, 2, item);
	//Партнер
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(3));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget_vhidni_nakladni->setItem(row, 3, item);
	//Сума
	item = new QTableWidgetItem(QString::number(query.value(4).toDouble(),'f',2).replace('.',','));
	//item->setData(Qt::EditRole, query.value(4));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget_vhidni_nakladni->setItem(row, 4, item);
	//Примітка
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(5));
	item->setTextAlignment(Qt::AlignVCenter |Qt::AlignLeft);
	ui.tableWidget_vhidni_nakladni->setItem(row, 5, item);
	
	//ui.tableWidget_vhidni_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::setTableWidgetPartnerDelegate()
{
	QHash<int, QString> hash;
	QSqlQuery query;
	query.exec("SELECT id, Name FROM dovpartners ORDER BY Name");
	while (query.next())
		hash.insert(query.value(0).toInt(), query.value(1).toString());
	ui.tableWidget_vhidni_nakladni->setItemDelegateForColumn(3, new UHashComboDelegate(ui.tableWidget_vhidni_nakladni, hash));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::action_openNakladna_activated()
{
	if (ui.tableWidget_vhidni_nakladni->rowCount() == 0)
		return;
	int id = ui.tableWidget_vhidni_nakladni->item(ui.tableWidget_vhidni_nakladni->currentRow(), 0)->data(Qt::EditRole).toInt();
	UDialog_editNakladnaSklad * d = new UDialog_editNakladnaSklad(id, this);
	d->exec();
	populateNakladniTableRow(ui.tableWidget_vhidni_nakladni->currentRow(), id);
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::action_createNakladna_activated()
{
	QSqlQuery query;
	if (!query.exec("INSERT INTO nakladnavh (Num, CDate, Partner_id) VALUES ('0000', CURDATE(), 0)")){
		QMessageBox::critical(0,"Помилка", "Помилка створення стрічки");
		return;
	}
	
	ui.tableWidget_vhidni_nakladni->setSortingEnabled(false);
	
	query.exec("SELECT last_insert_id()");
	query.next();
	int id = query.value(0).toInt();
	int row = ui.tableWidget_vhidni_nakladni->rowCount();
	ui.tableWidget_vhidni_nakladni->insertRow(row);
	populateNakladniTableRow(row, id);
	
	ui.tableWidget_vhidni_nakladni->resizeRowToContents(row);
	QTableWidgetItem * item;
	item = ui.tableWidget_vhidni_nakladni->item(row,1);
	ui.tableWidget_vhidni_nakladni->setCurrentItem(item);
	ui.tableWidget_vhidni_nakladni->scrollToItem(item);
	action_openNakladna_activated();
	
	ui.tableWidget_vhidni_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::action_deleteNakladna_activated()
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
	int row = ui.tableWidget_vhidni_nakladni->currentRow();
	int id = ui.tableWidget_vhidni_nakladni->item(row,0)->data(Qt::EditRole).toInt();
	bool doneOk=true;
	doneOk &= query.exec("DELETE FROM skladPakety WHERE NakladnaVh_id="+QString::number(id));
	doneOk &= query.exec("DELETE FROM nakladnaVh WHERE id="+QString::number(id));
	
	if (doneOk){
		ui.tableWidget_vhidni_nakladni->removeRow(row);
		if (row >= ui.tableWidget_vhidni_nakladni->rowCount())
			row = ui.tableWidget_vhidni_nakladni->rowCount()-1;
		QTableWidgetItem *item = ui.tableWidget_vhidni_nakladni->item(row,1);
		ui.tableWidget_vhidni_nakladni->setCurrentItem(item);
		ui.tableWidget_vhidni_nakladni->scrollToItem(item);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::action_printNakladna_activated()
{
	UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
	printDocs->print_vhidnaNakladna(ui.tableWidget_vhidni_nakladni->item(ui.tableWidget_vhidni_nakladni->currentRow(),0)->data(Qt::EditRole).toInt());
	connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::activateTableSorting()
{
	ui.tableWidget_vhidni_nakladni->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_VhidniNakladni::populateToolBar(QToolBar * tBar)
{
	tBar->addAction(ui.action_printNakladna);
}
//--------------------utech--------------------utech--------------------utech--------------------
UForm_VhidniNakladni::~UForm_VhidniNakladni()
{
	//Запис значень віджетів і розмірів головного вікна до ініфайлу
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	//фільтри по датах
	settings->setValue("Filters/Vhidn_nakl_poch_date", ui.dateEdit_vhidni_nakladni_filtr_from_date->date());
	settings->setValue("Filters/Vhidn_nakl_kinc_date", ui.dateEdit_vhidni_nakladni_filtr_to_date->date());
	delete settings;
}
//--------------------utech--------------------utech--------------------utech--------------------
