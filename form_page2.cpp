#include "form_page2.h"
#include "ui_form_page2.h"
#include "pdfgenerator/pdfgenerator.h"
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QDebug>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include"log4qt/logger.h"
#include"log4qt/log4qt.h"

using Log4Qt::Logger;
extern Logger* myLog;

Form_page2::Form_page2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_page2)
{
    ui->setupUi(this);
    // 设置初始页面大小
    this->resize(800, 600);
    
    // 设置布局管理器，实现pushbutton在最下方，表格在上方
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 创建表格视图容器
    tableContainer = new QWidget(this);
    tableContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // 添加到布局，设置伸展因子
    mainLayout->addWidget(tableContainer, 1);
    mainLayout->addWidget(ui->pushButton);
    
    // 绘制初始表格
    drawTable();
}

Form_page2::~Form_page2()
{
    delete ui;
}

// 绘制表格的函数
void Form_page2::drawTable()
{
    if (!tableContainer) return;
    
    // 创建一个QPixmap作为绘图设备
    QPixmap pixmap(tableContainer->size());
    pixmap.fill(Qt::white);
    
    // 创建QPainter并设置绘图属性
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // 设置画笔
    QPen pen(Qt::black, 1);
    painter.setPen(pen);
    
    // 计算表格尺寸和位置
    qreal margin = 20; // 边距
    qreal tableWidth = tableContainer->width() - 2 * margin;
    qreal tableHeight = tableContainer->height() - 2 * margin;
    qreal tableLeft = margin;
    qreal tableTop = margin;
    
    // 确保表格有足够的空间
    if (tableWidth <= 0 || tableHeight <= 0) return;
    
    // 计算每个单元格的宽度和高度
    int rows = 4;
    int cols = 7;
    qreal cellWidth = tableWidth / cols;
    qreal cellHeight = tableHeight / rows;
    
    // 绘制4行7列的表格
    // 绘制横线
    for (int i = 0; i <= rows; i++) {
        qreal y = tableTop + i * cellHeight;
        // 跳过2.1和3.1单元格之间的横线
        if (i == 2) {
            // 只绘制第一列之外的横线
            painter.drawLine(tableLeft + cellWidth, y, tableLeft + tableWidth, y);
        } else {
            painter.drawLine(tableLeft, y, tableLeft + tableWidth, y);
        }
    }
    
    // 绘制竖线（完整显示所有竖线）
    for (int j = 0; j <= cols; j++) {
        qreal x = tableLeft + j * cellWidth;
        painter.drawLine(x, tableTop, x, tableTop + tableHeight);
    }
    
    // 绘制表格内容
    painter.setFont(QFont("Arial", 10));
    // 第一行为标题行，显示标题一到标题七
    painter.setFont(QFont("Arial", 10, QFont::Bold)); // 标题行使用粗体
    for (int j = 0; j < cols; j++) {
        QRectF cellRect(tableLeft + j * cellWidth, tableTop, cellWidth, cellHeight);
        QString titleText = QString("标题%1").arg(j+1);
        painter.drawText(cellRect, Qt::AlignCenter, titleText);
    }
    
    // 其他行显示数据
    painter.setFont(QFont("Arial", 10)); // 恢复普通字体
    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // 处理合并单元格
            if (i == 1 && j == 0) {
                // 合并2.1和3.1单元格
                QRectF mergedCellRect(tableLeft, tableTop + cellHeight, cellWidth, 2 * cellHeight);
                QString mergedText = "合并单元格\n(2.1+3.1)";
                painter.drawText(mergedCellRect, Qt::AlignCenter | Qt::TextWordWrap, mergedText);
                // 跳过3.1单元格的绘制
                continue;
            }
            // 跳过已合并的3.1单元格
            if (i == 2 && j == 0) {
                continue;
            }
            QRectF cellRect(tableLeft + j * cellWidth, tableTop + i * cellHeight, cellWidth, cellHeight);
            QString cellText = QString("%1,%2").arg(i+1).arg(j+1);
            painter.drawText(cellRect, Qt::AlignCenter, cellText);
        }
    }
    
    // 将绘制结果显示在窗口上
    QPalette palette = tableContainer->palette();
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    tableContainer->setPalette(palette);
    tableContainer->setAutoFillBackground(true);
    
    qDebug() << "表格绘制完成：4行7列";
}

// 重写窗口的resizeEvent方法，实现表格自适应页面大小
void Form_page2::resizeEvent(QResizeEvent *event)
{
    // 调用父类的resizeEvent以确保正常功能
    QWidget::resizeEvent(event);
    
    // 重新绘制表格以适应新的窗口大小
    drawTable();
}

#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

//将表格内容画成一样的pdf
void Form_page2::on_pushButton_clicked()
{
    qDebug() << "按钮被点击，开始生成PDF";
    
    // 调用PDFGenerator的统一方法来处理临时文件、预览和保存逻辑
    PDFGenerator::generateAndManageTablePDF(this);
}
