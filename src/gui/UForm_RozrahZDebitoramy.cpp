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

#include "UForm_RozrahZDebitoramy.h"

#include "UDialog_editNakladnaSklad.h"
#include "UPrintDocs_Sklad.h"

#include <UTableWidgetSqlExt>

//--------------------utech--------------------utech--------------------utech--------------------
UForm_RozrahZDebitoramy::UForm_RozrahZDebitoramy(QWidget *parent)
        : USkladTabWidget(parent)
{
	ui.setupUi(this);
	
	setTableWidgetOplataPartnerDelegate();
	ui.tableWidget_oplata->setColumnHidden(0, true);
	ui.tableWidget_oplata->setColumnHidden(1, true);
	ui.tableWidget_oplata->setColumnWidth(2, 100);
	ui.tableWidget_oplata->setColumnWidth(3, 100);
	ui.tableWidget_oplata->setColumnWidth(4, 100);
	
	connect(ui.pushButton_closeProgram, SIGNAL(clicked(bool)), this, SIGNAL(mainWindowClosedThis()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_RozrahZDebitoramy::setTableWidgetOplataPartnerDelegate()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
void UForm_RozrahZDebitoramy::populateToolBar(QToolBar * tBar)
{
	//tBar->addAction(ui.action_printNakladna);
}
//--------------------utech--------------------utech--------------------utech--------------------
UForm_RozrahZDebitoramy::~UForm_RozrahZDebitoramy()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
