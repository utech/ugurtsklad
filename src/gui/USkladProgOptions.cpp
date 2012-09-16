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

#include "USkladProgOptions.h"
#include <QSqlRecord>
#include <UMath>
#include <UNumberToStringConverter>

//--------------------utech--------------------utech--------------------utech--------------------
/*
	Значення податків:
		0. Фіксований патент ПП
		1. ПДВ
		2. Єдиний податок
*/
int USkladProgOptions::organizPodatok()
{
	QSqlQuery query;
	query.exec("SELECT Podatok FROM orgInfo WHERE PidprId=1");
	if (query.seek(0)){
		return query.value(0).toInt();
	}
	else{
		qDebug() << "SQL Table orgInfo has no record with id=1.";
		return -1;
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladProgOptions::organizPodatokName()
{
	switch (organizPodatok()){
		case 0: return "Фіксований патент";
		case 1:	return "ПДВ";
		case 2: return "Єдиний податок";
		default: return "Фіксований патент";
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USkladProgOptions::organizCinaZPodatkom()
{
	QSqlQuery query;
	query.exec("SELECT CinaZPodatkom FROM orgInfo WHERE PidprId=1");
	if (query.seek(0)){
		return query.value(0).toBool();
	}
	else{
		qDebug() << "SQL Table orgInfo has no record with id=1.";
		return true;
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladProgOptions::organizVidsotokPodatku()
{
	QSqlQuery query;
	query.exec("SELECT PodatokVal FROM orgInfo WHERE PidprId=1");
	if (query.seek(0)){
		return query.value(0).toDouble();
	}
	else{
		qDebug() << "SQL Table orgInfo has no record with id=1.";
		return 0;
	}
}
//--------------------utech--------------------utech--------------------utech--------------------

//--------------------utech--------------------utech--------------------utech--------------------
USkladZbutRozrah::USkladZbutRozrah()
{
	USkladProgOptions *opt = new USkladProgOptions();
	_podatokType = opt->organizPodatok();
	_podatokVal = opt->organizVidsotokPodatku();
	_cinaZPodatkom = opt->organizCinaZPodatkom();
	_dockTovZPodatkom = true;
	delete opt;
	
	_curKilkist = 0;
	_curCina = 0;
	_sumaNakl = 0;
	_podatokCalculated = false;
	_podatokSum = 0;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::rekvizytyPostachalnyka(int PostachId)
{
	QSqlQuery query;
	QString str;
	query.exec("SELECT * FROM orginfo WHERE id="+QString::number(PostachId));
	query.next();
	QSqlRecord rec = query.record();
	QString Edrpoy = rec.value("Edrpoy").toString();
	QString PovnaNazvaPidpr = rec.value("PovnaNazvaPidpr").toString();
	QString Adresa = rec.value("Adresa").toString();
	QString Bank = rec.value("Bank").toString();
	QString MFO = rec.value("MFO").toString();
	QString BankRahunok = rec.value("BankRahunok").toString();
	QString Podatok = rec.value("Podatok").toString();
	QString IPN = rec.value("IPN").toString();
	QString SvidPDVNum = rec.value("SvidPDVNum").toString();
	QString TelefonDurekt = rec.value("TelefonDurekt").toString();
	QString Fax = rec.value("Fax").toString();
	QString Email = rec.value("Email").toString();
	str += PovnaNazvaPidpr + "\n";
	str += "ЄДРПОУ " + Edrpoy + ";\n";
	str += "Адреса: " + Adresa + ";\n";
	str += "Банк: " + Bank + ", МФО " + MFO + ", р/р " + BankRahunok + ";\n";
	if (_podatokType == 1){
		str += "Платник податку на прибуток на загальних підставах\n";
		str += "ІПН " + IPN + ", № свід. ПДВ " + SvidPDVNum + ";\n";
	}
	if (TelefonDurekt.size() > 0)
		str += "Тел." + TelefonDurekt + ", ";
	if (Fax.size() > 0)
		str += "Факс." + Fax + ", ";
	if (Email.size() > 0)
		str += "Е-пошта." + Email + ", ";
	return str;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::rekvizytyZamovnyka(int nPartnerId)
{
	QSqlQuery query;
	QString str;
	query.exec("SELECT EDRPOU, Name, Adresa FROM dovPartners WHERE id="+QString::number(nPartnerId));
	query.next();
	str = query.value(0).toString();
	return query.value(1).toString()+QString("\n")+
						(str.size()>0?QString("ЄДРПОУ: "):QString(""))+str+(str.size()>0?QString("\n"):QString(""))+
						query.value(2).toString();
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::headerCinaStr()
{
	if (_podatokType == 1){
		if (_dockTovZPodatkom)
			return "Ціна з ПДВ";
		else
			return "Ціна без ПДВ";
	}
	else
		return "Ціна";
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::headerSumaStr()
{
	if (_podatokType == 1){
		if (_dockTovZPodatkom)
			return "Сума з ПДВ";
		else
			return "Сума без ПДВ";
	}
	else
		return "Сума";
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::curKilkist(double kilkist)
{
	if (kilkist == -1)
		return _curKilkist;
	
	if (kilkist < 0)
		kilkist = 0;
	_curKilkist = kilkist;
	return _curKilkist;
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::curCina(double cina)
{
	if (cina == -1)
		return _curCina;
	
	if (cina < 0){
		_curCina = 0;
		return _curCina;
	}
	
	if (_podatokType == 1){ //ПДВ
		if (!(_cinaZPodatkom ^ _dockTovZPodatkom)) //однакові для бази і документа
			_curCina = cina;
		else if (_cinaZPodatkom == true && _dockTovZPodatkom == false) //в базі з пдв, в документі без
			_curCina = cina / (1 + _podatokVal/100.0);
		else if (_cinaZPodatkom == false && _dockTovZPodatkom == true) //в базі без пдв, в документі з ПДВ
			_curCina = cina * (1 + _podatokVal/100.0);
	}
	else if (_podatokType == 2){ //Єдиний податок
		if (!(_cinaZPodatkom ^ _dockTovZPodatkom)) //однакові для бази і документа
			_curCina = cina;
		else if (_cinaZPodatkom == true && _dockTovZPodatkom == false) //в базі з ЄП, в документі без ЄП
			_curCina = cina * (1 - _podatokVal/100.0);
		else if (_cinaZPodatkom == false && _dockTovZPodatkom == true) //в базі без ЄП, в документі з ЄП
			_curCina = cina / (1 - _podatokVal/100.0);
	}
	else //Фіксований податок + інші випадки
		_curCina = cina;
	return _curCina;
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::curSuma()
{
	return uDToM(_curKilkist * _curCina);
}
//--------------------utech--------------------utech--------------------utech--------------------
void USkladZbutRozrah::nextTovar()
{
	_sumaNakl += uDToM(_curKilkist * _curCina);
	_curKilkist = 0;
	_curCina = 0;
	_podatokCalculated = false;
	_podatokSum = 0;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsRazomStr()
{
	if (_podatokType == 1){
		if (_dockTovZPodatkom)
			return "Разом з ПДВ";
		else
			return "Разом без ПДВ";
	}
	else
		return "Разом";
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::pidsRazom()
{
	return _sumaNakl;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsPodatokStr()
{
	if (_podatokType == 1){ //ПДВ
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return "в т.ч. ПДВ";
		else //ціна товарів без податку в накладній
			return QString("ПДВ ") +QString::number(_podatokVal)+"%";
	}
	else if (_podatokType == 2){ //Єдиний податок
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return "";
		else //ціна товарів без податку в накладній
			return QString("Єдиний податок ") +QString::number(_podatokVal)+"%";
	}
	else //Фіксований податок + інші випадки
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::pidsPodatok()
{
	_podatokCalculated = true;
	if (_podatokType == 1){ //ПДВ
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			_podatokSum = uDToM(_sumaNakl * _podatokVal / (100 + _podatokVal));
		else //ціна товарів без податку в накладній
			_podatokSum = uDToM(_sumaNakl * _podatokVal / 100.0);
	}
	else if (_podatokType == 2){ //Єдиний податок
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			_podatokSum = uDToM(_sumaNakl * _podatokVal / 100.0);
		else //ціна товарів без податку в накладній
			_podatokSum = uDToM(_sumaNakl * _podatokVal / (100 - _podatokVal));
	}
	else //Фіксований податок + інші випадки
		_podatokSum = 0;
	
	return _podatokSum;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsPodatokValStr()
{
	if (_podatokType == 1){ //ПДВ
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return uMToStr2(pidsPodatok());
		else //ціна товарів без податку в накладній
			return uMToStr2(pidsPodatok());
	}
	else if (_podatokType == 2){ //Єдиний податок
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return "";
		else //ціна товарів без податку в накладній
			return uMToStr2(pidsPodatok());
	}
	else //Фіксований податок + інші випадки
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsDoPlatyStr()
{
	if (_podatokType == 1){ //ПДВ
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return "";
		else //ціна товарів без податку в накладній
			return "Разом з ПДВ";
	}
	else if (_podatokType == 2){ //Єдиний податок
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return "";
		else //ціна товарів без податку в накладній
			return "Разом з податком";
	}
	else //Фіксований податок + інші випадки
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::pidsDoOplaty()
{
	if (!_podatokCalculated){
		pidsPodatok();
	}
	
	if (_podatokType == 1){ //ПДВ
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return uDToM(_sumaNakl);
		else //ціна товарів без податку в накладній
			return uDToM(_sumaNakl + _podatokSum);
	}
	else if (_podatokType == 2){ //Єдиний податок
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return uDToM(_sumaNakl);
		else //ціна товарів без податку в накладній
			return uDToM(_sumaNakl + _podatokSum);
	}
	else //Фіксований податок + інші випадки
		return uDToM(_sumaNakl);
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsDoOplatyValStr()
{
	if (_podatokType == 1){ //ПДВ
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return "";
		else //ціна товарів без податку в накладній
			return uMToStr2(pidsDoOplaty());
	}
	else if (_podatokType == 2){ //Єдиний податок
		if (_dockTovZPodatkom) //ціна товарів з податком у накладній
			return "";
		else //ціна товарів без податку в накладній
			return uMToStr2(pidsDoOplaty());
	}
	else //Фіксований податок + інші випадки
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsHumanLangDoOplatyStr()
{
	return numberToString(uMToStr2(pidsDoOplaty()));
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsHumanLangPodatokStr()
{
	return numberToString(uMToStr2(pidsPodatok()));
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsHumanStr()
{
	if (_podatokType == 1) //ПДВ
		return  "До оплати: " + pidsHumanLangDoOplatyStr() + "\n  в т.ч. ПДВ: " + pidsHumanLangPodatokStr() + ".";
	else 
		return  "До оплати: " + pidsHumanLangDoOplatyStr() + "";
}
//--------------------utech--------------------utech--------------------utech--------------------
