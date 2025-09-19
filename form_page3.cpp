#include "form_page3.h"
#include "ui_form_page3.h"
#include<QVBoxLayout>

Form_page3::Form_page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_page3),
    mTableViewModel(new QStandardItemModel)
{
    ui->setupUi(this);
    QVBoxLayout *alllayout = new QVBoxLayout(this);
    alllayout->addWidget(ui->lineEdit);
    alllayout->addWidget(ui->label);
    alllayout->addWidget(ui->tableView);
    alllayout->addWidget(ui->textEdit);
    alllayout->addWidget(ui->pushButton);
    ui->textEdit->setFixedHeight(100);

    paintTable_all();
}

Form_page3::~Form_page3()
{
    delete ui;
}


//绘制序列总表
void Form_page3::paintTable_all()
{


    mHeader<< "检验项目"<<"检验结果"<<"算法检验结果";
    mTableViewModel->setHorizontalHeaderLabels(mHeader);
    mTableViewModel->setColumnCount(mHeader.size());

    datalist<<new stream_result_all_data("检验项目一","通过","未完全校验")
            <<new stream_result_all_data("检验项目2","未校验","未完全校验")
            <<new stream_result_all_data("检验项目3","未校验","未完全校验")
            <<new stream_result_all_data("检验项目4","未校验","未完全校验")
            <<new stream_result_all_data("检验项目5","未校验","未完全校验")
            <<new stream_result_all_data("检验项目6","未校验","未完全校验")
            <<new stream_result_all_data("检验项目7","未校验","未完全校验")
            <<new stream_result_all_data("检验项目8","未校验","未完全校验")
            <<new stream_result_all_data("检验项目9","未校验","未完全校验");

    for(int i=0;i<datalist.size();i++){
        stream_result_all_data *tmp = datalist.at(i);
        QStandardItem *item0 = new QStandardItem;
        item0->setData(tmp->m_testitem,Qt::DisplayRole);
        QStandardItem *item1 = new QStandardItem;
        item1->setData(tmp->m_testresult,Qt::DisplayRole);

        QStandardItem *item3 = new QStandardItem;
        item3->setData(tmp->m_wholeresult,Qt::DisplayRole);


        mTableViewModel->setItem(i,0,item0);
        mTableViewModel->setItem(i,1,item1);
        mTableViewModel->setItem(i,2,item3);

    }

    ui->tableView->setModel(mTableViewModel);

    //合并第3列到一行
    ui->tableView->setSpan(0,2,9,1);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}
