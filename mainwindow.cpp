#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QWidget>
#include"log4qt/logger.h"
#include"log4qt/log4qt.h"

using Log4Qt::Logger;
extern Logger* myLog;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    formpage1(new Form_page1),
    formpage2(new Form_page2),
    formpage3(new Form_page3)
{
    ui->setupUi(this);
    QHBoxLayout *wholelayout = new QHBoxLayout(ui->centralWidget);

    this->resize(1200,800);


    QWidget *widgetleft = new QWidget;
    QVBoxLayout *leftlayout  = new QVBoxLayout(widgetleft);
    leftlayout->addWidget(ui->pushButton);
    leftlayout->addWidget(ui->pushButton_2);
    leftlayout->addWidget(ui->pushButton_3);

    wholelayout->addWidget(widgetleft);
    wholelayout->addWidget(ui->stackedWidget);

    ui->stackedWidget->addWidget(formpage1);
    ui->stackedWidget->addWidget(formpage2);
    ui->stackedWidget->addWidget(formpage3);
    ui->stackedWidget->setCurrentWidget(formpage1);

    // 连接按钮的点击信号到槽函数
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
}

// 按钮1的点击事件槽函数：显示formpage1
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(formpage1);
}

// 按钮2的点击事件槽函数：显示formpage2
void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(formpage2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(formpage3);
}
