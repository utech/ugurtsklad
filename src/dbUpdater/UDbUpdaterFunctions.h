//------------------------------------------------------------
//
// Declaration of dbUpdaterFunctions
// Created by Roman Mulyk 16.01.2009
//
//------------------------------------------------------------

#ifndef DBUPDATERFUNCTIONS_H
#define DBUPDATERFUNCTIONS_H

#include <QtSQL>
#include <QMessageBox>

//------------------------------------------------------------
class UDbUpdater : public QObject
{
  public:
	UDbUpdater();
	~UDbUpdater();
	bool checkVersions();
	int updateVersions();
	static updateDataBase();
  private:
	int versionId;
	int curVersion;
	bool updateWarnins;
}; 
//------------------------------------------------------------
#endif
