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
    // 初始化表头 - 添加第四个表头名称
    mHeader<< "检验项目"<<"检验结果"<<"算法检验结果"<<"备注";
    
    // 初始化数据列表
    QStringList values1, values2, values3, values4, values5, values6, values7, values8, values9;
    values1 << "检验项目一" << "通过" << "未完全校验" << "ceshi1";
    values2 << "检验项目2" << "未校验" << "未完全校验" << "ceshi1";
    values3 << "检验项目3" << "未校验" << "未完全校验" << "ceshi1";
    values4 << "检验项目4" << "未校验" << "未完全校验" << "ceshi1";
    values5 << "检验项目5" << "未校验" << "未完全校验" << "ceshi1";
    values6 << "检验项目6" << "未校验" << "未完全校验" << "ceshi1";
    values7 << "检验项目7" << "未校验" << "未完全校验" << "ceshi1";
    values8 << "检验项目8" << "未校验" << "未完全校验" << "ceshi1";
    values9 << "检验项目9" << "未校验" << "未完全校验" << "ceshi1";
    
    datalist << new stream_result_all_data(values1)
             << new stream_result_all_data(values2)
             << new stream_result_all_data(values3)
             << new stream_result_all_data(values4)
             << new stream_result_all_data(values5)
             << new stream_result_all_data(values6)
             << new stream_result_all_data(values7)
             << new stream_result_all_data(values8)
             << new stream_result_all_data(values9);
    
    // 创建额外列数据，用于存储第四列的值
    QList<QMap<int, QString>> extraColumnsData;
    for (int i = 0; i < datalist.size(); ++i) {
        const stream_result_all_data *data = datalist.at(i);
        QMap<int, QString> rowData;
        // 如果有第四列数据，则添加到额外列数据中
        if (data->getAllFields().size() > 3) {
            rowData[3] = data->getAllFields().at(3);  // 第四列的数据
        }
        extraColumnsData.append(rowData);
    }
    
    // 使用TableGenerator生成表格数据 - 使用增强版方法支持多列
    mTableGenerator->generateTable(mTableViewModel, mHeader, datalist, extraColumnsData);
    
    // 使用TableGenerator设置表格视图
    mTableGenerator->setupTableView(ui->tableView, mTableViewModel);
    
    // 使用TableGenerator合并单元格
    mTableGenerator->mergeCells(ui->tableView, 0, 2, 9, 1);
}
