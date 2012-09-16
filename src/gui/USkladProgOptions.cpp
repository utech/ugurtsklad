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

#include "USkladProgOptions.h"
#include <QSqlRecord>
#include <UMath>
#include <UNumberToStringConverter>

//--------------------utech--------------------utech--------------------utech--------------------
/*
	�������� �������:
		0. Գ�������� ������ ��
		1. ���
		2. ������ �������
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
		case 0: return "Գ�������� ������";
		case 1:	return "���";
		case 2: return "������ �������";
		default: return "Գ�������� ������";
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
	str += "������ " + Edrpoy + ";\n";
	str += "������: " + Adresa + ";\n";
	str += "����: " + Bank + ", ��� " + MFO + ", �/� " + BankRahunok + ";\n";
	if (_podatokType == 1){
		str += "������� ������� �� �������� �� ��������� ��������\n";
		str += "��� " + IPN + ", � ���. ��� " + SvidPDVNum + ";\n";
	}
	if (TelefonDurekt.size() > 0)
		str += "���." + TelefonDurekt + ", ";
	if (Fax.size() > 0)
		str += "����." + Fax + ", ";
	if (Email.size() > 0)
		str += "�-�����." + Email + ", ";
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
						(str.size()>0?QString("������: "):QString(""))+str+(str.size()>0?QString("\n"):QString(""))+
						query.value(2).toString();
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::headerCinaStr()
{
	if (_podatokType == 1){
		if (_dockTovZPodatkom)
			return "ֳ�� � ���";
		else
			return "ֳ�� ��� ���";
	}
	else
		return "ֳ��";
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::headerSumaStr()
{
	if (_podatokType == 1){
		if (_dockTovZPodatkom)
			return "���� � ���";
		else
			return "���� ��� ���";
	}
	else
		return "����";
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
	
	if (_podatokType == 1){ //���
		if (!(_cinaZPodatkom ^ _dockTovZPodatkom)) //������� ��� ���� � ���������
			_curCina = cina;
		else if (_cinaZPodatkom == true && _dockTovZPodatkom == false) //� ��� � ���, � �������� ���
			_curCina = cina / (1 + _podatokVal/100.0);
		else if (_cinaZPodatkom == false && _dockTovZPodatkom == true) //� ��� ��� ���, � �������� � ���
			_curCina = cina * (1 + _podatokVal/100.0);
	}
	else if (_podatokType == 2){ //������ �������
		if (!(_cinaZPodatkom ^ _dockTovZPodatkom)) //������� ��� ���� � ���������
			_curCina = cina;
		else if (_cinaZPodatkom == true && _dockTovZPodatkom == false) //� ��� � ��, � �������� ��� ��
			_curCina = cina * (1 - _podatokVal/100.0);
		else if (_cinaZPodatkom == false && _dockTovZPodatkom == true) //� ��� ��� ��, � �������� � ��
			_curCina = cina / (1 - _podatokVal/100.0);
	}
	else //Գ�������� ������� + ���� �������
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
			return "����� � ���";
		else
			return "����� ��� ���";
	}
	else
		return "�����";
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::pidsRazom()
{
	return _sumaNakl;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsPodatokStr()
{
	if (_podatokType == 1){ //���
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return "� �.�. ���";
		else //���� ������ ��� ������� � ��������
			return QString("��� ") +QString::number(_podatokVal)+"%";
	}
	else if (_podatokType == 2){ //������ �������
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return "";
		else //���� ������ ��� ������� � ��������
			return QString("������ ������� ") +QString::number(_podatokVal)+"%";
	}
	else //Գ�������� ������� + ���� �������
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::pidsPodatok()
{
	_podatokCalculated = true;
	if (_podatokType == 1){ //���
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			_podatokSum = uDToM(_sumaNakl * _podatokVal / (100 + _podatokVal));
		else //���� ������ ��� ������� � ��������
			_podatokSum = uDToM(_sumaNakl * _podatokVal / 100.0);
	}
	else if (_podatokType == 2){ //������ �������
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			_podatokSum = uDToM(_sumaNakl * _podatokVal / 100.0);
		else //���� ������ ��� ������� � ��������
			_podatokSum = uDToM(_sumaNakl * _podatokVal / (100 - _podatokVal));
	}
	else //Գ�������� ������� + ���� �������
		_podatokSum = 0;
	
	return _podatokSum;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsPodatokValStr()
{
	if (_podatokType == 1){ //���
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return uMToStr2(pidsPodatok());
		else //���� ������ ��� ������� � ��������
			return uMToStr2(pidsPodatok());
	}
	else if (_podatokType == 2){ //������ �������
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return "";
		else //���� ������ ��� ������� � ��������
			return uMToStr2(pidsPodatok());
	}
	else //Գ�������� ������� + ���� �������
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsDoPlatyStr()
{
	if (_podatokType == 1){ //���
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return "";
		else //���� ������ ��� ������� � ��������
			return "����� � ���";
	}
	else if (_podatokType == 2){ //������ �������
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return "";
		else //���� ������ ��� ������� � ��������
			return "����� � ��������";
	}
	else //Գ�������� ������� + ���� �������
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
double USkladZbutRozrah::pidsDoOplaty()
{
	if (!_podatokCalculated){
		pidsPodatok();
	}
	
	if (_podatokType == 1){ //���
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return uDToM(_sumaNakl);
		else //���� ������ ��� ������� � ��������
			return uDToM(_sumaNakl + _podatokSum);
	}
	else if (_podatokType == 2){ //������ �������
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return uDToM(_sumaNakl);
		else //���� ������ ��� ������� � ��������
			return uDToM(_sumaNakl + _podatokSum);
	}
	else //Գ�������� ������� + ���� �������
		return uDToM(_sumaNakl);
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USkladZbutRozrah::pidsDoOplatyValStr()
{
	if (_podatokType == 1){ //���
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return "";
		else //���� ������ ��� ������� � ��������
			return uMToStr2(pidsDoOplaty());
	}
	else if (_podatokType == 2){ //������ �������
		if (_dockTovZPodatkom) //���� ������ � �������� � ��������
			return "";
		else //���� ������ ��� ������� � ��������
			return uMToStr2(pidsDoOplaty());
	}
	else //Գ�������� ������� + ���� �������
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
	if (_podatokType == 1) //���
		return  "�� ������: " + pidsHumanLangDoOplatyStr() + "\n  � �.�. ���: " + pidsHumanLangPodatokStr() + ".";
	else 
		return  "�� ������: " + pidsHumanLangDoOplatyStr() + "";
}
//--------------------utech--------------------utech--------------------utech--------------------
