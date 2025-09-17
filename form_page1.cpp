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
    
    // 绘制第一部分 - 合并算法名称的三行单元格
    // 绘制表格外边框
    painter.drawLine(tableLeft, tableTop, tableLeft, currentY + 3 * lineHeight); // 左侧边框
    painter.drawLine(tableLeft + col1Width, tableTop, tableLeft + col1Width, currentY + 3 * lineHeight); // 第一列右边框
    painter.drawLine(tableLeft, currentY + 3 * lineHeight, tableLeft + tableWidth, currentY + 3 * lineHeight); // 底部边框
    
    // 绘制其他列的垂直分隔线，但只在各自的行内显示
    // 第一行的其他列分隔线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第一行底部横线（从第一列右边框开始，保留其他列的横线）
    painter.drawLine(tableLeft + col1Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第二行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第二行底部横线（从第一列右边框开始，保留其他列的横线）
    painter.drawLine(tableLeft + col1Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第三行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 重置currentY到表格顶部，用于填充内容
    currentY = tableTop;
    
    // 填写算法名称 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, 3 * lineHeight), Qt::AlignCenter, "算法名称"); // 合并单元格文本居中
    
    // 填写第一行其他列内容
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.algorithmName);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "长度");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.length);
    
    // 填写第二行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "分组长度");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.groupLength);
    
    // 填写第三行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "");
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

    // 绘制检验项目部分 - 无标题行，第一列合并为一个单元格
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 9); // 第一列垂直分隔线
    painter.drawLine(tableLeft + col1Width * 2, currentY, tableLeft + col1Width * 2, currentY + lineHeight * 9); // 第二列垂直分隔线
    painter.drawLine(tableLeft + col1Width * 3, currentY, tableLeft + col1Width * 3, currentY + lineHeight * 9); // 第三列垂直分隔线

    // 填写检验项目内容 - 4列布局，第一列合并为一个单元格
    int projectCount = qMin(content.projectNames.size(), content.projectResults.size());
    
    // 先绘制第一列的合并单元格文本
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 9), Qt::AlignCenter | Qt::TextWordWrap, "检验项目");
    
    // 循环绘制9行内容（除第一列外）
    for (int i = 0; i < 9; i++) {
        currentY += lineHeight;
        painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY); // 行分隔线
        
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

    //绘制pdf
    drawpdf(tempFileName);


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
