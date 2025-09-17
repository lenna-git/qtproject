#include "form_page1.h"
#include "ui_form_page1.h"
#include "pdfgenerator.h"
#include<QVBoxLayout>
#include<QPushButton>
#include<QTextEdit>
#include<QGroupBox>
#include<QLabel>
#include <QGraphicsPixmapItem>
#include <QWidget>
#include <QPixmap>
#include<QDebug>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QApplication>
#include<QDateTime>
#include<QDesktopServices>
#include <QTemporaryFile>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QTimer>
#include<QIODevice>

Form_page1::Form_page1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_page1),
    scaleFactor(1.0)
{
    ui->setupUi(this);
    // 设置初始页面大小
//    this->resize(800, 600);
    QVBoxLayout *AllLayout = new QVBoxLayout(this);
//       this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      // 创建图形场景
      scene = new QGraphicsScene(this);
      scene->setBackgroundBrush(Qt::lightGray);

      // 创建图形视图
      view = new QGraphicsView(this);
      view->setScene(scene);
      // 设置视图属性
      view->setRenderHint(QPainter::SmoothPixmapTransform); // 平滑缩放
      view->setResizeAnchor(QGraphicsView::AnchorViewCenter); // 调整大小时以视图中心为锚点
      view->setAlignment(Qt::AlignCenter);
      view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

      // 加载图片并添加到场景
    QPixmap pixmap("D:/code/1.jpg");
    if (!pixmap.isNull()) {
        QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixmap);
        pixmapItem->setTransformationMode(Qt::SmoothTransformation);
        // 设置场景大小为图片大小
        scene->setSceneRect(pixmap.rect());
    }else {
        // 添加错误处理，方便调试
        qDebug() << "Failed to load image: D:\\code\\1.jpg";
    }

    // 重写视图的 resizeEvent 以实现自动缩放
    // 这里使用事件过滤器实现
    view->installEventFilter(this);

    // 添加调试信息，跟踪视图和场景初始化过程
    qDebug() << "Form_page1 constructor: Initial widget size" << this->size();
    qDebug() << "Form_page1 constructor: Initial viewport size" << view->viewport()->rect().size();
    
    // 使用多级延迟回调策略，确保在不同初始化阶段都能正确缩放
    // 第一级：快速初始缩放（200ms）
    QTimer::singleShot(200, this, [this]() {
        this->adjustImageSize();
        qDebug() << "First delayed scaling completed"; 
    });
    
    // 第二级：确保窗口完全显示后的缩放（500ms）
    QTimer::singleShot(500, this, [this]() {
        this->adjustImageSize();
        qDebug() << "Second delayed scaling completed"; 
    });
    
    // 第三级：最终调整（1000ms）
    QTimer::singleShot(1000, this, [this]() {
        this->adjustImageSize();
        qDebug() << "Final delayed scaling completed"; 
    });


      // 添加到布局，设置伸展因子为1
    AllLayout->addWidget(view, 1);
    AllLayout->addWidget(ui->pushButton);
    
    // showEvent方法在类中重写实现

}

Form_page1::~Form_page1()
{
    delete ui;
}

// 统一的图片尺寸调整方法，在多个时机被调用以确保图片正确缩放
void Form_page1::adjustImageSize() {
    if (view && scene && !scene->sceneRect().isEmpty()) {
        // 强制布局更新，确保视口大小准确
        this->updateGeometry();
        view->updateGeometry();
        view->viewport()->updateGeometry();
        
        // 强制重绘以确保视口正确更新
        this->repaint();
        view->repaint();
        view->viewport()->repaint();
        
        // 获取视口大小和场景大小
        QRectF viewportRect = view->viewport()->rect();
        QRectF sceneRect = scene->sceneRect();
        
        // 输出调试信息
        qDebug() << "Adjusting image size - Viewport:" << viewportRect.size() << "Scene:" << sceneRect.size();
        
        // 添加安全检查，确保视口和场景大小有效
        if (viewportRect.width() > 50 && viewportRect.height() > 50 && sceneRect.width() > 0 && sceneRect.height() > 0) {
            // 计算缩放因子，确保图片完全显示在视口内
            // 增加更大的边距（70%），防止图片显示过大
            qreal scaleX = (viewportRect.width() * 0.7) / sceneRect.width();
            qreal scaleY = (viewportRect.height() * 0.7) / sceneRect.height();
            qreal scale = qMin(scaleX, scaleY);
            
            // 添加最大和最小缩放限制
            const qreal minScale = 0.1;
            const qreal maxScale = 1.0;
            scale = qMax(scale, minScale);
            scale = qMin(scale, maxScale);
            
            qDebug() << "Calculated scale factor:" << scale;
            
            // 重置变换并应用计算的缩放因子
            view->resetTransform();
            view->scale(scale, scale);
            
            // 确保图片居中显示
            view->centerOn(sceneRect.center());
            
            // 再次强制重绘以应用更改
            view->repaint();
        }
    }
}

