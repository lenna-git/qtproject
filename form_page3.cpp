#include "form_page3.h"
#include "ui_form_page3.h"
#include<QVBoxLayout>
#include<QPrinter>
#include<QPainter>
#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include<QList>
#include<QThread>
#include "pdfgenerator/pdfgenerator.h"
#include "chkresultclass/chk_items_result.h"

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
    alllayout->addWidget(ui->pushButton_2);
     alllayout->addWidget(ui->pushButton_3);
    ui->textEdit->setFixedHeight(100);

    ui->label->setText("XXX检验结果");
    ui->label->setAlignment(Qt::AlignCenter);

    ui->textEdit->setText(
                "备注：空手道解放涉及多方了解实际发生经典服饰开始阶段了封建士大夫精神的"
                          "空手道解放涉及多方了解实际发生经典服饰开始阶段了封建士大夫精神的"
                          "空手道解放涉及多方了解实际发生经典服饰开始阶段了封建士大夫精神的"
                          "\n"
                          "123空手道解放涉及多方了解实际发生经典服饰开始阶段了封建士大夫精神的"
                          "456空手道解放涉及多方了解实际发生经典服饰开始阶段了封建士大夫精神的结束"
                );

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
//    PDFGenerator::generateAndManageFormPage3PDF(this);
    PDFGenerator::generateAndManagePDFwithTableModel(ui->label->text(),ui->tableView->model(),ui->textEdit->toPlainText());
}

