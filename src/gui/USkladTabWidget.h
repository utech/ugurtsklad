//--------------------------------------------------------------------------------------------------------
//
// Copyright (C) 2005-2007 Utech Company. All rights reserved.
//
// ���� � �������� �������� �������� ����� ������ �� �����
//
// ��������� ������������ ������ "����" 15.01.2008�.
// ���������� ���������� �������� QT4.3.1
//
//--------------------------------------------------------------------------------------------------------

#ifndef USKLADTABWIDGET_H
#define USKLADTABWIDGET_H

#include <QToolBar>
#include <QWidget>

class USkladTabWidget : public QWidget
{
  Q_OBJECT
  public:
	USkladTabWidget(QWidget *parent=0) : QWidget(parent){};
	~USkladTabWidget(){};
	virtual void populateToolBar(QToolBar * tBar){};
};

#endif
