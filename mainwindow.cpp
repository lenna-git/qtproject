#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    formpage1(new Form_page1),
    formpage2(new Form_page2)
{
    ui->setupUi(this);
    QHBoxLayout *wholelayout = new QHBoxLayout(ui->centralWidget);

    this->resize(1200,800);


    QWidget *widgetleft = new QWidget;
    QVBoxLayout *leftlayout  = new QVBoxLayout(widgetleft);
    leftlayout->addWidget(ui->pushButton);
    leftlayout->addWidget(ui->pushButton_2);

    wholelayout->addWidget(widgetleft);
    wholelayout->addWidget(ui->stackedWidget);

    ui->stackedWidget->addWidget(formpage1);
    ui->stackedWidget->addWidget(formpage2);
    ui->stackedWidget->setCurrentWidget(formpage2);



}

MainWindow::~MainWindow()
{
    delete ui;
}
