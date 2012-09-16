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
	//Перевірка існування таблиці версії бази даних
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
	//qDebug() << "Версія поточної бази даних: " << curVersion;
	if (curVersion < TKE_DB_VERSION ){ //Застаріла база даних
		int updatedToVersion = updateVersions();
		//int updatedToVersion = TKE_DB_VERSION;
		if (updatedToVersion == TKE_DB_VERSION){
			QMessageBox::information(0,
				"!!! Поновлення бази !!!",
				"Базу даних ПК \"ТКЕнерго менеджер\" було успішно поновлено до версії: "+QString::number(updatedToVersion)+"."+
				(updateWarnins ? "\nАле при поновленні сталося кілька помилок, таким чином у деяких місцях програма може працювати некоректно":"" ),
				QMessageBox::Ok,
				QMessageBox::NoButton);
			return true;
		}
		else if (updatedToVersion == -1){
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! Помилка поновлення бази ПК \"ТКЕнерго менеджер\" !!!",
									"Програма не змогла поновити базу даних ПК \"ТКЕнерго менеджер\" через помилку системи.\n"
									"Робота з застарілою версією БД може призвести до некоректної роботи частини функцій програми.\n"
									"ПОРАДА! Спробуйте відкрити програму ще раз або звверніться до адміністратора.\n"
									"Якщо ви бажаєте продовжити роботу з програмою незважаючи на можливі помилки, клікніть \"Продовжити\".");
			mBox->addButton("Вийти", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("Продовжити", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
		else{
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! Помилка поновлення бази ПК \"ТКЕнерго менеджер\" !!!",
									"Програма поновила БД до версії: "+QString::number(updatedToVersion)+",\nа для стабільної роботи програми необхідна база даних версії "+QString::number(TKE_DB_VERSION)+".\n"
									"Робота з застарілою версією БД може призвести до некоректної роботи частини функцій програми.\n"
									"ПОРАДА! Спробуйте відкрити програму ще раз або звверніться до адміністратора.\n"
									"Якщо ви бажаєте продовжити роботу з програмою незважаючи на можливі помилки, клікніть \"Продовжити\".");
			mBox->addButton("Вийти", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("Продовжити", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
	}
	else if (curVersion > TKE_DB_VERSION ){ //Застаріла програма
		QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! Застаріла версія ПК \"ТКЕнерго менеджер\" !!!",
									"Завантажено застарілу версію ПК \"ТКЕнерго менеджер\" відносно робочої бази даних,\n"
									"що може призвести до некоректної роботи частини функцій програми.\n"
									"ПОРАДА! Завантажте нову версію ПК \"ТКЕнерго менеджера\".\n"
									"Якщо ви бажаєте продовжити роботу з програмою незважаючи на можливі помилки, клікніть \"Продовжити\".");
		mBox->addButton("Вийти", QMessageBox::ActionRole);
		QPushButton *contButton = mBox->addButton("Продовжити", QMessageBox::ActionRole);
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
	
	//Під'єднання до бази даних запитів
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSqlDatabase updateTkeDataBaseVerDB = QSqlDatabase::addDatabase("QSQLITE", "updateTkeDataBaseVerDB" );
	updateTkeDataBaseVerDB.setDatabaseName("tkeDBUpdate.tkeDB");
	if (updateTkeDataBaseVerDB.open()){
		QSqlQuery updateQuery(updateTkeDataBaseVerDB);
		//Таблиця зміни структури
		updateQuery.exec(" \
CREATE TABLE IF NOT EXISTS updateFTFQuerys( \
	id INTEGER PRIMARY KEY AUTOINCREMENT, \
	Version INTEGER, \
	baseQuery TEXT, \
	trueQuery TEXT, \
	falseQuery TEXT, \
	failerQuery TEXT)");
		//Таблиця зміни даних
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
		qDebug() << "Помилка. Не вдалося під'єднатися до бази даних tke.testDb";
		return -1;
	}
	
	//Під'єднання до бази даних результатів поновлюючих запитів
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSqlDatabase updateResultsDB = QSqlDatabase::addDatabase("QSQLITE", "updateResultsDB" );
	updateResultsDB.setDatabaseName("tkeDBUpdateResults.tkeDB");
	if (updateResultsDB.open()){
		QSqlQuery updateQuery(updateResultsDB);
		//Таблиця запису результатів поновлення бази даних
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
		qDebug() << "Помилка. Не вдалося під'єднатися до бази даних tkeDBUpdateResults.tkeDB";
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
		//Виконання запитів true-false-fail
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
		
		//Виконання запитів negativ-zero-positiv (NZP)
		updateQuery.exec("SELECT * FROM updateNZPQuerys WHERE version="+QString::number(updateVersion)+" ORDER BY id");
		while (updateQuery.next()){
			int queryPos;
			if (tkeQuery.exec(updateQuery.value(2).toString()) && tkeQuery.seek(0)){
				double probValue = tkeQuery.value(0).toDouble();
				if (probValue>0.0000009)
					queryPos = 3;
				else if (probValue<-0.0000009)
					queryPos = 4;
				else //Нуль
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
