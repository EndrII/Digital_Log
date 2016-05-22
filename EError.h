/****************************************************************************
**
** Copyright (C) 2016 A.N.Yankovich.
** Contact: https://vk.com/digitalface
**
** $QT_BEGIN_LICENSE:LGPL21$
** This file is distributed under the GPLv3 license.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef EERROR_H
#define EERROR_H
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;
class EError //klass oshibok
{
    private:
      int i;//index polomki
      string s,file_; //stroka pod zapis
      void What_();//chto sluchilos
    public:
      EError(const string&I,const string &file);
      EError(const int &I,const string &file);
      std::string What();//chto sluchilos
      int getID();
      void clear();//otchistka log faila
};

#endif // EERROR_H
