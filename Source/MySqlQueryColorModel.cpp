#include "MySqlQueryColorModel.h"

MySqlQueryColorModel::MySqlQueryColorModel():
    QSqlQueryModel(){
    limit=-1;
   // db=dB;
}
void MySqlQueryColorModel::enter(const QModelIndex &index, int value, const StackItem &item){
   /* stack.push_back(item);
    if(columnCount()<2||index.column()==0)
        return;
    if(this->query().lastQuery().indexOf("sum as")!=-1){
       bool test= QSqlQueryModel::setData(this->index(index.row(),columnCount()-1),QSqlQueryModel::data(this->index(index.row(),columnCount()-1)).toInt()+value-QSqlQueryModel::data(index).toInt(),Qt::EditRole);
    }
    bool test=QSqlQueryModel::setData(index,value,Qt::EditRole);
    emit SavesChanged(stack.size());*/
}
void MySqlQueryColorModel::stackWrite(){
   /* if(stack.empty()){
        return;
    }else{
        emit startSave(stack.size());
        int i=0;
        while(!stack.isEmpty()){
            db->Query_no_update(stack.front().qyer);
            if(stack.front().index<0)
                db->sumCount(stack.front().Group);
            else
                db->sumCount(stack.front().Group,stack.front().predmet,stack.front().index);
            stack.pop_front();
            i++;
            emit tableSaveProgress(i);
        }
        emit tableSaveProgress(i);
        emit SavesChanged(stack.size());
    }*/
}
void MySqlQueryColorModel::save(){
    //stackWrite();
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
QVariant MySqlQueryColorModelU::data(const QModelIndex &index, int role) const{
    if(Qt::BackgroundRole==role){
        if(!isAll){
            switch (index.column()) {
            case 0:return QBrush(QColor(240,255,240));
            case 1:
                if(index.data().toInt()<limit[PC_]){
                    return QBrush(QColor(255,130,71));
                }else{
                    return QBrush(QColor(240,255,240));
                }
            case 2:
                if(index.data().toInt()<limit[LC_]){
                    return QBrush(QColor(255,130,71));
                }else{
                    return QBrush(QColor(240,255,240));
                }
            case 3:
            case 4:
                if(index.data().toInt()<limit[index.column()-1]){
                    return QBrush(QColor(255,130,71));
                }else{
                    return QBrush(QColor(240,255,240));
                }
            case 5:
                if(index.data().toInt()<limit[LC_]){
                    return QBrush(QColor(255,228,181));
                }else{
                    return QBrush(QColor(240,255,240));
                }
                break;
            case 6:

                if(index.data().toInt()<limit[KRC_]){
                    return QBrush(QColor(255,228,181));
                }else{
                    return QBrush(QColor(240,255,240));
                }
                break;
            case 7:

                if(index.data().toInt()<limit[RGRC_]){
                    return QBrush(QColor(255,228,181));
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
