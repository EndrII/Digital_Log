#include "PrinterDialog.h"
PrinterDialog::PrinterDialog(QTableView *table, QWidget *ptr)
    :QDialog(ptr)
{
  view=table;
  this->setModal(true);
  createPrintLine();
  QHBoxLayout *main=new QHBoxLayout;
  preview=new QTextBrowser();
  QVBoxLayout *box=new QVBoxLayout();

  preview->setMinimumSize(360,480);
  preview->setMaximumSize(360,480);
  box->addWidget(preview);
  font=new QPushButton(ELanguage::getWord(BUTTON_FONT));
  box->addWidget(font);
  main->addLayout(box);
  main->addWidget(printLine);
  this->setLayout(main);
  connect(font,SIGNAL(clicked(bool)),SLOT(font_()));
  update_View();
}
void PrinterDialog::font_(){
    bool ok;
    fontRes =QFontDialog::getFont(&ok,fontRes,this);
    if(ok){
        update_View();
    }
}
QString PrinterDialog::fontStyleGenerate(bool fromView){
    QString fromfont;

    fromfont="th,td{";

    fromfont+="font-family: "+fontRes.family()+";";
    if(fontRes.bold())
        fromfont+="font-weight: bold;";
    switch (fontRes.style()) {
    case  QFont::StyleItalic:
        fromfont+="font-style: italic;";
        break;
    case  QFont::StyleOblique:
        fromfont=+"font-style: oblique;";
        break;
    default:
        break;
    }
    if(fontRes.pointSizeF()>0.0)
        fromfont+=QString("font-size: %1pt;").arg(fontRes.pointSizeF()*((fromView)?0.7:1));
    else{
        if(fontRes.pointSize()>0)
            fromfont+=QString("font-size: %1pt;").arg(fontRes.pointSize()*((fromView)?0.7:1));
    }
    if (fontRes.underline() && fontRes.strikeOut())
        fromfont+= "text-decoration: underline line-through;";
    else
        if (fontRes.underline())
            fromfont+= "text-decoration: underline;";
        else
            if (fontRes.strikeOut())
                fromfont+= "text-decoration: line-through;";
    return fromfont+="}";
}
void PrinterDialog::comboIndexChanged(int){
    update_View();
}
void PrinterDialog::ok_(int result){
    if(result){
        if(printLine->selectedNameFilter()=="Portable Document Format (*.pdf)")
            Printer::printPDF(Printer::getHTML(view,styleTextGenerate(false)),printLine->selectedFiles()[0]);
        else
        if(printLine->selectedNameFilter()=="HyperText (*.html)")
            Printer::printPDF(Printer::getHTML(view,styleTextGenerate(false)),printLine->selectedFiles()[0]);
        else
        if(printLine->selectedNameFilter()=="Microsoft Word (*.docx)")
            Printer::print(Printer::getHTML(view,styleTextGenerate(false)),printLine->selectedFiles()[0],"docx");
        else
            Printer::print(Printer::getHTML(view,styleTextGenerate(false)),printLine->selectedFiles()[0],"odt");
    }
    this->close();
}
QString PrinterDialog::styleTextGenerate(bool fromView){
    QString result;
    result+="<style>";
    result+="table{"
            ""
            "}";
    result+=fontStyleGenerate(fromView)+fontStyleGenerate(fromView);
    result+="</style>";
    qDebug()<<result;
    return result;
}
void PrinterDialog::update_View(){
    preview->setText(Printer::getHTML(view,styleTextGenerate()));
}
void PrinterDialog::createPrintLine(){
    printLine=new QFileDialog(NULL,"Save","./",tr("Portable Document Format (*.pdf);;HyperText (*.html)"
                                                  ";;Microsoft Word (*.docx);;OpenDocument Format (*.odt)"));
    printLine->setAcceptMode(QFileDialog::AcceptSave);
    printLine->setMaximumHeight(480);
    printLine->setMinimumHeight(480);
    printLine->setMinimumWidth(640);
    connect(printLine,SIGNAL(finished(int)),SLOT(ok_(int)));

}
PrinterDialog::~PrinterDialog(){

}
