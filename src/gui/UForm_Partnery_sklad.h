//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна реєстрації користувача
//
// Створений програмістами Компанії "Утех" 29.03.2009р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UFORM_PARTNERY_SKLAD_H
#define UFORM_PARTNERY_SKLAD_H

#include <QtGui>
#include <QSettings>
#include <QDir>
#include <QtSql>

#include "ui_UForm_Partnery_sklad.h"
#include "USkladTabWidget.h"

#include <UTableWidgetSqlExt>
//--------------------utech--------------------utech--------------------utech--------------------

//--------------------utech--------------------utech--------------------utech--------------------
class UForm_Partnery_sklad : public USkladTabWidget
{
  Q_OBJECT

  public:
		UForm_Partnery_sklad(QWidget *parent = 0);
		~UForm_Partnery_sklad();
		void populateToolBar(QToolBar * tBar);
  public slots:
		void pushButton_create_clicked();
		void pushButton_delete_clicked();
		void dovPartnersChanged();
  signals:
		void partnersDovChanged();
  private:
		Ui::UForm_Partnery_sklad ui;
		
		UDataFormView *twExt;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
