#ifndef FORM_PAGE2_H
#define FORM_PAGE2_H

#include <QWidget>
#include <QResizeEvent>

namespace Ui {
class Form_page2;
}

class Form_page2 : public QWidget
{
    Q_OBJECT

public:
    explicit Form_page2(QWidget *parent = 0);
    ~Form_page2();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Form_page2 *ui;
    QWidget *tableContainer; // 表格容器
    void drawTable(); // 绘制表格的函数
};

#endif // FORM_PAGE2_H
