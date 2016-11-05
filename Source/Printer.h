#ifndef PRINTER_H
#define PRINTER_H
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QTextDocument>
#include <QPrinter>
#include <QTextStream>
class Printer
{
private:
   // QPdfWriter *writer;
public:
    explicit Printer();
    static bool printHtml(QTableWidget* table, const QString &dop, QString patch="./page.HTML");
    static inline QString getHTML(QTableWidget* table);
    static bool printPDF(QTableWidget* table, const QString &dop, QString patch="./page.pdf");
    virtual ~Printer();
};

#endif // PRINTER_H
