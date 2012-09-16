//--------------------utech--------------------utech--------------------utech--------------------
//
// Створений програмістами Компанії "Утех" 23.04.2009р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UPrintDocs_Sklad.h"

#include "../main/UGlobal_Sklad.h"
#include "USkladProgOptions.h"
#include <UPopulateTextTableCells>
#include <UMath>
#include <UPostupForm>

//--------------------utech--------------------utech--------------------utech--------------------
UPrintDocs_Sklad::UPrintDocs_Sklad()
{
	date= QDate::currentDate();
	
	monthlist 	<< "січень"
				<< "лютий"
				<< "березень"
				<< "квітень"
				<< "травень"
				<< "червень"
				<< "липень"
				<< "серпень"
				<< "вересень"
				<< "жовтень"
				<< "листопад"
				<< "грудень";
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs_Sklad::print_vhidnaNakladna(int nId)
{
	printform = new UPrintForm(0, "Вхідна накладна");
	
	QSqlQuery query;
	
	query.exec("SELECT count(*) FROM skladPakety WHERE NakladnaVh_id="+QString::number(nId));
	query.next();
	int rowCount = query.value(0).toInt();
	UPostupForm * postupForm = new UPostupForm(0, rowCount);
	postupForm->show();
	
	query.exec("SELECT Num, CDate, Partner_id FROM nakladnavh WHERE id="+QString::number(nId));
	query.next();
	QString nNum = query.value(0).toString();
	QDate nDate = query.value(1).toDate();
	int nPartnerId = query.value(2).toInt();
	QString str;
	QTextCursor cursor(printform->document());
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_small;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 8 );
	cursor.insertText("Документ створено програмою "+QString(USklad::programName())+" "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//Створення таблиці
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table=NULL;
	UPopulateCell tableCell(table);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 150);
	constraints << QTextLength(QTextLength::FixedLength, 550);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(5);
	tableFormat.setBorder(0);
	table = cursor.insertTable(2, 2, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 11 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,0,"Постачальник:", textCharFormat);
	tableCell.setAlignment(Qt::AlignLeft);
	query.exec("SELECT EDRPOU, Name, Adresa FROM dovPartners WHERE id="+QString::number(nPartnerId));
	query.next();
	str = query.value(0).toString();
	tableCell.set(0,1,query.value(1).toString()+QString("\n")+
						(str.size()>0?QString("ЄДРПОУ: "):QString(""))+str+(str.size()>0?QString("\n"):QString(""))+
						query.value(2).toString(), textCharFormat);
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(1,0,"Одержувач:", textCharFormat);
	tableCell.setAlignment(Qt::AlignLeft);
	query.exec("SELECT Edrpoy, PovnaNazvaPidpr, Adresa FROM orgInfo WHERE id=1");
	query.next();
	str = query.value(0).toString();
	tableCell.set(1,1,query.value(1).toString()+QString("\n")+
						(str.size()>0?QString("ЄДРПОУ: "):QString(""))+str+(str.size()>0?QString("\n"):QString(""))+
						query.value(2).toString(), textCharFormat);
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_small.setFontPointSize( 5 );
	cursor.insertText("ВХІДНА НАКЛАДНА № "+nNum, textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	cursor.insertText("Дата "+nDate.toString("dd.MM.yyyy"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 380);
	constraints << QTextLength(QTextLength::FixedLength, 40);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.5);
	tableFormat.setBorderBrush(QBrush(Qt::black)); 
	tableFormat.setHeaderRowCount(1);
	table = cursor.insertTable(2+rowCount, 6, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 10 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 10 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"№", textCharFormat);
	tableCell.set(0,1,"Найменування товару", textCharFormat);
	tableCell.set(0,2,"Один.", textCharFormat);
	tableCell.set(0,3,"К-сть", textCharFormat);
	tableCell.set(0,4,"Ціна", textCharFormat);
	tableCell.set(0,5,"Сума", textCharFormat);
	
	textCharFormat.setFontPointSize( 10 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 10 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	query.exec("SELECT tovPerelik.Name, tovPerelik.Odynyci, skladPakety.Kilkist, skladPakety.Cina FROM skladPakety \
				LEFT JOIN tovPerelik ON skladPakety.Tovar_id=tovPerelik.id\
				WHERE NakladnaVh_id="+QString::number(nId));
	int iter;
	double sum=0, dVal;
	for (iter=0; iter<rowCount && query.next(); iter++){
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,0,QString::number(iter+1), textCharFormat);
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(iter+1,1,query.value(0).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,2,query.value(1).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		dVal = query.value(2).toDouble();
		tableCell.set(iter+1,3,QString::number(dVal).replace('.',','), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		dVal = query.value(3).toDouble();
		tableCell.set(iter+1,4,QString::number(dVal).replace('.',','), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		dVal = uDToM(query.value(2).toDouble()*query.value(3).toDouble());
		sum += dVal;
		tableCell.set(iter+1,5,uMToStr2(dVal), textCharFormat);
		
		postupForm->incPos();
	}
	
	iter++;
	table->mergeCells(iter,0,1,5);
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(iter,4,"Всього", textCharFormat);
	tableCell.set(iter,5,uMToStr2(sum), textCharFormat);
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//Створення таблиці для підписів
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 350);
	constraints << QTextLength(QTextLength::FixedLength, 350);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(5);
	tableFormat.setBorder(0);
	table = cursor.insertTable(1, 2, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 11 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,0,"Відвантажив(ла)  _______________________", textCharFormat);
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,1,"Отримав(ла)  _______________________", textCharFormat);
	
	delete postupForm;
	printform->printer()->setDocName("Вхідна накладна");
	printform->printer()->setPageMargins( 10, 10, 10, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs_Sklad::print_vyhidnaNakladna(int nId, bool needDyrBuhPidpys)
{
	printform = new UPrintForm(0, "Вихідна накладна");
	
	QSqlQuery query;
	
	query.exec("SELECT count(*) FROM (SELECT count(*) \
				FROM (vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id) \
				LEFT JOIN tovPerelik ON tovPerelik.id=skladPakety.Tovar_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY tovPerelik.Name, vidpuskTovariv.Cina) AS countAll");
	query.next();
	int rowCount = query.value(0).toInt();
	UPostupForm * postupForm = new UPostupForm(0, rowCount);
	postupForm->show();
	
	query.exec("SELECT Num, CDate, Partner_id FROM nakladnavyh WHERE id="+QString::number(nId));
	query.next();
	QString nNum = query.value(0).toString();
	QDate nDate = query.value(1).toDate();
	int nPartnerId = query.value(2).toInt();
	QString str;
	QTextCursor cursor(printform->document());
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_small;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 8 );
	cursor.insertText("Документ створено програмою "+QString(USklad::programName())+" "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//Створення таблиці
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table=NULL;
	UPopulateCell tableCell(table);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 150);
	constraints << QTextLength(QTextLength::FixedLength, 550);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(5);
	tableFormat.setBorder(0);
	table = cursor.insertTable(2, 2, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 11 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	USkladZbutRozrah zbutRozrah;
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,0,"Постачальник:", textCharFormat);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(0,1,zbutRozrah.rekvizytyPostachalnyka(), textCharFormat);
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(1,0,"Одержувач:", textCharFormat);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(1,1,zbutRozrah.rekvizytyZamovnyka(nPartnerId), textCharFormat);
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_small.setFontPointSize( 5 );
	cursor.insertText("ВИДАТКОВА НАКЛАДНА № "+nNum, textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	cursor.insertText("Дата "+nDate.toString("dd.MM.yyyy"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 380);
	constraints << QTextLength(QTextLength::FixedLength, 50);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.5);
	tableFormat.setBorderBrush(QBrush(Qt::black)); 
	tableFormat.setHeaderRowCount(1);
	table = cursor.insertTable(1+rowCount, 6, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 10 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 10 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"№", textCharFormat);
	tableCell.set(0,1,"Найменування товару", textCharFormat);
	tableCell.set(0,2,"Один.", textCharFormat);
	tableCell.set(0,3,"К-сть", textCharFormat);
	tableCell.set(0,4,zbutRozrah.headerCinaStr(), textCharFormat);
	tableCell.set(0,5,zbutRozrah.headerSumaStr(), textCharFormat);
	
	textCharFormat.setFontPointSize( 10 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 10 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	query.exec("SELECT tovPerelik.Name, tovPerelik.Odynyci, sum(vidpuskTovariv.Kilkist), vidpuskTovariv.Cina \
				FROM (vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id) \
				LEFT JOIN tovPerelik ON tovPerelik.id=skladPakety.Tovar_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY tovPerelik.Name, tovPerelik.Odynyci, vidpuskTovariv.Cina");
	int iter;
	for (iter=0; iter<rowCount && query.next(); iter++){
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,0,QString::number(iter+1), textCharFormat);
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(iter+1,1,query.value(0).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,2,query.value(1).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,3,QString::number(zbutRozrah.curKilkist(query.value(2).toDouble())).replace('.',','), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,4,QString::number(zbutRozrah.curCina(query.value(3).toDouble())).replace('.',','), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,5,uMToStr2(zbutRozrah.curSuma()), textCharFormat);
		
		postupForm->incPos();
		zbutRozrah.nextTovar();
	}
	
	//Створення таблиці підсумків
	cursor.movePosition(QTextCursor::End);
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(2);
	tableFormat.setBorder(0);
	tableFormat.setBorderBrush(QBrush(Qt::white)); 
	tableFormat.setHeaderRowCount(0);
	table = cursor.insertTable(3, 6, tableFormat);
	tableCell.setTextTable(table);
	
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,1,zbutRozrah.pidsRazomStr(), textCharFormat_bold);
	tableCell.set(0,5,uMToStr2(zbutRozrah.pidsRazom()), textCharFormat_bold);
	tableCell.set(1,1,zbutRozrah.pidsPodatokStr(), textCharFormat_bold);
	tableCell.set(1,5,zbutRozrah.pidsPodatokValStr(), textCharFormat_bold);
	tableCell.set(2,1,zbutRozrah.pidsDoPlatyStr(), textCharFormat_bold);
	tableCell.set(2,5,zbutRozrah.pidsDoOplatyValStr(), textCharFormat_bold);
	
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertText(zbutRozrah.pidsHumanStr(), textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	
	//Створення таблиці для підписів відвантажувача/отримувача
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 350);
	constraints << QTextLength(QTextLength::FixedLength, 350);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(5);
	tableFormat.setBorder(0);
	table = cursor.insertTable(1, 2, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 11 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,0,"Відвантажив(ла)  _______________________", textCharFormat);
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,1,"Отримав(ла)  _______________________", textCharFormat);
	
	if (needDyrBuhPidpys){
		cursor.movePosition(QTextCursor::End);
		blockFormat.setAlignment( Qt::AlignLeft );
		cursor.insertBlock(blockFormat);
		cursor.insertBlock(blockFormat);
		cursor.insertBlock(blockFormat);
		cursor.insertBlock(blockFormat);
	
			//Створення таблиці для підписів відвантажувача/отримувача
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 20);
		constraints << QTextLength(QTextLength::FixedLength, 120);
		constraints << QTextLength(QTextLength::FixedLength, 380);
		constraints << QTextLength(QTextLength::FixedLength, 180);
		
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(5);
		tableFormat.setBorder(0);
		table = cursor.insertTable(3, 4, tableFormat);
		tableCell.setTextTable(table);
	
		blockFormat.setBottomMargin(0);
		blockFormat.setTopMargin(0);
	
		//Заповнення таблиці
		textCharFormat.setFontPointSize( 11 );
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
		textCharFormat_bold.setFontPointSize( 11 );
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
		
		QSqlQuery q("SELECT PipDurect, PipBuh FROM orginfo WHERE PidprId=1");
		q.next();
		
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(0,1,"Директор", textCharFormat);
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(0,2,"_______________________", textCharFormat);
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(0,3,q.value(0).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(2,1,"Гол.бухгалтер", textCharFormat);
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(2,2,"_______________________", textCharFormat);
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(2,3,q.value(1).toString(), textCharFormat);
	}
	
	delete postupForm;
	printform->printer()->setDocName("Вихідна накладна");
	printform->printer()->setPageMargins( 10, 10, 10, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs_Sklad::print_rahunok(int nId)
{
	printform = new UPrintForm(0, "Рахунок");
	
	QSqlQuery query;
	
	query.exec("SELECT count(*) FROM (SELECT count(*) \
				FROM (vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id) \
				LEFT JOIN tovPerelik ON tovPerelik.id=skladPakety.Tovar_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY tovPerelik.Name, vidpuskTovariv.Cina) AS countAll");
	query.next();
	int rowCount = query.value(0).toInt();
	UPostupForm * postupForm = new UPostupForm(0, rowCount);
	postupForm->show();
	
	query.exec("SELECT Num, CDate, Partner_id FROM nakladnavyh WHERE id="+QString::number(nId));
	query.next();
	QString nNum = query.value(0).toString();
	QDate nDate = query.value(1).toDate();
	int nPartnerId = query.value(2).toInt();
	QString str;
	QTextCursor cursor(printform->document());
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_small;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 8 );
	cursor.insertText("Документ створено програмою "+QString(USklad::programName())+" "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//Створення таблиці
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table=NULL;
	UPopulateCell tableCell(table);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 150);
	constraints << QTextLength(QTextLength::FixedLength, 550);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(5);
	tableFormat.setBorder(0);
	table = cursor.insertTable(1, 2, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 11 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	USkladZbutRozrah zbutRozrah;

	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,0,"Постачальник:", textCharFormat);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(0,1,zbutRozrah.rekvizytyPostachalnyka(), textCharFormat);
	
	cursor.movePosition(QTextCursor::End);
	cursor.insertHtml("<hr>");
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 12 );
	cursor.insertText("РАХУНОК № "+nNum, textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 11 );
	cursor.insertText("від "+nDate.toString("dd.MM.yyyy")+" року", textCharFormat_bold);
	cursor.insertHtml("<hr>");
	
	table = cursor.insertTable(1, 2, tableFormat);
	tableCell.setTextTable(table);
	
	textCharFormat.setFontPointSize( 11 );
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,0,"Одержувач:", textCharFormat);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(0,1,zbutRozrah.rekvizytyZamovnyka(nPartnerId), textCharFormat);
	
	cursor.movePosition(QTextCursor::End);
	cursor.insertBlock(blockFormat);
	
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 380);
	constraints << QTextLength(QTextLength::FixedLength, 50);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.5);
	tableFormat.setBorderBrush(QBrush(Qt::black)); 
	tableFormat.setHeaderRowCount(1);
	table = cursor.insertTable(1+rowCount, 6, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 10 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 10 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"№", textCharFormat);
	tableCell.set(0,1,"Найменування товару", textCharFormat);
	tableCell.set(0,2,"Один.", textCharFormat);
	tableCell.set(0,3,"К-сть", textCharFormat);
	tableCell.set(0,4,zbutRozrah.headerCinaStr(), textCharFormat);
	tableCell.set(0,5,zbutRozrah.headerSumaStr(), textCharFormat);
	
	textCharFormat.setFontPointSize( 10 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 10 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	query.exec("SELECT tovPerelik.Name, tovPerelik.Odynyci, sum(vidpuskTovariv.Kilkist), vidpuskTovariv.Cina \
				FROM (vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id) \
				LEFT JOIN tovPerelik ON tovPerelik.id=skladPakety.Tovar_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY tovPerelik.Name, tovPerelik.Odynyci, vidpuskTovariv.Cina");
	int iter;
	for (iter=0; iter<rowCount && query.next(); iter++){
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,0,QString::number(iter+1), textCharFormat);
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(iter+1,1,query.value(0).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,2,query.value(1).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,3,QString::number(zbutRozrah.curKilkist(query.value(2).toDouble())).replace('.',','), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,4,QString::number(zbutRozrah.curCina(query.value(3).toDouble())).replace('.',','), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+1,5,uMToStr2(zbutRozrah.curSuma()), textCharFormat);
		
		postupForm->incPos();
		zbutRozrah.nextTovar();
	}
	
	//Створення таблиці підсумків
	cursor.movePosition(QTextCursor::End);
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(2);
	tableFormat.setBorder(0);
	tableFormat.setBorderBrush(QBrush(Qt::white)); 
	tableFormat.setHeaderRowCount(0);
	table = cursor.insertTable(3, 6, tableFormat);
	tableCell.setTextTable(table);
	
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(0,1,zbutRozrah.pidsRazomStr(), textCharFormat_bold);
	tableCell.set(0,5,uMToStr2(zbutRozrah.pidsRazom()), textCharFormat_bold);
	tableCell.set(1,1,zbutRozrah.pidsPodatokStr(), textCharFormat_bold);
	tableCell.set(1,5,zbutRozrah.pidsPodatokValStr(), textCharFormat_bold);
	tableCell.set(2,1,zbutRozrah.pidsDoPlatyStr(), textCharFormat_bold);
	tableCell.set(2,5,zbutRozrah.pidsDoOplatyValStr(), textCharFormat_bold);
	
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertText(zbutRozrah.pidsHumanStr(), textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//Створення таблиці для підписів
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 350);
	constraints << QTextLength(QTextLength::FixedLength, 350);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(5);
	tableFormat.setBorder(0);
	table = cursor.insertTable(1, 2, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 11 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(0,0,"Підпис:", textCharFormat);
	tableCell.set(0,1,"М.П.", textCharFormat);
	
	
	delete postupForm;
	printform->printer()->setDocName("Рахунок");
	printform->printer()->setPageMargins( 10, 10, 10, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs_Sklad::print_podatkovaNakladna(int nId)
{
	printform = new UPrintForm(0, "Податкова накладна");
	
	QSqlQuery query;
	USkladZbutRozrah zbutRozrah;
	
	query.exec("SELECT count(*) FROM (SELECT count(*) \
				FROM (vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id) \
				LEFT JOIN tovPerelik ON tovPerelik.id=skladPakety.Tovar_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY tovPerelik.Name, vidpuskTovariv.Cina) AS countAll");
	query.next();
	int rowCount = query.value(0).toInt();
	UPostupForm * postupForm = new UPostupForm(0, rowCount);
	postupForm->show();
	
	query.exec("SELECT Num, CDate, Partner_id FROM nakladnavyh WHERE id="+QString::number(nId));
	query.next();
	QString nNum = query.value(0).toString();
	QDate nDate = query.value(1).toDate();
	int nPartnerId = query.value(2).toInt();
	
	query.exec("SELECT * FROM dovPartners WHERE id="+QString::number(nPartnerId));
	query.next();
	QSqlRecord rec = query.record();
	QString klientName = rec.value("Name").toString();
	QString klientAdresa = rec.value("Adresa").toString();
	QString klientTel = rec.value("Tel").toString();
	//bool klientPlatnPdv = rec.value("PlatnPdv").toBool();
	QString klientIPN = rec.value("IPN").toString();
	QString klientSvidPdvNum = rec.value("SvidPdvNum").toString();
	QString klientUgodaNum = rec.value("UgodaNum").toString();
	QDate klientUgodaDate = rec.value("UgodaDate").toDate();
	query.exec("SELECT * FROM orgInfo WHERE id=1");
	query.next();
	rec = query.record();
	QString prodavecPovnaNazvaPidpr = rec.value("PovnaNazvaPidpr").toString();
	QString prodavecAdresa = rec.value("Adresa").toString();
	QString prodavecTelefonDurekt = rec.value("TelefonDurekt").toString();
	//int prodavecPodatok = rec.value("Podatok").toInt();
	QString prodavecIPN = rec.value("IPN").toString();
	QString prodavecSvidPDVNum = rec.value("SvidPDVNum").toString();
	
	QFont font;
	
	QTextTable *tableFrame;
	QTextTable *table;
	QTextTable *tableIndPodNum;
	
	//виведення форми поступу
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_italic, textCharFormat_small, textCharFormat_header;
	QTextCharFormat textCharFormat_underline;
	textCharFormat_bold.setFontPointSize(9);
	font = textCharFormat_bold.font();
	font.setBold( true );
	textCharFormat_bold.setFont(font);
	textCharFormat_italic.setFontPointSize(9);
	textCharFormat_italic.setFontItalic(true);
	//Створення документа 
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize(9);
	textCharFormat_bold.setFontPointSize(9);
	textCharFormat_small.setFontPointSize( 8 );
	textCharFormat_header.setFontPointSize(11);
	font = textCharFormat_header.font();
	font.setBold( true );
	textCharFormat_header.setFont(font);
	textCharFormat_underline.setFontPointSize(9);
	textCharFormat_underline.setFontUnderline(true);
	
	//Найвищий запис оригінал - копія
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	constraints << QTextLength(QTextLength::FixedLength, 250);
	constraints << QTextLength(QTextLength::FixedLength, 450);
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setAlignment(Qt::AlignHCenter);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(2);
	tableFormat.setBorder(0);
	tableFrame = cursor.insertTable(1, 2, tableFormat);
	
		//Надпис Оригінал, Копія
		cell = tableFrame->cellAt(0, 0);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignLeft );
		cellCursor.setBlockFormat( blockFormat );
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 250);
		constraints << QTextLength(QTextLength::FixedLength, 30);
		tableFormat.setBorderBrush(QBrush(Qt::black));
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setAlignment(Qt::AlignHCenter);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(2);
		tableFormat.setBorder(0.8);
		table = cursor.insertTable(2, 2, tableFormat);
		
		cell = table->cellAt(0, 0);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignLeft );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("ОРИГІНАЛ ",textCharFormat_bold);
		cellCursor.insertText("(видається покупцю)",textCharFormat);
		
		cell = table->cellAt(0, 1);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("",textCharFormat_bold);
		
		cell = table->cellAt(1, 0);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignLeft );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("КОПІЯ ",textCharFormat_bold);
		cellCursor.insertText("(залишається у продавця)",textCharFormat);
		
		cell = table->cellAt(1, 1);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("",textCharFormat_bold);
		
		cell = tableFrame->cellAt(0, 0);
		cell.setFormat(textCharFormat);
		cellCursor = cell.lastCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.insertBlock( blockFormat );
		cellCursor.insertText("(Непотрібне виділити поміткою «Х»)", textCharFormat);
		
	cell = tableFrame->cellAt(0, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Затверджено наказом ДПА України від 30.05.97 р. No165 \n "
							"(у редакцїї наказу ДПА України від 08.10.98 р. No469) зі змінами,"
							"внесеними згідно з наказом ДПАУ від 30.06.2005 р. No244",textCharFormat);
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertText("ПОДАТКОВА НАКЛАДНА", textCharFormat_header);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 250);
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 250);
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setAlignment(Qt::AlignHCenter);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(2);
	tableFormat.setBorder(0);
	table = cursor.insertTable(9, 4, tableFormat);
	
	UPopulateCell pCell(table);
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	pCell.setAlignment( Qt::AlignLeft );
	pCell.setTextCharFormat(textCharFormat);
	pCell.setTextBlockFormat(blockFormat);
	
	table->mergeCells ( 0, 0, 1, 2 );
	table->mergeCells ( 0, 2, 1, 2 );
	
	table->mergeCells ( 2, 0, 2, 1 );
	table->mergeCells ( 2, 2, 2, 1 );
	
	table->mergeCells ( 4, 0, 1, 2 );
	table->mergeCells ( 4, 2, 1, 2 );
	table->mergeCells ( 5, 0, 1, 2 );
	table->mergeCells ( 5, 2, 1, 2 );
	table->mergeCells ( 6, 0, 1, 2 );
	table->mergeCells ( 6, 2, 1, 2 );
	
	table->mergeCells ( 7, 0, 1, 4 );
	table->mergeCells ( 8, 0, 1, 4 );
	
	cell = table->cellAt(0, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Дата виписки податкової накладної "+date.toString("dd.MM.yyyy"),textCharFormat);
	
	cell = table->cellAt(0, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Порядковий номер __________________",textCharFormat);
	
	cell = table->cellAt(1, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Продавець",textCharFormat);
	
	cell = table->cellAt(1, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Покупець",textCharFormat);
	
	cell = table->cellAt(2, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Особа (платник податку)-продавець",textCharFormat);
	
	pCell.setWithFrame(2, 1, prodavecPovnaNazvaPidpr, textCharFormat_bold);
	
	cell = table->cellAt(2, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Особа (платник податку)-покупець",textCharFormat);
	
	pCell.setWithFrame(2, 3, klientName, textCharFormat_bold);
	
	cell = table->cellAt(3, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("(назва)",textCharFormat);
		//Таблиця індивідуального податкового номера продавця
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setAlignment(Qt::AlignLeft);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(0.5);
		tableFormat.setBorder(1);
		tableIndPodNum = cellCursor.insertTable(1, 12, tableFormat);
		
		pCell.setTextTable(tableIndPodNum);
		pCell.setAlignment( Qt::AlignCenter);
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		pCell.setTextCharFormat(textCharFormat);
		QString indPodNumber = prodavecIPN;
		if (indPodNumber.size() > 11){
			pCell.set(0, 0, indPodNumber.at(0));
			pCell.set(0, 1, indPodNumber.at(1));
			pCell.set(0, 2, indPodNumber.at(2));
			pCell.set(0, 3, indPodNumber.at(3));
			pCell.set(0, 4, indPodNumber.at(4));
			pCell.set(0, 5, indPodNumber.at(5));
			pCell.set(0, 6, indPodNumber.at(6));
			pCell.set(0, 7, indPodNumber.at(7));
			pCell.set(0, 8, indPodNumber.at(8));
			pCell.set(0, 9, indPodNumber.at(9));
			pCell.set(0, 10, indPodNumber.at(10));
			pCell.set(0, 11, indPodNumber.at(11));
		}
		
		cellCursor = cell.lastCursorPosition();
	cellCursor.insertText("(індивідуальний податковий номер продавця)",textCharFormat);
	
	cell = table->cellAt(3, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("(назва)",textCharFormat);
		//Таблиця індивідуального податкового номера продавця
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		constraints << QTextLength(QTextLength::FixedLength, 17);
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setAlignment(Qt::AlignLeft);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(0.5);
		tableFormat.setBorder(1);
		tableIndPodNum = cellCursor.insertTable(1, 12, tableFormat);
		
		pCell.setTextTable(tableIndPodNum);
		pCell.setAlignment( Qt::AlignCenter);
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		pCell.setTextCharFormat(textCharFormat);
		indPodNumber = klientIPN;
		if (indPodNumber.size() > 11){
			pCell.set(0, 0, indPodNumber.at(0));
			pCell.set(0, 1, indPodNumber.at(1));
			pCell.set(0, 2, indPodNumber.at(2));
			pCell.set(0, 3, indPodNumber.at(3));
			pCell.set(0, 4, indPodNumber.at(4));
			pCell.set(0, 5, indPodNumber.at(5));
			pCell.set(0, 6, indPodNumber.at(6));
			pCell.set(0, 7, indPodNumber.at(7));
			pCell.set(0, 8, indPodNumber.at(8));
			pCell.set(0, 9, indPodNumber.at(9));
			pCell.set(0, 10, indPodNumber.at(10));
			pCell.set(0, 11, indPodNumber.at(11));
		}
		
		cellCursor = cell.lastCursorPosition();
	cellCursor.insertText("(індивідуальний податковий номер продавця)",textCharFormat);
	
	cell = table->cellAt(4, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Місцезнаходження продавця  ",textCharFormat);
	cellCursor.insertText(prodavecAdresa,textCharFormat_underline);
	
	cell = table->cellAt(4, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Місцезнаходження покупця  ",textCharFormat);
	cellCursor.insertText(klientAdresa,textCharFormat_underline);
	
	cell = table->cellAt(5, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Номер телефону  ",textCharFormat);
	cellCursor.insertText(prodavecTelefonDurekt,textCharFormat_underline);
	
	cell = table->cellAt(5, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Номер телефону  ",textCharFormat);
	cellCursor.insertText(klientTel,textCharFormat_underline);
	
	cell = table->cellAt(6, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Номер свідоцтва про реєстрацію платника податку на додану вартість (продавця)  ",textCharFormat);
	//Номер свідоцтва про реєстрацію платника податку на додану
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 100);

		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setAlignment(Qt::AlignRight);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(0.5);
		tableFormat.setBorder(1);
		tableIndPodNum = cellCursor.insertTable(1, 1, tableFormat);
		
		cell = tableIndPodNum->cellAt(0, 0);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText(prodavecSvidPDVNum,textCharFormat_bold);
	
	cell = table->cellAt(6, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Номер свідоцтва про реєстрацію платника податку на додану вартість (покупця)  ",textCharFormat);
		//Номер свідоцтва про реєстрацію платника податку на додану
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 100);

		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setAlignment(Qt::AlignRight);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(0.5);
		tableFormat.setBorder(1);
		tableIndPodNum = cellCursor.insertTable(1, 1, tableFormat);
		
		cell = tableIndPodNum->cellAt(0, 0);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText(klientSvidPdvNum,textCharFormat_bold);
	
	cell = table->cellAt(7, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Умова поставки   ", textCharFormat);
	cellCursor.insertText("УГОДА № "+klientUgodaNum+" від "+klientUgodaDate.toString("dd.MM.yyyy")+"р.", textCharFormat_underline);
	
	cell = table->cellAt(8, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Форма проведених розрахунків   ", textCharFormat);
	cellCursor.insertText("оплата з розрахункового рахунку.", textCharFormat_underline);
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 188);
	constraints << QTextLength(QTextLength::FixedLength, 40);
	constraints << QTextLength(QTextLength::FixedLength, 63);
	constraints << QTextLength(QTextLength::FixedLength, 63);
	constraints << QTextLength(QTextLength::FixedLength, 63);
	constraints << QTextLength(QTextLength::FixedLength, 63);
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 63);
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setAlignment(Qt::AlignHCenter);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.8);
	table = cursor.insertTable(rowCount+11, 11, tableFormat);
	
	pCell.setTextTable(table);
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	pCell.setAlignment( Qt::AlignCenter );
	pCell.setTextCharFormat(textCharFormat);
	pCell.setTextBlockFormat(blockFormat);
	
	table->mergeCells ( 0, 0, 2, 1 );
	table->mergeCells ( 0, 1, 2, 1 );
	table->mergeCells ( 0, 2, 2, 1 );
	table->mergeCells ( 0, 3, 2, 1 );
	table->mergeCells ( 0, 4, 2, 1 );
	table->mergeCells ( 0, 5, 2, 1 );
	table->mergeCells ( 0, 10, 2, 1 );
	
	table->mergeCells ( 0, 6, 1, 4 );
	
	pCell.set( 0, 0, "Роз-діл");
	pCell.set( 0, 1, "Дата відван-таження (вико-нання, поставки (оплати*) товарів (послуг)");
	pCell.set( 0, 2, "Номенклатура поставки товарів (послуг) продавця");
	pCell.set( 0, 3, "Оди-ниця виміру товару");
	pCell.set( 0, 4, "Кількість (об’єм, обсяг)");
	pCell.set( 0, 5, "Ціна поставки одиниці продукції без урахування ПДВ");
	pCell.set( 0, 6, "Обсяги поставки (база оподаткування) без урахування ПДВ, що підлягають оподаткуванню за ставками");
	pCell.set( 1, 6, "20%");
	pCell.set( 1, 7, "0% (реалізація на митній те-риторії України)");
	pCell.set( 1, 8, "0% (екс-порт)");
	pCell.set( 1, 9, "Звіль-нен-ня від ПДВ");
	pCell.set( 0, 10, "Загальна сума коштів, що підлягає оплаті");
	
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	pCell.setTextCharFormat(textCharFormat);
	pCell.set( 2, 0, "1");
	pCell.set( 2, 1, "2");
	pCell.set( 2, 2, "3");
	pCell.set( 2, 3, "4");
	pCell.set( 2, 4, "5");
	pCell.set( 2, 5, "6");
	pCell.set( 2, 6, "7");
	pCell.set( 2, 7, "8");
	pCell.set( 2, 8, "9");
	pCell.set( 2, 9, "10");
	pCell.set( 2, 10, "11");
	
	
	query.exec("SELECT tovPerelik.Name, tovPerelik.Odynyci, sum(vidpuskTovariv.Kilkist), vidpuskTovariv.Cina \
				FROM (vidpuskTovariv \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id) \
				LEFT JOIN tovPerelik ON tovPerelik.id=skladPakety.Tovar_id \
				WHERE vidpuskTovariv.nakladnaVyh_id="+QString::number(nId)+" \
				GROUP BY tovPerelik.Name, tovPerelik.Odynyci, vidpuskTovariv.Cina");
	int iter;
	for (iter=1; iter<=rowCount && query.next(); iter++){
		pCell.setAlignment( Qt::AlignCenter );
		pCell.set(iter+2, 0, "I."+QString::number(iter));
		pCell.set(iter+2, 1, nDate.toString("dd.MM.yy"));
		pCell.setAlignment( Qt::AlignLeft );
		pCell.set(iter+2, 2, query.value(0).toString());
		pCell.setAlignment( Qt::AlignCenter );
		pCell.set(iter+2, 3, query.value(1).toString());
		pCell.setAlignment( Qt::AlignRight );
		pCell.set(iter+2, 4, QString::number(zbutRozrah.curKilkist(query.value(2).toDouble())).replace('.',','));
		pCell.set(iter+2, 5, QString::number(zbutRozrah.curCina(query.value(3).toDouble())).replace('.',','));
		pCell.set(iter+2, 6, uMToStr2(zbutRozrah.curSuma()));
		pCell.set(iter+2, 10, uMToStr2(zbutRozrah.curSuma()));
		
		postupForm->incPos();
		zbutRozrah.nextTovar();
	}
	
	iter += 2;
	
	table->mergeCells ( iter, 1, 1, 2 );
	table->mergeCells ( iter+1, 1, 1, 2 );
	table->mergeCells ( iter+2, 1, 1, 2 );
	table->mergeCells ( iter+3, 0, 2, 1 );
	table->mergeCells ( iter+3, 1, 2, 1 );
	table->mergeCells ( iter+4, 1, 1, 2 );
	table->mergeCells ( iter+5, 1, 1, 2 );
	table->mergeCells ( iter+6, 1, 1, 2 );
	table->mergeCells ( iter+7, 1, 1, 2 );
	
	pCell.setAlignment( Qt::AlignCenter );
	pCell.set(iter, 1, "Усього по розділу I");
	pCell.setAlignment( Qt::AlignRight );
	pCell.set( iter, 6, uMToStr2(zbutRozrah.pidsRazom()));
	pCell.set( iter, 10, uMToStr2(zbutRozrah.pidsRazom()));
	
	iter++;
	pCell.setAlignment( Qt::AlignCenter );
	pCell.set(iter, 0, "II");
	pCell.set(iter, 1, "Товаротранспортні витрати");
	iter++;
	pCell.set(iter, 0, "III");
	pCell.set(iter, 1, "Зворотна (заставна) тара");
	pCell.set(iter, 3, "X");
	pCell.set(iter, 4, "X");
	pCell.set(iter, 5, "X");
	pCell.set(iter, 6, "X");
	pCell.set(iter, 7, "X");
	pCell.set(iter, 8, "X");
	pCell.set(iter, 9, "X");
	iter++;
	pCell.set(iter, 0, "IV");
	pCell.set(iter, 1, "Надано покупцю:");
	pCell.set(iter, 2, "надбавка (+)");
	iter++;
	pCell.set(iter, 2, "знижка (-)");
	iter++;
	pCell.set(iter, 0, "V");
	pCell.set(iter, 1, "Усього по розділах I +II + IV");
	pCell.setAlignment( Qt::AlignRight );
	pCell.set( iter, 6, uMToStr2(zbutRozrah.pidsRazom()));
	pCell.set( iter, 10, uMToStr2(zbutRozrah.pidsRazom()));
	iter++;
	pCell.setAlignment( Qt::AlignCenter );
	pCell.set(iter, 0, "VI");
	pCell.set(iter, 1, "Податок на додану вартість");
	pCell.setAlignment( Qt::AlignRight );
	pCell.set( iter, 6, uMToStr2(zbutRozrah.pidsPodatok()));
	pCell.set( iter, 10, uMToStr2(zbutRozrah.pidsPodatok()));
	iter++;
	pCell.setAlignment( Qt::AlignCenter );
	pCell.set(iter, 0, "VII");
	pCell.set(iter, 1, "Загальна сума з ПДВ");
	pCell.setAlignment( Qt::AlignRight );
	pCell.set( iter, 6, uMToStr2(zbutRozrah.pidsDoOplaty()));
	pCell.set( iter, 10, uMToStr2(zbutRozrah.pidsDoOplaty()));
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertText("Суми ПДВ, нараховані (сплачені) в зв’язку з поставкою товарів (послуг), зазначених у цій накладній, визначені правильно, відповідають сумі податкових зобов’язань продавця і включені до реєстру отриманих та виданих податкових накладних.", textCharFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertText("	М.П.", textCharFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertText("				________________________________________________", textCharFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertText("				(підпис і прізвище особи, яка склала податкову накладну)", textCharFormat_small);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertText("* Дата оплати ставиться у разі попередньої оплати поставки, на яку виписується податкова накладна; для бартерних операцій — дата оприбуткування товарів, попередньо одержаних в оплату поставки; дата попереднього одержання послуг в оплату поставки; для операції з поставки товарів (послуг) за касовим методом відповідно до Закону України «Про податок на додану вартість».", textCharFormat_small);
	
	delete postupForm;
	printform->printer()->setDocName("Податкова накладна");
	printform->printer()->setPageMargins( 10, 10, 10, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs_Sklad::print_obigIZalyshky(QDate begDate, QDate endDate, int groupId, bool printNotNullOnly)
{
	UPostupForm * postupForm = new UPostupForm(0, 0);
	postupForm->show();
	
	printform = new UPrintForm(0, "Відомість обігу та залишків на складі");
	
	QSqlQuery query;
	QString groupIdFilter = " 1 ";
	if (groupId > 0)
		groupIdFilter = " tovperelik.GroupId="+QString::number(groupId)+" ";
	QString noNullTovFilter = " 1 ";
	if (printNotNullOnly)
		noNullTovFilter = " vhZalyshky.tovVh>0 or vhZalyshky.cinaVh>0 or pryhCur.Kilk>0 or pryhCur.Suma>0 or rozhCur.Kilk>0 or rozhCur.Suma>0 ";
	QString pidprName;
	query.exec("SELECT ScorochNazvaPidpr FROM orginfo WHERE id=1");
	query.next();
	pidprName = query.value(0).toString();
	
	query.exec("SELECT tovperelik.Nomer, tovperelik.Name, tovperelik.Odynyci, \
						vhZalyshky.tovVh, vhZalyshky.cinaVh, \
						pryhCur.Kilk, pryhCur.Suma, rozhCur.Kilk, rozhCur.Suma, \
						rezZalyshky.tovZal, rezZalyshky.cinaZal \
				FROM ((((tovperelik \
				LEFT JOIN ( \
					SELECT Tovar_id, sum(pryhKilk-rozhKilk) AS tovVh, sum(ROUND(cina*(pryhKilk-rozhKilk),2)) AS cinaVh \
					FROM \
						(SELECT skladPakety.Tovar_id, \
								if(skladPakety.Kilkist is null,0,skladPakety.Kilkist) AS pryhKilk, \
								if(v.vKilkist is null,0,v.vKilkist) AS rozhKilk, \
								if(skladPakety.Cina is null,0,skladPakety.Cina) AS Cina \
						FROM (SELECT skladPakety.* FROM skladPakety \
								LEFT JOIN nakladnaVh \
								ON skladPakety.nakladnaVh_id=nakladnaVh.id \
								WHERE nakladnaVh.CDate<date('"+begDate.toString("yyyy-MM-dd")+"') \
								) AS skladPakety \
						LEFT JOIN (SELECT Paket_id, sum(Kilkist) AS vKilkist FROM \
                (SELECT vidpuskTovariv.* FROM vidpuskTovariv \
                LEFT JOIN nakladnaVyh \
                ON nakladnaVyh.id=vidpuskTovariv.Nakladnavyh_id \
                WHERE nakladnaVyh.CDate<date('"+begDate.toString("yyyy-MM-dd")+"')) AS vidpuskTovariv \
							GROUP BY Paket_id) AS v \
						ON skladPakety.id=v.Paket_id \
						WHERE skladPakety.Closed=false) \
						AS promZalyshky \
					GROUP BY Tovar_id \
				) AS vhZalyshky \
				ON vhZalyshky.Tovar_id = tovperelik.id) \
					\
				LEFT JOIN (SELECT Tovar_id, sum(Kilkist) AS Kilk, sum(ROUND(Kilkist*Cina,2)) AS Suma  FROM skladPakety  \
							INNER JOIN (SELECT id FROM nakladnaVh WHERE CDate BETWEEN date('"+begDate.toString("yyyy-MM-dd")+"') AND date('"+endDate.toString("yyyy-MM-dd")+"')) AS nVh ON nVh.id=skladPakety.NakladnaVh_id \
							WHERE closed=false \
							GROUP BY Tovar_id) AS pryhCur \
				ON pryhCur.Tovar_id=tovperelik.id ) \
				LEFT JOIN (SELECT Tovar_id, sum(v.kilkist) AS Kilk, sum(ROUND(v.Kilkist*v.cina,2)) AS Suma \
							FROM \
								(SELECT id, Kilkist, Tovar_id FROM skladPakety \
							WHERE closed=false) AS s \
							LEFT JOIN \
								(SELECT Paket_id, Kilkist, Cina FROM vidpuskTovariv \
							INNER JOIN (SELECT id FROM nakladnaVyh WHERE CDate BETWEEN date('"+begDate.toString("yyyy-MM-dd")+"') AND date('"+endDate.toString("yyyy-MM-dd")+"')) AS nVyh ON nVyh.id=vidpuskTovariv.NakladnaVyh_id \
							) AS v \
							ON v.Paket_id=s.id \
							GROUP BY s.Tovar_id) AS rozhCur \
				ON rozhCur.Tovar_id=tovperelik.id) \
				 \
				LEFT JOIN ( \
					SELECT Tovar_id, sum(pryhKilk-rozhKilk) AS tovZal, sum(ROUND(cina*(pryhKilk-rozhKilk),2)) AS cinaZal \
					FROM \
						(SELECT skladPakety.Tovar_id, \
								if(skladPakety.Kilkist is null,0,skladPakety.Kilkist) AS pryhKilk, \
								if(v.vKilkist is null,0,v.vKilkist) AS rozhKilk, \
								if(skladPakety.Cina is null,0,skladPakety.Cina) AS Cina \
						FROM (SELECT skladPakety.* FROM skladPakety \
								LEFT JOIN nakladnaVh \
								ON skladPakety.nakladnaVh_id=nakladnaVh.id \
								WHERE nakladnaVh.CDate<=date('"+endDate.toString("yyyy-MM-dd")+"') \
								) AS skladPakety \
						LEFT JOIN (SELECT Paket_id, sum(Kilkist) AS vKilkist FROM \
                (SELECT vidpuskTovariv.* FROM vidpuskTovariv \
                LEFT JOIN nakladnaVyh \
                ON nakladnaVyh.id=vidpuskTovariv.Nakladnavyh_id \
                WHERE nakladnaVyh.CDate<=date('"+endDate.toString("yyyy-MM-dd")+"')) AS vidpuskTovariv \
							GROUP BY Paket_id) AS v \
						ON skladPakety.id=v.Paket_id \
						WHERE skladPakety.Closed=false) \
						AS promZalyshky \
					GROUP BY Tovar_id \
				) AS rezZalyshky \
				ON rezZalyshky.Tovar_id = tovperelik.id) \
				 \
				WHERE ("+noNullTovFilter+") and "+groupIdFilter);
	int rowCount = query.size();
	postupForm->setNumPos(rowCount);
	
	QTextCursor cursor(printform->document());
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_small;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 8 );
	cursor.insertText("Документ створено програмою "+QString(USklad::programName())+" "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	
	textCharFormat.setFontPointSize( 12 );
	textCharFormat_bold.setFontPointSize( 12 );
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertText("ВІДОМІСТЬ\nобігу та залишків на складі "+pidprName+"\nза період з "+begDate.toString("dd.MM.yyyy")+" по "+endDate.toString("dd.MM.yyyy")+" р.", textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	
	//Змінні таблиці
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table=NULL;
	UPopulateCell tableCell(table);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 40);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 330);
	constraints << QTextLength(QTextLength::FixedLength, 40);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.5);
	tableFormat.setHeaderRowCount(2);
	table = cursor.insertTable(3+rowCount, 12, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 10 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 10 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	
	table->mergeCells ( 0, 0, 2, 1 );
	table->mergeCells ( 0, 1, 2, 1 );
	table->mergeCells ( 0, 2, 2, 1 );
	table->mergeCells ( 0, 3, 2, 1 );
	
	table->mergeCells ( 0, 4, 1, 2 );
	table->mergeCells ( 0, 6, 1, 2 );
	table->mergeCells ( 0, 8, 1, 2 );
	table->mergeCells ( 0, 10, 1, 2 );
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"№ з/п", textCharFormat);
	tableCell.set(0,1,"Інв. номер", textCharFormat);
	tableCell.set(0,2,"Назва", textCharFormat);
	tableCell.set(0,3,"Один.", textCharFormat);
	tableCell.set(0,4,"Вх.Сальдо", textCharFormat);
	tableCell.set(0,6,"Прихід", textCharFormat);
	tableCell.set(0,8,"Розхід", textCharFormat);
	tableCell.set(0,10,"Вих.Сальдо", textCharFormat);
	tableCell.set(1,4,"К-сть", textCharFormat);
	tableCell.set(1,5,"Сума", textCharFormat);
	tableCell.set(1,6,"К-сть", textCharFormat);
	tableCell.set(1,7,"Сума", textCharFormat);
	tableCell.set(1,8,"К-сть", textCharFormat);
	tableCell.set(1,9,"Сума", textCharFormat);
	tableCell.set(1,10,"К-сть", textCharFormat);
	tableCell.set(1,11,"Сума", textCharFormat);
	
	double dVal;
	int iter;
	double curRowVyhSaldoKilkist, curRowVyhSaldoSuma;
	double vhSaldoSumaAll = 0;
	double pryhidSumaAll = 0;
	double rozhidSumaAll = 0;
	double vyhSaldoSumaAll = 0;
	
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	for (iter=0; iter<rowCount && query.next(); iter++){
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(iter+2,0,iter+1, textCharFormat);
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(iter+2,1,query.value(0).toString(), textCharFormat);
		tableCell.set(iter+2,2,query.value(1).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(iter+2,3,query.value(2).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		//Вх сальдо
		dVal = query.value(3).toDouble();
		curRowVyhSaldoKilkist = dVal;
		tableCell.set(iter+2,4,QString::number(dVal).replace('.',','), textCharFormat);
		dVal = query.value(4).toDouble();
		curRowVyhSaldoSuma = dVal;
		vhSaldoSumaAll += dVal;
		tableCell.setDash(iter+2,5,dVal, textCharFormat);
		//Прихід
		dVal = query.value(5).toDouble();
		curRowVyhSaldoKilkist += dVal;
		tableCell.set(iter+2,6,QString::number(dVal).replace('.',','), textCharFormat);
		dVal = query.value(6).toDouble();
		curRowVyhSaldoSuma += dVal;
		pryhidSumaAll += dVal;
		tableCell.setDash(iter+2,7,dVal, textCharFormat);
		//Розхід
		dVal = query.value(7).toDouble();
		curRowVyhSaldoKilkist -= dVal;
		tableCell.set(iter+2,8,QString::number(dVal).replace('.',','), textCharFormat);
		dVal = query.value(8).toDouble();
		curRowVyhSaldoSuma -= dVal;
		rozhidSumaAll += dVal;
		tableCell.setDash(iter+2,9,dVal, textCharFormat);
		//Вихідне сальдо
		dVal = query.value(9).toDouble();
		tableCell.set(iter+2,10,QString::number(dVal).replace('.',','), textCharFormat);
		dVal = query.value(10).toDouble();
		vyhSaldoSumaAll += dVal;
		tableCell.setDash(iter+2,11,dVal, textCharFormat);
		
		postupForm->incPos();
	}
	
	table->mergeCells ( iter+2, 0, 1, 4 );
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(iter+2,0,"Разом", textCharFormat_bold);
	tableCell.set(iter+2,4,"-", textCharFormat_bold);
	tableCell.setDash(iter+2,5,vhSaldoSumaAll, textCharFormat_bold);
	tableCell.set(iter+2,6,"-", textCharFormat_bold);
	tableCell.setDash(iter+2,7,pryhidSumaAll, textCharFormat_bold);
	tableCell.set(iter+2,8,"-", textCharFormat_bold);
	tableCell.setDash(iter+2,9,rozhidSumaAll, textCharFormat_bold);
	tableCell.set(iter+2,10,"-", textCharFormat_bold);
	tableCell.setDash(iter+2,11,vyhSaldoSumaAll, textCharFormat_bold);
	
	delete postupForm;
	printform->printer()->setDocName("Відомість обігу та залишків на складі");
	printform->printer()->setPageMargins( 10, 10, 7, 10, QPrinter::Millimeter );
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs_Sklad::print_vidomistZbutuExt(QDate begDate, QDate endDate, int groupId, int tovarId, int klientId, int nakladnaId)
{
	printform = new UPrintForm(0, "Відомість збуту");
	
	QString whereStr = " WHERE Tovar_id>0 and (nakladnaVyh.CDate BETWEEN date('"+begDate.toString("yyyy-MM-dd")+"') AND date('"+endDate.toString("yyyy-MM-dd")+"')) ";
	if (groupId > 0)
		whereStr += " and tovPerelik.GroupId="+QString::number(groupId)+" ";
	if (tovarId > 0)
		whereStr += " and skladPakety.Tovar_id="+QString::number(tovarId)+" ";
	if (klientId > 0)
		whereStr += " and nakladnaVyh.Partner_id="+QString::number(klientId)+" ";
	if (nakladnaId > 0)
		whereStr += " and nakladnaVyh.id="+QString::number(nakladnaId)+" ";
	
	QSqlQuery query;
	QString pidprName;
	query.exec("SELECT ScorochNazvaPidpr FROM orginfo WHERE id=1");
	query.next();
	pidprName = query.value(0).toString();
	
	query.setForwardOnly(true);
	query.exec("SELECT tovPerelik.Name, tovPerelik.Odynyci, tovPerelik.Nomer, \
						nakladnaVh.Num, nakladnaVh.CDate, \
						nakladnaVyh.Num, nakladnaVyh.CDate, \
						dovPartners.Name, \
						skladPakety.Cina AS cinaKup, vidpuskTovariv.Cina AS cinaProd, \
						sum(vidpuskTovariv.Kilkist) AS kilk, \
						skladPakety.Tovar_id, nakladnaVyh.id AS nId \
				FROM ((((nakladnaVyh \
				LEFT JOIN vidpuskTovariv ON nakladnaVyh.id=vidpuskTovariv.NakladnaVyh_id) \
				LEFT JOIN skladPakety ON skladPakety.id=vidpuskTovariv.Paket_id) \
				LEFT JOIN dovPartners ON dovPartners.id=nakladnaVyh.Partner_id) \
				LEFT JOIN nakladnaVh ON nakladnaVh.id=skladPakety.nakladnaVh_id) \
				LEFT JOIN tovPerelik ON tovPerelik.id=skladPakety.Tovar_id \
				"+whereStr+" \
				GROUP BY skladPakety.Tovar_id, nakladnaVyh.id, vidpuskTovariv.Cina, skladPakety.Cina \
				ORDER BY nakladnaVyh.CDate, nakladnaVyh.Num, tovPerelik.Name, tovPerelik.Odynyci, tovPerelik.Nomer");
	int rowCount = query.size();
	UPostupForm * postupForm = new UPostupForm(0, rowCount);
	postupForm->show();
	
	QTextCursor cursor(printform->document());
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_small;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 8 );
	cursor.insertText("Документ створено програмою "+QString(USklad::programName())+" "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	
	textCharFormat.setFontPointSize( 11 );
	textCharFormat_bold.setFontPointSize( 11 );
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertText("ВІДОМІСТЬ ЗБУТУ\nпо "+pidprName+" за період з "+begDate.toString("dd.MM.yyyy")+" по "+endDate.toString("dd.MM.yyyy")+" р.", textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	
	//Змінні таблиці
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table=NULL;
	UPopulateCell tableCell(table);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 200);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 120);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.5);
	tableFormat.setHeaderRowCount(1);
	table = cursor.insertTable(2+rowCount, 13, tableFormat);
	tableCell.setTextTable(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 9 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 9 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"№ з/п", textCharFormat);
	tableCell.set(0,1,"Інвентарний номер", textCharFormat);
	tableCell.set(0,2,"Назва", textCharFormat);
	tableCell.set(0,3,"Вхідна накладна", textCharFormat);
	tableCell.set(0,4,"Вихідна накладна", textCharFormat);
	tableCell.set(0,5,"Клієнт", textCharFormat);
	tableCell.set(0,6,"Ціна купівлі", textCharFormat);
	tableCell.set(0,7,"Ціна продажу", textCharFormat);
	tableCell.set(0,8,"Націнка", textCharFormat);
	tableCell.set(0,9,"К-сть", textCharFormat);
	tableCell.set(0,10,"Сума купівлі", textCharFormat);
	tableCell.set(0,11,"Сума збуту", textCharFormat);
	tableCell.set(0,12,"Загальна націнка", textCharFormat);
	
	//Заповнення таблиці
	textCharFormat.setFontPointSize( 9 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 9 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	int row;
	double cinaKup, cinaZbutu, kilk;
	double dVal;
	double zakupSum=0, zbutSum=0, nacinkaSum=0;
	for (row=0; row<rowCount && query.next(); row++){	
		tableCell.setAlignment(Qt::AlignCenter);
		//"№ з/п"
		tableCell.set(row+1,0,row+1, textCharFormat);
		//"Інв. номер"
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(row+1,1,query.value(2).toString(), textCharFormat);
		//"Назва"
		tableCell.set(row+1,2,query.value(0).toString()+", "+query.value(1).toString(), textCharFormat);
		//"Вх.накладна"
		tableCell.set(row+1,3,"№ "+query.value(3).toString()+" від "+query.value(4).toDate().toString("dd.MM.yy"), textCharFormat);
		//"Вих.накладна"
		tableCell.set(row+1,4,"№ "+query.value(5).toString()+" від "+query.value(6).toDate().toString("dd.MM.yy"), textCharFormat);
		//"Клієнт"
		tableCell.set(row+1,5,query.value(7).toString(), textCharFormat);
		tableCell.setAlignment(Qt::AlignRight);
		//"Ціна купівлі"
		cinaKup = query.value(8).toDouble();
		tableCell.set(row+1,6,QString::number(cinaKup).replace('.',','), textCharFormat);
		//"Ціна продажу"
		cinaZbutu = query.value(9).toDouble();
		tableCell.set(row+1,7,QString::number(cinaZbutu).replace('.',','), textCharFormat);
		//"Націнка"
		tableCell.set(row+1,8,QString::number(cinaZbutu-cinaKup).replace('.',','), textCharFormat);
		//"К-сть"
		kilk = query.value(10).toDouble();
		tableCell.set(row+1,9,QString::number(kilk).replace('.',','), textCharFormat);
		//Сума купівлі
		dVal = uDToM(cinaKup * kilk);
		zakupSum += dVal;
		tableCell.set(row+1,10,uMToStr2(dVal), textCharFormat);
		//Сума збуту
		dVal = uDToM(cinaZbutu * kilk);
		zbutSum += dVal;
		tableCell.set(row+1,11,dVal, textCharFormat);
		//"Заг.націнка"
		dVal = uDToM((cinaZbutu - cinaKup) * kilk);
		nacinkaSum += dVal;
		tableCell.set(row+1,12,dVal, textCharFormat);
		
		postupForm->incPos();
	}
	
	table->mergeCells ( row+1, 0, 1, 10 );
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(row+1,0,"Всього", textCharFormat);
	//Сума купівлі
	tableCell.set(row+1,10,uMToStr2(zakupSum), textCharFormat);
	//Сума збуту
	tableCell.set(row+1,11,zbutSum, textCharFormat);
	//"Заг.націнка"
	tableCell.set(row+1,12,nacinkaSum, textCharFormat);
	
	delete postupForm;
	printform->printer()->setDocName("Відомість збуту");
	printform->printer()->setPageMargins( 10, 10, 7, 10, QPrinter::Millimeter );
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
UPrintDocs_Sklad::~UPrintDocs_Sklad()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
