#ifndef PRINTERDIALOG_H
#define PRINTERDIALOG_H
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFontDialog>
#include "languageindex.h"
#include <QDebug>
#include <QLabel>
#include <QSpinBox>
#include "Printer.h"
#include "SpaceEngineComponents/System/ELanguage.h"
class PrinterDialog :public QDialog
{
    Q_OBJECT
private:
    QFont fontRes;
    QTableView *view;
    QVBoxLayout *pane;
    QTextBrowser *preview;
    QFileDialog *printLine;
    QPushButton *font;
    QLineEdit *fileName;
    void createPrintLine();
    QString fontStyleGenerate(bool fromView);
    void update_View();
    QString styleTextGenerate(bool fromView=true);
private slots:
    void ok_(int);
    //void cancle_();
    void comboIndexChanged(int);
    void font_();
public:
    PrinterDialog(QTableView *table,QWidget* ptr=NULL);

    ~PrinterDialog();
};

#endif // PRINTERDIALOG_H
