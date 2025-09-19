#include "form_page3.h"
#include "ui_form_page3.h"
#include<QVBoxLayout>

Form_page3::Form_page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_page3),
    mTableViewModel(new QStandardItemModel),
    mTableGenerator(new TableGenerator)
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
    delete mTableGenerator;
}


//绘制序列总表
void Form_page3::paintTable_all()
{
    // 初始化表头
    mHeader<< "检验项目"<<"检验结果"<<"算法检验结果";
    
    // 初始化数据列表
    datalist<<new stream_result_all_data("检验项目一","通过","未完全校验")
            <<new stream_result_all_data("检验项目2","未校验","未完全校验")
            <<new stream_result_all_data("检验项目3","未校验","未完全校验")
            <<new stream_result_all_data("检验项目4","未校验","未完全校验")
            <<new stream_result_all_data("检验项目5","未校验","未完全校验")
            <<new stream_result_all_data("检验项目6","未校验","未完全校验")
            <<new stream_result_all_data("检验项目7","未校验","未完全校验")
            <<new stream_result_all_data("检验项目8","未校验","未完全校验")
            <<new stream_result_all_data("检验项目9","未校验","未完全校验");
    
    // 使用TableGenerator生成表格数据
    mTableGenerator->generateTable(mTableViewModel, mHeader, datalist);
    
    // 使用TableGenerator设置表格视图
    mTableGenerator->setupTableView(ui->tableView, mTableViewModel);
    
    // 使用TableGenerator合并单元格
    mTableGenerator->mergeCells(ui->tableView, 0, 2, 9, 1);
}
