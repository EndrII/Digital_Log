#include "Printer.h"

Printer::Printer()
{

}
bool Printer::print(QTableView *table, QString patch, const QString &type){
       return Printer::print(Printer::getHTML(table),patch,type);
}
bool Printer::print(const QString &code, QString patch,const QString&type ){
    if(patch.mid(patch.size()-type.length()).compare(type,Qt::CaseInsensitive)!=0)
            patch+="."+type;
        QFile f(patch);
        if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
            QTextStream stream(&f);
            stream.setCodec("UTF-8");
            stream<<code;
            f.close();
            return true;
        }else
            return false;
}
QString Printer::test(QTableView *tab,int i){
    switch (static_cast<MySqlQueryColorModel*>(tab->model())->checkLimit(tab->model()->index(i,0))
) {
    case filter::normal:return "#f0fff0";
    case filter::warning:return "#ffE4B5";
    case filter::critical:return "#ff8247";
    default: return "#f0fff0";
    }
}
QString Printer::getHTML(QTableView *table,const QString&style ){
    QString stream("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>");
    if(style.isEmpty()){
        stream+="<style>table {"
              "width:100%;"
              "border-collapse: collapse;"
              "border: 2px solid black;}"
              "td {    padding: 5px;"
              "padding-right: 30px;"
              "border: 1px solid lightgray;}"
              "th {}"
              "</style>";
    }else{
        stream+=style;
    }
    stream+="<table id=PrintPage><tr><th>ФИО</th>";
    for(int i=0;i<table->model()->columnCount();i++){
        stream+="<th>"+table->model()->headerData(i,Qt::Horizontal).toString()+"</th>";
    }
    stream+="</tr>";
    for(int i=0;i<table->model()->rowCount();i++){
        stream+="<tr backgrouning-color="+test(table,i)+"><td>"+table->model()->headerData(i,Qt::Vertical).toString()+":</td>";
        for(int j=0;j<table->model()->columnCount();j++){
             stream+="<td>"+table->model()->data(table->model()->index(i,j)).toString()+"</td>";
            }
        stream+="</tr>";
    }
    stream+="</table>";
    return stream;
}
bool Printer::printPDF(QTableView *table, QString patch){
    return Printer::printPDF(Printer::getHTML(table),patch);
}
bool Printer::printPDF(const QString& code ,QString patch){
    if(patch.mid(patch.size()-3).compare("pdf",Qt::CaseInsensitive)!=0)
        patch+=".pdf";
    QTextDocument doc;
    doc.setHtml(code);
    QPrinter p(QPrinter::HighResolution);
    p.setPageSize(QPrinter::A4);
    p.setOutputFormat(QPrinter::PdfFormat);
    p.setOutputFileName(patch);
    doc.print(&p);
    return true;
}
Printer::~Printer(){

}
