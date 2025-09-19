#include "pdfgenerator.h"
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QTimer>
#include <QApplication>
#include <QPixmap>
#include <QTableView>
#include <QTextEdit>
#include <QLabel>
#include <QFontDatabase>

PDFGenerator::PDFGenerator(QObject *parent) : QObject(parent)
{
}




// 绘制只有6个检验项目的PDF报告
bool PDFGenerator::generateReportPDFWith6Projects(const QString &fileName, const ReportContent &content, QWidget *parent)
{

    // 创建QPrinter对象并设置为PDF输出模式
    QPrinter printer(QPrinter::ScreenResolution); // 降低分辨率，提高兼容性
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // 设置页面属性
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印

    // 设置边距
    printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

    // 创建QPainter并关联到QPrinter
    QPainter painter;
    if (!painter.begin(&printer))
    {
        qWarning("无法创建PDF内容");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建PDF内容");
        }
        return false;
    }

    // 获取页面的可打印区域（考虑边距）
    QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
    qreal pageWidth = printableRect.width();
    qreal pageHeight = printableRect.height();

    // 设置渲染质量
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    // 设置字体
    QFont font;
    // 尝试多种中文字体以确保兼容性
    QStringList chineseFonts = {"SimHei", "WenQuanYi Micro Hei", "Heiti TC", "Arial Unicode MS"};
    QString selectedFont;
    
    // 检查系统中可用的中文字体
    QFontDatabase fontDatabase;
    foreach (const QString &fontFamily, chineseFonts) {
        if (fontDatabase.hasFamily(fontFamily)) {
            selectedFont = fontFamily;
            break;
        }
    }
    
    // 如果没有找到指定的中文字体，使用默认字体
    if (selectedFont.isEmpty()) {
        selectedFont = "SimHei";
        qDebug() << "没有找到指定的中文字体，使用默认字体";
    }
    
    font.setFamily(selectedFont);
    qDebug() << "选择的字体: " << selectedFont;
    painter.setFont(font);

    // 计算内容区域
    qreal margin = 10; // 进一步减小边距，最大化表格可用宽度
    qreal tableLeft = margin;
    qreal tableTop = 50; // 降低表格顶部位置，为表格提供更多垂直空间
    qreal tableWidth = pageWidth - 2 * margin;
    qreal tableHeight = pageHeight - 100; // 增加表格高度，使其占满整个页面
    qreal totalRows = 14; // 减少总行数的估算，增加行高
    qreal lineHeight = tableHeight / totalRows; // 重新计算行高，确保表格占满页面
    qDebug() << "页面尺寸(6项目): " << pageWidth << "x" << pageHeight;
    qDebug() << "表格尺寸(6项目): " << tableWidth << "x" << tableHeight;
    qDebug() << "行高(6项目): " << lineHeight;
    qreal col1Width = 150; // 增加第一列宽度，使其能够显示4个中文字符
    qreal col2Width = 170; // 调整第二列宽度，保持表格整体平衡
    qreal col3Width = 130; // 增加第三列宽度，使其能够显示"分组轮数"4个中文字符
    qreal col4Width = 130; // 增加第四列宽度，确保"分组轮数二"单元格中的"分"字能够完整显示

    // 先填充表格背景色
    painter.fillRect(tableLeft, tableTop, tableWidth, tableHeight, Qt::white);

    // 绘制标题
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRectF(tableLeft, tableTop - 50, tableWidth, 50), Qt::AlignCenter, content.reportTitle); // 增加标题高度从30到50，解决标题文字显示不全问题
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
    painter.drawLine(tableLeft + col1Width + col2Width, tableTop, tableLeft + col1Width + col2Width, currentY + 3 * lineHeight); // 第二列右边框
    painter.drawLine(tableLeft, currentY + 3 * lineHeight, tableLeft + tableWidth, currentY + 3 * lineHeight); // 底部边框
    
    // 绘制其他列的垂直分隔线，但只在各自的行内显示
    // 第一行的其他列分隔线
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第一行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第二行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第二行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第三行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 重置currentY到表格顶部，用于填充内容
    currentY = tableTop;
    
    // 填写算法名称 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, 3 * lineHeight), Qt::AlignCenter, "算法名称");
    
    // 填写算法一 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, 3 * lineHeight), Qt::AlignCenter, content.algorithmName);
    
    // 填写第一行其他列内容
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_1);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_1);
    
    // 填写第二行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_2);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_2);
    
    // 填写第三行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_3);
    // 增加内边距并调整对齐方式，确保'分组轮数二'单元格中的文字完整显示
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width + 5, currentY, col4Width - 10, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_3);
    
    currentY += lineHeight; // 增加到下一行

    // 绘制第二行 - 设计信息
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

    // 绘制检验项目部分（修改为6个项目）
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 6); // 第一列垂直分隔线（已增加到150宽度）
    painter.drawLine(tableLeft + col1Width + 80, currentY, tableLeft + col1Width + 80, currentY + lineHeight * 6); // 序号列垂直分隔线（固定宽度80）
    painter.drawLine(tableLeft + col1Width + 80 + 150, currentY, tableLeft + col1Width + 80 + 150, currentY + lineHeight * 6); // 项目名称列垂直分隔线（调整宽度到150）

    // 填写检验项目内容
    int projectCount = qMin(content.projectNames.size(), content.projectResults.size());
    projectCount = qMin(projectCount, 6); // 限制最多显示6个项目
    
    // 先绘制第一列的合并单元格文本 - 现在可以显示4个中文字符
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 6), Qt::AlignCenter | Qt::TextWordWrap, "检验项目");
    
    // 循环绘制6行内容
    for (int i = 0; i < 6; i++) {
        currentY += lineHeight;
        // 绘制其他三列的行分隔线（不包括检验项目列）
        painter.drawLine(tableLeft + col1Width, currentY, tableLeft + tableWidth, currentY);
        
        // 为其他列绘制垂直分隔线
        painter.drawLine(tableLeft + col1Width, currentY - lineHeight, tableLeft + col1Width, currentY);
        painter.drawLine(tableLeft + col1Width + 80, currentY - lineHeight, tableLeft + col1Width + 80, currentY);
        painter.drawLine(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableLeft + col1Width + 80 + 150, currentY);
        
        // 第二列：序号列 - 分6行，序号1-6（固定宽度80）
        painter.drawText(QRectF(tableLeft + col1Width, currentY - lineHeight, 80, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, QString::number(i + 1));
        
        // 第三列：项目名称列（调整宽度到150，保持表格整体平衡）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80, currentY - lineHeight, 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectNames[i]);
        }
        
        // 第四列：是否通过列（剩余宽度）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableWidth - col1Width - 80 - 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectResults[i]);
        }
    }

    // 绘制检验结论行
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写检验结论
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
    
    // 绘制表格外边框
    painter.drawRect(tableLeft, tableTop, tableWidth, tableFinalHeight);

    // 显示附加说明文字
    qreal additionalNoteY = currentY + lineHeight * 3; // 直接基于备注部分的结束位置
    painter.drawText(QRectF(tableLeft, additionalNoteY, tableWidth, lineHeight), Qt::AlignLeft | Qt::TextSingleLine, "附：各检验分项详细结果。");

    // 结束绘制
    painter.end();
    qDebug() << "PDF文件已成功生成（6个检验项目版本）";

    // 强制刷新和释放资源
    printer.newPage(); // 确保所有数据都写入文件
    
    return true;
}

