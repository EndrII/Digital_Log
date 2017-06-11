#ifndef MOUNTHDIALOG_H
#define MOUNTHDIALOG_H
#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
class MounthDialog : public QDialog
{
    Q_OBJECT
private:
    bool *mounth;
    QCheckBox *January,
    *February,
    *March,
    *April,
    *May,
    *June,
    *July,
    *August,
    *September,
    *October,
    *November,
    *December;
    QPushButton *ok;
protected slots:
    void apply(bool);
public:
    MounthDialog(bool*mounthData, QWidget* ptr=NULL);
    ~MounthDialog();
};

#endif // MOUNTHDIALOG_H
