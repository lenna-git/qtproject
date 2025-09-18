#ifndef FORM_PAGE3_H
#define FORM_PAGE3_H

#include <QWidget>
#include<QStandardItemModel>


namespace Ui {
class Form_page3;
}

class Form_page3 : public QWidget
{
    Q_OBJECT

public:
    explicit Form_page3(QWidget *parent = 0);
    ~Form_page3();
    void initchkresult(int Type,QString filepath);

private slots:
    void on_pushButton_gen_all_clicked();

private:
    Ui::Form_page3 *ui;
    QStandardItemModel*  mTableViewModel;
    void paintTable_randomtest();
    void paintTable_streamcipher_all();
    void paintTable_blockcipher_all();
signals:
    swichrightpage(int Type);
};

#endif // FORM_PAGE3_H
