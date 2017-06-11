#ifndef PRINTER_H
#define PRINTER_H
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QTextDocument>
#include <QPrinter>
#include "MySqlQueryColorModel.h"
#include <QTextStream>
class Printer
{
private:
   // QPdfWriter *writer;
public:
    explicit Printer();
    static bool print(QTableView* table, QString patch="./page",const QString& type="HTML");
    static bool print(const QString &code, QString patch="./page",const QString& type="HTML");
    static QString getHTML(QTableView *table, const QString &style="");
    static bool printPDF(QTableView *table, QString patch="./page.pdf");
    static bool printPDF(const QString &code, QString patch="./page.pdf");
    static QString test(QTableView *tab, int i);

    virtual ~Printer();
};

#endif // PRINTER_H