// 为Form_page1绘制DF报告（接受自定义内容）
bool PDFGenerator::generateReportPDF(const QString &fileName, const ReportContent &content, QWidget *parent)
{

    // 创建QPrinter对象并设置为PDF输出模式
    QPrinter printer(QPrinter::ScreenResolution); // 降低分辨率，提高兼容性
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // 设置页面属性
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印

    // 设置边距
    printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

    // 创建QPainter并关联到QPrinter
    QPainter painter;
    if (!painter.begin(&printer))
    {
        qWarning("无法创建PDF内容");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建PDF内容");
        }
        return false;
    }

    // 获取页面的可打印区域（考虑边距）
    QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
    qreal pageWidth = printableRect.width();
    qreal pageHeight = printableRect.height();

    // 设置渲染质量
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    // 设置字体
    QFont font;
    // 尝试多种中文字体以确保兼容性
    QStringList chineseFonts = {"SimHei", "WenQuanYi Micro Hei", "Heiti TC", "Arial Unicode MS"};
    QString selectedFont;
    
    // 检查系统中可用的中文字体
    QFontDatabase fontDatabase;
    foreach (const QString &fontFamily, chineseFonts) {
        if (fontDatabase.hasFamily(fontFamily)) {
            selectedFont = fontFamily;
            break;
        }
    }
    
    // 如果没有找到指定的中文字体，使用默认字体
    if (selectedFont.isEmpty()) {
        selectedFont = "SimHei";
        qDebug() << "没有找到指定的中文字体，使用默认字体";
    }
    
    font.setFamily(selectedFont);
    qDebug() << "选择的字体: " << selectedFont;
    painter.setFont(font);

    // 计算内容区域
    qreal margin = 15; // 减小边距，增加标题和表格的可用宽度
    qreal tableLeft = margin;
    qreal tableTop = 80; // 增加表格顶部位置，为标题提供更多空间
    qreal tableWidth = pageWidth - 2 * margin;
    qreal tableHeight = pageHeight - 140; // 相应调整表格高度
    qreal totalRows = 16; // 减少总行数的估算，增加行高
    qreal lineHeight = tableHeight / totalRows; // 使用新的表格高度计算行高，确保与修改的尺寸一致
    qDebug() << "页面尺寸: " << pageWidth << "x" << pageHeight;
    qDebug() << "表格尺寸: " << tableWidth << "x" << tableHeight;
    qDebug() << "行高: " << lineHeight;
    qreal col1Width = 150; // 增加第一列宽度，使其能够显示4个中文字符
    qreal col2Width = 170; // 调整第二列宽度，保持表格整体平衡
    qreal col3Width = 130; // 增加第三列宽度，使其能够显示"分组轮数"4个中文字符
    qreal col4Width = 130; // 增加第四列宽度，确保"分组轮数二"单元格中的"分"字能够完整显示

    // 先填充表格背景色
    painter.fillRect(tableLeft, tableTop, tableWidth, tableHeight, Qt::white);

    // 绘制标题
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRectF(tableLeft, tableTop - 50, tableWidth, 50), Qt::AlignCenter, content.reportTitle); // 增加标题高度从30到50，解决标题文字显示不全问题
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
    painter.drawLine(tableLeft + col1Width + col2Width, tableTop, tableLeft + col1Width + col2Width, currentY + 3 * lineHeight); // 第二列右边框
    painter.drawLine(tableLeft, currentY + 3 * lineHeight, tableLeft + tableWidth, currentY + 3 * lineHeight); // 底部边框
    
    // 绘制其他列的垂直分隔线，但只在各自的行内显示
    // 第一行的其他列分隔线
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第一行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第二行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第二行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第三行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 重置currentY到表格顶部，用于填充内容
    currentY = tableTop;
    
    // 填写算法名称 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, 3 * lineHeight), Qt::AlignCenter, "算法名称");
    
    // 填写算法一 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, 3 * lineHeight), Qt::AlignCenter, content.algorithmName);
    
    // 填写第一行其他列内容
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_1);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_1);
    
    // 填写第二行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_2);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_2);
    
    // 填写第三行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_3);
    // 增加内边距并调整对齐方式，确保'分组轮数二'单元格中的文字完整显示
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width + 5, currentY, col4Width - 10, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_3);
    
    currentY += lineHeight; // 增加到下一行

    // 绘制第二行 - 设计信息
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
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 9); // 第一列垂直分隔线（已增加到150宽度）
    painter.drawLine(tableLeft + col1Width + 80, currentY, tableLeft + col1Width + 80, currentY + lineHeight * 9); // 序号列垂直分隔线（固定宽度80）
    painter.drawLine(tableLeft + col1Width + 80 + 150, currentY, tableLeft + col1Width + 80 + 150, currentY + lineHeight * 9); // 项目名称列垂直分隔线（调整宽度到150）

    // 填写检验项目内容
    int projectCount = qMin(content.projectNames.size(), content.projectResults.size());
    
    // 先绘制第一列的合并单元格文本 - 现在可以显示4个中文字符
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 9), Qt::AlignCenter | Qt::TextWordWrap, "检验项目");
    
    // 循环绘制9行内容
    for (int i = 0; i < 9; i++) {
        currentY += lineHeight;
        // 绘制其他三列的行分隔线（不包括检验项目列）
        painter.drawLine(tableLeft + col1Width, currentY, tableLeft + tableWidth, currentY);
        
        // 为其他列绘制垂直分隔线
        painter.drawLine(tableLeft + col1Width, currentY - lineHeight, tableLeft + col1Width, currentY);
        painter.drawLine(tableLeft + col1Width + 80, currentY - lineHeight, tableLeft + col1Width + 80, currentY);
        painter.drawLine(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableLeft + col1Width + 80 + 150, currentY);
        
        // 第二列：序号列 - 分9行，序号1-9（固定宽度80）
        painter.drawText(QRectF(tableLeft + col1Width, currentY - lineHeight, 80, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, QString::number(i + 1));
        
        // 第三列：项目名称列（调整宽度到150，保持表格整体平衡）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80, currentY - lineHeight, 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectNames[i]);
        }
        
        // 第四列：是否通过列（剩余宽度）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableWidth - col1Width - 80 - 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectResults[i]);
        }
    }

    // 绘制检验结论行
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写检验结论
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
    
    // 绘制表格外边框
    painter.drawRect(tableLeft, tableTop, tableWidth, tableFinalHeight);

    // 显示附加说明文字
    qreal additionalNoteY = currentY + lineHeight * 3; // 直接基于备注部分的结束位置
    painter.drawText(QRectF(tableLeft, additionalNoteY, tableWidth, lineHeight), Qt::AlignLeft | Qt::TextSingleLine, "附：各检验分项详细结果。");

    // 结束绘制
    painter.end();
    qDebug() << "PDF文件已成功生成";

    // 强制刷新和释放资源
    printer.newPage(); // 确保所有数据都写入文件
    
    return true;
}

