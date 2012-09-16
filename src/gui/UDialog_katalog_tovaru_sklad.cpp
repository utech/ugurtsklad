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

#include "UDialog_katalog_tovaru_sklad.h"
#include "UDialogCreatePrice.h"

//--------------------utech--------------------utech--------------------utech--------------------
UDialog_katalog_tovaru::UDialog_katalog_tovaru(QWidget *parent)
        : USkladTabWidget(parent)
{
	ui.setupUi(this);
	
	ui.tableWidget_tovPerelik->addAction(ui.action_pasteTovStrings);
	ui.tableWidget_tovPerelik->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	populateTovGroupsTableWidget();
	ui.tableWidget_tovGroups->setColumnWidth(1, 120);
	ui.tableWidget_tovGroups->setColumnWidth(2, 70);
	ui.tableWidget_tovGroups->setColumnHidden(0, true);
	ui.tableWidget_tovGroups->setItemDelegateForColumn(2, new USpinBoxDelegate(0, 1000000, ui.tableWidget_tovGroups));
	tovGroupsColNames << "id" << "Name" << "Rahunok";
	
	twExt = new UTableWidgetSqlExt(ui.tableWidget_tovPerelik, "tovPerelik");
	twExt->colNamesList << "Id"
						<< "GroupID"
						<< "Nomer"
						<< "Name"
						<< "Odynyci" 
						<< "Cina" 
						<< "ForPrice"
						<< "Prymitka";
	twExt->addColAlignment("GroupID", Qt::AlignLeft | Qt::AlignVCenter);
	twExt->addColAlignment("Nomer", Qt::AlignLeft | Qt::AlignVCenter);
	twExt->addColAlignment("Name", Qt::AlignLeft | Qt::AlignVCenter);
	twExt->addColAlignment("Prymitka", Qt::AlignLeft | Qt::AlignVCenter);
	
	setTableWidgetGroupTovDelegate();
	ui.tableWidget_tovPerelik->setItemDelegateForColumn(twExt->columnNumber("Cina"), new UDoubleSpinBoxDelegate(0,1000000,5,ui.tableWidget_tovPerelik));
	ui.tableWidget_tovPerelik->setItemDelegateForColumn(twExt->columnNumber("ForPrice"), new UCheckBoxDelegate(ui.tableWidget_tovPerelik,"У прайсі","Поза прайсом"));
	
	ui.tableWidget_tovPerelik->setColumnHidden(0, true);
	twExt->setColumnWidth("GroupID" , 150);
	twExt->setColumnWidth("Nomer", 70);
	twExt->setColumnWidth("Name", 360);
	twExt->setColumnWidth("Odynyci", 60);
	twExt->setColumnWidth("Cina" , 80);
	twExt->setColumnWidth("ForPrice" , 80);
	twExt->setColumnWidth("Prymitka", 120);
	
	setTovGroupFilter();
	tableWidget_tovPerelik_currentItemChanged();
	
	connect(ui.action_pasteTovStrings, SIGNAL(activated()), this, SLOT(action_pasteTovStrings_activated()));
	connect(ui.action_createPrice, SIGNAL(activated()), this, SLOT(action_createPrice_activated()));
	
	connect(ui.pushButton_addTov, SIGNAL(clicked()), this, SLOT(pushButton_addTov_clicked()));
	connect(ui.pushButton_delTov, SIGNAL(clicked()), this, SLOT(pushButton_delTov_clicked()));
	
	connect(ui.pushButton_addGroupTov, SIGNAL(clicked()), this, SLOT(pushButton_addGroupTov_clicked()));
	connect(ui.pushButton_delGroupTov, SIGNAL(clicked()), this, SLOT(pushButton_delGroupTov_clicked()));
	connect(ui.tableWidget_tovGroups, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(tableWidget_tovGroups_itemChanged(QTableWidgetItem *)));
	connect(ui.tableWidget_tovGroups->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(setTovGroupFilter()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::populateTovGroupsTableWidget()
{
	QSqlQuery query;
	query.exec("SELECT count(*) FROM tovGroups");
	query.next();
	int rowCount = query.value(0).toInt();
	ui.tableWidget_tovGroups->setRowCount(rowCount+1);
	QTableWidgetItem *item;
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, 0);
	ui.tableWidget_tovGroups->setItem(0,0,item);
	item = new QTableWidgetItem("Усі групи");
	item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));
	ui.tableWidget_tovGroups->setItem(0,1,item);
	item = new QTableWidgetItem("- - -");
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));
	ui.tableWidget_tovGroups->setItem(0,2,item);
	
	query.exec("SELECT id, Name, Rahunok FROM tovGroups ORDER BY name");
	for (int row=1; row<=rowCount && query.next(); row++){
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(0).toInt());
		ui.tableWidget_tovGroups->setItem(row,0,item);
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(1).toString());
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		ui.tableWidget_tovGroups->setItem(row,1,item);
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, query.value(2).toString());
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget_tovGroups->setItem(row,2,item);
	}
	ui.tableWidget_tovGroups->setCurrentCell(0,1);
	ui.tableWidget_tovGroups->resizeRowsToContents();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::pushButton_addGroupTov_clicked()
{
	QSqlQuery query;
	query.exec("SELECT max(id) FROM tovGroups");
	query.next();
	query.value(0).toString();
	int id = query.value(0).toInt()+1;
	if (!query.exec("INSERT INTO tovGroups (id, name) VALUES ("+QString::number(id)+",'Нова група')")){
		QMessageBox::critical(0, "Помилка", "Помилка створення стрічки "+QString::number(id));
		return;
	}
	query.exec("SELECT id, Name, Rahunok FROM tovGroups WHERE id="+QString::number(id));
	query.next();
	int row = ui.tableWidget_tovGroups->rowCount();
	ui.tableWidget_tovGroups->insertRow(row);
	QTableWidgetItem *item;
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(0).toInt());
	ui.tableWidget_tovGroups->setItem(row,0,item);
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(1).toString());
	item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableWidget_tovGroups->setItem(row,1,item);
	item = new QTableWidgetItem();
	item->setData(Qt::EditRole, query.value(2).toString());
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	ui.tableWidget_tovGroups->setItem(row,2,item);
	ui.tableWidget_tovGroups->resizeRowToContents(row);
	item = ui.tableWidget_tovGroups->item(row,1);
	ui.tableWidget_tovGroups->setCurrentItem(item);
	ui.tableWidget_tovGroups->scrollToItem(item);
	ui.tableWidget_tovGroups->editItem(item);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::pushButton_delGroupTov_clicked()
{
	QSqlQuery query;
	if (!query.exec("DELETE FROM tovGroups WHERE id="+ui.tableWidget_tovGroups->item(ui.tableWidget_tovGroups->currentRow(),0)->data(Qt::UserRole).toString())){
		QMessageBox::critical(0, "Помилка", "Помилка видалення стрічки");
		return;
	}
	int curRow = ui.tableWidget_tovGroups->currentRow();
	ui.tableWidget_tovGroups->removeRow(curRow);
	if (ui.tableWidget_tovGroups->rowCount() > 0){
		if (curRow >= ui.tableWidget_tovGroups->rowCount())
			curRow = ui.tableWidget_tovGroups->rowCount()-1;
		ui.tableWidget_tovGroups->setCurrentCell(curRow, 1);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::tableWidget_tovGroups_itemChanged(QTableWidgetItem *item)
{
	disconnect(ui.tableWidget_tovGroups, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(tableWidget_tovGroups_itemChanged(QTableWidgetItem *)));
	
	QSqlQuery query;
	int id = ui.tableWidget_tovGroups->item(item->row(),0)->data(Qt::EditRole).toInt();
	query.exec("UPDATE tovGroups SET "+tovGroupsColNames.at(item->column())+"='"+item->data(Qt::EditRole).toString()+"' WHERE id="+QString::number(id));
	query.exec("SELECT "+tovGroupsColNames.at(item->column())+" FROM tovGroups WHERE id="+QString::number(id));
	query.next();
	item->setData(Qt::EditRole, query.value(0));
	
	setTableWidgetGroupTovDelegate();
	
	connect(ui.tableWidget_tovGroups, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(tableWidget_tovGroups_itemChanged(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::tableWidget_tovPerelik_currentItemChanged()
{
	if (ui.tableWidget_tovPerelik->rowCount()==0){
		emit curTovarChanged(0, 0);
		__curTovarId = 0;
		__curTovarCina = 0;
		return;
	}
	int tovId = ui.tableWidget_tovPerelik->item(ui.tableWidget_tovPerelik->currentRow(),0)->data(Qt::EditRole).toInt();
	double tovCina = ui.tableWidget_tovPerelik->item(ui.tableWidget_tovPerelik->currentRow(),5)->data(Qt::EditRole).toDouble();
	emit curTovarChanged(tovId, tovCina);
	__curTovarId = tovId;
	__curTovarCina = tovCina;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::pushButton_addTov_clicked()
{
	if (!twExt->addRow("INSERT INTO tovPerelik (Odynyci, GroupId) VALUES ('шт.', 0)"))
		QMessageBox::critical(0,"Помилка створення запису","Запис шляхового листа не створено");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::pushButton_delTov_clicked()
{
	if	( QMessageBox::question(
                this,
                "!!! Видалення стрічки !!!",
                "Ви дійсно бажаєте видалити сирічку?",
                "Так",
				"Ні-ні-ні",
                QString(), 0, 1)
		)
		return;
	if (!twExt->deleteRow())
		QMessageBox::critical(0,"Помикла видалення запису","Запис не видалено");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::setTableWidgetGroupTovDelegate()
{
	QSqlQuery query;
	QHash<int, QString> delegateHash;
		//Делегат назвигрупи
	delegateHash.insert(0, "");
	query.exec("SELECT Id, name FROM tovGroups");
	while (query.next()){
		delegateHash.insert(query.value(0).toInt(), query.value(1).toString());
	}
	ui.tableWidget_tovPerelik->setItemDelegateForColumn(1, new UHashComboDelegate(ui.tableWidget_tovPerelik, delegateHash));	
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialog_katalog_tovaru::curTovarId()
{
	return __curTovarId;
}
//--------------------utech--------------------utech--------------------utech--------------------
double UDialog_katalog_tovaru::curTovarCina()
{
	return __curTovarCina;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::setTovGroupFilter()
{
	disconnect(ui.tableWidget_tovPerelik->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)), this, SLOT(tableWidget_tovPerelik_currentItemChanged()));
	
	if (ui.tableWidget_tovGroups->currentRow() == 0){
		twExt->clearSqlFilter();
	}
	else{
		twExt->setSqlFilter("GroupID="+ui.tableWidget_tovGroups->item(ui.tableWidget_tovGroups->currentRow(),0)->data(Qt::EditRole).toString());
	}
	twExt->populateTable();
	tableWidget_tovPerelik_currentItemChanged();
	
	connect(ui.tableWidget_tovPerelik->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)), this, SLOT(tableWidget_tovPerelik_currentItemChanged()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::action_pasteTovStrings_activated()
{
	
	const QMimeData * data = QApplication::clipboard()->mimeData();
	if (!data->hasText())
		return;
	
	QString clBoardText = data->text();
	QTextStream textStream(&clBoardText, QIODevice::ReadOnly);
	QString textLine;
	int currentRow = ui.tableWidget_tovPerelik->currentRow();
	QTableWidgetItem *item;
	while (!textStream.atEnd()){
		textLine = textStream.readLine().trimmed();
		while (textLine.contains("\t\t"))
			textLine = textLine.replace("\t\t","\t");
		
		ui.tableWidget_tovPerelik->insertRow(++currentRow);
		
		item = new QTableWidgetItem();
		QSqlQuery query("INSERT INTO tovPerelik (Odynyci, GroupId) VALUES ('шт.', 0)");
		query.exec("SELECT last_insert_id()");
		query.next();
		item->setData(Qt::EditRole, query.value(0));
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		ui.tableWidget_tovPerelik->setItem(currentRow, 0, item);
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, ui.tableWidget_tovGroups->item(ui.tableWidget_tovGroups->currentRow(), 0)->data(Qt::DisplayRole));
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		ui.tableWidget_tovPerelik->setItem(currentRow, 1, item);
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, "");
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		ui.tableWidget_tovPerelik->setItem(currentRow, 2, item);
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, textLine.section('\t',0,0).trimmed());
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		ui.tableWidget_tovPerelik->setItem(currentRow, 3, item);
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, textLine.section('\t',1,1).trimmed());
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		ui.tableWidget_tovPerelik->setItem(currentRow, 4, item);
		item = new QTableWidgetItem();
		item->setData(Qt::EditRole, cinaTovaru(textLine.section('\t',2,2)));
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget_tovPerelik->setItem(currentRow, 5, item);
		/*item = new QTableWidgetItem();
		item->setData(Qt::EditRole, 0);
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget_tovPerelik->setItem(currentRow, 6, item);*/
		
		//QMessageBox::information(0, "Буфер обміну", textLine.section('\t',0,0)+"\n"+textLine.section('\t',1,1)+"\n"+cinaTovaru(textLine.section('\t',2,2)));
		
		ui.tableWidget_tovPerelik->resizeRowToContents(currentRow);
	}
	item = ui.tableWidget_tovPerelik->item(currentRow, 1);
	ui.tableWidget_tovPerelik->setCurrentItem(item);
	ui.tableWidget_tovPerelik->scrollToItem(item);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::action_createPrice_activated()
{
	UDialogCreatePrice *d = new UDialogCreatePrice(this);
	if (d->exec()){
		createPriceFile(d->begDate(), d->endDate(),d->minZamovl(), d->priceType(), d->dbVersion());
	}
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
double UDialog_katalog_tovaru::cinaTovaru(QString str)
{
	QString newStr;
	str = str.replace(',','.');
	bool isNumber=false;
	for (int i=0; i<str.size(); i++){
		if (str[i].isNumber() || (isNumber && str[i]=='.')){
			isNumber = true;
			newStr += str[i];
		}
		else if (isNumber && (str[i].isLetter()))
			break;
	}
	return newStr.toDouble();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::populateToolBar(QToolBar * tBar)
{
	tBar->addAction(ui.action_createPrice);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialog_katalog_tovaru::createPriceFile(QDate begDate,QDate endDate,double minZamovl,int priceType,int dbVersion)
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings settings("./gurtSklad.ini",	QSettings::IniFormat);
	QSqlQuery query;
	QSqlRecord rec;
	QString fileName;
	fileName = QFileDialog::getSaveFileName(this,"Збереження файлу прайсу", settings.value("Price/DefaultSavePricePath").toString(), "Прайси (*.oiko)");
	if (fileName.size() < 5)
		return;
	QFile f(fileName);
	if (f.exists(fileName))
		f.remove(fileName);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text)){
		QMessageBox::critical(0,"Помилка","Помилка відкривання файлу\n"+fileName+".");
		return;
	}
	
	QTextStream stream(&f);
	stream.setCodec("UTF-8");
	int groupsCount=0;
	int tovCount=0;
	
stream << "BEGIN TRANSACTION;\n";
//Формування таблиці групи товарів
stream << "CREATE TABLE tovGroups( \
  id INTEGER PRIMARY KEY AUTOINCREMENT, \
  name TEXT \
);\n";
query.exec("SELECT tovGroups.id, tovGroups.Name FROM tovGroups \
			LEFT JOIN tovPerelik ON tovGroups.id=tovPerelik.GroupId \
			WHERE ForPrice=true \
			GROUP BY tovGroups.id, tovGroups.Name \
			ORDER BY tovGroups.Name");
while (query.next()){
	stream << "INSERT INTO tovGroups VALUES("+sqlStr(query.value(0).toInt())+", "+escapeSqlStr(query.value(1).toString(), USql::USQLITE)+");\n";
	++groupsCount;
}
//Формування таблиці перелік товару
stream << "CREATE TABLE tovPerelik( \
  id INTEGER PRIMARY KEY AUTOINCREMENT, \
  GroupID INTEGER, \
  Name TEXT, \
  Odynyci TEXT, \
  Cina DOUBLE, \
  Kilkist INTEGER \
);\n";
query.exec("SELECT * FROM tovPerelik \
			WHERE ForPrice=true ORDER BY GroupID, Name, Odynyci");
while (query.next()){
	rec = query.record();
	stream << "INSERT INTO tovPerelik VALUES("+sqlStr(rec.value("id").toInt())+", "+sqlStr(rec.value("GroupID").toInt())+", "+escapeSqlStr(rec.value("Name").toString(), USql::USQLITE)+", "+escapeSqlStr(rec.value("Odynyci").toString(), USql::USQLITE)+", "+sqlStr(rec.value("Cina").toDouble(),false)+", 0);\n";
	++tovCount;
}
//Версія бази даних
stream << "CREATE TABLE version( \
  id INTEGER PRIMARY KEY AUTOINCREMENT, \
  version TEXT \
);\n";
stream << "INSERT INTO version VALUES(1, '"+sqlStr(dbVersion)+"');\n";
//Термін дії прайсу
stream << "CREATE TABLE terminDiyi( \
  id INTEGER PRIMARY KEY AUTOINCREMENT, \
  beginDate TEXT, \
  endDate TEXT, \
  cinaType INTEGER, \
  minZamovlSum DOUBLE \
);\n";
stream << "INSERT INTO terminDiyi VALUES(1, '"+begDate.toString("yyyy-MM-dd")+"', '"+endDate.toString("yyyy-MM-dd")+"', "+sqlStr(priceType)+", "+sqlStr(minZamovl,false)+");\n";
//Закінчення
stream << "COMMIT;\n";

	settings.beginGroup("Price");
	settings.setValue("DefaultSavePricePath",fileName);
	settings.setValue("DefaultSavePricePath",fileName);
	settings.endGroup();
	
	QMessageBox::information(0, "Завершено формування прайсу","Завершено формування прайсу. \nЗаписано\n     груп: "+QString::number(groupsCount)+";\n     позицій товару: "+QString::number(tovCount)+".");
}
//--------------------utech--------------------utech--------------------utech--------------------