// 重写窗口的resizeEvent方法
void Form_page1::resizeEvent(QResizeEvent *event)
{
    // 调用父类的resizeEvent以确保正常功能
    QWidget::resizeEvent(event);
//    this->resizeEvent(event);

    // 调用统一的图片尺寸调整方法
    this->adjustImageSize();
}

// 重写窗口的showEvent方法
void Form_page1::showEvent(QShowEvent *event)
{
    // 调用父类的showEvent以确保正常功能
    QWidget::showEvent(event);
    
    // 延迟调用图片尺寸调整方法，确保窗口已经完全显示
    QTimer::singleShot(100, this, &Form_page1::adjustImageSize);
}

bool Form_page1::eventFilter(QObject *watched, QEvent *event)
{
    // 只处理view的resize事件
    if (watched == view && event->type() == QEvent::Resize) {
        // 调用统一的图片尺寸调整方法
        this->adjustImageSize();
    }
    return QWidget::eventFilter(watched, event);
}


// 创建一个结构体用于存储可变动的报告内容
typedef struct {
    QString reportTitle;         // 报告标题
    QString algorithmName;       // 算法名称
    QString length;              // 长度
    QString groupLength;         // 分组长度
    QString groupRounds;         // 分组轮数
    QString groupRoundsLevel2;   // 分组轮数二轮
    QString designUnit;          // 设计单位
    QString algorithmCategory;   // 算法类别
    QString testPersonnel;       // 检验人员
    QString testDate;            // 检验时间
    QString testLevel;           // 检验水平
    QStringList projectNames;    // 项目名称列表
    QStringList projectResults;  // 项目结果列表
    QString testConclusion;      // 检验结论
    QString remarks;             // 备注
} ReportContent;

// 创建默认的报告内容
ReportContent getDefaultReportContent() {
    ReportContent content;
    content.reportTitle = "某算法统计检验报告";
    content.algorithmName = "算法一";
    content.length = "32字节";
    content.groupLength = "16字节";
    content.groupRounds = "分组轮数";
    content.groupRoundsLevel2 = "分组轮数二轮";
    content.designUnit = "设计单位二";
    content.algorithmCategory = "算法类型二";
    content.testPersonnel = "设计人员二";
    content.testDate = "2025年9月4日";
    content.testLevel = "一级检验: 0.01; 二级检验通过者: 0.0027; 二级检验一致性: 0.0001";
    content.projectNames << "项目一" << "项目二" << "项目三" << "项目四" << "项目五" << "项目六" << "项目七" << "项目八" << "项目九";
    content.projectResults << "通过" << "通过" << "通过" << "通过" << "通过" << "通过" << "通过" << "通过" << "不通过";
    content.testConclusion = "不通过";
    content.remarks = "备注二";
    return content;
}

