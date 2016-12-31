#include "MySqlQueryColorModel.h"

MySqlQueryColorModel::MySqlQueryColorModel():
    QSqlQueryModel(){
    limit=-1;
}
void MySqlQueryColorModel::setLimit(const int row){
        limit=row;
}
QVariant MySqlQueryColorModel::data(const QModelIndex &index, int role) const{
    if(Qt::BackgroundRole==role){
        float temp=limit/(QSqlQueryModel::data(QSqlQueryModel::index(index.row(),columnCount()-1)).toFloat()+1);
        if(temp<0){
            return QBrush(QColor(240,255,240));//good
        }else{
            if(temp<1.0){
                return QBrush(QColor(255,130,71));//bad

            }else{
                if(temp<2.0){
                    return QBrush(QColor(255,228,181));//warning

                }else{
                    return QBrush(QColor(240,255,240));//good
                }
            }
        }
    }
    return QSqlQueryModel::data(index,role);
}
MySqlQueryColorModel::~MySqlQueryColorModel(){
}


MySqlQueryColorModelU::MySqlQueryColorModelU():
    QSqlQueryModel(){
    isAll=false;
    limit[0]=-1;
    limit[1]=-1;
    limit[2]=-1;
    limit[3]=-1;
    last=0;
}
void MySqlQueryColorModelU::setLimit(const int row,pref prefix,bool all){
        limit[prefix]=row;
        isAll=all;
        last=prefix;
}
void MySqlQueryColorModelU::clear(){
    limit[0]=-1;
    limit[1]=-1;
    limit[2]=-1;
    limit[3]=-1;
}
QVariant MySqlQueryColorModelU::data(const QModelIndex &index, int role) const{
    if(Qt::BackgroundRole==role){
        if(!isAll){
            switch (index.column()) {
            case 0:return QBrush(QColor(240,255,240));
            case 1:
            case 2:
            case 3:
            case 4:
                if(index.data().toInt()<limit[index.column()-1]){
                    return QBrush(QColor(255,228,181));
                }else{
                    return QBrush(QColor(240,255,240));
                }
            case 5:
                if(index.data().toInt()<limit[LC_]){
                    return QBrush(QColor(255,130,71));
                }else{
                    return QBrush(QColor(240,255,240));
                }
                break;
            case 6:

                if(index.data().toInt()<limit[KRC_]){
                    return QBrush(QColor(255,130,71));
                }else{
                    return QBrush(QColor(240,255,240));
                }
                break;
            case 7:

                if(index.data().toInt()<limit[RGRC_]){
                    return QBrush(QColor(255,130,71));
                }else{
                    return QBrush(QColor(240,255,240));
                }
                break;
            default:
                break;
            }
        }else{
            float temp=limit[last]/(QSqlQueryModel::data(QSqlQueryModel::index(index.row(),columnCount()-1)).toFloat()+1);
            if(temp<0){
                return QBrush(QColor(240,255,240));//good
            }else{
                if(temp<1.0){
                    return QBrush(QColor(240,255,240));//good
                }else{
                    if(temp<2.0){
                        return QBrush(QColor(255,228,181));//warning
                    }else{
                        return QBrush(QColor(255,130,71));//bad
                    }
                }
            }
        }
    }
    return QSqlQueryModel::data(index,role);
}
MySqlQueryColorModelU::~MySqlQueryColorModelU(){
}
