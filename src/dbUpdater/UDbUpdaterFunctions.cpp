//------------------------------------------------------------
//
// Implamantation of dbUpdaterFunctions
// Created by Roman Mulyk 16.01.2009
//
//------------------------------------------------------------

#include <USqlAccessible>

#include "UDbUpdaterFunctions.h"

//------------------------------------------------------------
UDbUpdater::UDbUpdater()
{
	
}
//------------------------------------------------------------
UDbUpdater::~UDbUpdater()
{
	
}
//------------------------------------------------------------
bool UDbUpdater::checkVersions()
{
	bool ver_ok=false;
	QSqlQuery query;
	//�������� ��������� ������� ���� ���� �����
	if (!query.exec("SELECT true FROM skladDbVersion WHERE id=1") || !query.next()){
		query.exec("CREATE TABLE skladDbVersion( \
						id INTEGER UNSIGNED, \
						ver INTEGER UNSIGNED\
					)");
		query.exec("INSERT INTO skladDbVersion(id, ver) VALUES (1,0)");
	}
	query("SELECT id, ver FROM skladDbVersion ORDER BY id");
	query.seek(0);
	versionId = query.value(0).toInt();
	curVersion = query.value(1).toInt();
	//qDebug() << "����� ������� ���� �����: " << curVersion;
	if (curVersion < TKE_DB_VERSION ){ //�������� ���� �����
		int updatedToVersion = updateVersions();
		//int updatedToVersion = TKE_DB_VERSION;
		if (updatedToVersion == TKE_DB_VERSION){
			QMessageBox::information(0,
				"!!! ���������� ���� !!!",
				"���� ����� �� \"�������� ��������\" ���� ������ ��������� �� ����: "+QString::number(updatedToVersion)+"."+
				(updateWarnins ? "\n��� ��� ��������� ������� ����� �������, ����� ����� � ������ ����� �������� ���� ��������� ����������":"" ),
				QMessageBox::Ok,
				QMessageBox::NoButton);
			return true;
		}
		else if (updatedToVersion == -1){
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! ������� ���������� ���� �� \"�������� ��������\" !!!",
									"�������� �� ������ �������� ���� ����� �� \"�������� ��������\" ����� ������� �������.\n"
									"������ � ��������� ����� �� ���� ��������� �� ���������� ������ ������� ������� ��������.\n"
									"������! ��������� ������� �������� �� ��� ��� ���������� �� ������������.\n"
									"���� �� ������ ���������� ������ � ��������� ���������� �� ������ �������, ������ \"����������\".");
			mBox->addButton("�����", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("����������", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
		else{
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! ������� ���������� ���� �� \"�������� ��������\" !!!",
									"�������� �������� �� �� ����: "+QString::number(updatedToVersion)+",\n� ��� �������� ������ �������� ��������� ���� ����� ���� "+QString::number(TKE_DB_VERSION)+".\n"
									"������ � ��������� ����� �� ���� ��������� �� ���������� ������ ������� ������� ��������.\n"
									"������! ��������� ������� �������� �� ��� ��� ���������� �� ������������.\n"
									"���� �� ������ ���������� ������ � ��������� ���������� �� ������ �������, ������ \"����������\".");
			mBox->addButton("�����", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("����������", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
	}
	else if (curVersion > TKE_DB_VERSION ){ //�������� ��������
		QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! �������� ����� �� \"�������� ��������\" !!!",
									"����������� �������� ����� �� \"�������� ��������\" ������� ������ ���� �����,\n"
									"�� ���� ��������� �� ���������� ������ ������� ������� ��������.\n"
									"������! ���������� ���� ����� �� \"�������� ���������\".\n"
									"���� �� ������ ���������� ������ � ��������� ���������� �� ������ �������, ������ \"����������\".");
		mBox->addButton("�����", QMessageBox::ActionRole);
		QPushButton *contButton = mBox->addButton("����������", QMessageBox::ActionRole);
		mBox->exec();
		if (mBox->clickedButton() == contButton)
			ver_ok = true;
		delete mBox;
	}
	else
		return true;
	return ver_ok;
}
//------------------------------------------------------------
int UDbUpdater::updateVersions()
{
	int returnVersion = curVersion;
	
	updateWarnins = false;
	
	//ϳ�'������� �� ���� ����� ������
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSqlDatabase updateTkeDataBaseVerDB = QSqlDatabase::addDatabase("QSQLITE", "updateTkeDataBaseVerDB" );
	updateTkeDataBaseVerDB.setDatabaseName("tkeDBUpdate.tkeDB");
	if (updateTkeDataBaseVerDB.open()){
		QSqlQuery updateQuery(updateTkeDataBaseVerDB);
		//������� ���� ���������
		updateQuery.exec(" \
CREATE TABLE IF NOT EXISTS updateFTFQuerys( \
	id INTEGER PRIMARY KEY AUTOINCREMENT, \
	Version INTEGER, \
	baseQuery TEXT, \
	trueQuery TEXT, \
	falseQuery TEXT, \
	failerQuery TEXT)");
		//������� ���� �����
		updateQuery.exec(" \
CREATE TABLE IF NOT EXISTS updateNZPQuerys( \
	id INTEGER PRIMARY KEY AUTOINCREMENT, \
	Version INTEGER, \
	baseQuery TEXT, \
	positiveQuery TEXT, \
	negativeQuery TEXT, \
	zeroQuery TEXT)");
	}
	else{
		qDebug() << "�������. �� ������� ��'�������� �� ���� ����� tke.testDb";
		return -1;
	}
	
	//ϳ�'������� �� ���� ����� ���������� ����������� ������
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSqlDatabase updateResultsDB = QSqlDatabase::addDatabase("QSQLITE", "updateResultsDB" );
	updateResultsDB.setDatabaseName("tkeDBUpdateResults.tkeDB");
	if (updateResultsDB.open()){
		QSqlQuery updateQuery(updateResultsDB);
		//������� ������ ���������� ���������� ���� �����
		updateQuery.exec(" \
CREATE TABLE IF NOT EXISTS updateDbResults( \
	id INTEGER PRIMARY KEY AUTOINCREMENT, \
	q_datetime TEXT, \
	q_id INTEGER, \
	q_version INTEGER, \
	q_type INTEGER, \
	q_executed INTEGER, \
	q_result TEXT, \
	q_ok TEXT)");
	}
	else{
		qDebug() << "�������. �� ������� ��'�������� �� ���� ����� tkeDBUpdateResults.tkeDB";
		return -1;
	}
	
	QSqlQuery updateQuery(updateTkeDataBaseVerDB);
	QSqlQuery versionsQuery(updateTkeDataBaseVerDB);
	QSqlQuery resultQuery(updateResultsDB);
	QSqlQuery tkeQuery;
	
	versionsQuery.exec("SELECT version FROM updateFTFQuerys \
		WHERE version BETWEEN "+QString::number(curVersion+1)+" AND "+QString::number(TKE_DB_VERSION)+" \
		GROUP BY version");
	QDateTime cDateTime;
	while (versionsQuery.next()){
		int updateVersion = versionsQuery.value(0).toInt();
		//��������� ������ true-false-fail
		updateQuery.exec("SELECT * FROM updateFTFQuerys WHERE version="+QString::number(updateVersion)+" ORDER BY id");
		while (updateQuery.next()){
			int queryPos;
			if (tkeQuery.exec(updateQuery.value(2).toString()) && tkeQuery.seek(0)){
				if (tkeQuery.value(0).toBool()) //true query
					queryPos = 3;
				else //false query
					queryPos = 4;
			}
			else{ //fail Query
				queryPos = 5;
			}
			bool okQuery = tkeQuery.exec(updateQuery.value(queryPos).toString());
			if (!okQuery)
				updateWarnins = true;
			cDateTime = QDateTime::currentDateTime();
			resultQuery.exec("INSERT INTO updateDbResults (q_datetime, q_id, \
									q_version, q_type, q_executed, \
									q_result, q_ok) \
								VALUES ("+sqlStr(cDateTime.toString("yyyy-MM-dd hh:mm:ss"))+", "+sqlStr(updateQuery.value(0).toInt())+", \
									"+sqlStr(updateVersion)+", 1, "+escapeSqlStr(tkeQuery.lastQuery(),USql::USQLITE)+", \
									"+escapeSqlStr(tkeQuery.lastError().text(),USql::USQLITE)+", '"+sqlStr(okQuery)+"')");
		}
		
		//��������� ������ negativ-zero-positiv (NZP)
		updateQuery.exec("SELECT * FROM updateNZPQuerys WHERE version="+QString::number(updateVersion)+" ORDER BY id");
		while (updateQuery.next()){
			int queryPos;
			if (tkeQuery.exec(updateQuery.value(2).toString()) && tkeQuery.seek(0)){
				double probValue = tkeQuery.value(0).toDouble();
				if (probValue>0.0000009)
					queryPos = 3;
				else if (probValue<-0.0000009)
					queryPos = 4;
				else //����
					queryPos = 5;
			}
			else{ //fail Query
				queryPos = 5;
			}
			bool okQuery = tkeQuery.exec(updateQuery.value(queryPos).toString());
			if (!okQuery)
				updateWarnins = true;
			cDateTime = QDateTime::currentDateTime();
			resultQuery.exec("INSERT INTO updateDbResults (q_datetime, q_id, \
									q_version, q_type, q_executed, \
									q_result, q_ok) \
								VALUES ("+sqlStr(cDateTime.toString("yyyy-MM-dd hh:mm:ss"))+", "+sqlStr(updateQuery.value(0).toInt())+", \
									"+sqlStr(updateVersion)+", 2, "+escapeSqlStr(tkeQuery.lastQuery(),USql::USQLITE)+", \
									"+escapeSqlStr(tkeQuery.lastError().text(),USql::USQLITE)+", '"+sqlStr(okQuery)+"')");
		}
		
		tkeQuery.exec("UPDATE skladDbVersion SET ver="+sqlStr(updateVersion));
		returnVersion = updateVersion;
	}
	
	return returnVersion;
}
//------------------------------------------------------------
bool UDbUpdater::updateDataBase()
{
	UDbUpdater upd;
	return upd.checkVersions();
}
//------------------------------------------------------------
