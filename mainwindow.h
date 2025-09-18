#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"form_page1.h"
#include"form_page2.h"
#include"form_page3.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // 按钮1的点击事件槽函数
    void on_pushButton_clicked();
    
    // 按钮2的点击事件槽函数
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    Form_page1 *formpage1;
    Form_page2 *formpage2;
    Form_page3 *formpage3;

};

#endif // MAINWINDOW_H