//绘制表
void Form_page3::paintTable_all()
{
    // 初始化表头 - 添加序号列和第四个表头名称
    mHeader<< "序号"<<"检验项目"<<"检验结果"<<"算法检验结果"<<"备注"<<"备注2";
    
    // 初始化数据列表，请初始化100个数据
    QStringList values1, values2, values3, values4, values5, values6, values7, values8, values9;
    QStringList values10, values11, values12, values13, values14, values15, values16, values17, values18, values19;
    QStringList values20, values21, values22, values23, values24, values25, values26, values27, values28, values29;
    QStringList values30, values31, values32, values33, values34, values35, values36, values37, values38, values39, values40;
    QStringList values41, values42, values43, values44, values45, values46, values47, values48, values49, values50;
    QStringList values51, values52, values53, values54, values55, values56, values57, values58, values59, values60;
    QStringList values61, values62, values63, values64, values65, values66, values67, values68, values69, values70;
    QStringList values71, values72, values73, values74, values75, values76, values77, values78, values79, values80;
    QStringList values81, values82, values83, values84, values85, values86, values87, values88, values89, values90;
    QStringList values91, values92, values93, values94, values95, values96, values97, values98, values99, values100;
    
    // 前9组数据保留原有内容
    values1 << "1"<<"检验项目一" << "通过" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values2 << "2"<<"检验项目2" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values3 << "3"<<"检验项目3" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values4 << "4"<<"检验项目4" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values5 << "5"<<"检验项目5" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values6 << "6"<<"检验项目6" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values7 << "7"<<"检验项目7" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values8 << "8"<<"检验项目8" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values9 << "9"<<"检验项目9" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 10-19组数据
    values10 << "10"<<"检验项目10" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values11 << "11"<<"检验项目11" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values12 << "12"<<"检验项目12" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values13 << "13"<<"检验项目13" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values14 << "14"<<"检验项目14" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values15 << "15"<<"检验项目15" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values16 << "16"<<"检验项目16" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values17 << "17"<<"检验项目17" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values18 << "18"<<"检验项目18" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values19 << "19"<<"检验项目19" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    
    // 20-29组数据
    values20 << "20"<<"检验项目20" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values21 << "21"<<"检验项目21" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values22 << "22"<<"检验项目22" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values23 << "23"<<"检验项目23" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values24 << "24"<<"检验项目24" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values25 << "25"<<"检验项目25" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values26 << "26"<<"检验项目26" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values27 << "27"<<"检验项目27" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values28 << "28"<<"检验项目28" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values29 << "29"<<"检验项目29" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 30-40组数据
    values30 << "30"<<"检验项目30" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values31 << "31"<<"检验项目31" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values32 << "32"<<"检验项目32" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values33 << "33"<<"检验项目33" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values34 << "34"<<"检验项目34" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values35 << "35"<<"检验项目35" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values36 << "36"<<"检验项目36" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values37 << "37"<<"检验项目37" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values38 << "38"<<"检验项目38" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values39 << "39"<<"检验项目39" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values40 << "40"<<"检验项目40" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 41-50组数据
    values41 << "41"<<"检验项目41" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values42 << "42"<<"检验项目42" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values43 << "43"<<"检验项目43" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values44 << "44"<<"检验项目44" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values45 << "45"<<"检验项目45" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values46 << "46"<<"检验项目46" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values47 << "47"<<"检验项目47" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values48 << "48"<<"检验项目48" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values49 << "49"<<"检验项目49" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values50 << "50"<<"检验项目50" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 51-60组数据
    values51 << "51"<<"检验项目51" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values52 << "52"<<"检验项目52" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values53 << "53"<<"检验项目53" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values54 << "54"<<"检验项目54" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values55 << "55"<<"检验项目55" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values56 << "56"<<"检验项目56" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values57 << "57"<<"检验项目57" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values58 << "58"<<"检验项目58" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values59 << "59"<<"检验项目59" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values60 << "60"<<"检验项目60" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 61-70组数据
    values61 << "61"<<"检验项目61" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values62 << "62"<<"检验项目62" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values63 << "63"<<"检验项目63" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values64 << "64"<<"检验项目64" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values65 << "65"<<"检验项目65" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values66 << "66"<<"检验项目66" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values67 << "67"<<"检验项目67" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values68 << "68"<<"检验项目68" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values69 << "69"<<"检验项目69" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values70 << "70"<<"检验项目70" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 71-80组数据
    values71 << "71"<<"检验项目71" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values72 << "72"<<"检验项目72" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values73 << "73"<<"检验项目73" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values74 << "74"<<"检验项目74" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values75 << "75"<<"检验项目75" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values76 << "76"<<"检验项目76" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values77 << "77"<<"检验项目77" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values78 << "78"<<"检验项目78" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values79 << "79"<<"检验项目79" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values80 << "80"<<"检验项目80" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 81-90组数据
    values81 << "81"<<"检验项目81" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values82 << "82"<<"检验项目82" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values83 << "83"<<"检验项目83" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values84 << "84"<<"检验项目84" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values85 << "85"<<"检验项目85" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values86 << "86"<<"检验项目86" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values87 << "87"<<"检验项目87" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values88 << "88"<<"检验项目88" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values89 << "89"<<"检验项目89" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values90 << "90"<<"检验项目90" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 91-100组数据
    values91 << "91"<<"检验项目91" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values92 << "92"<<"检验项目92" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values93 << "93"<<"检验项目93" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values94 << "94"<<"检验项目94" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values95 << "95"<<"检验项目95" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values96 << "96"<<"检验项目96" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values97 << "97"<<"检验项目97" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2_1";
    values98 << "98"<<"检验项目98" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values99 << "99"<<"检验项目99" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    values100 << "100"<<"检验项目100" << "未校验" << "未完全校验" << "ceshi1"<< "ceshi2";
    
    // 将所有100组数据添加到datalist中
    datalist << new chk_singleitem_result(values1)
           << new chk_singleitem_result(values2)
           << new chk_singleitem_result(values3)
           << new chk_singleitem_result(values4)
           << new chk_singleitem_result(values5)
           << new chk_singleitem_result(values6)
           << new chk_singleitem_result(values7)
           << new chk_singleitem_result(values8)
           << new chk_singleitem_result(values9)
           << new chk_singleitem_result(values10)
           << new chk_singleitem_result(values11)
           << new chk_singleitem_result(values12)
           << new chk_singleitem_result(values13)
           << new chk_singleitem_result(values14)
           << new chk_singleitem_result(values15)
           << new chk_singleitem_result(values16)
           << new chk_singleitem_result(values17)
           << new chk_singleitem_result(values18)
           << new chk_singleitem_result(values19)
           << new chk_singleitem_result(values20)
           << new chk_singleitem_result(values21)
           << new chk_singleitem_result(values22)
           << new chk_singleitem_result(values23)
           << new chk_singleitem_result(values24)
           << new chk_singleitem_result(values25)
           << new chk_singleitem_result(values26)
           << new chk_singleitem_result(values27)
           << new chk_singleitem_result(values28)
           << new chk_singleitem_result(values29)
           << new chk_singleitem_result(values30)
           << new chk_singleitem_result(values31)
           << new chk_singleitem_result(values32)
           << new chk_singleitem_result(values33)
           << new chk_singleitem_result(values34)
           << new chk_singleitem_result(values35)
           << new chk_singleitem_result(values36)
           << new chk_singleitem_result(values37)
           << new chk_singleitem_result(values38)
           << new chk_singleitem_result(values39)
           << new chk_singleitem_result(values40)
           << new chk_singleitem_result(values41)
           << new chk_singleitem_result(values42)
           << new chk_singleitem_result(values43)
           << new chk_singleitem_result(values44)
           << new chk_singleitem_result(values45)
           << new chk_singleitem_result(values46)
           << new chk_singleitem_result(values47)
           << new chk_singleitem_result(values48)
           << new chk_singleitem_result(values49)
           << new chk_singleitem_result(values50)
           << new chk_singleitem_result(values51)
           << new chk_singleitem_result(values52)
           << new chk_singleitem_result(values53)
           << new chk_singleitem_result(values54)
           << new chk_singleitem_result(values55)
           << new chk_singleitem_result(values56)
           << new chk_singleitem_result(values57)
           << new chk_singleitem_result(values58)
           << new chk_singleitem_result(values59)
           << new chk_singleitem_result(values60)
           << new chk_singleitem_result(values61)
           << new chk_singleitem_result(values62)
           << new chk_singleitem_result(values63)
           << new chk_singleitem_result(values64)
           << new chk_singleitem_result(values65)
           << new chk_singleitem_result(values66)
           << new chk_singleitem_result(values67)
           << new chk_singleitem_result(values68)
           << new chk_singleitem_result(values69)
           << new chk_singleitem_result(values70)
           << new chk_singleitem_result(values71)
           << new chk_singleitem_result(values72)
           << new chk_singleitem_result(values73)
           << new chk_singleitem_result(values74)
           << new chk_singleitem_result(values75)
           << new chk_singleitem_result(values76)
           << new chk_singleitem_result(values77)
           << new chk_singleitem_result(values78)
           << new chk_singleitem_result(values79)
           << new chk_singleitem_result(values80)
           << new chk_singleitem_result(values81)
           << new chk_singleitem_result(values82)
           << new chk_singleitem_result(values83)
           << new chk_singleitem_result(values84)
           << new chk_singleitem_result(values85)
           << new chk_singleitem_result(values86)
           << new chk_singleitem_result(values87)
           << new chk_singleitem_result(values88)
           << new chk_singleitem_result(values89)
           << new chk_singleitem_result(values90)
           << new chk_singleitem_result(values91)
           << new chk_singleitem_result(values92)
           << new chk_singleitem_result(values93)
           << new chk_singleitem_result(values94)
           << new chk_singleitem_result(values95)
           << new chk_singleitem_result(values96)
           << new chk_singleitem_result(values97)
           << new chk_singleitem_result(values98)
           << new chk_singleitem_result(values99)
           << new chk_singleitem_result(values100);
    
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

void Form_page3::on_pushButton_2_clicked()
{
    qDebug()<<"on_pushButton_2_clicked开始执行PDF生成功能";    
    // 获取界面中的标题、备注信息和已有的数据列表
    QString title = ui->label->text();
    QString remarks = ui->textEdit->toPlainText();

    
    // 调用PDFGenerator中的新方法，使用数据列表和表头直接生成PDF
    bool result = PDFGenerator::generateSinglePDFWithDataList(title, datalist, mHeader, remarks);
    
    // 已移除PDF生成结果弹窗提示
    // 如果需要调试信息，可以通过qDebug查看结果
    qDebug() << "PDF生成结果: " << (result ? "成功" : "失败");
    
    qDebug()<<"on_pushButton_2_clicked执行完成";

}

void Form_page3::on_pushButton_3_clicked()
{
    qDebug()<<"on_pushButton_3_clicked";

    // 初始化一个size为3的QList<chk_items_result>
    QList<chk_items_result *> itemsResultList;
    
    // 为每个chk_items_result对象设置实际值
    for(int i = 0; i < 3; i++){
        chk_items_result *itemResult = new chk_items_result();
        
        // 设置标题
        itemResult->setTitle(QString("检验结果集 %1").arg(i+1));
        
        // 创建并设置表头
        QStringList header;
        header << "序号" << "检验项目" << "检验结果" << "备注";
        itemResult->setTableHeader(header);
        
        QList<chk_singleitem_result *> list1 ;
        // 为每个结果集创建2个示例数据项
        for(int j = 0; j < 2; j++){
            // 创建字段值列表 - 确保与header元素数量一致（4个元素）
            QStringList fieldValues;
            // 为每个字段设置实际的初始化值
            fieldValues << QString("%1").arg(j+1)                 // 序号：实际的数字序号
                        << QString("检验项目 %1-%2").arg(i+1).arg(j+1)  // 检验项目：具体的项目名称
                        << (j % 2 == 0 ? "通过" : "未通过")           // 检验结果：通过或未通过的实际状态
                        << QString("示例备注 %1-%2").arg(i+1).arg(j+1);  // 备注：具体的备注信息
            
            // 创建字段名列表 - 与header保持一致
            QStringList fieldNames;
            fieldNames << "序号" << "检验项目" << "检验结果" << "备注";
            
            // 验证字段值数量与表头数量一致
            if(fieldValues.size() != header.size()) {
                qDebug() << QString("警告：结果集 %1 的数据项 %2 字段值数量(%3)与表头数量(%4)不一致！").
                            arg(i+1).arg(j+1).arg(fieldValues.size()).arg(header.size());
            }
            
            // 创建并初始化chk_singleitem_result对象，使用字段名和字段值
            chk_singleitem_result *singleItem = new chk_singleitem_result(fieldValues);

            list1<<singleItem;
        }
        itemResult->setDataList(list1);
        // 设置备注
        itemResult->setRemark(QString("这是第 %1 个检验结果集的备注信息").arg(i+1));
        
        // 添加到列表
        itemsResultList<<itemResult;
    }
    
    qDebug() << "初始化的QList<chk_items_result>大小: " << itemsResultList.size();
    
    chk_items_result *test = itemsResultList.at(1);

    PDFGenerator::generateSinglePDFWithDataList(test->getTitle(),
                                                                test->getDataList(),
                                                                test->getTableHeader(),
                                                                test->getRemark());
    qDebug() << "on_pushButton_3_clicked函数执行完毕";
}
