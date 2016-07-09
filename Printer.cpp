#include "Printer.h"

Printer::Printer()
{

}
bool Printer::printHtml(QTableWidget *table, const QString &dop, QString patch){
    if(patch.mid(patch.size()-4).compare("html",Qt::CaseInsensitive)!=0)
        patch+=".html";
    QFile f(patch);
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QTextStream stream(&f);
        /*stream<<"<meta charset='UTF-8'>"
              <<"<style>table {"
              <<"width:100%;"
              <<"border-collapse: collapse;"
              <<"border: 2px solid black;}"
              <<"td {    padding: 5px;"
              <<"padding-right: 30px;"
              <<"border: 1px solid lightgray;}"
              <<"th {}"
              <<"</style>"
              <<"<table>"<<
                "<tr><td></td>";
       // int temp=table->columnCount();
        for(int i=0;i<table->columnCount();i++){
            stream<<"<td>"<<table->horizontalHeaderItem(i)->text()<<"</td>";
        }
        stream<<"</tr>";
        for(int i=0;i<table->rowCount();i++){
            stream<<"<tr>"<<"<td>"<<table->verticalHeaderItem(i)->text()<<"</td>";
            for(int j=0;j<table->columnCount();j++){
                if(qobject_cast<QLabel*>(table->cellWidget(i,j))!=NULL){
                    stream<<"<td>"<<((QLabel*)table->cellWidget(i,j))->text()<<"</td>";
                }else{
                    stream<<"<td>"<<((QLineEdit*)table->cellWidget(i,j))->text()<<"</td>";
                }
            }
            stream<<"</tr>";
        }
        stream<<"</table>";*/
        stream<<dop;
        stream<<Printer::getHTML(table);
        f.close();
        return true;
    }else
        return false;
}

QString Printer::getHTML(QTableWidget *table){
    QString stream;
        stream+="<meta charset='UTF-8'>"
              "<style>table {"
              "width:100%;"
              "border-collapse: collapse;"
              "border: 2px solid black;}"
              "td {    padding: 5px;"
              "padding-right: 30px;"
              "border: 1px solid lightgray;}"
              "th {}"
              "</style>"
              "<table>"
                "<tr><td></td>";
       // int temp=table->columnCount();
        for(int i=0;i<table->columnCount();i++){
            stream+="<td>"+table->horizontalHeaderItem(i)->text()+"</td>";
        }
        stream+="</tr>";
        for(int i=0;i<table->rowCount();i++){
            stream+="<tr><td>"+table->verticalHeaderItem(i)->text()+"</td>";
            for(int j=0;j<table->columnCount();j++){
                if(qobject_cast<QLabel*>(table->cellWidget(i,j))!=NULL){
                    stream+="<td>"+((QLabel*)table->cellWidget(i,j))->text()+"</td>";
                }else{
                    stream+="<td>"+((QLineEdit*)table->cellWidget(i,j))->text()+"</td>";
                }
            }
            stream+="</tr>";
        }
        stream+="</table>";
        return stream;

}
bool Printer::printPDF(QTableWidget *table,const QString& dop ,QString patch){
    if(patch.mid(patch.size()-3).compare("pdf",Qt::CaseInsensitive)!=0)
        patch+=".pdf";
    QTextDocument doc;
    QString temp=dop+Printer::getHTML(table);
    doc.setHtml(temp);
    QPrinter p(QPrinter::HighResolution);
    p.setPageSize(QPrinter::A4);
    p.setOutputFormat(QPrinter::PdfFormat);
    p.setOutputFileName(patch);

    doc.print(&p);
    return true;
}
Printer::~Printer(){

}
