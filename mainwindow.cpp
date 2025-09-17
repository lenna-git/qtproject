#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    formpage1(new Form_page1)
{
    ui->setupUi(this);
    QHBoxLayout *wholelayout = new QHBoxLayout(ui->centralWidget);



    QWidget *widgetleft = new QWidget;
    QVBoxLayout *leftlayout  = new QVBoxLayout(widgetleft);
    leftlayout->addWidget(ui->pushButton);
    leftlayout->addWidget(ui->pushButton_2);

    wholelayout->addWidget(widgetleft);
    wholelayout->addWidget(ui->stackedWidget);

    ui->stackedWidget->addWidget(formpage1);
    ui->stackedWidget->setCurrentWidget(formpage1);



}

MainWindow::~MainWindow()
{
    delete ui;
}
