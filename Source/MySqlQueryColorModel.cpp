#include "MySqlQueryColorModel.h"

MySqlQueryColorModel::MySqlQueryColorModel():
    QStandardItemModel(){
    limit=-1;
    isSumm=false;
    loaded=false;
}
bool MySqlQueryColorModel::check(const filter &filt) const{
    return filter_&static_cast<char>(filt);
}
bool MySqlQueryColorModel::isSummColumn()const
{
    return isSumm;
}
void MySqlQueryColorModel::calc(int i){
    int summ=0;
    for(int j=0;j<this->columnCount()-1;j++){
        summ+=this->data(index(i,j),Qt::EditRole).toInt();
    }
    this->setData(index(i,this->columnCount()-1),QVariant(summ));
}
void MySqlQueryColorModel::calc(QStandardItem *item){
    calc(item->row());
}
void MySqlQueryColorModel::recalc(){
    for(int i=0;i<this->rowCount()-1;i++)
        calc(i);
}
void MySqlQueryColorModel::setLoad(bool l){
    loaded=l;
    if(isSumm&&loaded){
        connect(this,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(calc(QStandardItem*)));
    }else{
        disconnect(this,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(calc(QStandardItem*)));
    }
}
void MySqlQueryColorModel::setSummColumn(bool summ){
    if(summ&&loaded){
        connect(this,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(calc(QStandardItem*)));
    }else{
        disconnect(this,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(calc(QStandardItem*)));
    }
    isSumm=summ;
}
filter MySqlQueryColorModel::checkLimit(const QModelIndex &index)const{
    float temp=limit/(QStandardItemModel::data(QStandardItemModel::index(index.row(),columnCount()-1)).toFloat()+1);
    if(temp<0){
        return filter::normal;//good
    }else{
        if(temp<1.0){
            return filter::critical;//bad

        }else{
            if(temp<2.0){
                return filter::warning;//warning

            }else{
                return filter::normal;//good
            }
        }
    }
}
void MySqlQueryColorModel::setFilter(const char filter){
    filter_=filter;
}
void MySqlQueryColorModel::setLimit(const int row){
        limit=row;
}
QVariant MySqlQueryColorModel::data(const QModelIndex &index, int role) const{
    if(role==Qt::BackgroundRole){
        switch (checkLimit(index)) {
        case filter::normal:
            return QBrush(QColor(240,255,240));//good       #f0fff0
            break;
        case filter::warning:
            return QBrush(QColor(255,228,181));//warning    #ffE4B5
            break;
        case filter::critical:
            return QBrush(QColor(255,130,71));//bad         #ff8247
            break;
        default:
            return QBrush(QColor(240,255,240));//good
            break;
        }
    }
    return QStandardItemModel::data(index,role);
}
MySqlQueryColorModel::~MySqlQueryColorModel(){
}


MySqlQueryColorModelU::MySqlQueryColorModelU():
    MySqlQueryColorModel(){
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
filter MySqlQueryColorModelU::checkLimit(const QModelIndex &index)const{
    if(!isAll){
        switch (index.column()) {
        case 0:
            if(index.data().toInt()<limit[PC_]){
                return filter::critical;
            }else{
                return filter::normal;
            }
        case 1:
            if(index.data().toInt()<limit[LC_]){
                return filter::critical;
            }else{
                return filter::normal;
            }
        case 2:
        case 3:
            if(index.data().toInt()<limit[index.column()-1]){
                return filter::critical;
            }else{
                return filter::normal;
            }
        case 4:
            if(index.data().toInt()<limit[LC_]){
                return filter::warning;
            }else{
                return filter::normal;
            }
            break;
        case 5:

            if(index.data().toInt()<limit[KRC_]){
                return filter::warning;
            }else{
                return filter::normal;
            }
            break;
        case 6:

            if(index.data().toInt()<limit[RGRC_]){
                return filter::warning;
            }else{
                return filter::normal;
            }
            break;
        default:
            return filter::normal;
        }
    }else{
        float temp=limit[last]/(QStandardItemModel::data(QStandardItemModel::index(index.row(),columnCount()-1)).toFloat()+1);
        if(temp<0){
            return filter::normal;
        }else{
            if(temp<1.0){
                return filter::normal;
            }else{
                if(temp<2.0){
                    return filter::warning;
                }else{
                    return filter::critical;
                }
            }
        }
    }
}
MySqlQueryColorModelU::~MySqlQueryColorModelU(){
}
