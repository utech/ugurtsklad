//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGFIRMINFO_SKLAD_H
#define UDIALOGFIRMINFO_SKLAD_H

#include <QDialog>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>

#include "ui_UDialogFirmInfo_sklad.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UDialogFirmInfo_sklad : public QDialog
{
  Q_OBJECT

  public:
		UDialogFirmInfo_sklad(QWidget *parent = 0);
		~UDialogFirmInfo_sklad();
		
		bool saveFirmInfo();
		void populateOrgInfo();
  public slots:
		void acceptDialog();
		void onlySaveInfo();
		
	  private:
		Ui::UDialogFirmInfo_sklad ui;

};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
