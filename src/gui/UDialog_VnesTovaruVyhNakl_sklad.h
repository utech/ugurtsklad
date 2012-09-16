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

#ifndef UDIALOG_VNESTOVARUVYHNAKL_SKLAD_H
#define UDIALOG_VNESTOVARUVYHNAKL_SKLAD_H

#include <QDialog>
#include <QSettings>
#include <QSqlQuery>

#include "ui_UDialog_VnesTovaruVyhNakl_sklad.h"

#include <QtDebug>
#include "UDialog_katalog_tovaru_sklad.h"

class UDialog_VnesTovaruVyhNakl_sklad : public QDialog
{
        Q_OBJECT
    public:
        UDialog_VnesTovaruVyhNakl_sklad(int nakladnaId, QWidget *parent = 0);
		~UDialog_VnesTovaruVyhNakl_sklad();
    public slots:
		void pushButton_accept_clicked();
		void pushButton_reject_clicked();
		
		void calcTovVartist();
		void dKatalogTovaru_curTovarChanged(int tovId, double cina);
		void activateNakladnaControls(int tovId);
		void activateCinaSpinbox();
		void populateZalyshkyTable(int tovarId);
	signals:
	
    private:
		Ui::UDialog_VnesTovaruVyhNakl_sklad ui;
		UDialog_katalog_tovaru *dKatalogTovaru;
		
		int nId;
		int curTovarId;
};

class USkladskyiRuhTovaru : public QObject
{
		Q_OBJECT
  public:
	static bool vydatyTovar(const int nId, const int tovId, const double kilkist, const double cina, bool zakupCina=false);
	static bool unVydatyTovar(const int nId, const int tovId, const double kilkist, const double cina);
	static bool changeTovarCina(const int nId, const int tovId, const double prevCina, const double curCina);
	static bool changeTovarKilkist(const int nId, const int tovId, const double prevKilkist, const double curKilkist, const double cina);
	static double calcTovZalyshky(int tovId);
};

#endif
