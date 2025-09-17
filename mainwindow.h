#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"form_page1.h"
#include"form_page2.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Form_page1 *formpage1;
    Form_page2 *formpage2;
};

#endif // MAINWINDOW_H
