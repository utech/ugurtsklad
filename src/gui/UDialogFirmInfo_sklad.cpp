//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "QtDebug"

#include "UDialogFirmInfo_sklad.h"
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UDialogFirmInfo_sklad::UDialogFirmInfo_sklad(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	
	setMaximumHeight(height());
	setMinimumSize(width(), height());
	
	ui.comboBox_podatokType->addItem("", 0);
	ui.comboBox_podatokType->addItem("ПДВ", 1);
	ui.comboBox_podatokType->addItem("Єдиний податок", 2);
	ui.comboBox_podatokType->addItem("Фіксований патент", 3);
	
	ui.comboBox_pdvVal->addItem("");
	ui.comboBox_pdvVal->addItem("20");
	
	populateOrgInfo();
	
	connect(ui.pushButton_accept, SIGNAL(clicked ()), this, SLOT(acceptDialog()));
	connect(ui.pushButton_saveOnly, SIGNAL(clicked ()), this, SLOT(onlySaveInfo()));
	connect(ui.pushButton_reject, SIGNAL(clicked ()), this, SLOT(reject()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogFirmInfo_sklad::acceptDialog()
{
	if (saveFirmInfo())
		accept();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogFirmInfo_sklad::onlySaveInfo()
{
	saveFirmInfo();
}
//--------------------utech--------------------utech--------------------utech--------------------
bool UDialogFirmInfo_sklad::saveFirmInfo()
{
	bool doneOk;
	QSqlQuery query;
	query.exec("SELECT count(*) FROM orgInfo WHERE PidprId=1");
	query.seek(0);
	if (query.value(0).toInt() < 0){
		query.exec("INSERT INTO `orgInfo` (PidprId) VALUES (1)");
	}
	doneOk = query.exec("UPDATE orgInfo SET \
							PovnaNazvaPidpr="+mysqlStr(ui.lineEdit_povnaNazvaPidpr->text().replace('\'','\"'))+", \
							ScorochNazvaPidpr="+mysqlStr(ui.lineEdit_scorochNazvaPidpr->text().replace('\'','\"'))+", \
							Edrpoy="+mysqlStr(ui.lineEdit_edrpoy->text().replace('\'','\"'))+", \
							Adresa="+mysqlStr(ui.lineEdit_adresa->text().replace('\'','\"'))+", \
							PipDurect="+mysqlStr(ui.lineEdit_PipDurect->text().replace('\'','\"'))+", \
							PipBuh="+mysqlStr(ui.lineEdit_PipBuh->text().replace('\'','\"'))+", \
							TelefonDurekt="+mysqlStr(ui.lineEdit_telefonDurekt->text().replace('\'','\"'))+", \
							TelefonBuh="+mysqlStr(ui.lineEdit_telefonBuh->text().replace('\'','\"'))+", \
							Email="+mysqlStr(ui.lineEdit_email->text().replace('\'','\"'))+",  \
							Fax="+mysqlStr(ui.lineEdit_fax->text().replace('\'','\"'))+", \
							Bank="+mysqlStr(ui.lineEdit_bank->text().replace('\'','\"'))+", \
							MFO="+mysqlStr(ui.lineEdit_MFO->text().replace('\'','\"'))+", \
							BankRahunok="+mysqlStr(ui.lineEdit_rahunok->text().replace('\'','\"'))+", \
							Podatok="+mysqlStr(ui.comboBox_podatokType->itemData(ui.comboBox_podatokType->currentIndex(),Qt::UserRole).toString())+", \
							SvidPDVNum="+mysqlStr(ui.lineEdit_svidPlatnPdv->text().replace('\'','\"'))+", \
							IPN="+mysqlStr(ui.lineEdit_ipn->text().replace('\'','\"'))+", \
							PodatokVal="+mysqlStr(ui.comboBox_pdvVal->currentText().toInt())+", \
							CinaZPodatkom="+mysqlStr(ui.checkBox_cinaMistytPdv->isChecked())+" \
						WHERE PidprId=1 LIMIT 1");
	if (doneOk)
		QMessageBox::information(this,"!!! Успішна зміна !!!","Інформацію про підприємство успішно змінено.");
	else
		QMessageBox::critical(this,"!!! Невдала зміна запису !!!","Програма не змогла змінити дані про підприємство.");
	
	if (doneOk)
		populateOrgInfo();
	return doneOk;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogFirmInfo_sklad::populateOrgInfo()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT * FROM orgInfo WHERE PidprId=1 LIMIT 1");
	query->seek(0);
	QSqlRecord rec = query->record();
	
	ui.lineEdit_povnaNazvaPidpr->setText(rec.value("PovnaNazvaPidpr").toString());
	ui.lineEdit_scorochNazvaPidpr->setText(rec.value("ScorochNazvaPidpr").toString());
	ui.lineEdit_edrpoy->setText(rec.value("Edrpoy").toString());
	ui.lineEdit_adresa->setText(rec.value("Adresa").toString());
	ui.lineEdit_PipDurect->setText(rec.value("PipDurect").toString());
	ui.lineEdit_PipBuh->setText(rec.value("PipBuh").toString());
	ui.lineEdit_telefonDurekt->setText(rec.value("TelefonDurekt").toString());
	ui.lineEdit_telefonBuh->setText(rec.value("TelefonBuh").toString());
	ui.lineEdit_email->setText(rec.value("Email").toString());
	ui.lineEdit_fax->setText(rec.value("Fax").toString());
	ui.lineEdit_bank->setText(rec.value("Bank").toString());
	ui.lineEdit_MFO->setText(rec.value("MFO").toString());
	ui.lineEdit_rahunok->setText(rec.value("BankRahunok").toString());
	
	ui.comboBox_podatokType->setCurrentIndex(ui.comboBox_podatokType->findData((rec.value("Podatok").toInt())));
	ui.lineEdit_svidPlatnPdv->setText(rec.value("SvidPDVNum").toString());
	ui.lineEdit_ipn->setText(rec.value("IPN").toString());
	ui.comboBox_pdvVal->setCurrentIndex(ui.comboBox_pdvVal->findData(rec.value("PodatokVal").toDouble(), Qt::DisplayRole));
	ui.checkBox_cinaMistytPdv->setChecked(rec.value("CinaZPodatkom").toBool());
	
	delete query;
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogFirmInfo_sklad::~UDialogFirmInfo_sklad()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
