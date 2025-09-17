#include "form_page1.h"
#include "ui_form_page1.h"
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
          // 调整视图以适应场景（显示完整图片）
          view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
      }else {
          // 添加错误处理，方便调试
          qDebug() << "Failed to load image: D:\\code\\1.jpg";
      }

      // 重写视图的 resizeEvent 以实现自动缩放
      // 这里使用事件过滤器实现
      view->installEventFilter(this);


      // 添加到布局，设置伸展因子为1
    AllLayout->addWidget(view, 1);
    AllLayout->addWidget(ui->pushButton);

}

Form_page1::~Form_page1()
{
    delete ui;
}

// 重写窗口的resizeEvent方法
void Form_page1::resizeEvent(QResizeEvent *event)
{
    // 调用父类的resizeEvent以确保正常功能
    QWidget::resizeEvent(event);
//    this->resizeEvent(event);

    // 确保在窗口大小变化（包括最大化/还原）时，图片能够自适应缩放
    if (view && scene && !scene->sceneRect().isEmpty()) {
        // 保存当前视图的中心位置
        QPointF center = view->mapToScene(view->viewport()->rect().center());

        // 先重置视图，然后应用缩放因子
        view->resetTransform();
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        view->scale(scaleFactor, scaleFactor);

        // 恢复中心位置
        view->centerOn(center);
    }
}

bool Form_page1::eventFilter(QObject *watched, QEvent *event)
{
    // 只处理view的resize事件
    if (watched == view && event->type() == QEvent::Resize) {
        // 添加安全检查
        if (scene && !scene->sceneRect().isEmpty()) {
            // 保存当前视图的中心位置
            QPointF center = view->mapToScene(view->viewport()->rect().center());

            // 重置变换并应用缩放因子
            view->resetTransform();
            view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
            view->scale(scaleFactor, scaleFactor);

            // 恢复中心位置
            view->centerOn(center);
        }
    }
    return QWidget::eventFilter(watched, event);
}