// 为Form_page2生成表格PDF
bool PDFGenerator::generateTablePDF(const QString &fileName, QWidget *parent)
{
    // 创建打印机对象
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // 创建QPainter用于PDF绘制
    QPainter painter;
    if (!painter.begin(&printer)) {
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建PDF文件");
        }
        return false;
    }

    // 设置绘图属性
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置画笔
    QPen pen(Qt::black, 1);
    painter.setPen(pen);

    // 获取PDF页面的尺寸
    QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);

    // 计算表格尺寸和位置（保留边距）
    qreal margin = 50; // 边距
    qreal tableWidth = pageRect.width() - 2 * margin;
    qreal tableHeight = pageRect.height() - 2 * margin;
    qreal tableLeft = margin;
    qreal tableTop = margin;

    // 确保表格有足够的空间
    if (tableWidth <= 0 || tableHeight <= 0) {
        painter.end();
        return false;
    }

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

    // 完成绘制
    painter.end();
    return true;
}

// 为Form_page3生成PDF报告
bool PDFGenerator::generateFormPage3PDF(const QString &fileName, QWidget *parent)
{
    // 确保传入的父窗口不为空且是Form_page3类型
    if (!parent) {
        qWarning("父窗口为空，无法获取Form_page3的数据");
        return false;
    }

    // 创建QPrinter对象并配置为PDF输出
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // 设置页面属性
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印

    // 设置边距
    printer.setPageMargins(20, 20, 20, 20, QPrinter::Millimeter);

    // 创建QPainter对象用于绘制PDF内容
    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning("无法创建PDF文件");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建PDF文件");
        }
        return false;
    }

    // 设置渲染质量
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 获取页面的可打印区域
    QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
    qreal pageWidth = printableRect.width();
    qreal pageHeight = printableRect.height();

    // 设置字体 - 改进字体设置以解决乱码问题
    QFont font;
    font.setFamily("SimHei"); // 使用黑体，确保中文正常显示
    font.setPointSize(7); // 进一步减小字体大小以解决拥挤问题
    painter.setFont(font);
    qDebug() << "设置字体: " << font.family() << ", 大小: " << font.pointSize();

    // 从父窗口获取Form_page3的UI元素
    QLabel *titleLabel = parent ? parent->findChild<QLabel*>("label") : nullptr;
    QTableView *tableView = parent ? parent->findChild<QTableView*>("tableView") : nullptr;
    QTextEdit *textEdit = parent ? parent->findChild<QTextEdit*>("textEdit") : nullptr;

    // 调试信息
    qDebug() << "正在生成Form_page3的PDF报告";
    qDebug() << "是否找到label:" << (titleLabel ? "是" : "否") << "，文本:" << (titleLabel ? titleLabel->text() : "无");
    qDebug() << "是否找到tableView:" << (tableView ? "是" : "否") << "，是否有model:" << (tableView && tableView->model() ? "是" : "否");
    qDebug() << "是否找到textEdit:" << (textEdit ? "是" : "否") << "，文本:" << (textEdit ? textEdit->toPlainText() : "无");

    // 1. 绘制标题（使用label的内容）
    QString titleText = titleLabel ? titleLabel->text() : "检验结果报告";
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    // 按照用户要求将标题行高度从40增加到300
    painter.drawText(QRectF(0, 20, pageWidth, 300), Qt::AlignCenter, titleText);
    font.setPointSize(7); // 使用一致的小字体大小
    font.setBold(false);
    painter.setFont(font);

    // 2. 绘制表格内容（与tableView一致）
    qreal tableTop = 350; // 增加表格起始位置，避免与标题重叠
    qreal tableLeft = 30; // 增加左边距
    qreal tableWidth = pageWidth - 60; // 减小表格宽度，增加页边距
    
    if (tableView && tableView->model()) {
        QAbstractItemModel *model = tableView->model();
        if (model) {
            int rowCount = model->rowCount();
            int columnCount = model->columnCount();
            
            // 计算行高和列宽 - 按照用户要求将行高大幅增加到200
            qreal headerHeight = 200;
            qreal rowHeight = 200;
            qreal tableHeight = headerHeight + rowHeight * rowCount; // 表头和数据行
            qDebug() << "表格行高设置 - 表头: " << headerHeight << ", 数据行: " << rowHeight;
            
            // 检查是否需要更多页面空间
            // 为每个表格预留足够空间，防止跨页导致的布局问题
            if (tableTop + tableHeight + 150 > pageHeight) { // 为备注留出更多空间
                printer.newPage();
                tableTop = 20;
            }
            
            // 绘制表头
            font.setBold(true);
            font.setPointSize(7); // 确保表头使用正确的字体大小
            painter.setFont(font);
            
            // 直接使用均匀分布的列宽，避免从tableView继承的窄列宽问题
            QVector<qreal> columnWidths;
            qreal uniformWidth = tableWidth / columnCount;
            for (int col = 0; col < columnCount; ++col) {
                columnWidths.append(uniformWidth);
            }
            
            // 添加调试信息
            qDebug() << "表格列宽设置 - 列数: " << columnCount << ", 每列宽度: " << uniformWidth;
            
            // 绘制表头
            qreal currentX = tableLeft;
            for (int col = 0; col < columnCount; ++col) {
                QRectF headerRect(currentX, tableTop, columnWidths[col], headerHeight);
                painter.drawRect(headerRect);
                QString headerText = model->headerData(col, Qt::Horizontal).toString();
                painter.drawText(headerRect, Qt::AlignCenter | Qt::TextWordWrap, headerText);
                currentX += columnWidths[col];
            }
            
            // 绘制数据行
            font.setBold(false);
            font.setPointSize(7); // 确保数据行使用正确的字体大小
            painter.setFont(font);
            
            for (int row = 0; row < rowCount; ++row) {
                currentX = tableLeft;
                qreal currentY = tableTop + headerHeight + row * rowHeight;
                
                for (int col = 0; col < columnCount; ++col) {
                    QRectF cellRect(currentX, currentY, columnWidths[col], rowHeight);
                    painter.drawRect(cellRect);
                    QModelIndex index = model->index(row, col);
                    QString cellText = model->data(index).toString();
                    // 绘制单元格文本，确保中文正常显示并自动换行
                    painter.drawText(cellRect, Qt::AlignCenter | Qt::TextWordWrap, cellText);
                    currentX += columnWidths[col];
                }
            }
            
            // 添加调试信息
            qDebug() << "Form_page3 PDF生成完成 - 使用的设置：";
            qDebug() << "页面尺寸: " << pageWidth << "x" << pageHeight;
            qDebug() << "字体: " << font.family() << ", 大小: " << font.pointSize();
            qDebug() << "表格设置 - 表头高度: " << headerHeight << ", 数据行高度: " << rowHeight;
            qDebug() << "表格总高度: " << tableHeight;
            
            // 3. 绘制textEdit的内容（表格下方）
            qreal remarksTop = tableTop + tableHeight + 20;
            QString remarksText = textEdit ? textEdit->toPlainText() : "";
            
            if (!remarksText.isEmpty()) {
                QRectF remarksRect(tableLeft, remarksTop, tableWidth, 100);
                painter.drawText(remarksRect, Qt::AlignTop | Qt::TextWordWrap, remarksText);
            }
        }
    } else {
        // 当无法获取tableView时，显示提示信息
        painter.drawText(QRectF(tableLeft, tableTop, tableWidth, 40), 
                         Qt::AlignCenter, "无法获取表格数据");
        
        // 如果有备注，仍然显示备注
        qreal remarksTop = tableTop + 60;
        QString remarksText = textEdit ? textEdit->toPlainText() : "";
        if (!remarksText.isEmpty()) {
            QRectF remarksRect(tableLeft, remarksTop, tableWidth, 100);
            painter.drawText(remarksRect, Qt::AlignTop | Qt::TextWordWrap, remarksText);
        }
    }

    // 完成绘制
    painter.end();
    return true;
}



