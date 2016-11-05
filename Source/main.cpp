/****************************************************************************
**
** Copyright (C) 2016 A.N.Yankovich.
** Contact: https://vk.com/id223266985
**
** $QT_BEGIN_LICENSE:GPLv3$
** This file is distributed under the GPLv3 license.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ELanguage::selectedLang()=":/language/RUS";
   // QApplication::setLibraryPaths(QStringList()<<"/media/D/Builds/build-Digital_Log-Desktop_Qt_5_7_0_GCC_64bit2-Release/plugins/");
    MainWindow w;
    w.show();
    return a.exec();
}
