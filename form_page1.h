#ifndef FORM_PAGE1_H
#define FORM_PAGE1_H

#include <QWidget>
#include<QGraphicsView>
#include<QGraphicsScene>

namespace Ui {
class Form_page1;
}

class Form_page1 : public QWidget
{
    Q_OBJECT

public:
    explicit Form_page1(QWidget *parent = 0);
    ~Form_page1();
    QGraphicsView *view;
    QGraphicsScene *scene;

protected:
    // 重写resizeEvent来捕获窗口大小变化
    void resizeEvent(QResizeEvent *event) override;
    // 重写showEvent来捕获窗口显示事件
    void showEvent(QShowEvent *event) override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Form_page1 *ui;
    bool eventFilter(QObject *watched, QEvent *event) override;
    double scaleFactor; // 用于跟踪当前缩放比例
    void drawpdf(QString tempFileName);
    // 统一的图片尺寸调整方法
    void adjustImageSize();
};

#endif // FORM_PAGE1_H
