#include "form_page3.h"
#include "ui_form_page3.h"
#include<QVBoxLayout>
#include<QPrinter>
#include<QPainter>
#include<QFileDialog>
#include<QMessageBox>
#include "pdfgenerator/pdfgenerator.h"

Form_page3::Form_page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_page3),
    mTableViewModel(new QStandardItemModel),
    mTableGenerator(new TableGenerator)
{
    ui->setupUi(this);
    QVBoxLayout *alllayout = new QVBoxLayout(this);
    ui->lineEdit->setVisible(false);
//    alllayout->addWidget(ui->lineEdit);
    alllayout->addWidget(ui->label);
    alllayout->addWidget(ui->tableView);
    alllayout->addWidget(ui->textEdit);
    alllayout->addWidget(ui->pushButton);
    ui->textEdit->setFixedHeight(100);

    ui->label->setText("XXX检验结果");
    ui->label->setAlignment(Qt::AlignCenter);

    ui->textEdit->setText("备注：空手道解放涉及多方了解实际发生经典服饰开始阶段了封建士大夫精神的");

    paintTable_all();
}

Form_page3::~Form_page3()
{
    delete ui;
    delete mTableGenerator;
}

void Form_page3::on_pushButton_clicked()
{
    // 调用PDFGenerator中的方法生成、预览和管理PDF文件
    PDFGenerator::generateAndManageFormPage3PDF(this);
}

//绘制表
void Form_page3::paintTable_all()
{
    // 初始化表头 - 添加序号列和第四个表头名称
    mHeader<< "序号"<<"检验项目"<<"检验结果"<<"算法检验结果"<<"备注"<<"备注2";
    
    // 初始化数据列表
    QStringList values1, values2, values3, values4, values5, values6, values7, values8, values9;
    values1 << "1"<<"检验项目一" << "通过" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values2 << "2"<<"检验项目2" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values3 << "3"<<"检验项目3" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values4 << "4"<<"检验项目4" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values5 << "5"<<"检验项目5" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values6 << "6"<<"检验项目6" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values7 << "7"<<"检验项目7" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values8 << "8"<<"检验项目8" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values9 << "9"<<"检验项目9" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    datalist << new stream_result_all_data(values1)
             << new stream_result_all_data(values2)
             << new stream_result_all_data(values3)
             << new stream_result_all_data(values4)
             << new stream_result_all_data(values5)
             << new stream_result_all_data(values6)
             << new stream_result_all_data(values7)
             << new stream_result_all_data(values8)
             << new stream_result_all_data(values9);
    
    // 使用TableGenerator生成表格数据 - 使用增强版方法动态生成行列，并自动合并相同内容单元格
    mTableGenerator->generateTable(mTableViewModel, ui->tableView, mHeader, datalist);
    
    // 使用TableGenerator设置表格视图
    mTableGenerator->setupTableView(ui->tableView, mTableViewModel);
    
    // 设置序号列宽度（第0列），适合显示两个中文字符
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0, 50);
    
    // 为其他列设置自动调整宽度模式
    for (int i = 1; i < mHeader.size(); i++) {
        ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    
}