// 用于Form_page1生成只有6个检验项目的pdf
void PDFGenerator::generateAndManageReportPDFWith6Projects(const ReportContent &content, QWidget *parent)
{
    qDebug() << "开始生成Form_page1的PDF报告（6个检验项目版本）"; 
    
    // 调用通用管理函数，传入默认文件名和生成函数
    managePDFReport("document.pdf", 
                  [content, parent](const QString &fileName) { return generateReportPDFWith6Projects(fileName, content, parent); }, 
                  parent);
}

// Form_page1生成9个检验项目的pdf
void PDFGenerator::generateAndManageReportPDF(const ReportContent &content, QWidget *parent)
{
    qDebug() << "开始生成Form_page1的PDF报告";
    
    // 调用通用管理函数，传入默认文件名和生成函数
    managePDFReport("document.pdf", 
                  [content, parent](const QString &fileName) { return generateReportPDF(fileName, content, parent); }, 
                  parent);
}

// 生成临时PDF文件，预览并询问是否保存（用于Form_page2）
void PDFGenerator::generateAndManageTablePDF(QWidget *parent)
{
    qDebug() << "开始生成Form_page2的PDF表格";
    
    // 调用通用管理函数，传入默认文件名和生成函数
    managePDFReport("table.pdf", 
                  [parent](const QString &fileName) { return generateTablePDF(fileName, parent); }, 
                  parent);
    
    // 显示成功消息
    // 已移除PDF生成成功弹窗
    // if (parent) {
    //     QMessageBox::information(parent, "成功", "PDF文件已成功生成！");
    // }
}

