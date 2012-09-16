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

#include "UDialog_editNakladnaSklad.h"
#include "UPrintDocs_Sklad.h"
#include "../main/UGlobal_Sklad.h"
#include "UDialog_fastTovarInsert.h"

#include <UStandardDelegate>

#define ID_COL 0
#define TOV_ORDER_COL 1
#define TOVAR_ID_COL 2
#define KILKIST_COL 3
#define CINA_COL 4
#define SUMA_COL 5

//--------------------utech--------------------utech--------------------utech--------------------
UDialog_editNakladnaSklad::UDialog_editNakladnaSklad(int nakladnaId, QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	nId = nakladnaId;
	ui.tableWidget->addAction(ui.action_addTovarString);
	ui.tableWidget->addAction(ui.action_deleteTovarString);
	ui.tableWidget->addAction(ui.action_printNakladna);
	ui.tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	//Налаштування вигляду вікна
	ui.tableWidget->setColumnHidden( ID_COL, true);
	ui.tableWidget->setColumnHidden( TOV_ORDER_COL, true);
	ui.tableWidget->setColumnWidth(TOVAR_ID_COL, 400);
	ui.tableWidget->setColumnWidth(KILKIST_COL, 90);
	ui.tableWidget->setColumnWidth(CINA_COL, 90);
	ui.tableWidget->setColumnWidth(SUMA_COL, 90);
	//ui.tableWidget->setItemDelegate(new UNakladna_delegate(ui.tableWidget));
	
	paketsTableColNamesList << "id" << "TovOrder" << "Tovar_id" << "Kilkist" << "Cina";
	
	ui.tableWidget->setItemDelegateForColumn(KILKIST_COL, new UDoubleSpinBoxDelegate(0, 1000000, 6, ui.tableWidget));
	ui.tableWidget->setItemDelegateForColumn(CINA_COL, new UDoubleSpinBoxDelegate(0, 1000000, 6, ui.tableWidget));
	setTableWidgetTovarDelegate();
	populatePartnersComboBox();
	populateNakladnaHeader();
	populateTableWidget();
	
	connect(ui.pushButton_accept, SIGNAL(clicked(bool)), this, SLOT(pushButton_accept_clicked()));
	connect(ui.pushButton_reject, SIGNAL(clicked(bool)), this, SLOT(pushButton_reject_clicked()));
	connect(ui.pushButton_add_tovar_str, SIGNAL(clicked(bool)), this, SLOT(pushButton_add_tovar_str_clicked()));
	connect(ui.pushButton_addTovarDovNakl, SIGNAL(clicked(bool)), this, SLOT(pushButton_addTovarDovNakl_clicked()));
	connect(ui.pushButton_del_tovar_str, SIGNAL(clicked(bool)), this, SLOT(pushButton_del_tovar_str_clicked()));
	connect(ui.pushButton_printNakladna, SIGNAL(clicked(bool)), this, SLOT(pushButton_printNakladna_clicked()));
	
	connect(ui.action_addTovarString, SIGNAL(activated()), this, SLOT(pushButton_add_tovar_str_clicked()));
	connect(ui.action_deleteTovarString, SIGNAL(activated()), this, SLOT(pushButton_del_tovar_str_clicked()));
	connect(ui.action_printNakladna, SIGNAL(activated()), this, SLOT(pushButton_printNakladna_clicked()));
	
	connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
	connect(ui.tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(activateTableSorting()));
	
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::setNakladnaNumText(QString text)
{
	ui.lineEdit_nucl_number->setText(text);
}
//--------------------utech--------------------utech--------------------utech--------------------
QString UDialog_editNakladnaSklad::nakladnaNumText()
{
	return ui.lineEdit_nucl_number->text();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::setPostachalnykID(int id)
{
	ui.comboBox_postachalnyk->setCurrentIndex(postachHash.key(id));
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_editNakladnaSklad::postachalnykID()
{
	return postachHash.value(ui.comboBox_postachalnyk->currentIndex());
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::setNakladnaCreateDate(QDate date)
{
	ui.dateEdit_stvor_nucl->setDate(date);
}
//--------------------utech--------------------utech--------------------utech--------------------
QDate UDialog_editNakladnaSklad::nakladnaCreateDate()
{
	return ui.dateEdit_stvor_nucl->date();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::populateNakladnaHeader()
{
	disconnectHeaderEditors();
	ui.pushButton_accept->setEnabled(false);
	
	QSqlQuery query;
	query.exec("SELECT Num, CDate, Partner_id, Prymitka FROM nakladnavh WHERE id="+QString::number(nId));
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
void UDialog_editNakladnaSklad::populateTableWidget()
{
	disconnect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
	ui.tableWidget->setSortingEnabled(false);
	
	int prevId = 0;
	int prevCol = TOVAR_ID_COL;
	bool needSelectFirstRow = true;
	if (ui.tableWidget->rowCount() > 0){
		prevId = ui.tableWidget->item(ui.tableWidget->currentRow(), ID_COL)->data(Qt::EditRole).toInt();
		prevCol = ui.tableWidget->currentColumn();
	}
	QSqlQuery query;
	query.exec("SELECT count(*) FROM skladPakety \
				WHERE NakladnaVh_id="+QString::number(nId));
	query.next();
	int rowCount = query.value(0).toInt();
	ui.tableWidget->setRowCount(rowCount);
	
	query.exec("SELECT * FROM skladPakety \
				WHERE NakladnaVh_id="+QString::number(nId));
	QTableWidgetItem *item;
	for (int row=0; row<rowCount && query.next(); row++){
		QSqlRecord rec = query.record();
		populateRow(row, rec);
		
		if (needSelectFirstRow && (prevId == rec.value("id").toInt())){
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
	
	activationOfControlButtons();
	ui.tableWidget->setSortingEnabled(true);
	connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::populateRow(int row, QSqlRecord & rec, bool disconcon)
{
	if (disconcon){
		disconnect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
		ui.tableWidget->setSortingEnabled(false);
	}
	
	QTableWidgetItem *item;
	//id
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, rec.value("id"));
	//item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget->setItem(row, ID_COL, item);
	//Порядковість товару
	item = new QTableWidgetItem();
	item->setBackground(QBrush(tableWidgetItemEditableColor));
	item->setData(Qt::EditRole, rec.value("TovOrder").toInt());
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget->setItem(row, TOV_ORDER_COL, item);
	//Назва
	item = new QTableWidgetItem();
	item->setBackground(QBrush(tableWidgetItemEditableColor));
	item->setData(Qt::EditRole, rec.value("Tovar_id"));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget->setItem(row, TOVAR_ID_COL, item);
	//Кількість
	item = new QTableWidgetItem();
	item->setBackground(QBrush(tableWidgetItemEditableColor));
	item->setData(Qt::EditRole, rec.value("Kilkist"));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget->setItem(row, KILKIST_COL, item);
	//Ціна
	item = new QTableWidgetItem();
	item->setBackground(QBrush(tableWidgetItemEditableColor));
	item->setData(Qt::EditRole, rec.value("Cina"));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget->setItem(row, CINA_COL, item);
	//Сума
	item = new QTableWidgetItem(QString::number(rec.value("Kilkist").toDouble()*rec.value("Cina").toDouble(),'f',2).replace('.',','));
	//item->setData(Qt::EditRole, query.value(4));
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
	ui.tableWidget->setItem(row, SUMA_COL, item);
	
	ui.tableWidget->resizeRowToContents(row);
	
	if (disconcon)
		connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::pushButton_accept_clicked()
{
	QSqlQuery query;
	if (query.exec("UPDATE nakladnaVh SET \
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
void UDialog_editNakladnaSklad::pushButton_reject_clicked()
{
	reject();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::pushButton_add_tovar_str_clicked()
{	
	QSqlQuery query;
	query.exec("SELECT max(TovOrder) FROM skladPakety WHERE NakladnaVh_id="+QString::number(nId));
	query.next();
	int curTovOrder = query.value(0).toInt()+1;
	if (!query.exec("INSERT INTO skladPakety (NakladnaVh_id, TovOrder, Closed) VALUE ("+QString::number(nId)+", "+QString::number(curTovOrder)+",false)")){
		QMessageBox::critical(0,"Помилка", "Помилка створення стрічки");
		return;
	}
	
	query.exec("SELECT last_insert_id()");
	query.seek(0);
	int id = query.value(0).toInt();
	
	ui.tableWidget->insertRow(ui.tableWidget->rowCount());
	int row = ui.tableWidget->rowCount()-1;
	query.exec("SELECT * FROM skladPakety \
				WHERE id="+QString::number(id));
	query.next();
	QSqlRecord rec = query.record();
	populateRow(row, rec, true);
	
	QTableWidgetItem *item = ui.tableWidget->item(row, TOVAR_ID_COL);
	ui.tableWidget->setCurrentItem(item);
	ui.tableWidget->scrollToItem(item);
	ui.tableWidget->editItem(item);
	
	showNakladnaSum();
	activationOfControlButtons();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::pushButton_addTovarDovNakl_clicked()
{
	UDialog_fastTovarInsert *d = new UDialog_fastTovarInsert(nId);
	if (!d->exec())
		return;
	int packetId = d->newPacketId();
	delete d;
	
	setTableWidgetTovarDelegate();
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);
	QSqlQuery query;
	query.exec("SELECT * FROM skladPakety WHERE id="+QString::number(packetId));
	query.next();
	QSqlRecord rec = query.record();
	populateRow(row, rec, true);
	ui.tableWidget->setCurrentCell(row, TOVAR_ID_COL);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::pushButton_del_tovar_str_clicked()
{
	QSqlQuery query;
	if	( QMessageBox::question(
                this,
                "!!! Видалення стрічки товару !!!",
                "Ви дійсно бажаєте видалити стрічку товару?",
                "Так",
				"Ні-ні-ні",
                QString(), 0, 1)
		)
		return;
	
	
	int id = ui.tableWidget->item(ui.tableWidget->currentRow(),ID_COL)->data(Qt::EditRole).toInt();
	if (!query.exec("DELETE FROM skladPakety WHERE id="+QString::number(id))){
		QMessageBox::information(0,"Помилка", "Помилка видалення стрічки");
		return;
	}
	
	disconnect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
	ui.tableWidget->setSortingEnabled(false);
	
	ui.tableWidget->removeRow(ui.tableWidget->currentRow());
	
	showNakladnaSum();
	activationOfControlButtons();
	connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::pushButton_printNakladna_clicked()
{
	accept();
	UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
	printDocs->print_vhidnaNakladna(nId);
	connect(this->parent(), SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::showNakladnaSum()
{
	double allSum = 0;
	for (int row=0; row<ui.tableWidget->rowCount(); row++)
		allSum += qRound(ui.tableWidget->item(row,KILKIST_COL)->data(Qt::EditRole).toDouble() * ui.tableWidget->item(row,CINA_COL)->data(Qt::EditRole).toDouble()*100)/100.0;
	ui.label_nucludna_suma->setText("Загальна сума по накладній: " + QString::number(allSum,'f',2).replace('.',',')+" грн.");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::tableWidget_cellChanged(int row, int col)
{
	disconnect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
	
	if (col>ID_COL && col<SUMA_COL){
		QSqlQuery query;
		query.exec("UPDATE skladPakety SET "+paketsTableColNamesList.at(col)+"="+ui.tableWidget->item(row,col)->data(Qt::EditRole).toString()+" \
					WHERE id="+ui.tableWidget->item(row, ID_COL)->data(Qt::EditRole).toString());
		query.exec("SELECT "+paketsTableColNamesList.at(col)+" FROM skladPakety WHERE id="+ui.tableWidget->item(row, ID_COL)->data(Qt::EditRole).toString());
		query.next();
		ui.tableWidget->item(row, col)->setData(Qt::EditRole, query.value(0));
	}
	
	if (col == KILKIST_COL || col == CINA_COL){
		ui.tableWidget->item(row, SUMA_COL)->setText(QString::number(ui.tableWidget->item(row,KILKIST_COL)->data(Qt::EditRole).toDouble()*ui.tableWidget->item(row,CINA_COL)->data(Qt::EditRole).toDouble(),'f',2).replace('.',','));
		showNakladnaSum();
	}
	
	connect(ui.tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(tableWidget_cellChanged(int,int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::setTableWidgetTovarDelegate()
{
	QHash<int, QString> strHash;
	strHash.insert(0, "");
	QSqlQuery query("SELECT id, concat(Name, ', ', Odynyci) FROM tovPerelik ORDER BY concat(Name, ', ', Odynyci)");
	while (query.next())
		strHash.insert(query.value(0).toInt(), query.value(1).toString());
	ui.tableWidget->setItemDelegateForColumn(TOVAR_ID_COL, new UHashComboDelegate(ui.tableWidget, strHash));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::populatePartnersComboBox()
{
	QSqlQuery query;
	query.exec("SELECT id, Name FROM dovpartners WHERE IsPostach=true ORDER BY Name");
	ui.comboBox_postachalnyk->clear();
	ui.comboBox_postachalnyk->addItem("", 0);
	while (query.next())
		ui.comboBox_postachalnyk->addItem(query.value(1).toString(), query.value(0));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::headerEdited()
{
	ui.pushButton_accept->setEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::activateTableSorting()
{
	ui.tableWidget->setSortingEnabled(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::connectHeaderEditors()
{
	connect(ui.lineEdit_nucl_number, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
	connect(ui.dateEdit_stvor_nucl, SIGNAL(dateChanged(const QDate &)), this, SLOT(headerEdited()));
	connect(ui.comboBox_postachalnyk, SIGNAL(currentIndexChanged(int)), this, SLOT(headerEdited()));
	connect(ui.lineEdit_prymitka, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::disconnectHeaderEditors()
{
	disconnect(ui.lineEdit_nucl_number, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
	disconnect(ui.dateEdit_stvor_nucl, SIGNAL(dateChanged(const QDate &)), this, SLOT(headerEdited()));
	disconnect(ui.comboBox_postachalnyk, SIGNAL(currentIndexChanged(int)), this, SLOT(headerEdited()));
	disconnect(ui.lineEdit_prymitka, SIGNAL(textEdited(const QString &)), this, SLOT(headerEdited()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_editNakladnaSklad::activationOfControlButtons()
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
UDialog_editNakladnaSklad::~UDialog_editNakladnaSklad()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
