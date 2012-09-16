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

#ifndef USKLADPROGOPTIONS_H
#define USKLADPROGOPTIONS_H

#include <QtDebug>
#include <QSqlQuery>
#include <QVariant>

//--------------------utech--------------------utech--------------------utech--------------------
class USkladProgOptions
{
  public:
	int organizPodatok();
	QString organizPodatokName();
	bool organizCinaZPodatkom();
	double organizVidsotokPodatku();
};
//--------------------utech--------------------utech--------------------utech--------------------
class USkladZbutRozrah
{
  public:
	USkladZbutRozrah();
	
	QString rekvizytyPostachalnyka(int PostachId=1);
	QString rekvizytyZamovnyka(int nPartnerId);
	QString headerCinaStr();
	QString headerSumaStr();
	double curKilkist(double kilkist = -1);
	double curCina(double cina = -1);
	double curSuma();
	void nextTovar();
	QString pidsRazomStr();
	double pidsRazom();
	QString pidsPodatokStr();
	double pidsPodatok();
	QString pidsPodatokValStr();
	QString pidsDoPlatyStr();
	double pidsDoOplaty();
	QString pidsDoOplatyValStr();
	QString pidsHumanLangDoOplatyStr();
	QString pidsHumanLangPodatokStr();
	QString pidsHumanStr();
  private:
	int _podatokType;
	double _podatokVal;
	bool _cinaZPodatkom;
	bool _dockTovZPodatkom;
	double _sumaNakl;
	double _curKilkist;
	double _curCina;
	bool _podatokCalculated;
	double _podatokSum;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