// 通用的PDF生成和管理函数
//生成临时pdf文件用于预览，并在关闭时由用户选择是否保存
void PDFGenerator::managePDFReport(const QString &defaultFileName, 
                                  std::function<bool(const QString &)> generatorFunc, 
                                  QWidget *parent)
{
    qDebug() << "开始生成PDF报告";
    
    // 创建临时文件，用于存储PDF内容
    QString tempFileName = QDir::temp().absoluteFilePath(QString("qt_temp_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")));

    // 测试是否可以创建文件
    QFile testFile(tempFileName);
    if (!testFile.open(QIODevice::WriteOnly)) {
        qWarning("无法创建临时文件");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建临时PDF文件");
        }
        return;
    }
    testFile.close();

    qDebug() << "PDF临时文件路径：" << tempFileName;
    qDebug() << "临时文件是否存在：" << QFile::exists(tempFileName);
    qDebug() << "临时文件所在目录是否存在：" << QFileInfo(tempFileName).absoluteDir().exists();

    // 使用传入的生成函数生成PDF
    if (!generatorFunc(tempFileName)) {
        return; // 如果生成失败，直接返回
    }

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
            if (parent) {
                QMessageBox::warning(parent, "警告", "无法打开PDF预览，请手动查看临时文件：\n" + tempFileName);
            }
        }
    } else {
        qWarning("文件仍然被锁定，无法打开预览");
        if (parent) {
            QMessageBox::warning(parent, "警告", "PDF文件被锁定，无法打开预览。请稍后重试。");
        }
    }

    // 使用QTimer延迟询问用户是否保存，给预览程序足够的时间打开文件
    QTimer::singleShot(1000, parent, [=]() {
        // 显示对话框，询问用户是否保存PDF
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "保存PDF", "是否保存此PDF文件？",
                                     QMessageBox::Save | QMessageBox::Discard);

        if (reply == QMessageBox::Save) {
            // 用户选择保存，打开文件保存对话框
            QString saveFileName = QFileDialog::getSaveFileName(
                  nullptr, "保存PDF文件", defaultFileName, "PDF文件 (*.pdf)");

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
//                        QMessageBox::information(nullptr, "成功", "PDF文件已保存\n" + saveFileName);
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
    
    // 已移除PDF生成成功弹窗
    // 显示成功消息
    // if (parent) {
    //     QMessageBox::information(parent, "成功", "PDF文件已成功生成！");
    // }
}

// 生成临时PDF文件，预览并询问是否保存（用于Form_page3）
void PDFGenerator::generateAndManageFormPage3PDF(QWidget *parent)
{
    qDebug() << "开始生成Form_page3的PDF报告";
    
    // 调用通用管理函数，传入默认文件名和生成函数
    managePDFReport("form_page3.pdf", 
                  [parent](const QString &fileName) { return generateFormPage3PDF(fileName, parent); }, 
                  parent);
}