// 绘制PDF的主要函数，内容可变
void Form_page1::drawpdf(QString tempFileName)
{
    // 使用默认内容，也可以通过参数传入自定义内容
    ReportContent content = getDefaultReportContent();

    // 创建QPrinter对象并设置为PDF输出模式
    QPrinter printer(QPrinter::ScreenResolution); // 降低分辨率，提高兼容性
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(tempFileName);

    // 设置页面属性 - Qt 5.9.1中的方法
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印

    // Qt 5.9.1中的边距设置方法
    printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

    // 创建QPainter并关联到QPrinter
    QPainter painter;
    if (!painter.begin(&printer))
    {
        qWarning("无法创建PDF内容");
        QMessageBox::critical(this, "错误", "无法创建PDF内容");
        // 删除临时文件
        QFile::remove(tempFileName);
        return;
    }

    // 获取页面的可打印区域（考虑边距）
    QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
    qreal pageWidth = printableRect.width();
    qreal pageHeight = printableRect.height();

    // 设置渲染质量 - 降低一些设置以提高兼容性
    painter.setRenderHint(QPainter::TextAntialiasing, true); // 仅保留文本抗锯齿

    // 设置字体 - 简化字体设置，确保中文正常显示
    QFont font;
    font.setFamily("SimHei"); // 直接使用黑体，避免字体列表解析问题
    painter.setFont(font);

    // 计算内容区域，调整边距和表格大小以提高兼容性
    qreal margin = 30;
    qreal tableLeft = margin;
    qreal tableTop = 60;
    qreal tableWidth = pageWidth - 2 * margin;
    qreal tableHeight = pageHeight - 120; // 让表格占据整个页面
    // 计算合适的行高使表格占满页面
    // 表格约有19个标准行高（包括备注的3行）
    qreal totalRows = 19; // 估算的总行数
    qreal lineHeight = (pageHeight - 120) / totalRows; // 动态计算行高使表格占满页面
    qreal col1Width = 100;
    qreal col2Width = 160;
    qreal col3Width = 100;
    qreal col4Width = 100;

    // 设置表格初始大小（预估）用于背景填充
    qreal estimatedTableHeight = pageHeight - 120; // 预估表格高度
    
    // 先填充表格背景色
    painter.fillRect(tableLeft, tableTop, tableWidth, estimatedTableHeight, Qt::white);

    // 绘制标题
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRectF(tableLeft, tableTop - 40, tableWidth, 30), Qt::AlignCenter, content.reportTitle);
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);

    // 绘制表格顶部边框
    painter.drawLine(tableLeft, tableTop, tableLeft + tableWidth, tableTop);

    // 标准表格布局 - 将算法名称单元格与其上下两个单元格合并为一个单元格
    qreal currentY = tableTop;
    
    // 绘制第一部分 - 合并算法名称和算法一的三行单元格
    // 绘制表格外边框
    painter.drawLine(tableLeft, tableTop, tableLeft, currentY + 3 * lineHeight); // 左侧边框
    painter.drawLine(tableLeft + col1Width, tableTop, tableLeft + col1Width, currentY + 3 * lineHeight); // 第一列右边框
    painter.drawLine(tableLeft + col1Width + col2Width, tableTop, tableLeft + col1Width + col2Width, currentY + 3 * lineHeight); // 第二列右边框（与算法一合并单元格对齐）
    painter.drawLine(tableLeft, currentY + 3 * lineHeight, tableLeft + tableWidth, currentY + 3 * lineHeight); // 底部边框
    
    // 绘制其他列的垂直分隔线，但只在各自的行内显示
    // 第一行的其他列分隔线
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第一行底部横线（从第二列右边框开始，保留其他列的横线）
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第二行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第二行底部横线（从第二列右边框开始，保留其他列的横线）
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第三行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 重置currentY到表格顶部，用于填充内容
    currentY = tableTop;
    
    // 填写算法名称 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, 3 * lineHeight), Qt::AlignCenter, "算法名称"); // 合并单元格文本居中
    
    // 填写算法一 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, 3 * lineHeight), Qt::AlignCenter, content.algorithmName); // 合并单元格文本居中
    
    // 填写第一行其他列内容
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "长度");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.length);
    
    // 填写第二行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "分组长度");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.groupLength);
    
    // 填写第三行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "分组轮数");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.groupRoundsLevel2);
    
    currentY += lineHeight; // 增加到下一行

    // 绘制第二行 - 设计信息
    // 设计信息从第四行开始
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第二行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "设计单位");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter, content.designUnit);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter, "算法类别");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter, content.algorithmCategory);

    // 绘制第三行 - 检验信息
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第三行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "检验人员");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter, content.testPersonnel);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter, "检验时间");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter, content.testDate);

    // 绘制第四行 - 检验水平
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第四行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "检验水平");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight), Qt::AlignCenter, content.testLevel);

    // 绘制检验项目部分
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 9); // 第一列垂直分隔线
    painter.drawLine(tableLeft + col1Width * 2, currentY, tableLeft + col1Width * 2, currentY + lineHeight * 9); // 第二列垂直分隔线
    painter.drawLine(tableLeft + col1Width * 3, currentY, tableLeft + col1Width * 3, currentY + lineHeight * 9); // 第三列垂直分隔线

    // 填写检验项目内容 - 4列布局，第一列合并为一个单元格
    int projectCount = qMin(content.projectNames.size(), content.projectResults.size());
    
    // 先绘制第一列的合并单元格文本
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 9), Qt::AlignCenter | Qt::TextWordWrap, "检验项目");
    
    // 循环绘制9行内容（只删除检验项目列的横线，保留其他三列的横线）
    for (int i = 0; i < 9; i++) {
        currentY += lineHeight;
        // 绘制其他三列的行分隔线（不包括检验项目列）
        painter.drawLine(tableLeft + col1Width, currentY, tableLeft + tableWidth, currentY);
        
        // 为其他列绘制垂直分隔线
        painter.drawLine(tableLeft + col1Width, currentY - lineHeight, tableLeft + col1Width, currentY);
        painter.drawLine(tableLeft + col1Width * 2, currentY - lineHeight, tableLeft + col1Width * 2, currentY);
        painter.drawLine(tableLeft + col1Width * 3, currentY - lineHeight, tableLeft + col1Width * 3, currentY);
        
        // 第二列：序号列 - 分9行，序号1-9
        painter.drawText(QRectF(tableLeft + col1Width, currentY - lineHeight, col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, QString::number(i + 1));
        
        // 第三列：项目名称列
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width * 2, currentY - lineHeight, col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectNames[i]);
        }
        
        // 第四列：是否通过列
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width * 3, currentY - lineHeight, tableWidth - col1Width * 3, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectResults[i]);
        }
    }

    // 绘制检验结论行 - 去掉空行
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写检验结论 - 使用安全的文本格式
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "检验结论");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.testConclusion);

    // 绘制备注行 - 高度调整为3行
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 3);
    painter.drawLine(tableLeft, currentY + lineHeight * 3, tableLeft + tableWidth, currentY + lineHeight * 3);

    // 填写备注 - 支持多行显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 3), Qt::AlignCenter | Qt::TextWordWrap, "备 注");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight * 3), Qt::AlignLeft | Qt::TextWordWrap, content.remarks);

    // 计算从表格顶部到备注底部的准确高度
    qreal tableFinalHeight = currentY + lineHeight * 3 - tableTop;
    
    // 绘制表格外边框，确保备注是表格最后一行
    painter.drawRect(tableLeft, tableTop, tableWidth, tableFinalHeight);

    // 显示附加说明文字 - 直接位于备注下方，不使用表格内坐标，无任何边框
    qreal additionalNoteY = currentY + lineHeight * 3; // 直接基于备注部分的结束位置
    painter.drawText(QRectF(tableLeft, additionalNoteY, tableWidth, lineHeight), Qt::AlignLeft | Qt::TextSingleLine, "附：各检验分项详细结果。");

    // 结束绘制
    painter.end();
    qDebug() << "PDF文件已成功生成";

    // 强制刷新和释放资源
    printer.newPage(); // 确保所有数据都写入文件
}

//绘制并生成pdf
void Form_page1::on_pushButton_clicked()
{
    qDebug()<<"开始生成pdf";
    
    // 调用PDFGenerator的统一方法来处理临时文件、预览和保存逻辑
    PDFGenerator::generateAndManageReportPDF(this);
}
