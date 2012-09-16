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

#include "UDialog_editVyhNakladnaSklad.h"
#include "UDialog_VnesTovaruVyhNakl_sklad.h"
#include "UPrintDocs_Sklad.h"
#include "../main/UGlobal_Sklad.h"
#include <UStandardDelegate.h>

#define TOV_ORDER_COL 0
#define TOVAR_ID_COL 1
#define KILKIST_COL 2
#define CINA_COL 3
#define SUMA_COL 4

//--------------------utech--------------------utech--------------------utech--------------------
UDialog_editVyhNakladnaSklad::UDialog_editVyhNakladnaSklad(int nakladnaId, QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	nId = nakladnaId;
	ui.tableWidget->addAction(ui.action_addTovarString);
	ui.tableWidget->addAction(ui.action_deleteTovarString);
	ui.tableWidget->addAction(ui.action_printNakladna);
	ui.tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	//Налаштування вигляду вікна
	ui.tableWidget->setColumnHidden( TOV_ORDER_COL, true);
	ui.tableWidget->setColumnWidth(TOVAR_ID_COL, 400);
	ui.tableWidget->setColumnWidth(KILKIST_COL, 90);
	ui.tableWidget->setColumnWidth(CINA_COL, 90);
	ui.tableWidget->setColumnWidth(SUMA_COL, 90);
	
	ui.tableWidget->setItemDelegateForColumn(KILKIST_COL, new UDoubleSpinBoxDelegate(0, 1000000, 6, ui.tableWidget));
	ui.tableWidget->setItemDelegateForColumn(CINA_COL, new UDoubleSpinBoxDelegate(0, 1000000, 6, ui.tableWidget));
	setTableWidgetTovarDelegate();
	populatePartnersComboBox();
	populateNakladnaHeader();
	populateTableWidget();
	
	connect(ui.pushButton_accept, SIGNAL(clicked(bool)), this, SLOT(pushButton_accept_clicked()));
	connect(ui.pushButton_reject, SIGNAL(clicked(bool)), this, SLOT(pushButton_reject_clicked()));
	connect(ui.pushButton_add_tovar_str, SIGNAL(clicked(bool)), this, SLOT(pushButton_add_tovar_str_clicked()));
	connect(ui.pushButton_del_tovar_str, SIGNAL(clicked(bool)), this, SLOT(pushButton_del_tovar_str_clicked()));
	connect(ui.pushButton_printNakladna, SIGNAL(clicked(bool)), this, SLOT(pushButton_printNakladna_clicked()));
	
	connect(ui.action_addTovarString, SIGNAL(activated()), this, SLOT(pushButton_add_tovar_str_clicked()));
	connect(ui.action_deleteTovarString, SIGNAL(activated()), this, SLOT(pushButton_del_tovar_str_clicked()));
	connect(ui.action_printNakladna, SIGNAL(activated()), this, SLOT(pushButton_printNakladna_clicked()));
	
	connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
	connect(ui.tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(activateTableSorting()));
	connect(ui.tableWidget->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)), this, SLOT(tableWidget_currentRowChanged()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::setNakladnaNumText(QString text)
{
	ui.lineEdit_nucl_number->setText(text);
}
//--------------------utech--------------------utech--------------------utech--------------------
QString UDialog_editVyhNakladnaSklad::nakladnaNumText()
{
	return ui.lineEdit_nucl_number->text();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::setPostachalnykID(int id)
{
	ui.comboBox_postachalnyk->setCurrentIndex(postachHash.key(id));
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_editVyhNakladnaSklad::postachalnykID()
{
	return postachHash.value(ui.comboBox_postachalnyk->currentIndex());
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::setNakladnaCreateDate(QDate date)
{
	ui.dateEdit_stvor_nucl->setDate(date);
}
//--------------------utech--------------------utech--------------------utech--------------------
QDate UDialog_editVyhNakladnaSklad::nakladnaCreateDate()
{
	return ui.dateEdit_stvor_nucl->date();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::populateNakladnaHeader()
{
	disconnectHeaderEditors();
	ui.pushButton_accept->setEnabled(false);
	
	QSqlQuery query;
	query.exec("SELECT Num, CDate, Partner_id, Prymitka FROM nakladnaVyh WHERE id="+QString::number(nId));
	query.next();
	
	ui.lineEdit_nucl_number->setText(query.value(0).toString());
	ui.dateEdit_stvor_nucl->setDate(query.value(1).toDate());
	int index = ui.comboBox_postachalnyk->findData(query.value(2));
	if (index != -1)
		ui.comboBox_postachalnyk->setCurrentIndex(index);
	else
		ui.comboBox_postachalnyk->setCurrentIndex(0);
	ui.lineEdit_prymitka->setText(query.value(3).toString());
	
	connectHeaderEditors();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::populateTableWidget()
{
	disconnect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
	ui.tableWidget->setSortingEnabled(false);
	
	int prevId = 0;
	int prevCol = TOVAR_ID_COL;
	bool needSelectFirstRow = true;
	if (ui.tableWidget->rowCount() > 0){
		prevId = ui.tableWidget->item(ui.tableWidget->currentRow(), 0)->data(Qt::EditRole).toInt();
		prevCol = ui.tableWidget->currentColumn();
	}
	QSqlQuery query;
	query.exec("SELECT count(*) FROM (SELECT count(*) AS cnt \
				FROM vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY skladPakety.Tovar_id, vidpuskTovariv.Cina, vidpuskTovariv.Closed) AS vidpuskTovCount");
	query.next();
	int rowCount = query.value(0).toInt();
	ui.tableWidget->setRowCount(rowCount);
	
	query.exec("SELECT skladPakety.Tovar_id, sum(vidpuskTovariv.Kilkist), vidpuskTovariv.Cina, vidpuskTovariv.Closed \
				FROM vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY skladPakety.Tovar_id, vidpuskTovariv.Cina, vidpuskTovariv.Closed");
	QTableWidgetItem *item;
	bool closedVidpuskTov;
	for (int row=0; row<rowCount && query.next(); row++){
		closedVidpuskTov = query.value(3).toBool();
		//OrderTov
		item = new QTableWidgetItem();
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		item->setData(Qt::EditRole, 0);
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget->setItem(row, TOV_ORDER_COL, item);
		//Назва
		item = new QTableWidgetItem();
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		item->setData(Qt::EditRole, query.value(0));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.tableWidget->setItem(row, TOVAR_ID_COL, item);
		//Кількість
		item = new QTableWidgetItem();
		if (closedVidpuskTov)
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		else
			item->setBackground(QBrush(tableWidgetItemEditableColor));
		item->setData(Qt::EditRole, query.value(1).toDouble());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget->setItem(row, KILKIST_COL, item);
		//Ціна
		item = new QTableWidgetItem();
		if (closedVidpuskTov)
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		else
			item->setBackground(QBrush(tableWidgetItemEditableColor));
		item->setData(Qt::EditRole, query.value(2).toDouble());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget->setItem(row, CINA_COL, item);
		//Сума
		item = new QTableWidgetItem(QString::number(query.value(1).toDouble()*query.value(2).toDouble(),'f',2).replace('.',','));
		//item->setData(Qt::EditRole, query.value(4));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		ui.tableWidget->setItem(row, SUMA_COL, item);
		
		if (needSelectFirstRow && (prevId == query.value(0).toInt())){
			item = ui.tableWidget->item(row, prevCol);
			ui.tableWidget->setCurrentItem(item);
			ui.tableWidget->scrollToItem(item);
			needSelectFirstRow = false;
		}
	}
	ui.tableWidget->resizeRowsToContents();
	
	if (needSelectFirstRow && ui.tableWidget->rowCount() > 0){
		item = ui.tableWidget->item(0, prevCol);
		ui.tableWidget->setCurrentItem(item);
		ui.tableWidget->scrollToItem(item);
	}
	showNakladnaSum();
	tableWidget_currentRowChanged();
	activationOfControlButtons();
	connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::pushButton_accept_clicked()
{
	QSqlQuery query;
	if (query.exec("UPDATE nakladnaVyh SET \
						Num='"+ui.lineEdit_nucl_number->text()+"', \
						CDate='"+ui.dateEdit_stvor_nucl->date().toString("yyyy-MM-dd")+"', \
						Partner_id="+ui.comboBox_postachalnyk->itemData(ui.comboBox_postachalnyk->currentIndex(), Qt::UserRole).toString()+", \
						Prymitka='"+ui.lineEdit_prymitka->text()+"' \
					WHERE id="+QString::number(nId))){
		accept();
	}
	else{
		QMessageBox::critical(0,"Помилка", "Помилка зміни реквізитів накладної");
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::pushButton_reject_clicked()
{
	reject();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::pushButton_add_tovar_str_clicked()
{	
	UDialog_VnesTovaruVyhNakl_sklad * d = new UDialog_VnesTovaruVyhNakl_sklad(nId, this);
	if (d->exec())
		populateTableWidget();
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::pushButton_del_tovar_str_clicked()
{
	int tovId = ui.tableWidget->item(ui.tableWidget->currentRow(), TOVAR_ID_COL)->data(Qt::EditRole).toInt();
	double kilkist = ui.tableWidget->item(ui.tableWidget->currentRow(), KILKIST_COL)->data(Qt::EditRole).toDouble();
	double cina = ui.tableWidget->item(ui.tableWidget->currentRow(), CINA_COL)->data(Qt::EditRole).toDouble();
	USkladskyiRuhTovaru::unVydatyTovar(nId, tovId, kilkist, cina);
	populateTableWidget();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::pushButton_printNakladna_clicked()
{
	accept();
	UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
	printDocs->print_vyhidnaNakladna(nId);
	connect(this->parent(), SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::showNakladnaSum()
{
	double allSum = 0;
	for (int row=0; row<ui.tableWidget->rowCount(); row++)
		allSum += qRound(ui.tableWidget->item(row,KILKIST_COL)->data(Qt::EditRole).toDouble() * ui.tableWidget->item(row,CINA_COL)->data(Qt::EditRole).toDouble()*100)/100.0;
	ui.label_nucludna_suma->setText("Загальна сума по накладній: " + QString::number(allSum,'f',2).replace('.',',')+" грн.");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::tableWidget_cellChanged(int row, int col)
{
	disconnect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
	
	if (col==2){ //Кількість
		int tovId = ui.tableWidget->item(ui.tableWidget->currentRow(),TOVAR_ID_COL)->data(Qt::EditRole).toInt();
		double kilk = ui.tableWidget->item(ui.tableWidget->currentRow(),KILKIST_COL)->data(Qt::EditRole).toDouble();
		double cina = ui.tableWidget->item(ui.tableWidget->currentRow(),CINA_COL)->data(Qt::EditRole).toDouble();
		USkladskyiRuhTovaru::changeTovarKilkist(nId, tovId, __curTovarKilkist, kilk, cina);
		populateTableWidget();
	}
	else if (col==3){ //Ціна
		int tovId = ui.tableWidget->item(ui.tableWidget->currentRow(),TOVAR_ID_COL)->data(Qt::EditRole).toInt();
		double curCina = ui.tableWidget->item(ui.tableWidget->currentRow(),CINA_COL)->data(Qt::EditRole).toDouble();
		USkladskyiRuhTovaru::changeTovarCina(nId, tovId, __curTovarCina, curCina);
		populateTableWidget();
	}
	
	connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::tableWidget_currentRowChanged()
{
	if (ui.tableWidget->rowCount() == 0){
		__curTovarKilkist = 0;
		__curTovarCina = 0;
		return;
	}
	__curTovarKilkist = ui.tableWidget->item(ui.tableWidget->currentRow(),KILKIST_COL)->data(Qt::EditRole).toDouble();
	__curTovarCina = ui.tableWidget->item(ui.tableWidget->currentRow(),CINA_COL)->data(Qt::EditRole).toDouble();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::setTableWidgetTovarDelegate()
{
	QHash<int, QString> strHash;
	strHash.insert(0, "");
	QSqlQuery query("SELECT id, concat(Name, ', ', Odynyci) FROM tovPerelik ORDER BY concat(Name, ', ', Odynyci)");
	while (query.next())
		strHash.insert(query.value(0).toInt(), query.value(1).toString());
	ui.tableWidget->setItemDelegateForColumn(TOVAR_ID_COL, new UHashComboDelegate(ui.tableWidget, strHash));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::populatePartnersComboBox()
{
	QSqlQuery query;
	query.exec("SELECT id, Name FROM dovpartners WHERE IsKlient=true ORDER BY Name");
	ui.comboBox_postachalnyk->clear();
	ui.comboBox_postachalnyk->addItem("", 0);
	while (query.next())
		ui.comboBox_postachalnyk->addItem(query.value(1).toString(), query.value(0));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::headerEdited()
{
	ui.pushButton_accept->setEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::activateTableSorting()
{
	ui.tableWidget->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::connectHeaderEditors()
{
	connect(ui.lineEdit_nucl_number, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
	connect(ui.dateEdit_stvor_nucl, SIGNAL(dateChanged(const QDate &)), this, SLOT(headerEdited()));
	connect(ui.comboBox_postachalnyk, SIGNAL(currentIndexChanged(int)), this, SLOT(headerEdited()));
	connect(ui.lineEdit_prymitka, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::disconnectHeaderEditors()
{
	disconnect(ui.lineEdit_nucl_number, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
	disconnect(ui.dateEdit_stvor_nucl, SIGNAL(dateChanged(const QDate &)), this, SLOT(headerEdited()));
	disconnect(ui.comboBox_postachalnyk, SIGNAL(currentIndexChanged(int)), this, SLOT(headerEdited()));
	disconnect(ui.lineEdit_prymitka, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editVyhNakladnaSklad::activationOfControlButtons()
{
	bool active;
	if (ui.tableWidget->rowCount() > 0)
		active = true;
	else
		active = false;
	ui.pushButton_del_tovar_str->setEnabled(active);
	ui.pushButton_printNakladna->setEnabled(active);
	ui.action_deleteTovarString->setEnabled(active);
	ui.action_printNakladna->setEnabled(active);
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialog_editVyhNakladnaSklad::~UDialog_editVyhNakladnaSklad()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
