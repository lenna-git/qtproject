#ifndef FORM_PAGE3_H
#define FORM_PAGE3_H

#include <QWidget>
#include<QStandardItemModel>
#include"chkresultclass/chk_singleitem_result.h"
#include"tablegenerator/tablegenerator.h"
#include"log4qt/logger.h"

namespace Ui {
class Form_page3;
}

class Form_page3 : public QWidget
{
    Q_OBJECT

public:
    explicit Form_page3(QWidget *parent = 0);
    ~Form_page3();

private:
    Ui::Form_page3 *ui;
    QStandardItemModel*  mTableViewModel ;
    QStringList mHeader;
    QList<chk_singleitem_result *> datalist;
    TableGenerator *mTableGenerator;
    Log4Qt::Logger *mLogger;

    void paintTable_all();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // FORM_PAGE3_H
