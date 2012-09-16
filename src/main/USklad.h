//------------------------------------------------------------
//
// Declaration of dbUpdaterFunctions
// Created by Roman Mulyk 16.01.2009
//
//------------------------------------------------------------

#ifndef USKLAD_H
#define USKLAD_H

#include <QtSql>
#include <QMessageBox>
//------------------------------------------------------------
class USklad
{
  public:
	static QString version();
	static int requiredDbVersion();
	static int currentDbVersion();
	static QString programName();
	static bool connectToDbSklad();
};
//------------------------------------------------------------
class UDbUpdater : public QObject
{
  public:
	UDbUpdater();
	~UDbUpdater();
	bool checkVersions();
	int updateVersions();
	static bool updateDataBase();
	static void checkExistanceOfVersionTable();
  private:
	int versionId;
	int curVersion;
	bool updateWarnins;
}; 
//------------------------------------------------------------
#endif
