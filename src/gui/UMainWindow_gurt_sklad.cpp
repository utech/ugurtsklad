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

#include "UMainWindow_gurt_sklad.h"
#include <QDir>
#include <QMessageBox>

#include "UDialog_katalog_tovaru_sklad.h"
#include "UForm_Partnery_sklad.h"
#include "UForm_VhidniNakladni.h"
#include "UForm_VyhidniNakladni.h"
#include "UDialog_dateGroupIdFilter.h"
#include "UDialog_zbutFilter.h"
#include "UPrintDocs_Sklad.h"
#include "UDialogFirmInfo_sklad.h"

#include "../main/UGlobal_Sklad.h"

//--------------------utech--------------------utech--------------------utech--------------------
UMainWindow_gurt_sklad::UMainWindow_gurt_sklad(QWidget *parent)
        : QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tabWidget_sklad->removeTab(0);
	UForm_Partnery_sklad *partneryForm = new UForm_Partnery_sklad(ui.tabWidget_sklad);
	UForm_VhidniNakladni *vhNakladni = new UForm_VhidniNakladni(ui.tabWidget_sklad);
	UForm_VyhidniNakladni *vyhNakladni = new UForm_VyhidniNakladni(ui.tabWidget_sklad);
	ui.tabWidget_sklad->insertTab(0, new UDialog_katalog_tovaru(ui.tabWidget_sklad), "Довідник товарів" );
	ui.tabWidget_sklad->insertTab(1, partneryForm, "Партнери" );
	ui.tabWidget_sklad->insertTab(2, vhNakladni, "Вхідні накладні" );
	ui.tabWidget_sklad->insertTab(3, vyhNakladni, "Вихідні накладні" );
	connect(partneryForm, SIGNAL(partnersDovChanged()), vhNakladni, SLOT(setTableWidgetPartnerDelegate()));
	connect(partneryForm, SIGNAL(partnersDovChanged()), vyhNakladni, SLOT(setTableWidgetPartnerDelegate()));
	connect(this, SIGNAL(mainWindowClosed()), vhNakladni, SIGNAL(mainWindowClosed()));
	connect(this, SIGNAL(mainWindowClosed()), vyhNakladni, SIGNAL(mainWindowClosed()));
	connect(vhNakladni, SIGNAL(mainWindowClosedThis()), this, SLOT(close()));
	connect(vyhNakladni, SIGNAL(mainWindowClosedThis()), this, SLOT(close()));
	setAttribute(Qt::WA_DeleteOnClose);
	
	date = QDate::currentDate();
	
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	ui.tabWidget_sklad->setCurrentIndex(settings->value("GUI/PrevTabNumber").toInt());
	delete settings;
	
	tabWidget_sklad_currentChanged();
	
	//connectors
	connect(ui.action_show_debuger, SIGNAL(activated()), this, SLOT(request_debuger_window()));
	connect(ui.action_close_program, SIGNAL(activated()), this, SLOT(close()));
	connect(ui.action_printObigIZalyshky, SIGNAL(activated()), this, SLOT(action_printObigIZalyshky_activated()));
	connect(ui.action_printVidomistZbutuExt, SIGNAL(activated()), this, SLOT(action_printVidomistZbutuExt_activated()));
	connect(ui.action_options, SIGNAL(activated()), this, SLOT(action_options_activated()));
	connect(ui.action_about_programm, SIGNAL(activated()), this, SLOT(action_about_programm_activated()));
	
	connect(ui.tabWidget_sklad, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_sklad_currentChanged()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMainWindow_gurt_sklad::request_debuger_window()
{
	emit requestDebugWindow();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMainWindow_gurt_sklad::tabWidget_sklad_currentChanged()
{
	USkladTabWidget * w = qobject_cast<USkladTabWidget *>(ui.tabWidget_sklad->currentWidget());
	if (w){
		ui.toolBar_printDocks->clear();
		w->populateToolBar(ui.toolBar_printDocks);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMainWindow_gurt_sklad::action_printObigIZalyshky_activated()
{
	UDialog_dateGroupIdFilter * d = new UDialog_dateGroupIdFilter(this);
	if (d->exec()){
		UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
		printDocs->print_obigIZalyshky(d->begDate(), d->endDate(), d->groupId(), d->notNullOnly());
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMainWindow_gurt_sklad::action_printVidomistZbutuExt_activated()
{
	UDialog_zbutFilter * d = new UDialog_zbutFilter(this);
	if (d->exec()){
		UPrintDocs_Sklad *printDocs = new UPrintDocs_Sklad();
		printDocs->print_vidomistZbutuExt(d->begDate(), d->endDate(), d->groupId(), d->tovarId(), d->klientId(), d->nakladnaId());
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMainWindow_gurt_sklad::action_options_activated()
{
	UDialogFirmInfo_sklad *d = new UDialogFirmInfo_sklad();
	d->exec();
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMainWindow_gurt_sklad::action_about_programm_activated()
{
	QMessageBox::about(this,"Про програму",QString("Програма ")+QString(USklad::programName())+" версії "+QString(USklad::version())+" призначення для ведення обліку складу підприємства.");
}
//--------------------utech--------------------utech--------------------utech--------------------
UMainWindow_gurt_sklad::~UMainWindow_gurt_sklad()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./gurtSklad.ini",	QSettings::IniFormat);
	settings->setValue("GUI/PrevTabNumber", ui.tabWidget_sklad->currentIndex());
	delete settings;
	
	emit mainWindowClosed();
}
//--------------------utech--------------------utech--------------------utech--------------------
