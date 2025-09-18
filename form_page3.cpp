#include"form_page3.h"
#include"ui_form_page3.h"
#include<QList>
#include<QVBoxLayout>


Form_page3::Form_page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_page3)


{
    ui->setupUi(this);
    QVBoxLayout *Layout = new QVBoxLayout(this);


    ui->label_chkresult_name->setAlignment(Qt::AlignCenter);
    ui->pushButton_gen_all->setVisible(false);
    ui->pushButton_print_all->setVisible(false);

    ui->textEdit_filepath->setFixedHeight(20);

    mTableViewModel = new QStandardItemModel(this);
    mTableViewModel->setColumnCount(7);
    QStringList mHeader;
    mHeader<< "序号"<<"检验项目"<<"准则"<<"检验次数"<<"通过率检验"<<"一致性检验"<<"子项判定结果";
    mTableViewModel->setHorizontalHeaderLabels(mHeader);

}

Form_page3::~Form_page3()
{
    delete ui;
}


//响应点击“检验结果”按钮，初始化右边的结果界面
void Form_page3::initchkresult(int Type,QString filepath)
{
     ui->textEdit_filepath->setText(filepath);
//     switch (Type) {
//     case RANDOM_TEST:
//         ui->label_chkresult_name->setText("随机数检验结果");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
//         paintTable_randomtest();
//         break;
//     case STREAM_CIPHER_CHK1:
//         ui->label_chkresult_name->setText("1 序列连续乱数检验结果");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_randomtest();
//         break;
//     case STREAM_CIPHER_CHK2:
//         ui->label_chkresult_name->setText("2 函数检验结果");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_randomtest();
//         break;
//     case STREAM_CIPHER_CHK3:
//         ui->label_chkresult_name->setText("3 密钥初始化过程随机性检验结果");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_randomtest();
//         break;
//     case STREAM_CIPHER_CHK4:
//         ui->label_chkresult_name->setText("4 相关密钥检验结果");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_randomtest();
//         break;
//     case STREAM_CIPHER_CHK5:
//         ui->label_chkresult_name->setText("5 内部状态熵检验结果");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_randomtest();
//         break;
//     case STREAM_CIPHER_CHK6:
//         ui->label_chkresult_name->setText("6 少相符合检验结果");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_randomtest();
//         break;
//     case STREAM_CIPHER_ALL:
//         ui->label_chkresult_name->setText("序列检验总表");
//         ui->pushButton_chkresult_print->setVisible(false);
//         ui->pushButton_gen_all->setVisible(true);
//         ui->pushButton_print_all->setVisible(true);
//         paintTable_streamcipher_all();
//         break;
//     case BLOCK_CIPHER_ALL:
//         ui->label_chkresult_name->setText("分组检验总表");
//         ui->pushButton_chkresult_print->setVisible(false);
//         ui->pushButton_gen_all->setVisible(true);
//         ui->pushButton_print_all->setVisible(true);
//         paintTable_blockcipher_all();
//         break;
//     case BLOCK_CIPHER_CHK1:
//         ui->label_chkresult_name->setText("1 密文随机性检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_streamcipher_all();
//         break;
//     case BLOCK_CIPHER_CHK2:
//         ui->label_chkresult_name->setText("2 明文和密文独立性检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_streamcipher_all();
//          break;
//     case BLOCK_CIPHER_CHK3:
//         ui->label_chkresult_name->setText("3 低密度明文检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_streamcipher_all();
//         break;
//     case BLOCK_CIPHER_CHK4:
//         ui->label_chkresult_name->setText("4 高密度明文检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_streamcipher_all();
//         break;
//     case BLOCK_CIPHER_CHK5:
//         ui->label_chkresult_name->setText("5 低密度密钥检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_streamcipher_all();
//         break;
//     case BLOCK_CIPHER_CHK6:
//         ui->label_chkresult_name->setText("6 高密度密钥检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_streamcipher_all();
//         break;
//     case BLOCK_CIPHER_CHK7:
//         ui->label_chkresult_name->setText("7 数学特性检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
////         paintTable_streamcipher_all();
//         break;
//     case BLOCK_CIPHER_CHK8:
//         ui->label_chkresult_name->setText("8 雪崩效应检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
//         break;
//     case BLOCK_CIPHER_CHK9:
//         ui->label_chkresult_name->setText("9 密钥编排算法检验");
//         ui->pushButton_chkresult_print->setVisible(true);
//         ui->pushButton_gen_all->setVisible(false);
//         ui->pushButton_print_all->setVisible(false);
//         break;
//     default:
//         break;
//     }
}