//绘制并生成pdf
void Form_page1::on_pushButton_clicked()
{
    qDebug()<<"开始生成pdf";


    // 创建临时文件，用于存储PDF内容
   // 直接使用QFile创建临时文件，避免QTemporaryFile自动删除的问题
    QString tempFileName = QDir::temp().absoluteFilePath(QString("qt_temp_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")));

    // 测试是否可以创建文件
    QFile testFile(tempFileName);
    if (!testFile.open(QIODevice::WriteOnly)) {
       qWarning("无法创建临时文件");
       QMessageBox::critical(this, "错误", "无法创建临时PDF文件");
       return;
    }
    testFile.close();

    qDebug() << "PDF临时文件路径：" << tempFileName;
    qDebug() << "临时文件是否存在：" << QFile::exists(tempFileName);
    qDebug() << "临时文件所在目录是否存在：" << QFileInfo(tempFileName).absoluteDir().exists();

    // 创建QPrinter对象并设置为PDF输出模式
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(tempFileName);

    // 设置页面属性 - Qt 5.9.1中的方法
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 全页打印

    // Qt 5.9.1中的边距设置方法
    printer.setPageMargins(20, 20, 20, 20, QPrinter::Millimeter);

    // 创建QPainter并关联到QPrinter
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

    // 设置渲染质量
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 设置字体 - 更全面的字体设置
    QFont font;
    font.setFamily("SimHei, WenQuanYi Micro Hei, Heiti TC"); // 添加备选字体
    font.setPointSize(12);
    painter.setFont(font);

    // 计算内容区域，留出适当的边距
    qreal contentLeft = 20;
    qreal contentTop = 20;
    qreal contentWidth = pageWidth - 40;
    qreal contentHeight = pageHeight - 40;

    // 绘制标题文本
    painter.drawText(contentLeft, contentTop + 20, "Qt 5.9.1 PDF生成示例");
    painter.drawText(contentLeft, contentTop + 45, "这是一个使用Qt 5.9.1创建的PDF文档");

    // 绘制分隔线 - 自适应页面宽度
    painter.drawLine(contentLeft, contentTop + 60, contentLeft + contentWidth, contentTop + 60);

    // 绘制矩形 - 更合理的尺寸
    //          painter.drawRect(contentLeft, contentTop + 70, 100, 50);
    painter.drawRect(60,60,500,500);

    // 结束绘制
    painter.end();
    qDebug() << "PDF文件已成功生成";


    // 强制刷新和释放资源
    printer.newPage(); // 确保所有数据都写入文件
    qApp->processEvents(); // 处理所有待处理的事件

    qDebug() << "PDF内容已生成，准备打开预览";

    // 打开文件进行预览前，添加一个小延迟，确保文件完全写入和释放
    QThread::msleep(300); // 延迟300毫秒
    qApp->processEvents(); // 再次处理事件

    // 检查文件是否存在且可读，而不是直接打开
    qDebug() << "准备打开预览前 - 临时文件路径：" << tempFileName;
    qDebug() << "准备打开预览前 - 临时文件是否存在：" << QFile::exists(tempFileName);

    QFileInfo fileInfo(tempFileName);
    if (fileInfo.exists() && fileInfo.isReadable()) {

       // 自动打开PDF文件进行预览
       QUrl url = QUrl::fromLocalFile(tempFileName);
       if (!QDesktopServices::openUrl(url)) {
           qWarning("无法打开PDF文件进行预览");
           QMessageBox::warning(this, "警告", "无法打开PDF预览，请手动查看临时文件：\n" + tempFileName);
       }
    } else {
       qWarning("文件仍然被锁定，无法打开预览");
       QMessageBox::warning(this, "警告", "PDF文件被锁定，无法打开预览。请稍后重试。");
    }

       // 使用QTimer延迟询问用户是否保存，给预览程序足够的时间打开文件
    QTimer::singleShot(1000, this, [=]() {
       // 显示对话框，询问用户是否保存PDF
       QMessageBox::StandardButton reply;
       reply = QMessageBox::question(nullptr, "保存PDF", "是否保存此PDF文件？",
                                    QMessageBox::Save | QMessageBox::Discard);

       if (reply == QMessageBox::Save) {
           // 用户选择保存，打开文件保存对话框
           QString saveFileName = QFileDialog::getSaveFileName(
                 nullptr, "保存PDF文件", "document.pdf", "PDF文件 (*.pdf)");

           if (!saveFileName.isEmpty()) {
               // 尝试多次复制文件，增加重试次数和延迟以处理文件锁定
               bool copied = false;
               int retryCount = 0;
               const int maxRetries = 5; // 增加重试次数

               while (!copied && retryCount < maxRetries) {
                   int delayMs = 500 + (retryCount * 300); // 递增的延迟时间
                   QThread::msleep(delayMs); // 每次重试前的延迟递增
                   qApp->processEvents(); // 处理事件，确保文件系统操作正常

                   qDebug() << "尝试复制文件（" << retryCount + 1 << "/" << maxRetries << ")，延迟：" << delayMs << "ms";
                   qDebug() << "源文件：" << tempFileName;
                   qDebug() << "目标文件：" << saveFileName;
                   qDebug() << "源文件是否存在：" << QFile::exists(tempFileName);
                   qDebug() << "目标文件是否存在：" << QFile::exists(saveFileName);

                   // 如果目标文件存在，先尝试删除它
                   if (QFile::exists(saveFileName)) {
                       qDebug() << "目标文件已存在，尝试删除";
                       if (!QFile::remove(saveFileName)) {
                           qWarning() << "无法删除现有文件，可能被其他程序占用";
                           retryCount++;
                           continue;
                       }
                   }

                   if (QFile::copy(tempFileName, saveFileName)) {
                       copied = true;
                       qDebug() << "PDF文件已保存到：" << saveFileName;
                       QMessageBox::information(nullptr, "成功", "PDF文件已保存\n" + saveFileName);
                   } else {
                       retryCount++;
                       qWarning() << "无法保存PDF文件到指定位置，正在重试（" << retryCount << "/" << maxRetries << ")";
                   }
               }

               if (!copied) {
                   qWarning("多次尝试后仍然无法保存PDF文件");
                   // 提供一个替代方案
                   QString message = QString("无法保存PDF文件。可能文件正被其他程序占用。\n\n")
                                   + "临时文件位置：" + tempFileName + "\n\n"
                                   + "请手动打开此文件夹并复制该文件。\n\n"
                                   + "是否现在打开临时文件夹？";

                   if (QMessageBox::Yes == QMessageBox::question(nullptr, "保存失败", message, QMessageBox::Yes | QMessageBox::No)) {
                       QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(tempFileName).absolutePath()));
                   }
               }
           }
       }

       // 尝试删除临时文件，但不强制
       QTimer::singleShot(2000, [=]() {
           if (QFile::exists(tempFileName)) {
               if (QFile::remove(tempFileName)) {
                   qDebug() << "已删除临时PDF文件";
               } else {
                   qDebug() << "无法删除临时PDF文件，可能被其他程序占用";
                   // 不显示错误消息，因为这是正常现象
               }
           }
       });
    });
}
