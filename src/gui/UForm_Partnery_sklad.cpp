//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна налаштування програми
//
// Створений програмістами Компанії "Утех" 29.03.2009р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UForm_Partnery_sklad.h"

#include <UStandardDelegate>

#include <QtDebug>
//--------------------utech--------------------utech--------------------utech--------------------
UForm_Partnery_sklad::UForm_Partnery_sklad(QWidget *parent)
        : USkladTabWidget(parent)
{
	ui.setupUi(this);	
	
	twExt = new UDataFormView(ui.tableWidget_partnery, "dovPartners");
	twExt->setSortingEnabled(true);
	twExt->colNamesList << "Id"
						<< "Name"
						<< "EDRPOU"
						<< "Adresa"
						<< "Tel"
						<< "PlatnPdv"
						<< "IPN"
						<< "SvidPdvNum"
						<< "UgodaNum"
						<< "UgodaDate"
						<< "Prymitka"
						<< "IsKlient"
						<< "IsPostach";
	
	twExt->addColAlignment("Name", Qt::AlignLeft | Qt::AlignVCenter);
	twExt->addColAlignment("EDRPOU", Qt::AlignRight | Qt::AlignVCenter);
	twExt->addColAlignment("Adresa", Qt::AlignLeft | Qt::AlignVCenter);
	twExt->addColAlignment("Tel", Qt::AlignLeft | Qt::AlignVCenter);
	twExt->addColAlignment("PlatnPdv", Qt::AlignRight | Qt::AlignVCenter);
	twExt->addColAlignment("IPN", Qt::AlignRight | Qt::AlignVCenter);
	twExt->addColAlignment("SvidPdvNum", Qt::AlignRight | Qt::AlignVCenter);
	twExt->addColAlignment("UgodaNum", Qt::AlignRight | Qt::AlignVCenter);
	twExt->addColAlignment("UgodaDate", Qt::AlignRight | Qt::AlignVCenter);
	twExt->addColAlignment("Prymitka", Qt::AlignLeft | Qt::AlignVCenter);
	twExt->addColAlignment("IsKlient", Qt::AlignCenter | Qt::AlignVCenter);
	twExt->addColAlignment("IsPostach", Qt::AlignCenter | Qt::AlignVCenter);
	
	twExt->insertWidget(ui.lineEdit_organizName, "Name");
	twExt->insertWidget(ui.lineEdit_edrpou, "EDRPOU");
	twExt->insertWidget(ui.lineEdit_adresa, "Adresa");
	twExt->insertWidget(ui.lineEdit_telefon, "Tel");
	twExt->insertWidget(ui.checkBox_platnykPdv, "PlatnPdv");
	twExt->insertWidget(ui.lineEdit_ipn, "IPN");
	twExt->insertWidget(ui.lineEdit_svidPdv, "SvidPdvNum");
	twExt->insertWidget(ui.lineEdit_ugodaNum, "UgodaNum");
	twExt->insertWidget(ui.dateEdit_ugodaDate, "UgodaDate");
	twExt->insertWidget(ui.lineEdit_prymitka, "Prymitka");
	twExt->insertWidget(ui.checkBox_klient, "IsKlient");
	twExt->insertWidget(ui.checkBox_postachaln, "IsPostach");
	
	ui.tableWidget_partnery->setItemDelegateForColumn(twExt->columnNumber("PlatnPdv"), new UCheckBoxDelegate(ui.tableWidget_partnery));
	ui.tableWidget_partnery->setItemDelegateForColumn(twExt->columnNumber("IsKlient"), new UCheckBoxDelegate(ui.tableWidget_partnery));
	ui.tableWidget_partnery->setItemDelegateForColumn(twExt->columnNumber("IsPostach"), new UCheckBoxDelegate(ui.tableWidget_partnery));
	
	ui.tableWidget_partnery->setItemDelegateForColumn(twExt->columnNumber("UgodaDate"), new UDateTimeDelegate(ui.tableWidget_partnery, "dd.MM.yyyy"));
	
	ui.tableWidget_partnery->setColumnHidden(0, true);
	twExt->setColumnWidth("Name", 300);
	twExt->setColumnWidth("EDRPOU", 70);
	twExt->setColumnWidth("Adresa", 250);
	twExt->setColumnWidth("Tel", 70);
	twExt->setColumnWidth("PlatnPdv", 60);
	twExt->setColumnWidth("IPN", 90);
	twExt->setColumnWidth("SvidPdvNum", 90);
	twExt->setColumnWidth("UgodaNum", 70);
	twExt->setColumnWidth("UgodaDate", 80);
	twExt->setColumnWidth("Prymitka", 150);
	twExt->setColumnWidth("IsKlient", 60);
	twExt->setColumnWidth("IsPostach", 60);
	
	twExt->populateTable();
	
	connect(ui.pushButton_create, SIGNAL(clicked()), this, SLOT(pushButton_create_clicked()));
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(pushButton_delete_clicked()));
	connect(ui.pushButton_reload, SIGNAL(clicked()), twExt, SLOT(populateTable()));
	
	connect(twExt, SIGNAL(tableItemEdited(QTableWidgetItem *)), this, SLOT(dovPartnersChanged()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_Partnery_sklad::pushButton_create_clicked()
{
	if (!twExt->addRow("INSERT INTO dovPartners (IsKlient, IsPostach) VALUES (true, true)"))
		QMessageBox::critical(0,"Помилка створення запису","Запис не створено");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_Partnery_sklad::pushButton_delete_clicked()
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
	if (!twExt->deleteSelectedRows())
		QMessageBox::critical(0,"Помикла видалення запису","Запис не видалено");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_Partnery_sklad::dovPartnersChanged()
{
	emit partnersDovChanged();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_Partnery_sklad::populateToolBar(QToolBar * tBar)
{
	//tBar->addAction(ui.action_printLystkyRegulyarnostiRuhu);
}
//--------------------utech--------------------utech--------------------utech--------------------
UForm_Partnery_sklad::~UForm_Partnery_sklad()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