//绘制右边的检验结果表格--随机数检验
void Form_page3::paintTable_randomtest()
{
    //填充数据
    QStandardItem *item0 = new QStandardItem;
    item0->setData("1",Qt::DisplayRole);
    QStandardItem *item1 = new QStandardItem;
    item1->setData("zhunze1",Qt::DisplayRole);

    QStandardItem *item3 = new QStandardItem;
    item3->setData("xiangmu2",Qt::DisplayRole);
    QStandardItem *item4 = new QStandardItem;
    item4->setData("zhunze2",Qt::DisplayRole);

    mTableViewModel->setItem(0,0,item0);
    mTableViewModel->setItem(0,1,item1);

    mTableViewModel->setItem(1,0,item3);
    mTableViewModel->setItem(1,1,item4);


    //将数据模型填到tabelview里
    ui->tableView_chkresult->setModel(mTableViewModel);

    //合并第一行第一列、第二行第一列到一行
    ui->tableView_chkresult->setSpan(0,0,2,1);
    ui->tableView_chkresult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//绘制序列总表
void Form_page3::paintTable_streamcipher_all()
{
    mTableViewModel->setColumnCount(3);
    QStringList mHeader;
    mHeader<< "检验项目"<<"检验结果"<<"算法检验结果";
    mTableViewModel->setHorizontalHeaderLabels(mHeader);


    QList<stream_result_all_data *> datalist;
    datalist<<new stream_result_all_data("序列连续乱数检验","未完全校验","未完全校验")
            <<new stream_result_all_data("函数检验","未校验","未完全校验")
            <<new stream_result_all_data("密钥初始化过程随机性检验结果","未校验","未完全校验")
            <<new stream_result_all_data("相关密钥检验结果","未校验","未完全校验")
            <<new stream_result_all_data("内部状态熵检验结果","未校验","未完全校验")
            <<new stream_result_all_data("少项符合检验结果","未校验","未完全校验");

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

    ui->tableView_chkresult->setModel(mTableViewModel);

    //合并第3列到一行
    ui->tableView_chkresult->setSpan(0,2,6,1);


    ui->tableView_chkresult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);






}


//绘制序列总表
void Form_page3::paintTable_blockcipher_all()
{
    mTableViewModel->setColumnCount(3);
    QStringList mHeader;
    mHeader<< "检验项目"<<"检验结果"<<"算法检验结果";
    mTableViewModel->setHorizontalHeaderLabels(mHeader);


    QList<stream_result_all_data *> datalist;
    datalist<<new stream_result_all_data("密文随机性检验","通过","未完全校验")
            <<new stream_result_all_data("明文和密文独立性检验","未校验","未完全校验")
            <<new stream_result_all_data("低密度明文检验","未校验","未完全校验")
            <<new stream_result_all_data("高密度明文检验","未校验","未完全校验")
            <<new stream_result_all_data("低密度密钥检验","未校验","未完全校验")
            <<new stream_result_all_data("高密度密钥检验","未校验","未完全校验")
            <<new stream_result_all_data("数学特性检验","未校验","未完全校验")
            <<new stream_result_all_data("雪崩效应检验","未校验","未完全校验")
            <<new stream_result_all_data("密钥编排算法检验","未校验","未完全校验");

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

    ui->tableView_chkresult->setModel(mTableViewModel);

    //合并第3列到一行
    ui->tableView_chkresult->setSpan(0,2,9,1);


    ui->tableView_chkresult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);






}

void Form_page3::on_pushButton_gen_all_clicked()
{
  emit swichrightpage(STREAM_CIPHER_ALL);
}
