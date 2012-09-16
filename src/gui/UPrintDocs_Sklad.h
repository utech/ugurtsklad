//--------------------utech--------------------utech--------------------utech--------------------
//
// Створений програмістами Компанії "Утех" 23.04.2009р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UPRINTDOCS_SKLAD_H
#define UPRINTDOCS_SKLAD_H

#include <QString>
#include <QVariant>
#include <QTextLength>
#include <QTextTable>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QTextTableCell>
#include <QTextCursor>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QTextFrameFormat>
#include <UPrintForm>
#include <UMath>
//--------------------utech--------------------utech--------------------utech--------------------
class UPrintDocs_Sklad
{
    public:
        UPrintDocs_Sklad();
		~UPrintDocs_Sklad();
		
		void print_vhidnaNakladna(int nId);
		void print_vyhidnaNakladna(int nId, bool needDyrBuhPidpys=false);
		void print_rahunok(int nId);
		void print_podatkovaNakladna(int nId);
		void print_obigIZalyshky(QDate begDate, QDate endDate, int groupId=0, bool printNotNullOnly = false);
		void print_vidomistZbutuExt(QDate begDate, QDate endDate, int groupId, int tovarId, int klientId, int nakladnaId);
		
		UPrintForm *printform;
    private:
		QDate date;
		QStringList monthlist;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
