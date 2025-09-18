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
    AllLayout->addWidget(ui->pushButton_2);
    
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
//绘制并生成pdf
void Form_page1::on_pushButton_clicked()
{
    qDebug()<<"开始生成pdf";
    
    // 初始化ReportContent结构体
    ReportContent content;
    content.reportTitle = "某算法统计检验报告XXXXXXXX";
    content.algorithmName = "算法一";
    content.title1_1 = "标题一";
    content.content1_1 = "内容一";
    content.title1_2 = "标题2";
    content.content1_2 = "内容2";
    content.title1_3 = "标题一3";
    content.content1_3 = "内容3";
    content.designUnit = "设计单位二";
    content.algorithmCategory = "算法类型二";
    content.testPersonnel = "设计人员二";
    content.testDate = "2025年9月4日";
    content.testLevel = "一级检验: 0.01; 二级检验通过者: 0.0027; 二级检验一致性: 0.0001";
    content.projectNames << "项目一" << "项目二" << "项目三" << "项目四" << "项目五" << "项目六" << "项目七" << "项目八" << "项目九"<<"项目十";
    content.projectResults << "通过" << "通过" << "通过" << "通过" << "通过" << "通过" << "通过" << "通过" << "不通过"<< "通过" ;
    content.testConclusion = "不通过";
    content.remarks = "备注二";
    
    // 调用PDFGenerator的方法，传入自定义的ReportContent
    PDFGenerator::generateAndManageReportPDF(content, this);
}

void Form_page1::on_pushButton_2_clicked()
{
    // 初始化ReportContent结构体
    ReportContent content;
    content.reportTitle = "某算法统计检验报告XXXXXXXX";
    content.algorithmName = "算法一";
    content.title1_1 = "标题一";
    content.content1_1 = "内容一";
    content.title1_2 = "标题2";
    content.content1_2 = "内容2";
    content.title1_3 = "标题一3";
    content.content1_3 = "内容3";
    content.designUnit = "设计单位二";
    content.algorithmCategory = "算法类型二";
    content.testPersonnel = "设计人员二";
    content.testDate = "2025年9月4日";
    content.testLevel = "一级检验: 0.01; 二级检验通过者: 0.0027; 二级检验一致性: 0.0001";
    content.projectNames << "项目一" << "项目二" << "项目三" << "项目四" << "项目五" << "项目六"  ;
    content.projectResults << "通过" << "通过" << "通过" << "通过" << "通过" << "通过"  ;
    content.testConclusion = "不通过";
    content.remarks = "备注二";

    // 调用PDFGenerator的方法，传入自定义的ReportContent
    PDFGenerator::generateAndManageReportPDFWith6Projects(content, this);
}
